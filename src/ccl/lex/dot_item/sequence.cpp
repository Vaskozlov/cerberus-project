#include <ccl/lex/dot_item/sequence.hpp>

namespace ccl::lex::dot_item
{
    using namespace ccl::string_view_literals;

    Sequence::Sequence(
        SequenceFlags sequence_flags, string_view sequence_begin_and_end,
        TextIterator &rule_iterator, Id item_id)
      : Sequence{
            sequence_flags, sequence_begin_and_end, sequence_begin_and_end, rule_iterator, item_id}
    {}

    Sequence::Sequence(
        SequenceFlags sequence_flags, string_view sequence_starter,
        string_view sequence_ender, TextIterator &rule_iterator, Id item_id)
      : DotItemConcept{item_id, Flags{
                               .sequenceIsMultiline=sequence_flags.multiline,
                               .noEscapingSymbols=sequence_flags.noEscaping
                           }}
      , starter{sequence_starter}
      , ender{sequence_ender}
    {
        static constexpr StaticFlatmap<char32_t, char32_t, 2> special_symbols_for_sequence{
            {U'[', U'['}, {U']', U']'}};

        auto begin_iterator_state = rule_iterator;

        checkSequenceArguments(rule_iterator);
        skipStringDefinition(rule_iterator);

        while (true) {
            auto chr = U'\0';
            auto is_escaping = false;

            if (getFlags().noEscapingSymbols) {
                chr = rule_iterator.advance();
            } else {
                auto [escaping, character] = rule_iterator.nextCharWithEscapingSymbols(std::span{
                    special_symbols_for_sequence.begin(), special_symbols_for_sequence.end()});
                is_escaping = escaping;
                chr = character;
            }

            checkForUnexpectedEnd(begin_iterator_state, is_escaping, chr);

            if (isStringEnd(rule_iterator, is_escaping)) {
                rule_iterator.skip(ender.size() - 1);
                break;
            }

            utf8::appendUtf32ToUtf8Container(sequenceValue, chr);
        }
    }

    auto Sequence::scanIteration(const ForkedGenerator &text_iterator) const -> Optional<size_t>
    {
        string_view future_text = text_iterator.getRemainingText();

        if (future_text.startsWith(sequenceValue) != isReversed()) [[unlikely]] {
            return isReversed() ? utf8::size(future_text[0]) : sequenceValue.size();
        }

        return std::nullopt;
    }

    CCL_INLINE auto Sequence::isStringEnd(const TextIterator &rule_iterator, bool is_escaping) const
        -> bool
    {
        if (is_escaping) {
            return false;
        }

        auto text = rule_iterator.getRemainingWithCurrent();
        return text.startsWith(as<string_view>(ender));
    }

    CCL_INLINE auto Sequence::checkForUnexpectedEnd(
        TextIterator &rule_iterator,
        bool is_escaping,
        char32_t chr) const -> void
    {
        if (is_escaping) {
            return;
        }

        if (isEoF(chr)) {
            throwUnterminatedString(rule_iterator, "unterminated sequence");
        }

        if ('\n' == chr && !getFlags().sequenceIsMultiline) [[unlikely]] {
            constexpr auto message = "new line is reached, but sequence has not been terminated"_sv;
            auto suggestion = fmt::format("use multiline sequence or close it with `{}`", ender);

            throwUnterminatedString(rule_iterator, message, suggestion);
        }
    }

    CCL_INLINE auto Sequence::skipStringDefinition(TextIterator &rule_iterator) const -> void
    {
        rule_iterator.skip(starter.size() - 1);
    }

    CCL_INLINE auto Sequence::checkSequenceArguments(TextIterator &rule_iterator) const -> void
    {
        auto text = rule_iterator.getRemainingWithCurrent();

        if (starter.empty()) [[unlikely]] {
            throwEmptyStringBegin(rule_iterator);
        }

        if (ender.empty()) [[unlikely]] {
            throwEmptyStringEnd(rule_iterator);
        }

        if (!text.startsWith(starter)) [[unlikely]] {
            throwStringBeginException(rule_iterator);
        }
    }

    CCL_INLINE auto Sequence::throwEmptyStringBegin(TextIterator &rule_iterator) -> void
    {
        rule_iterator.throwPanicError(
            AnalysisStage::LEXICAL_ANALYSIS, "sequence item begin cannot be empty"_sv);
        throw UnrecoverableError{"unrecoverable error in SequenceType"};
    }

    CCL_INLINE auto Sequence::throwEmptyStringEnd(TextIterator &rule_iterator) -> void
    {
        rule_iterator.throwPanicError(
            AnalysisStage::LEXICAL_ANALYSIS, "sequence item end cannot be empty"_sv);
        throw UnrecoverableError{"unrecoverable error in SequenceType"};
    }

    CCL_INLINE auto Sequence::throwUnterminatedString(
        TextIterator &rule_iterator,
        string_view message,
        string_view suggestion) -> void
    {
        rule_iterator.throwPanicError(AnalysisStage::LEXICAL_ANALYSIS, message, suggestion);
        throw UnrecoverableError{"unrecoverable error in SequenceType"};
    }

    CCL_INLINE auto Sequence::throwStringBeginException(TextIterator &rule_iterator) const -> void
    {
        auto message = fmt::format("string literal must begin with {}", starter);

        rule_iterator.throwPanicError(AnalysisStage::LEXICAL_ANALYSIS, message);
        throw UnrecoverableError{"unrecoverable error in SequenceType"};
    }
}// namespace ccl::lex::dot_item
