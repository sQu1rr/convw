#ifndef SQ_CONVW_BENCH_TARGET_HARDCODED
#define SQ_CONVW_BENCH_TARGET_HARDCODED

#include <functional>

#include "base.hpp"

namespace convw_bench_hardcoded {

template <typename Callback>
class Base : public convw_bench_base::Base {
public:
    Base(const std::function<Callback>& cb) noexcept : cb_(cb) { }

protected:
    std::function<Callback> cb_;
};

class Full : public Base<FullCb> {
public:
    using Base::Base;
    float call();
};

class Wrap : public Base<WrapCb> {
public:
    using Base::Base;
    float call();
};

class Empty : public Base<EmptyCb> {
public:
    using Base::Base;
    float call();
};

}

#endif
