#include <iostream>
#include <chrono>
#include <vector>
#include <cmath>
#include "benchmark_registry.hpp"
#include "benchmark_runner.hpp"

using Clock = std::chrono::high_resolution_clock;

static double computePolygonArea(const std::vector<std::pair<double, double>>& v)
{
    double sum = 0.0;
    for (size_t i = 0; i < v.size(); ++i) {
        auto& p1 = v[i];
        auto& p2 = v[(i + 1) % v.size()];
        sum += p1.first * p2.second - p2.first * p1.second;
    }
    return std::abs(sum) * 0.5;
}

void benchmarkGeometryHeavy()
{
    std::cout << "Benchmark Geometria Pesada\n";

    const std::size_t polygons = 30000;
    const std::size_t vertices = 32;

    std::vector<std::vector<std::pair<double,double>>> mesh;
    mesh.reserve(polygons);

    for (std::size_t i = 0; i < polygons; ++i) {
        std::vector<std::pair<double,double>> poly;
        poly.reserve(vertices);

        for (std::size_t j = 0; j < vertices; ++j) {
            double ang = (j * 6.28318) / vertices;
            poly.emplace_back(std::cos(ang), std::sin(ang));
        }
        mesh.push_back(std::move(poly));
    }

    auto start = Clock::now();

    double totalArea = 0.0;
    for (auto& p : mesh)
        totalArea += computePolygonArea(p);

    auto end = Clock::now();

    double ms = std::chrono::duration<double, std::milli>(end - start).count();

    std::cout << "  Area total: " << totalArea << "\n";
    std::cout << "  Tempo -> " << ms << " ms\n\n";

    BenchmarkRunner::instance().pushResult(
        "benchmarkGeometryHeavy",
        {
            {"time_ms", std::to_string(ms)},
            {"polygons", std::to_string(polygons)}
        }
    );
}

REGISTER_BENCHMARK(benchmarkGeometryHeavy);
