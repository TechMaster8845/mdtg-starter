// src/main.cpp
#include <windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "benchmarks/benchmark_runner.hpp"

static void configureConsoleUTF8()
{
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif
}

int main(int argc, char** argv)
{
    configureConsoleUTF8();

    std::string benchArg = "all";
    std::string exportCsv;
    std::string exportJson;
    std::vector<int> threadsList;

    for (int i = 1; i < argc; ++i) {
        std::string a = argv[i];
        if (a == "--bench" && i + 1 < argc) { benchArg = argv[++i]; }
        else if (a == "--export-csv" && i + 1 < argc) { exportCsv = argv[++i]; }
        else if (a == "--export-json" && i + 1 < argc) { exportJson = argv[++i]; }
        else if (a == "--threads" && i + 1 < argc) {
            std::string list = argv[++i];
            size_t pos = 0;
            while (pos < list.size()) {
                size_t comma = list.find(',', pos);
                if (comma == std::string::npos) comma = list.size();
                threadsList.push_back(std::stoi(list.substr(pos, comma - pos)));
                pos = comma + 1;
            }
        }
    }

    std::cout << "MDTG Benchmark Suite (CLI)\n\n";

    // Configure runner options
    BenchmarkRunner::instance().setThreadList(threadsList); // optional: empty means default
    BenchmarkRunner::instance().setExportPaths(exportCsv, exportJson);

    if (benchArg == "all") {
        BenchmarkRunner::instance().runAll();
    } else {
        BenchmarkRunner::instance().runByName(benchArg);
    }

    std::cout << "\nBenchmarks finalizados.\n";
    return 0;
}
