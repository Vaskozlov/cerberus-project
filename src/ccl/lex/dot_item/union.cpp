#include <ccl/lex/dot_item/union.hpp>

namespace ccl::lex::dot_item
{
    using namespace ccl::string_view_literals;

    Union::Union(TextIterator &rule_iterator_, size_t id_) : BasicItem(id_)
    {
        auto is_range = false;
        auto previous_chr = static_cast<char32_t>(0);
        auto &rule_iterator = rule_iterator_;
        auto begin_iterator_state = rule_iterator;

        checkUnionBegin(rule_iterator);

        while (true) {
            auto [is_escaping, chr] = rule_iterator.nextRawCharWithEscapingSymbols(
                { { U'[', U'[' }, { U']', U']' }, { U'-', U'-' } });

            checkForUnexpectedEnd(begin_iterator_state, is_escaping, chr);

            if (isUnionEnd(is_escaping, chr)) {
                break;
            }

            if (isRange(is_range, chr)) {
                is_range = true;
                continue;
            }

            addCharactersToTheBitset(is_range, previous_chr, chr);
            previous_chr = chr;
        }

        checkForClosedRange(rule_iterator, is_range);
    }

    auto Union::scanIteration(const ForkedGenerator &text_iterator) const -> size_t
    {
        auto next_carriage_value = text_iterator.getNextCarriageValue();

        if ((bitset.at(std::bit_cast<char8_t>(next_carriage_value)) ||
             bitset.at(text_iterator.futureChar(1))) ^
            isReversed()) {
            return utf8::size(next_carriage_value);
        }

        return 0;
    }

    CCL_INLINE auto Union::isRange(bool is_escaping, char32_t chr) noexcept -> bool
    {
        if (is_escaping) {
            return false;
        }

        return chr == U'-';
    }

    CCL_INLINE auto Union::isUnionEnd(bool is_escaping, char32_t chr) noexcept -> bool
    {
        return land(not is_escaping, chr == U']');
    }

    CCL_INLINE auto
        Union::addCharactersToTheBitset(bool &is_range, char32_t previous_chr, char32_t chr) -> void
    {
        if (is_range) {
            bitset.set(previous_chr, chr, true);
            is_range = false;
        } else {
            bitset.set(chr, true);
        }
    }

    CCL_INLINE auto
        Union::checkForUnexpectedEnd(TextIterator &rule_iterator, bool is_escaping, char32_t chr)
            -> void
    {
        if (land(not is_escaping, isEoF(chr))) {
            throwUnterminatedUnion(rule_iterator);
        }
    }

    CCL_INLINE auto Union::checkUnionBegin(TextIterator &rule_iterator) -> void
    {
        if (rule_iterator.getCurrentChar() != U'[') {
            throwUnionBeginException(rule_iterator);
        }
    }

    CCL_INLINE auto Union::checkForClosedRange(TextIterator &rule_iterator, bool is_ranged_opened)
        -> void
    {
        if (is_ranged_opened) {
            throwUnterminatedRangeException(rule_iterator);
        }
    }

    CCL_INLINE auto Union::throwUnterminatedUnion(TextIterator &rule_iterator) -> void
    {
        rule_iterator.throwPanicError(
            AnalysationStage::LEXICAL_ANALYSIS, "unterminated union item"_sv);
        throw UnrecoverableError{ "unrecoverable error in Union" };
    }

    CCL_INLINE auto Union::throwUnterminatedRangeException(TextIterator &rule_iterator) -> void
    {
        rule_iterator.throwPanicError(AnalysationStage::LEXICAL_ANALYSIS, "unterminated range"_sv);
        throw UnrecoverableError{ "unrecoverable error in Union" };
    }

    CCL_INLINE auto Union::throwUnionBeginException(TextIterator &rule_iterator) -> void
    {
        auto buffer = std::string{};
        utf8::appendUtf32ToUtf8Container(buffer, rule_iterator.getCurrentChar());

        auto message =
            fmt::format("expected `[` at the beginning of union item declaration, got {}", buffer);

        rule_iterator.throwPanicError(AnalysationStage::LEXICAL_ANALYSIS, message);
        throw UnrecoverableError{ "unrecoverable error in Union" };
    }
}// namespace ccl::lex::dot_item
