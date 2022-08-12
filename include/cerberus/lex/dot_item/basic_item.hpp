#ifndef CERBERUS_PROJECT_BASIC_ITEM_HPP
#define CERBERUS_PROJECT_BASIC_ITEM_HPP

#include <cerberus/lex/analysis_shared.hpp>
#include <cerberus/lex/dot_item/repetition.hpp>
#include <cerberus/text/text_iterator.hpp>

namespace cerb::lex::dot_item
{
    class BasicItem
    {
    public:
        using TextIterator = text::TextIterator;
        using CommentTokens = text::CommentTokens;
        using ExceptionAccumulator = analysis::ExceptionAccumulator<text::TextIteratorException>;

        enum struct ScanStatus : bool
        {
            FAILURE = false,
            SUCCESS = true
        };

        [[nodiscard]] auto getRepetition() const noexcept -> Repetition
        {
            return repetition;
        }

        [[nodiscard]] auto isReversed() const noexcept -> bool
        {
            return reversed;
        }

        auto reverse() noexcept -> void
        {
            reversed = !reversed;
        }

        auto setRepetition(Repetition new_repetition) noexcept -> void
        {
            repetition = new_repetition;
        }

        [[nodiscard]] auto canBeOptimized() const noexcept -> bool
        {
            return repetition.from == 0 && empty();
        }

        [[nodiscard]] virtual auto empty() const noexcept -> bool = 0;
        [[nodiscard]] auto isNextCharNotForScanning(const TextIterator &text_iterator) const
            -> bool;
        [[nodiscard]] auto scan(const TextIterator &text_iterator, bool main_scan = false) const
            -> std::pair<bool, TextIterator>;

    private:
        [[nodiscard]] virtual auto scanIteration(TextIterator &text_iterator) const -> bool = 0;
        [[nodiscard]] auto
            computeScanResult(const TextIterator &text_iterator, size_t times, bool main_scan) const
            -> bool;

    public:
        auto operator=(const BasicItem &) -> BasicItem & = delete;
        auto operator=(BasicItem &&) noexcept -> BasicItem & = delete;

        explicit BasicItem(AnalysisShared &analysis_shared_) : analysis_shared{ analysis_shared_ }
        {}

        BasicItem(BasicItem &&) noexcept = default;
        BasicItem(const BasicItem &) = default;

        virtual ~BasicItem() = default;

    protected:
        Repetition repetition{ Repetition::basic() };
        AnalysisShared &analysis_shared;
        bool reversed{ false };
    };
}// namespace cerb::lex::dot_item

#endif /* CERBERUS_PROJECT_BASIC_ITEM_HPP */
