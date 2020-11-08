#pragma once

#include <array>
#include <limits>
#include <tuple>
#include <type_traits>
#include <utility>

namespace Akura {
template <class... Args> struct Iterator {
  Iterator(Args... args) : its(args...) {}
  std::tuple<Args...> its;
  constexpr inline auto &operator++() {
    std::apply([](auto &&... args) { (++args, ...); }, its);
    return *this;
  }
  constexpr inline auto operator==(const Iterator<Args...> &other) {
    return std::apply(
        [](Args &&... a, Args &&... b) { return ((a == b) || ...); },
        std::tuple_cat(its, other.its));
  }
  constexpr inline auto operator!=(const Iterator<Args...> &other) {
    return !(*this == other);
  }
  constexpr inline auto operator*() {
    return std::apply([](auto &&... args) { return std::tuple(*args...); },
                      its);
  }
};

template <class... Args> struct Zip {
  static_assert(sizeof...(Args) >= 1, "Zip requires at least 1 argument");
  using iterator = Iterator<typename std::decay_t<Args>::iterator...>;
  iterator _begin;
  iterator _end;
  Zip(Args &&... args)
      : _begin(std::begin(std::forward<Args>(args))...),
        _end(std::end(std::forward<Args>(args))...) {}

  constexpr inline auto begin() { return _begin; }
  constexpr inline auto end() { return _end; }
  constexpr inline auto begin() const { return _begin; }
  constexpr inline auto end() const { return _end; }
};

template <class... Args> Zip(Args &&...) -> Zip<Args...>;

struct IndexIterator {
  int i = 0;
  constexpr IndexIterator(int i) : i(i) {}
  constexpr int size() const { return std::numeric_limits<int>::max(); }
  using iterator = IndexIterator;
  IndexIterator &operator++() {
    ++i;
    return *this;
  }
  int &operator*() { return i; }
  bool operator==(const IndexIterator &other) const { return i == other.i; }
  bool operator!=(const IndexIterator &other) const {
    return !(*this == other);
  }
  constexpr auto &begin() { return *this; }
  constexpr auto end() { return IndexIterator{size()}; }
  constexpr auto &begin() const { return *this; }
  constexpr auto end() const { return IndexIterator{size()}; }
};

template <class T> auto enumerate(T &&t) {
  return Akura::Zip(std::forward<T>(t), IndexIterator{0});
}

}; // namespace Akura
