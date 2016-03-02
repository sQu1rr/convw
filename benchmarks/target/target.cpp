#include "target.hpp"

#include "benchmark/benchmark.h"

namespace convw_bench_target {

Target::Target(Convw<FullCb> cb) : cb_(std::move(cb)) { }

float Target::call()
{
    auto args = work();
    return cb_(args.a, args.b, std::move(args.c));
}

}
