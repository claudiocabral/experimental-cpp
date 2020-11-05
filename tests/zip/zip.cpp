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

TEST_CASE("Zip same size", "[zip]") {
    std::array arr1{1, 2, 3};
    std::array arr2{1, 2, 3};
    int i = 0;
    loop([](auto && a, auto && b){
            REQUIRE(a == b);
            },
            arr1, arr2);
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
