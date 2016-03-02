#include "benchmark/benchmark.h"

#include "target/target.hpp"
#include "target/hardcoded.hpp"

namespace hc = convw_bench_hardcoded;
namespace tg = convw_bench_target;

static void BM_Hardcoded_Full(benchmark::State& state)
{
    volatile auto dummy = 1.0f;
    hc::Full object([&dummy](Argument a, Argument& b, Argument&& c) {
        return c.b + dummy;
    });
    while (state.KeepRunning()) {
        benchmark::DoNotOptimize( object.call() );
    }
}
BENCHMARK(BM_Hardcoded_Full);

static void BM_Hardcoded_Wrap(benchmark::State& state)
{
    volatile auto dummy = 1.0f;
    hc::Wrap object([&dummy](Argument&& c) { return c.b + dummy; });
    while (state.KeepRunning()) {
        benchmark::DoNotOptimize( object.call() );
    }
}
BENCHMARK(BM_Hardcoded_Wrap);

static void BM_Hardcoded_Empty(benchmark::State& state)
{
    volatile auto dummy = 1.0f;
    hc::Empty object([&dummy]() { return 1.0f + dummy; });
    while (state.KeepRunning()) {
        benchmark::DoNotOptimize( object.call() );
    }
}
BENCHMARK(BM_Hardcoded_Empty);

static void BM_Target_Full(benchmark::State& state)
{
    volatile auto dummy = 1.0f;
    tg::Target object([&dummy](Argument a, Argument& b, Argument&& c) {
        return c.b + dummy;
    });
    while (state.KeepRunning()) {
        benchmark::DoNotOptimize( object.call() );
    }
}
BENCHMARK(BM_Target_Full);

static void BM_Target_Wrap(benchmark::State& state)
{
    volatile auto dummy = 1.0f;
    tg::Target object([&dummy](Argument&& c) { return c.b + dummy; });
    while (state.KeepRunning()) {
        benchmark::DoNotOptimize( object.call() );
    }
}
BENCHMARK(BM_Target_Wrap);

static void BM_Target_Empty(benchmark::State& state)
{
    volatile auto dummy = 1.0f;
    tg::Target object([&dummy]() { return 1.0f + dummy; });
    while (state.KeepRunning()) {
        benchmark::DoNotOptimize( object.call() );
    }
}
BENCHMARK(BM_Target_Empty);

BENCHMARK_MAIN()
