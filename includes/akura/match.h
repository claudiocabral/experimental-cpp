#pragma once
#include <type_traits>

namespace akura {
namespace impl {
template <class T, class Predicate, class Expression, class... Args>
constexpr auto match(T && t, Predicate && pred, Expression && expr, Args... args) {
    constexpr bool predicate_is_function = std::is_invocable_r<bool, Predicate, T>();
    constexpr bool expression_is_function = std::is_invocable<Expression, T>();
    bool hasMatched;
    if constexpr (predicate_is_function) hasMatched = pred(std::forward<T>(t));
    else hasMatched = t == pred;
    if constexpr (expression_is_function) {
        constexpr bool expression_is_void_function = std::is_same_v<
            void,
            std::invoke_result_t<Expression, T>
                >;
        if (hasMatched) {
            if constexpr (expression_is_void_function) expr(t);
            else return expr(t);
        } else {
            if constexpr (sizeof...(Args) >= 2) {
                if constexpr(expression_is_void_function)
                    match(std::forward<T>(t), std::forward<Args>(args)...);
                else
                    return match(std::forward<T>(t), std::forward<Args>(args)...);
            } else if constexpr (sizeof...(Args) == 1) {
                auto tup = std::tuple(std::forward<Args>(args)...);
                using FinalExpr = decltype (std::get<0>(tup));
                if constexpr (std::is_invocable<FinalExpr, T>()) {
                    constexpr bool is_void = std::is_same_v<
                        void,
                        std::invoke_result_t<Expression, T>
                            >;
                    if constexpr(is_void) std::get<0>(tup)(std::forward<T>(t));
                    else return std::get<0>(tup)(std::forward<T>(t));
                } else {
                    return std::get<0>(tup);
                }
            } else {
                return expr(t);
            }
        }
    } else {
        if (hasMatched) return expr;
        else { 
            if constexpr (sizeof...(Args) == 1)
                return std::get<0>(std::tuple(std::forward<Args>(args)...));
            else if constexpr (sizeof...(Args) >= 2)
                return match(std::forward<T>(t), std::forward<Args>(args)...);
            else {
                return expr;
            }
        }
    }
}
}

template <class T, class... Args>
constexpr auto match(T && t, Args... args) {
    constexpr bool is_void = std::is_same_v<
        void,
        std::invoke_result_t<decltype(impl::match<T, Args...>), T, Args...>
    >;
    if constexpr (is_void) impl::match(std::forward<T>(t), std::forward<Args>(args)...);
    else return impl::match(std::forward<T>(t), std::forward<Args>(args)...);
}
}
