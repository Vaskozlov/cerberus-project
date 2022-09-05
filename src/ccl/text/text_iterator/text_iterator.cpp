#include <ccl/text/text_iterator.hpp>

namespace ccl::text
{
    auto TextIterator::onMove(char chr) -> void
    {
        location.intermediateNext(chr);
    }

    auto TextIterator::onCharacter(char32_t chr) -> void
    {
        location.next(chr);
        ts_tracker.next(chr);
        line_tracker.next(chr);
    }

    auto TextIterator::utfError(char /* chr */) -> void
    {
        throwPanicError("invalid utf symbol");
        throw UnrecoverableError{ "unable to recover, because of invalid utf symbol" };
    }

    auto TextIterator::nextRawCharWithEscapingSymbols(
        const TextIterator::extra_symbols_t &extra_symbols) -> Pair<bool, char32_t>
    {
        auto escaping = false;
        auto chr = next();

        if (chr == U'\\') {
            escaping = true;
            chr = doEscapeSymbolizing(*this, extra_symbols);
        }

        Base::setCurrentChar(chr);
        return { escaping, chr };
    }

    auto TextIterator::doEscapeSymbolizing(
        TextIterator &text_iterator, TextIterator::extra_symbols_t const &extra_symbols_)
        -> char32_t
    {
        return EscapingSymbolizer(text_iterator, extra_symbols_).matchNextChar();
    }

    auto TextIterator::calculateNotationEscapeSymbol(
        TextIterator &text_iterator, u16 max_times, u16 notation_power, bool need_all_chars)
        -> char32_t
    {
        return NotationEscapingSymbolizer(text_iterator, max_times, notation_power, need_all_chars)
            .get();
    }

    auto TextIterator::throwToHandle(
        const TextIterator &iterator_location, ExceptionCriticality criticality,
        const string_view &message, const string_view &suggestion) -> void
    {
        auto exception = TextIteratorException(
            criticality, iterator_location.getLocation(), iterator_location.getWorkingLine(),
            message, suggestion);

        if (exception_handler == nullptr) {
            throw std::move(exception);
        }

        exception_handler->handle(exception);
    }
}// namespace ccl::text
