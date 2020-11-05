#pragma once

#include <tuple>
#include <utility>

namespace Akura
{
template <class... Args>
struct Zip;

template <class... Args>
Zip(Args&&...) -> Zip<Args...>;

template <class... Args>
struct Iterator;

template <class T>
struct Iterator<T>
{
    using Type = std::decay_t<T>;
    using IteratorType = typename Type::iterator;
    IteratorType it;
    constexpr inline Iterator(IteratorType t) : it(t) {}
    constexpr inline auto operator++()
    {
        ++it;
        return this;
    }
    constexpr inline auto operator!=(const Iterator<T> &other)
    {
        return it != other.it;
    }
    constexpr inline auto operator*()
    {
        return std::forward_as_tuple(*it);
    }
};

template <class T, class... Args>
struct Iterator<T, Args...> : public Iterator<Args...>
{
    using Type = std::decay_t<T>;
    using IteratorType = typename Type::iterator;
    IteratorType it;
    constexpr inline Iterator(IteratorType t, Iterator<Args...> parent) : Iterator<Args...>(parent), it(t) {};
    constexpr inline auto operator++()
    {
        ++it;
        Iterator<Args...>::operator++();
        return this;
    }
    constexpr inline auto operator!=(const Iterator<T, Args...> &other)
    {
        return it != other.it
            && Iterator<Args...>::operator!=(other);
    }
    constexpr inline auto operator*()
    {
        return std::tuple_cat(std::forward_as_tuple(*it), Iterator<Args...>::operator*());
    }
};

template <class T, class F>
constexpr inline auto iteratorGenerator(F && f, T&& t)
{
    return Iterator<T>(f(t));
}

template <class T, class F, class... Args>
constexpr inline auto iteratorGenerator(F &&f, T&& t, Args &&... args)
{
    return Iterator<T, Args...>(f(t), iteratorGenerator(f, args...));
}


template < class... Args>
struct Zip
{
    Iterator<Args...> _begin;
    Iterator<Args...> _end;
    Zip(Args&&... args)  : _begin(iteratorGenerator([](auto & cont) { return cont.begin();}, args...)),
            _end(iteratorGenerator([](auto & cont) { return cont.end();}, args...)) {}

    constexpr inline auto begin() {
        return _begin;
    }
    constexpr inline auto end() {
        return _end;
    }
};
};
