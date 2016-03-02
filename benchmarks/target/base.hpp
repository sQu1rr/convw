#ifndef SQ_CONVW_BENCH_TARGET_BASE
#define SQ_CONVW_BENCH_TARGET_BASE

#include "argument.hpp"

#include "belks/random.hpp"

using FullCb = float (Argument, Argument&, Argument&&);
using WrapCb = float (Argument&&);
using EmptyCb = float ();

namespace convw_bench_base {

struct Args { Argument a, b, c; };

class Base {
public:
    Base();
    Args work() noexcept;

private:
    decltype(sq::belks::randomDevice<>()) mt_;
};

}

#endif
