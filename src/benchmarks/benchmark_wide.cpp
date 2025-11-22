#include <iostream>
#include <chrono>
#include <vector>
#include <memory>
#include "benchmark_registry.hpp"
#include "benchmark_runner.hpp"
#include "mdtg/mdtg_node.hpp"
#include "mdtg/thread_pool.hpp"

using Clock = std::chrono::high_resolution_clock;

static std::unique_ptr<MDTGNode> buildWideTree(std::size_t depth, std::size_t children)
{
    auto root = std::make_unique<MDTGNode>("root");
    std::vector<MDTGNode*> level{root.get()};

    for (std::size_t d = 0; d < depth; ++d) {
        std::vector<MDTGNode*> next;
        for (auto n : level) {
            for (std::size_t c = 0; c < children; ++c) {
                auto child = std::make_unique<MDTGNode>("n_" + std::to_string(d) + "_" + std::to_string(c));
                child->vertices.resize(200);
                next.push_back(n->addChild(std::move(child)));
            }
        }
        level.swap(next);
    }
    return root;
}

static void process(MDTGNode* root, std::size_t t)
{
    ThreadPool pool(t);

    std::vector<std::vector<MDTGNode*>> levels;
    std::vector<MDTGNode*> cur{root};

    while (!cur.empty()) {
        levels.push_back(cur);
        std::vector<MDTGNode*> next;
        for (auto n : cur) {
            auto c = n->takeChildrenSnapshot();
            next.insert(next.end(), c.begin(), c.end());
        }
        cur.swap(next);
    }

    for (auto &layer : levels) {
        std::atomic<std::size_t> done{0};
        std::mutex m;
        std::condition_variable cv;

        for (auto n : layer) {
            pool.enqueue([n, &done, &layer, &cv]() {
                n->computeGlobalTransform();
                n->resolveProperties();
                n->evaluateNode();
                if (done.fetch_add(1) + 1 == layer.size())
                    cv.notify_one();
            });
        }

        std::unique_lock<std::mutex> ul(m);
        cv.wait(ul, [&]() { return done.load() >= layer.size(); });
    }

    pool.shutdown();
}

void benchmarkWide()
{
    std::cout << "Benchmark Árvore Larga (~20k nós)\n";

    auto tree = buildWideTree(6, 5);

    for (size_t t : {1, 2, 4, 8, 16}) {

        auto start = Clock::now();
        process(tree.get(), t);
        auto end = Clock::now();

        double ms = std::chrono::duration<double, std::milli>(end - start).count();
        std::cout << "  Threads " << t << " -> " << ms << " ms\n";

        BenchmarkRunner::instance().pushResult(
            "benchmarkWide",
            {
                {"threads", std::to_string(t)},
                {"time_ms", std::to_string(ms)}
            }
        );
    }

    std::cout << "\n";
}

REGISTER_BENCHMARK(benchmarkWide);
