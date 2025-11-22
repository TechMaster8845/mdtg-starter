#pragma once
#include <vector>
#include <functional>
#include <string>

struct BenchResult {
    std::string name;
    // generic key:value (simple)
    std::vector<std::pair<std::string,std::string>> metrics;
};

class BenchmarkRegistry {
public:
    struct Entry {
        std::string name;
        std::function<void()> fn;
    };

    static BenchmarkRegistry& instance() {
        static BenchmarkRegistry reg;
        return reg;
    }

    void add(const std::string& n, std::function<void()> fn) {
        entries.push_back({n, fn});
    }

    const std::vector<Entry>& list() const { return entries; }

private:
    std::vector<Entry> entries;
};

#define REGISTER_BENCHMARK(fn) \
    static bool _reg_##fn = [](){ \
        BenchmarkRegistry::instance().add(#fn, fn); \
        return true; \
    }()
