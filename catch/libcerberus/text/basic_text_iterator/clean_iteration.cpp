#include <cerberus/debug/debug.hpp>
#include <cerberus/text/basic_text_iterator.hpp>

using namespace cerb::text;
using namespace cerb::debug;
using namespace std::string_view_literals;
using namespace cerb::string_view_literals;

constexpr auto testCleanIteration(auto input, auto expected) -> bool
{
    auto text_iterator = BasicTextIterator{ input };

    for (const auto &chr : expected) {
        assertEqual(chr, text_iterator.nextCleanChar());
        assertEqual(chr, text_iterator.getCurrentChar());
    }

    return true;
}

static_assert(testCleanIteration("Hello, \t\nWorld!"_sv, "Hello,World!"_sv));
static_assert(testCleanIteration(u"Hello, \t\nWorld!"_sv, u"Hello,World!"_sv));
