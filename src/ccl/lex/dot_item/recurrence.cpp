#include <ccl/lex/dot_item/recurrence.hpp>

namespace ccl::lex::dot_item
{
    using namespace text;
    using namespace ccl::string_view_literals;

    Recurrence::Recurrence(TextIterator &text_iterator)
    {
        checkRangeStart(text_iterator);

        auto iterator_copy = text_iterator;

        from = parseNumber(text_iterator, U',');// NOLINT initialization via =
        to = parseNumber(text_iterator, U'}');  // NOLINT

        checkCorrectnessOfValues(iterator_copy);
    }

    auto Recurrence::parseNumber(TextIterator &text_iterator, char32_t terminator) -> size_t
    {
        auto result = static_cast<size_t>(0);
        constexpr auto decimal_base = static_cast<size_t>(10);
        text_iterator.moveToCleanChar();

        while (text_iterator.next() != terminator) {
            auto chr = text_iterator.getCurrentChar();

            if (isDigit(chr)) {
                result = result * decimal_base + static_cast<size_t>(chr - U'0');
                continue;
            }

            throwUnexpectedCharacter(text_iterator, chr);
        }

        return result;
    }

    auto Recurrence::checkRangeStart(TextIterator &text_iterator) -> void
    {
        if (text_iterator.getCurrentChar() != U'{') {
            throwRangeBeginException(text_iterator);
        }
    }

    auto Recurrence::checkCorrectnessOfValues(TextIterator &text_iterator) const -> void
    {
        if (from > to) {
            throwBadValues(text_iterator);
        }
    }

    auto Recurrence::throwBadValues(TextIterator &text_iterator) const -> void
    {
        auto message = fmt::format(
            "the beginning of the recurrence ({}) is greater than the end "
            "({})",
            from, to);

        text_iterator.throwCriticalError(message);
    }

    auto Recurrence::throwUnexpectedCharacter(TextIterator &text_iterator, char32_t chr) -> void
    {
        auto buffer = std::string{};
        utf8::appendUtf32ToUtf8Container(buffer, chr);

        auto message = fmt::format("expected a number, but found `{}`", buffer);

        text_iterator.throwPanicError(message);
        throw UnrecoverableError{ "unrecoverable error in Recurrence" };
    }

    auto Recurrence::throwRangeBeginException(TextIterator &text_iterator) -> void
    {
        text_iterator.throwPanicError("expected '{' at the beginning of recurrence range"_sv);
        throw UnrecoverableError{ "unrecoverable error in Recurrence" };
    }
}// namespace ccl::lex::dot_item