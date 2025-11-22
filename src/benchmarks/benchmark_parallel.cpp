#include <iostream>
#include <chrono>
#include <vector>
#include <atomic>
#include <thread>
#include <cmath>
#include <condition_variable>
#include "benchmark_registry.hpp"
#include "benchmark_runner.hpp"
#include "mdtg/thread_pool.hpp"

using Clock = std::chrono::high_resolution_clock;

static void heavyTask(std::size_t work)
{
    double x = 0.0;
    for (std::size_t i = 0; i < work; ++i)
        x += std::sin((double)i) * 0.000001;
    (void)x;
}

void benchmarkParallelReal()
{
    std::cout << "Benchmark Paralelo Real\n";

    std::vector<std::size_t> threadList = {1, 2, 4, 8, 16};

    const std::size_t tasks = 8000;
    const std::size_t workPerTask = 400;

    for (auto t : threadList) {

        ThreadPool pool(t);

        std::atomic<std::size_t> done{0};
        std::mutex m;
        std::condition_variable cv;

        auto start = Clock::now();

        for (std::size_t i = 0; i < tasks; ++i) {
            pool.enqueue([&]() {
                heavyTask(workPerTask);
                if (done.fetch_add(1) + 1 == tasks)
                    cv.notify_one();
            });
        }

        std::unique_lock<std::mutex> lock(m);
        cv.wait(lock, [&]() { return done.load() >= tasks; });

        pool.shutdown();
        auto end = Clock::now();

        double ms = std::chrono::duration<double, std::milli>(end - start).count();

        std::cout << "  Threads " << t << " -> " << ms << " ms\n";

        BenchmarkRunner::instance().pushResult(
            "benchmarkParallelReal",
            {
                {"threads", std::to_string(t)},
                {"time_ms", std::to_string(ms)}
            }
        );
    }

    std::cout << "\n";
}

REGISTER_BENCHMARK(benchmarkParallelReal);
