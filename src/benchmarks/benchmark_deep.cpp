#include <iostream>
#include <chrono>
#include <memory>
#include "benchmark_registry.hpp"
#include "benchmark_runner.hpp"
#include "mdtg/mdtg_node.hpp"

using Clock = std::chrono::high_resolution_clock;

static std::unique_ptr<MDTGNode> buildDeepTree(std::size_t depth)
{
    auto root = std::make_unique<MDTGNode>("root");
    MDTGNode* cur = root.get();

    for (std::size_t i = 0; i < depth; ++i) {
        auto c = std::make_unique<MDTGNode>("deep_" + std::to_string(i));
        c->vertices.resize(100);
        cur = cur->addChild(std::move(c));
    }

    return root;
}

static void processDeep(MDTGNode* root)
{
    root->computeGlobalTransform();
    root->resolveProperties();
    root->evaluateNode();

    MDTGNode* cur = nullptr;
    auto children = root->takeChildrenSnapshot();
    if (!children.empty())
        cur = children[0];

    while (cur) {
        cur->computeGlobalTransform();
        cur->resolveProperties();
        cur->evaluateNode();

        auto next = cur->takeChildrenSnapshot();
        if (next.empty())
            break;

        cur = next[0];
    }
}

void benchmarkDeep()
{
    std::cout << "Benchmark Árvore Profunda (5000 níveis)\n";

    auto tree = buildDeepTree(5000);

    auto start = Clock::now();
    processDeep(tree.get());
    auto end = Clock::now();

    double ms = std::chrono::duration<double, std::milli>(end - start).count();
    std::cout << "  Tempo -> " << ms << " ms\n\n";

    BenchmarkRunner::instance().pushResult(
        "benchmarkDeep",
        {
            {"time_ms", std::to_string(ms)}
        }
    );
}

REGISTER_BENCHMARK(benchmarkDeep);
