#ifndef CERBERUS_PROJECT_TEMPLATE_STRING_HPP
#define CERBERUS_PROJECT_TEMPLATE_STRING_HPP

#include <algorithm>
#include <cerberus/cerberus.hpp>

namespace cerb
{
    template<size_t N, CharacterLiteral CharT>
    struct TemplateString
    {
        using storage_t = std::array<CharT, N>;
        using const_iterator = typename storage_t::const_iterator;
        using const_reverse_iterator = typename storage_t::const_reverse_iterator;

        CERBLIB_DECL auto begin() const -> const_iterator
        {
            return string.begin();
        }

        CERBLIB_DECL auto end() const -> const_iterator
        {
            return string.begin() + size();
        }

        CERBLIB_DECL auto cbegin() const -> const_iterator
        {
            return begin();
        }

        CERBLIB_DECL auto cend() const -> const_iterator
        {
            return end();
        }

        CERBLIB_DECL auto rbegin() const -> const_reverse_iterator
        {
            return const_reverse_iterator{ end() };
        }

        CERBLIB_DECL auto rend() const -> const_reverse_iterator
        {
            return const_reverse_iterator{ begin() };
        }

        CERBLIB_DECL auto crbegin() const -> const_reverse_iterator
        {
            return rbegin();
        }

        CERBLIB_DECL auto crend() const -> const_reverse_iterator
        {
            return rend();
        }

        CERBLIB_DECL auto size() const -> size_t
        {
            return string.size() - 1;
        }

        CERBLIB_DECL auto empty() const -> bool
        {
            return size() == 0;
        }

        CERBLIB_DECL auto strView() const -> std::basic_string_view<CharT>
        {
            return { string.data(), size() };
        }

        CERBLIB_DECL auto at(size_t index) const -> CharT
        {
            return string.at(index);
        }

        CERBLIB_DECL auto operator[](size_t index) const -> CharT
        {
            return string[index];
        }

        CERBLIB_DECL auto operator<=>(TemplateString const &other) const
            -> std::strong_ordering = default;

        // NOLINTNEXTLINE
        consteval TemplateString(const CharT (&str)[N])
        {
            std::copy_n(str, N, string.begin());
        }

        storage_t string{};
    };
}// namespace cerb

#endif /* CERBERUS_PROJECT_TEMPLATE_STRING_HPP */