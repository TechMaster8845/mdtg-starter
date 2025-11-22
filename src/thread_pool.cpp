#include "mdtg/mdtg_node.hpp"
#include "mdtg/thread_pool.hpp"

ThreadPool::ThreadPool(size_t w) {
    if (w == 0) w = 1;
    threads_.reserve(w);
    for (size_t i = 0; i < w; ++i)
        threads_.emplace_back([this]{ workerLoop(); });
}

ThreadPool::~ThreadPool() { shutdown(); }

void ThreadPool::enqueue(std::function<void()> fn) {
    {
        std::lock_guard<std::mutex> l(tasksMutex_);
        tasks_.push(std::move(fn));
    }
    cv_.notify_one();
}

void ThreadPool::shutdown() {
    if (!running_.exchange(false)) return;
    cv_.notify_all();
    for (auto& t : threads_)
        if (t.joinable() && t.get_id() != std::this_thread::get_id())
            t.join();
}

void ThreadPool::workerLoop() {
    while (running_) {
        std::function<void()> task;
        {
            std::unique_lock<std::mutex> ul(tasksMutex_);
            cv_.wait(ul, [this]{ return !running_ || !tasks_.empty(); });
            if (!running_ && tasks_.empty()) return;
            task = std::move(tasks_.front());
            tasks_.pop();
        }
        task();
    }
}
