#pragma once

#include <array>
#include <limits>
#include <tuple>
#include <utility>

namespace Akura {
    template <class... Args> struct Iterator;

    template <class T> struct Iterator<T> {
        using Type = std::decay_t<T>;
        using IteratorType = typename Type::iterator;
        IteratorType it;
        constexpr inline Iterator(IteratorType t) : it(t) {}
        constexpr inline auto operator++() {
            ++it;
            return *this;
        }
        constexpr inline auto operator!=(const Iterator<T> &other) {
            return it != other.it;
        }
        constexpr inline auto operator*() { return std::forward_as_tuple(*it); }
    };

    template <class T, class... Args>
        struct Iterator<T, Args...> : public Iterator<Args...> {
            using Type = std::decay_t<T>;
            using IteratorType = typename Type::iterator;
            IteratorType it;
            constexpr inline Iterator(IteratorType t, Iterator<Args...> parent)
                : Iterator<Args...>(parent), it(t){};
            constexpr inline auto &operator++() {
                ++it;
                Iterator<Args...>::operator++();
                return *this;
            }
            constexpr inline auto operator!=(const Iterator<T, Args...> &other) {
                return it != other.it && Iterator<Args...>::operator!=(other);
            }
            constexpr inline auto operator*() {
                return std::tuple_cat(std::forward_as_tuple(*it),
                        Iterator<Args...>::operator*());
            }
        };

    template <class T, class F>
        constexpr inline auto iteratorGenerator(F &&f, T &&t) {
            return Iterator<T>(f(std::forward<T>(t)));
        }

    template <class T, class F, class... Args>
        constexpr inline auto iteratorGenerator(F &&f, T &&t, Args &&... args) {
            return Iterator<T, Args...>(
                    f(std::forward<T>(t)),
                    iteratorGenerator(std::forward<F>(f), std::forward<Args>(args)...));
        }

    template <class T, class... Args> struct Zip;

    template <class T, class... Args> Zip(T&&, Args &&...) -> Zip<T, Args...>;

    template <class T, class... Args> struct Zip {
        using iterator = Iterator<T, Args...>;
        iterator _begin;
        iterator _end;
        Zip(T && t, Args &&... args)
            : _begin(iteratorGenerator([](auto &&cont) { return cont.begin(); },
                        std::forward<T>(t),
                        std::forward<Args>(args)...)),
            _end(iteratorGenerator([](auto &&cont) { return cont.end(); },
                        std::forward<T>(t),
                        std::forward<Args>(args)...)) {}

        constexpr inline auto begin() { return _begin; }
        constexpr inline auto end() { return _end; }
    };

struct IndexIterator {
    int i = 0;
    // IndexIterator() = default;
    constexpr IndexIterator(int i) : i(i) {}
    constexpr int size() const { return std::numeric_limits<int>::max(); }
    using iterator = IndexIterator;
    IndexIterator &operator++() {
        ++i;
        return *this;
    }
    int &operator*() {
        // std::cout << "operator * " << i << std::endl;
        return i;
    }
    bool operator!=(const IndexIterator &other) const { return i != other.i; }
    constexpr auto &begin() const { return *this; }
    constexpr auto end() const { return IndexIterator{size()}; }
};

template <class T> auto enumerate(T &&t) { return Akura::Zip(t, IndexIterator{0}); }

}; // namespace Akura
