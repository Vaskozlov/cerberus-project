#ifndef CCL_PROJECT_LINE_TRACKER_HPP
#define CCL_PROJECT_LINE_TRACKER_HPP

#include <ccl/string_view.hpp>

namespace ccl::text
{
    class LineTracker
    {
    public:
        CCL_DECL auto get() const noexcept -> const string_view &
        {
            return line;
        }

        constexpr auto next(char32_t chr) noexcept -> void
        {
            if (newLinePassed) [[unlikely]] {
                updateLine();
                newLinePassed = false;
            }

            newLinePassed = chr == '\n';
        }

        constexpr explicit LineTracker(const string_view &text_) noexcept : text(text_)
        {
            auto new_line_index = text.find('\n');
            line = { text.begin(), *new_line_index.or_else(
                                       [this]() -> std::optional<size_t> { return text.size(); }) };
        }

    private:
        constexpr auto updateLine() -> void
        {
            const auto *new_line_begin = std::min(text.end(), line.end() + 1);
            const auto new_line_index = text.find('\n', new_line_begin);

            line = { new_line_begin,
                     text.begin() + *new_line_index.or_else([this]() -> std::optional<size_t> {
                         return text.size();
                     }) };
        }

        string_view text{};
        string_view line{};
        bool newLinePassed{ false };
    };
}// namespace ccl::text

#endif /* CCL_PROJECT_LINE_TRACKER_HPP */
