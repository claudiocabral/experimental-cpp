#include <catch2/catch.hpp>
#include <zip/zip.h>

using namespace Akura;

TEST_CASE("Zip with itself", "[zip]") {
    std::vector v{1, 2, 3};
    int i = 0;
    for (auto &&[a, b] : Zip(v, v)) {
        ++i;
        REQUIRE(a == b);
    }
    REQUIRE(i == v.size());
}
