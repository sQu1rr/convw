#include "base.hpp"

std::uniform_real_distribution<double> dd;
std::uniform_real_distribution<float> df;
std::uniform_int_distribution<int> di;

namespace convw_bench_base {

Base::Base() : mt_(sq::belks::randomDevice<>()) { }

Args Base::work() noexcept
{
    return {
        {di(mt_), df(mt_), dd(mt_)},
        {di(mt_), df(mt_), dd(mt_)},
        {di(mt_), df(mt_), dd(mt_)}
    };
}

}
