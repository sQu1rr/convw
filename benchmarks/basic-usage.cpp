#include "benchmark/benchmark.h"

#include "target/target.hpp"
#include "target/hardcoded.hpp"

namespace hc = convw_bench_hardcoded;
namespace tg = convw_bench_target;

static void BM_Hardcoded_Full(benchmark::State& state)
{
    hc::Full object([](Argument a, Argument& b, Argument&& c) {
        return c.b;
    });
    while (state.KeepRunning()) {
        benchmark::DoNotOptimize( object.call() );
    }
}
BENCHMARK(BM_Hardcoded_Full);

static void BM_Hardcoded_Wrap(benchmark::State& state)
{
    hc::Wrap object([](Argument&& c) { return c.b; });
    while (state.KeepRunning()) {
        benchmark::DoNotOptimize( object.call() );
    }
}
BENCHMARK(BM_Hardcoded_Wrap);

static void BM_Hardcoded_Empty(benchmark::State& state)
{
    hc::Empty object([]() { return 1.0f; });
    while (state.KeepRunning()) {
        benchmark::DoNotOptimize( object.call() );
    }
}
BENCHMARK(BM_Hardcoded_Empty);

static void BM_Target_Full(benchmark::State& state)
{
    tg::Target object([](Argument a, Argument& b, Argument&& c) {
        return c.b;
    });
    while (state.KeepRunning()) {
        benchmark::DoNotOptimize( object.call() );
    }
}
BENCHMARK(BM_Target_Full);

static void BM_Target_Wrap(benchmark::State& state)
{
    tg::Target object([](Argument&& c) { return c.b; });
    while (state.KeepRunning()) {
        benchmark::DoNotOptimize( object.call() );
    }
}
BENCHMARK(BM_Target_Wrap);

static void BM_Target_Empty(benchmark::State& state)
{
    tg::Target object([]() { return 1.0f; });
    while (state.KeepRunning()) {
        benchmark::DoNotOptimize( object.call() );
    }
}
BENCHMARK(BM_Target_Empty);

BENCHMARK_MAIN()
