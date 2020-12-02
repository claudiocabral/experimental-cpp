#ifdef CATCH_CONFIG_ENABLE_BENCHMARKING

#include <catch2/catch.hpp>
#include <zip/zip.h>

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
    for (auto &&[item1, item2] : Zip(arr1, arr2)) {
      ret = item1 == item2;
    }
    return ret;
  };
}

#endif
