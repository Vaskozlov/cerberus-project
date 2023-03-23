#include <ccl/debug/debug_file.hpp>
#include <ccl/text/location.hpp>

using namespace ccl;
using namespace text;

TEST_CASE("LocationTracking", "[Location]")
{
    auto location = Location{"none"};

    REQUIRE(location.getLine() == 1U);
    REQUIRE(location.getColumn() == 0U);
    REQUIRE(location.getRealColumn() == 0U);
    REQUIRE(location.getFilename() == "none");

    location.intermediateNext('a');
    location.next(U'a');

    REQUIRE(location.getLine() == 1U);
    REQUIRE(location.getColumn() == 1U);
    REQUIRE(location.getRealColumn() == 1U);

    location.intermediateNext('\n');
    location.next(U'\n');

    REQUIRE(location.getLine() == 2U);
    REQUIRE(location.getColumn() == 0U);
    REQUIRE(location.getRealColumn() == 0U);

    location.intermediateNext(as<char>(0b1100'0010));// NOLINT utf8 2 byte character

    REQUIRE(location.getLine() == 2U);
    REQUIRE(location.getColumn() == 0U);
    REQUIRE(location.getRealColumn() == 1U);

    location.intermediateNext(as<char>(0b1000'0000));// NOLINT utf8 2 byte character end
    location.next(U'a');                             // just some character

    REQUIRE(location.getLine() == 2U);
    REQUIRE(location.getColumn() == 1U);
    REQUIRE(location.getRealColumn() == 2U);
}
