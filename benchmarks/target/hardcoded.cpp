#include "hardcoded.hpp"

#include "benchmark/benchmark.h"

namespace convw_bench_hardcoded {

float Full::call()
{
    auto args = work();
    return cb_(args.a, args.b, std::move(args.c));
}

float Wrap::call()
{
    auto args = work();
    return cb_(std::move(args.c));
}

float Empty::call()
{
    float result;
    auto args = work();
    benchmark::DoNotOptimize( result = args.a.b + args.b.b + args.c.b );
    return cb_();
}

}
