#define CATCH_CONFIG_ENABLE_BENCHMARKING
#include <catch2/catch.hpp>
#include <zip/zip.h>

using namespace Akura;

template <class F, class... Args>
static void loop(F && f, Args &&... args) {
    auto size = std::min({args.size()...});
    size_t iterations = 0;
    for (auto && tup : Zip(args...)) {
        ++iterations;
        std::apply(f, tup);
    }
    REQUIRE(size == iterations);
}

TEST_CASE("Zip empty container", "[zip]") {
    std::vector<int> v{};
    REQUIRE_NOTHROW(loop([](auto && a){ throw; }, v));
}

TEST_CASE("Zip same size", "[zip]") {
    std::array arr1{1, 2, 3};
    std::array arr2{1, 2, 3};
    int i = 0;
    loop([](auto && a, auto && b){ REQUIRE(a == b); }, arr1, arr2);
}

TEST_CASE("Zip with itself", "[zip]") {
    std::vector v{1, 2, 3};
    int i = 0;
    for (auto &&[a, b] : Zip(v, v)) {
        ++i;
        REQUIRE(a == b);
    }
    REQUIRE(i == v.size());
}

TEST_CASE("Zip different types", "[zip]") {
    std::vector v{1, 2, 3};
    std::vector arr{1, 2, 3};
    int i = 0;
    for (auto &&[a, b] : Zip(v, v)) {
        ++i;
        REQUIRE(a == b);
    }
    REQUIRE(i == v.size());
}

TEST_CASE("Enumerate", "[zip]") {
    std::vector v{0, 2, 4, 6, 8, 10};
    int i = 0;
    for (auto &&[item, index] : enumerate(v)) {
        REQUIRE(index == i);
        REQUIRE(item == index * 2);
        ++i;
    }
    REQUIRE(i == v.size());
}

TEST_CASE("Enumerate empty container", "[zip]") {
    std::vector<int> v{};
    REQUIRE_NOTHROW([&]() {
            for (auto &&[item, index] : enumerate(v)) { throw; }
    });
}

TEST_CASE("Zip a zip", "[zip]") {
    std::vector<int> v{0, 1, 2};
    for (auto &&[item, enumeration] : Zip(v, enumerate(v))) {
        auto & [other_item, i] = enumeration;
        REQUIRE(item == i);
    }
}

TEST_CASE("Benchmark zip vs raw", "[zip]") {
    std::array arr1{1, 2, 3};
    std::array arr2{1, 2, 3};
    auto size = std::min(arr1.size(), arr2.size());
    bool ret = false;
    BENCHMARK("raw loop") { 
        for (int i = 0; i < size; ++i) {
            ret = arr1[i] == arr2[i];
        }
        return ret;
    };
    BENCHMARK("zip loop") { 
        for (auto && [item1, item2] : Zip(arr1, arr2)) {
            ret = item1 == item2;
        }
        return ret;
    };
}
