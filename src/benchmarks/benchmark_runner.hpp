#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <mutex>
#include <fstream>
#include <sstream>
#include "benchmark_registry.hpp"

// Simple global results collector (thread-safe-ish; benchmarks run serially here)
class ResultsCollector {
public:
    void add(const std::string& benchName, const std::vector<std::pair<std::string,std::string>>& metrics) {
        std::lock_guard<std::mutex> lg(m_);
        results_.push_back({benchName, metrics});
    }
    const std::vector<std::pair<std::string, std::vector<std::pair<std::string,std::string>>>>& all() const {
        return results_;
    }
    void clear() { results_.clear(); }
private:
    std::mutex m_;
    std::vector<std::pair<std::string, std::vector<std::pair<std::string,std::string>>>> results_;
};

class BenchmarkRunner {
public:
    static BenchmarkRunner& instance() {
        static BenchmarkRunner r;
        return r;
    }

    void setThreadList(const std::vector<int>& t) { if(!t.empty()) threadsOverride_ = t; }
    void setExportPaths(const std::string& csv, const std::string& json) { csvPath_ = csv; jsonPath_ = json; }

    void runAll() {
        auto list = BenchmarkRegistry::instance().list();
        std::cout << "Benchmarks registrados: " << list.size() << "\n\n";
        for (auto &e : list) {
            runSingle(e.name, e.fn);
        }
        if (!csvPath_.empty()) exportCsv(csvPath_);
        if (!jsonPath_.empty()) exportJson(jsonPath_);
    }

    void runByName(const std::string& name) {
        auto list = BenchmarkRegistry::instance().list();
        for (auto &e : list) {
            if (e.name == name) {
                runSingle(e.name, e.fn);
                if (!csvPath_.empty()) exportCsv(csvPath_);
                if (!jsonPath_.empty()) exportJson(jsonPath_);
                return;
            }
        }
        std::cout << "Benchmark '" << name << "' não encontrado.\n";
    }

    // Utility used by benchmarks to get threads to test (if they use override)
    std::vector<int> getThreadsToUse(const std::vector<int>& defaultList) const {
        if (!threadsOverride_.empty()) return threadsOverride_;
        return defaultList;
    }

    // Allow benchmarks to push results:
    void pushResult(const std::string& benchName, const std::vector<std::pair<std::string,std::string>>& metrics) {
        results_.add(benchName, metrics);
    }

private:
    BenchmarkRunner() = default;
    void runSingle(const std::string& name, std::function<void()> fn) {
        std::cout << "Executando: " << name << "\n";
        fn();
        std::cout << "\n-----------------------------\n";
    }

    void exportCsv(const std::string& path) {
        std::ofstream f(path);
        if (!f) { std::cout << "Erro ao abrir CSV: " << path << "\n"; return; }
        // header
        f << "benchmark,metric,value\n";
        for (auto &r : results_.all()) {
            const auto &bname = r.first;
            for (auto &kv : r.second) {
                f << bname << "," << kv.first << "," << kv.second << "\n";
            }
        }
        f.close();
        std::cout << "Exportado CSV para " << path << "\n";
    }

    void exportJson(const std::string& path) {
        std::ofstream f(path);
        if (!f) { std::cout << "Erro ao abrir JSON: " << path << "\n"; return; }
        f << "[\n";
        bool firstB = true;
        for (auto &r : results_.all()) {
            if (!firstB) f << ",\n";
            firstB = false;
            f << "  {\n    \"benchmark\": \"" << escapeJson(r.first) << "\",\n    \"metrics\": [\n";
            bool first = true;
            for (auto &kv : r.second) {
                if (!first) f << ",\n";
                first = false;
                f << "      { \"k\": \"" << escapeJson(kv.first) << "\", \"v\": \"" << escapeJson(kv.second) << "\" }";
            }
            f << "\n    ]\n  }";
        }
        f << "\n]\n";
        f.close();
        std::cout << "Exportado JSON para " << path << "\n";
    }

    static std::string escapeJson(const std::string& s) {
        std::ostringstream o;
        for (char c : s) {
            switch (c) {
                case '\n': o << "\\n"; break;
                case '\r': o << "\\r"; break;
                case '\t': o << "\\t"; break;
                case '"': o << "\\\""; break;
                case '\\': o << "\\\\"; break;
                default: o << c; break;
            }
        }
        return o.str();
    }

    std::vector<int> threadsOverride_;
    std::string csvPath_;
    std::string jsonPath_;
    ResultsCollector results_;
};
