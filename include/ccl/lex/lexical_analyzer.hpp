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

        struct CCL_TRIVIAL_ABI Rule
        {
            size_t id{};
            string_view repr{};
        };

        struct Tokenizer
        {
            Tokenizer(
                LexicalAnalyzer &lexical_analyzer_, string_view text, string_view filename_ = {})
              : lexical_analyzer(lexical_analyzer_),
                text_iterator(text, lexical_analyzer_.exception_handler, filename_)
            {}

            Tokenizer(
                LexicalAnalyzer &lexical_analyzer_, string_view text, string_view filename_,
                ExceptionHandler &exception_handler_)
              : lexical_analyzer(lexical_analyzer_),
                text_iterator(text, exception_handler_, filename_)
            {}

            [[nodiscard]] auto getIterator() const -> const TextIterator &
            {
                return text_iterator;
            }

            [[nodiscard]] auto getHandler() -> ExceptionHandler &
            {
                return text_iterator.getHandler();
            }

            auto throwException(
                ExceptionCriticality criticality, string_view message, string_view suggestion = {})
                -> void
            {
                text_iterator.throwToHandle(text_iterator, criticality, message, suggestion);
            }

            [[nodiscard]] auto yield() -> Token;

        private:
            [[nodiscard]] auto shouldIgnoreToken(const Token &token) const -> bool;

            [[nodiscard]] auto constructBadToken() -> Token;
            [[nodiscard]] auto constructEOIToken() -> Token;

            LexicalAnalyzer &lexical_analyzer;
            TextIterator text_iterator;
        };

        LexicalAnalyzer(
            ExceptionHandler &exception_handler_, const std::initializer_list<Rule> &rules_,
            string_view filename = {}, std::basic_string<size_t> ignored_ids_ = {});

        [[nodiscard]] auto getIgnoredIds() const -> const std::basic_string<size_t> &
        {
            return ignored_ids;
        }

        [[nodiscard]] auto getTokenizer(string_view text, string_view filename = {}) -> Tokenizer
        {
            return { *this, text, filename };
        }

        [[nodiscard]] auto
            getTokenizer(string_view text, string_view filename, ExceptionHandler &handler)
                -> Tokenizer
        {
            return { *this, text, filename, handler };
        }

    private:
        auto createContainer(string_view rule, size_t id, string_view filename) -> void;

        std::set<Container> items;
        SpecialItems special_items{};
        std::basic_string<size_t> ignored_ids{};
        ExceptionHandler &exception_handler;
        size_t errors{};
    };
}// namespace ccl::lex

#endif /* CCL_PROJECT_LEXICAL_ANALYZER_HPP */