#include <iostream>
#include <vector>
#include <memory>
#include <condition_variable>
#include "mdtg/mdtg_node.hpp"
#include "mdtg/thread_pool.hpp"


void collect(std::vector<std::vector<MDTGNode*>>& lv, MDTGNode* r){
    std::vector<MDTGNode*> cur{r};
    while (!cur.empty()) {
        lv.push_back(cur);
        std::vector<MDTGNode*> next;
        for (auto n : cur) {
            auto ch = n->takeChildrenSnapshot();
            next.insert(next.end(), ch.begin(), ch.end());
        }
        cur.swap(next);
    }
}

int main() {
    auto root = std::make_unique<MDTGNode>("root");
    root->localTransform.identity();

    std::vector<std::vector<MDTGNode*>> levels;
    collect(levels, root.get());

    ThreadPool pool;
    for (auto& layer : levels) {
        std::atomic<size_t> done{0};
        std::mutex m;
        std::condition_variable cv;

        for (auto n : layer) {
            pool.enqueue([n,&done,&layer,&cv](){
                n->computeGlobalTransform();
                n->resolveProperties();
                n->evaluateNode();
                if (done.fetch_add(1) + 1 == layer.size())
                    cv.notify_one();
            });
        }

        std::unique_lock<std::mutex> ul(m);
        cv.wait(ul, [&]{ return done.load() >= layer.size(); });
    }

    pool.shutdown();
    std::cout << "MDTG Finalizado\n";
}
