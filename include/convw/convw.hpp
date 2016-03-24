/** 
 * @file convw.hpp
 * @brief Convinient Callback Wrapper
 */
#ifndef SQ_CONVW_HPP
#define SQ_CONVW_HPP

#include <functional>

#include "belks/mpl.hpp"

namespace sq {

namespace convw {

namespace convw_detail {

template <size_t N, typename... Types> struct Wrap {
    template <typename Func, typename... Args, typename A>
    static constexpr auto call(Func&& func, A&& arg, Args&&... args)
    {
        return Wrap<N-1>::call(std::forward<Func>(func),
                               std::forward<Args>(args)...);
    }
};

template <size_t N, typename Type, typename... Types>
struct Wrap<N, Type, Types...> {
    template <typename Func, typename... Args>
    static constexpr auto call(Func&& func, Type&& arg, Args&&... args)
    {
        return Wrap<N-1, Types...>::call(std::forward<Func>(func),
                                         std::forward<Args>(args)...,
                                         std::forward<Type>(arg));
    }

    template <typename Func, typename... Args, typename A>
    static constexpr auto call(Func&& func, A&& arg, Args&&... args)
    {
        return Wrap<N-1, Type, Types...>::call(std::forward<Func>(func),
                                               std::forward<Args>(args)...);
    }

    template <typename Func>
    static constexpr auto call(Func&& func)
    {
        static_assert(func, "Mismatch in argument types, count or order");
    }
};

template <> struct Wrap<0> {
    template <typename Func, typename... Args>
    static constexpr auto call(Func&& func, Args&&... args)
    {
        return func(std::forward<Args>(args)...);
    }
};

template <typename>
struct Convw;

template <typename Ret, typename... Par>
struct Convw<Ret (Par...)> {
    template <typename R, typename... Args>
    static constexpr auto wrap(std::function<R (Args...)>&& function)
    {
        static_assert(std::is_same<R, Ret>::value, "Incorrect return type");
        return belks::toFunction([function](Par... params) -> Ret {
            return Wrap<sizeof...(Par), Args...>::call(
                function, std::forward<Par>(params)...
            );
        });
    }
    
    static constexpr auto wrap(std::function<Ret (Par...)>&& function)
    {
        using Function = std::function<Ret (Par...)>;
        return std::forward<Function>(function);
    }
};

} // \convw_detail

/** 
 * @brief Conviniently wrap given function into callback
 * 
 * @param function to wrap
 * 
 * @return wrapped callback
 */
template <typename Callback, typename Function>
constexpr auto convw(Function&& function)
{
    return convw_detail::Convw<Callback>::wrap(
        belks::toFunction(std::forward<Function>(function))
    );
}

template <typename> class Convw;

/** 
 * @brief Convinient class that wraps its function argument into callback
 *
 * Mimics std::function interface
 */
template <typename Ret, typename... Args> class Convw<Ret (Args...)> { 
    using Callback = Ret (Args...);
    using type = std::function<Callback>;

public:
    Convw() = default;

    template <typename Function>
    Convw(Function&& func) noexcept : function_(convw<Callback>(func)) { }

    auto operator () (Args... args) const
    {
        return function_(std::forward<Args>(args)...);
    }

    explicit operator bool() const noexcept { return bool(function_); }

private:
    type function_;
};

} // \convw

} // \sq

#endif
