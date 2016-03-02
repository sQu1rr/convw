#ifndef SQ_CONVW_BENCH_TARGET_TARGET
#define SQ_CONVW_BENCH_TARGET_TARGET

#include <functional>

#include "base.hpp"
#include "convw/convw.hpp"

namespace convw_bench_target {

using sq::convw::Convw;

class Target : public convw_bench_base::Base {
public:
    Target(Convw<FullCb> cb);
    float call();

protected:
    Convw<FullCb> cb_;
};

}

#endif
