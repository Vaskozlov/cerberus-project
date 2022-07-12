#include <cerberus/debug/debug.hpp>
#include <cerberus/template_string.hpp>

using namespace cerb::debug;
using namespace cerb::string_view_literals;

template<cerb::TemplateString String>
constexpr static auto testTemplateString(auto input) -> bool
{
    auto test_string = input;

    assertEqual(String.empty(), test_string.empty());
    assertEqual(String.size(), test_string.size());
    assertEqual(String.strView(), test_string);

    assertTrue(std::ranges::equal(String, test_string));
    assertTrue(std::equal(String.begin(), String.end(), test_string.begin()));

    return true;
}

static_assert(testTemplateString<"">(""_sv));
static_assert(testTemplateString<"Hello, World!">("Hello, World!"_sv));

static_assert(testTemplateString<u"">(u""_sv));
static_assert(testTemplateString<u"Hello, World!">(u"Hello, World!"_sv));
