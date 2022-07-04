#include <cerberus/debug/debug.hpp>
#include <cerberus/text/basic_text_iterator.hpp>

using namespace cerb::text;
using namespace std::string_view_literals;

// NOLINTBEGIN

DEBUG_SCOPE
{
    constexpr auto input_string = "Hello, \t\nWorld!"sv;
    constexpr auto expected_string = "Hello,World!"sv;

    auto text_iterator = BasicTextIterator<char>{ input_string };

    for (auto &chr : expected_string) {
        ASSERT_EQ(chr, text_iterator.nextCleanChar());
        ASSERT_EQ(chr, text_iterator.getCurrent());
    }

    return {};
}

// NOLINTEND