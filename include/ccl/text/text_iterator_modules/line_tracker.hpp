#ifndef CCL_PROJECT_LINE_TRACKER_HPP
#define CCL_PROJECT_LINE_TRACKER_HPP

#include <ccl/string_view.hpp>

namespace ccl::text
{
    class LineTracker
    {
    private:
        string_view text{};
        string_view line{};
        bool newLinePassed{false};

    public:
        [[nodiscard]] constexpr explicit LineTracker(string_view text_to_track) noexcept
          : text{text_to_track}
        {
            const auto new_line_index = text.find('\n');
            const auto line_end_index = new_line_index.value_or(text.size());

            line = {text.begin(), line_end_index};
        }

        CCL_DECL auto get() const noexcept -> const string_view &
        {
            return line;
        }

        constexpr auto next(const char32_t chr) noexcept -> void
        {
            if (newLinePassed) [[unlikely]] {
                updateLine();
                newLinePassed = false;
            }

            newLinePassed = '\n' == chr;
        }

    private:
        constexpr auto updateLine() -> void
        {
            const auto *new_line_begin = std::min(text.end(), line.end() + 1);
            const auto new_line_index = text.find('\n', new_line_begin);
            const auto *line_end = text.begin() + new_line_index.value_or(text.size());

            line = {new_line_begin, std::min(text.end(), line_end)};
        }
    };
}// namespace ccl::text

#endif /* CCL_PROJECT_LINE_TRACKER_HPP */
