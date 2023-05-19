#include <ccl/debug/debug_file.hpp>
#include <ccl/flatmap.hpp>

using namespace ccl;
using namespace std::string_view_literals;

// NOLINTBEGIN

TEST_CASE("FlatmapOverflow", "[Flatmap]")
{
    auto flatmap = StaticFlatmap<int, int, 10>{{10, 20}, {20, 30}, {30, 40}, {30, 40},  {40, 50},
                                               {60, 70}, {70, 80}, {80, 80}, {90, 100}, {100, 110}};

    REQUIRE(flatmap.size() == flatmap.capacity());
    REQUIRE_THROWS_AS(flatmap.tryEmplace(110, 120), std::out_of_range);
}

// NOLINTEND
