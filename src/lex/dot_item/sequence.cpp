#include <cerberus/lex/dot_item/sequence.hpp>

namespace cerb::lex::dot_item
{
    auto Sequence::empty() const noexcept -> bool
    {
        return string.empty();
    }

    Sequence::Sequence(
        bool multiline_, u8string_view str_token_, TextIterator &rule_iterator_,
        AnalysisShared &analysis_shared_)
      : BasicItem(analysis_shared_), str_token(str_token_), multiline(multiline_)
    {
        auto &rule_iterator = rule_iterator_;
        auto begin_iterator_state = rule_iterator;

        checkStringBegin(rule_iterator);
        skipStringDefinition(rule_iterator);

        while (true) {
            auto [is_escaping, chr] = rule_iterator.nextRawCharWithEscapingSymbols();

            checkForUnexpectedEnd(begin_iterator_state, is_escaping, chr);

            if (isStringEnd(rule_iterator, is_escaping)) {
                break;
            }

            utf8::appendUtf32ToUtf8Container(string, chr);
        }

        addWarningIfEmpty(rule_iterator);
    }

    auto Sequence::scanIteration(TextIterator &text_iterator) const -> bool
    {
        auto future_text = text_iterator.getRemainingFutureAfterSymbols(1);

        if (future_text.substr(0, string.size()) == string) {
            text_iterator.rawSkip(string.size());
            return true;
        }

        return false;
    }

    auto Sequence::isStringEnd(TextIterator &rule_iterator, bool is_escaping) const -> bool
    {
        if (is_escaping) {
            return false;
        }

        auto text = rule_iterator.getRemaining();
        return text.substr(0, str_token.size()) == str_token;
    }

    auto Sequence::checkForUnexpectedEnd(
        TextIterator &rule_iterator, bool is_escaping, char32_t chr) const -> void
    {
        using namespace std::string_view_literals;

        if (is_escaping) {
            return;
        }

        if (isEoF(chr)) {
            throwUnterminatedString(rule_iterator, u8"unterminated sequence");
        }

        if (land(chr == '\n', not multiline)) {
            auto message = u8"new line is reached, but sequence has not been terminated"sv;
            auto suggestion =
                fmt::format<u8"use multiline sequence or close it with `{}`">(str_token);

            throwUnterminatedString(rule_iterator, message, suggestion);
        }
    }

    auto Sequence::skipStringDefinition(TextIterator &rule_iterator) const -> void
    {
        rule_iterator.rawSkip(str_token.size() - 1);
    }

    auto Sequence::checkStringBegin(TextIterator &rule_iterator) const -> void
    {
        auto text = rule_iterator.getRemaining();

        if (str_token.empty()) {
            throwEmptyStringBegin(rule_iterator);
        }

        if (text.substr(0, str_token.size()) != str_token) {
            throwStringBeginException(rule_iterator);
        }
    }

    auto Sequence::addWarningIfEmpty(TextIterator &rule_iterator) -> void
    {
        using namespace std::string_view_literals;

        if (string.empty()) {
            rule_iterator.template throwWarning<SequenceException>(
                u8"empty string should not be used"sv);
        }
    }

    auto Sequence::throwEmptyStringBegin(TextIterator &rule_iterator) -> void
    {
        using namespace std::string_view_literals;

        rule_iterator.throwException<SequenceException>(u8"sequence item begin cannot be empty"sv);
        throw UnrecoverableError{ "unreachable error in SequenceType" };
    }

    auto Sequence::throwUnterminatedString(
        TextIterator &rule_iterator,
        u8string_view message,
        u8string_view suggestion) -> void
    {
        rule_iterator.template throwException<SequenceException>(message, suggestion);
        throw UnrecoverableError{ "unreachable error in SequenceType" };
    }

    auto Sequence::throwStringBeginException(TextIterator &rule_iterator) const -> void
    {
        auto message = fmt::format<u8"string literal must begin with {}">(str_token);

        rule_iterator.template throwException<SequenceException>(message);
        throw UnrecoverableError{ "unreachable error in SequenceType" };
    }
}// namespace cerb::lex::dot_item
