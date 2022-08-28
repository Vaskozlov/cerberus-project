#ifndef CCL_PROJECT_LEXICAL_ANALYZER_HPP
#define CCL_PROJECT_LEXICAL_ANALYZER_HPP

#include <ccl/lex/dot_item/container.hpp>
#include <set>

namespace ccl::lex
{
    class LexicalAnalyzer
    {
    public:
        using Container = dot_item::Container;
        using BasicItem = dot_item::BasicItem;
        using TextIterator = typename BasicItem::TextIterator;
        using CommentTokens = typename BasicItem::CommentTokens;
        using ExceptionAccumulator = typename BasicItem::ExceptionAccumulator;

        struct Tokenizer
        {
            Tokenizer(
                LexicalAnalyzer &lexical_analyzer_, u8string_view text,
                u8string_view filename_ = {})
              : text_iterator(
                    text, &exception_accumulator, lexical_analyzer_.shared.comment_tokens,
                    filename_),
                lexical_analyzer(lexical_analyzer_)
            {}

            Tokenizer(
                LexicalAnalyzer &lexical_analyzer_, u8string_view text, u8string_view filename_,
                ExceptionAccumulator *exception_accumulator_)
              : text_iterator(
                    text, exception_accumulator_, lexical_analyzer_.shared.comment_tokens,
                    filename_),
                lexical_analyzer(lexical_analyzer_)
            {}

            [[nodiscard]] auto getErrors() -> decltype(auto)
            {
                return exception_accumulator.getErrors();
            }

            [[nodiscard]] auto getWarnings() -> decltype(auto)
            {
                return exception_accumulator.getWarnings();
            }

            template<Exception T, typename... Args>
            auto throwError(Args &&...args) -> void
            {
                text_iterator.throwException<T>(std::forward<Args>(args)...);
            }

            template<Exception T, typename... Args>
            auto throwWarning(Args &&...args) -> void
            {
                text_iterator.throwWarning<T>(std::forward<Args>(args)...);
            }

            [[nodiscard]] auto yield() -> Token;
            [[nodiscard]] auto constructBadToken() -> Token;

        private:
            TextIterator text_iterator;
            ExceptionAccumulator exception_accumulator{};
            const LexicalAnalyzer &lexical_analyzer;
        };

        LexicalAnalyzer(
            const std::initializer_list<std::pair<size_t, u8string_view>> &rules_,
            u8string_view filename = {}, const CommentTokens &comment_tokens_ = { u8"#" });

        [[nodiscard]] auto getTokenizer(u8string_view text, u8string_view filename = {})
            -> Tokenizer
        {
            return { *this, text, filename };
        }

        [[nodiscard]] auto getTokenizer(
            u8string_view text, u8string_view filename, ExceptionAccumulator *accumulator)
            -> Tokenizer
        {
            return { *this, text, filename, accumulator };
        }

        [[nodiscard]] auto getExceptionAccumulator() noexcept -> ExceptionAccumulator &
        {
            return exception_accumulator;
        }

        [[nodiscard]] auto getExceptionAccumulator() const noexcept -> const ExceptionAccumulator &
        {
            return exception_accumulator;
        }

    private:
        auto createContainer(
            u8string_view rule, size_t id, const CommentTokens &comment_tokens,
            u8string_view filename) -> void;

        std::set<Container> items;
        AnalysisShared shared{};
        ExceptionAccumulator exception_accumulator{};
        size_t errors{};
    };
}// namespace ccl::lex

#endif /* CCL_PROJECT_LEXICAL_ANALYZER_HPP */
