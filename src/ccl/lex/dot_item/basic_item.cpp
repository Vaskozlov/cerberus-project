#include <ccl/lex/dot_item/basic_item.hpp>

using namespace ccl::integral_literals;

namespace ccl::lex::dot_item
{
    [[nodiscard]] static auto isNotEOI(const text::TextIterator &text_iterator) -> bool
    {
        return not text_iterator.isEOI();
    }

    auto BasicItem::alwaysRecognizedSuggestion(TextIterator &text_iterator, bool condition) -> void
    {
        if (condition) {
            text_iterator.throwSuggestion("item will be always recognized", "delete it");
        }
    }

    auto BasicItem::neverRecognizedSuggestion(TextIterator &text_iterator, bool condition) -> void
    {
        if (condition) {
            text_iterator.throwSuggestion("item will never be recognized");
        }
    }

    auto BasicItem::scan(const TextIterator &text_iterator, const Token &token, ScanType scan_type)
        const -> std::optional<std::pair<TextIterator, Token>>
    {
        auto times = 0_ZU;
        auto local_token = token;
        auto local_iterator = text_iterator;

        while (times < recurrence.to) {
            if (isNotEOI(local_iterator) && callScanIteration(local_iterator)) {
                ++times;
                continue;
            }

            break;
        }

        if (isSuccessfullyScanned(local_iterator, times, scan_type)) {
            if (times != 0) {
                modifyToken(text_iterator, local_iterator, local_token);
            }

            return std::optional<std::pair<TextIterator, Token>>(
                std::in_place, local_iterator, local_token);
        }

        return std::nullopt;
    }

    auto BasicItem::callScanIteration(TextIterator &local_iterator) const
        -> bool
    {
        auto iterator_copy = local_iterator;

        if (scanIteration(iterator_copy) ^ reversed) {
            local_iterator = std::move(iterator_copy);
            return true;
        }

        return false;
    }

    auto BasicItem::isSuccessfullyScanned(
        const TextIterator &text_iterator, size_t times, ScanType scan_type) const -> bool
    {
        auto is_next_value_special = false;

        if (scan_type == ScanType::MAIN) {
            auto iterator_copy = text_iterator;
            iterator_copy.next();

            is_next_value_special = special_items.scan(text_iterator).has_value();
        }

        bool is_not_main_scan = scan_type != ScanType::MAIN;

        return recurrence.inRange(times) && (is_not_main_scan || is_next_value_special ||// TODO
                                             isLayoutOrEoF(text_iterator.getNextCarriageValue()));
    }

    auto BasicItem::modifyToken(
        const TextIterator &before_scan_iterator, const TextIterator &after_scan_iterator,
        Token &token) const -> void
    {
        const auto *new_token_end = after_scan_iterator.getRemainingAsCarriage();
        token.setEnd(new_token_end);

        if (hasPrefix()) {
            token.addPrefix({ before_scan_iterator.getRemainingAsCarriage(), new_token_end });
        } else if (hasPostfix()) {
            token.addPostfix({ before_scan_iterator.getRemainingAsCarriage(), new_token_end });
        }
    }
}// namespace ccl::lex::dot_item
