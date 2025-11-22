#pragma once
#include <vector>
#include <thread>
#include <functional>
#include <atomic>
#include <queue>
#include <mutex>
#include <condition_variable>

class ThreadPool {
public:
    explicit ThreadPool(size_t workers = std::thread::hardware_concurrency());
    ~ThreadPool();

    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;

    void enqueue(std::function<void()> fn);
    void shutdown();

private:
    std::vector<std::thread> threads_;
    std::queue<std::function<void()>> tasks_;
    std::mutex tasksMutex_;
    std::condition_variable cv_;
    std::atomic<bool> running_{true};
    void workerLoop();
};
