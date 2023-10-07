#ifndef CCL_PROJECT_UTF_SET_HPP
#define CCL_PROJECT_UTF_SET_HPP

#include <ccl/ccl.hpp>

namespace ccl
{
    class UtfSet
    {
        static constexpr auto smallStorageSize = as<size_t>(128);

        SmallBitset<smallStorageSize> smallStorage{};
        std::unordered_set<char32_t> storage{};

    public:
        UtfSet() = default;

        [[nodiscard]] auto empty() const noexcept -> bool;
        [[nodiscard]] auto at(char32_t chr) const noexcept -> bool;

        auto set(char32_t chr, bool value = true) -> void;
        auto set(Range<char32_t> range, bool value = true) -> void;

    private:
        auto setBigChar(char32_t chr, bool value) -> void;
    };
}// namespace ccl

#endif /* CCL_PROJECT_UTF_SET_HPP */
