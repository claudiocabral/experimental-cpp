#include <catch2/catch.hpp>
#include <akura/match.h>

using namespace akura;

TEST_CASE("Match number", "[match]") {
    std::string_view a = match(7,
            0, "no",
            7, "first match",
            7, "ops"
            );
    //std::string_view a = match(7,
    //        [](auto x) { return x < 3; }, [](auto x) { (void)x; return "no"; },
    //        [](auto x) { return x > 3; }, [](auto x) { (void)x; return "first match"; },
    //        [](auto x) { return x == 7; }, [](auto x) { (void)x; return "ops"; }
    //        );
    REQUIRE(a == "first match");
}

TEST_CASE("Match predicate lambda", "[match]") {
    std::string_view a = match(7,
            [](auto x) { return x < 3; }, "no",
            [](auto x) { return x > 3; }, "first match",
            [](auto x) { return x == 7; }, "ops"
            );
    REQUIRE(a == "first match");
}

TEST_CASE("Match mix predicate", "[match]") {
    std::string_view a = match(7,
            [](auto x) { return x < 3; }, "no",
            [](auto x) { return x > 3; }, "first match",
            7, "ops"
            );
    REQUIRE(a == "first match");
}

TEST_CASE("Match return function", "[match]") {
    std::string_view a = match(7,
            0, [](auto && x) { (void)x; return "no"; },
            7, [](auto && x) { (void)x; return "first match"; },
            7, [](auto && x) { (void)x; return "ops"; }
    );
    REQUIRE(a == "first match");
}

TEST_CASE("Match mix predicate return function", "[match]") {
    std::string_view a = match(7,
            0, [](auto && x) { (void)x; return "no"; },
            [](auto x) { return x > 3; }, [](auto && x) { (void)x; return "first match"; },
            7, [](auto && x) { (void)x; return "ops"; }
    );
    REQUIRE(a == "first match");
}

TEST_CASE("Match mixed return", "[match]") {
    std::string_view a = match(7,
            0, [](auto && x) { (void)x; return "no"; },
            7, "first match",
            7, [](auto && x) { (void)x; return "ops"; }
    );
    REQUIRE(a == "first match");
}

TEST_CASE("No matches", "[match]") {
    std::string_view a = match(7,
            0, [](auto && x) { (void)x; return "no"; },
            3, "wrong",
            5, [](auto && x) { (void)x; return "wrong again"; },
            "no matches"
    );
    REQUIRE(a == "no matches");
}

TEST_CASE("No matches default function", "[match]") {
    std::string_view a = match(7,
            0, [](auto && x) { (void)x; return "no"; },
            3, "wrong",
            5, [](auto && x) { (void)x; return "wrong again"; },
            [](auto && x) { (void)x; return "no matches"; }
    );
    REQUIRE(a == "no matches");
}

TEST_CASE("No matches no default ", "[match]") {
    std::string_view a = match(7,
            0, [](auto && x) { (void)x; return "no"; },
            3, "wrong",
            5, [](auto && x) { (void)x; return "wrong again"; }
    );
    REQUIRE(a == "wrong again");
}

TEST_CASE("Integers", "[match]") {
    auto v = akura::match(5,
        1, 1,
        2, 2,
        3, 3,
        4, 4,
        5, 5,
        [](auto a) { return a == 32;}, 32,
        7
    );
    REQUIRE(v == 5);
}
