#ifndef CCL_PROJECT_TOKEN_HPP
#define CCL_PROJECT_TOKEN_HPP

#include <ccl/text/text_iterator.hpp>
#include <optional>
#include <string>
#include <utility>
#include <vector>

namespace ccl::lex
{
    // NOLINTNEXTLINE
    CCL_ENUM(ReservedTokenType, size_t, EOI, BAD_TOKEN);

    struct TokenAttributes
    {
        TokenAttributes() = default;

        explicit TokenAttributes(const text::TextIterator &text_iterator_)
          : tabs_and_spaces(text_iterator_.getTabsAndSpaces()),
            location(text_iterator_.getLocation())
        {}

        std::string tabs_and_spaces{};
        text::Location location{};
    };

    class Token
    {
    public:
        Token() = default;

        explicit Token(size_t id_) : id(id_)
        {}

        Token(TokenAttributes &&attributes_, const string_view &repr_, size_t id_)
          : attributes(attributes_), repr(repr_), id(id_)
        {}

        Token(TokenAttributes &&attributes_, typename string_view::iterator begin_, size_t id_)
          : Token(std::move(attributes_), { begin_, static_cast<size_t>(0) }, id_)
        {}

        Token(const text::TextIterator &text_iterator_, size_t id_)
          : attributes(text_iterator_), repr(text_iterator_.getRemaining()), id(id_)
        {}

        [[nodiscard]] auto getId() const noexcept -> size_t
        {
            return id;
        }

        [[nodiscard]] auto emptyRepr() const noexcept -> bool
        {
            return repr.empty();
        }

        [[nodiscard]] explicit operator bool() const noexcept
        {
            return getId() != 0;
        }

        [[nodiscard]] auto getLocation() const noexcept -> const text::Location &
        {
            return attributes.location;
        }

        [[nodiscard]] auto getLine() const noexcept -> size_t
        {
            return attributes.location.getLine();
        }

        [[nodiscard]] auto getColumn() const noexcept -> size_t
        {
            return attributes.location.getColumn();
        }

        [[nodiscard]] auto getRealColumn() const noexcept -> size_t
        {
            return attributes.location.getRealColumn();
        }

        [[nodiscard]] auto getFilename() const noexcept -> const string_view &
        {
            return attributes.location.getFilename();
        }

        [[nodiscard]] auto getRepr() const noexcept -> string_view
        {
            return repr;
        }

        [[nodiscard]] auto getPrefixes() noexcept -> const std::vector<string_view> &
        {
            return prefixes;
        }

        [[nodiscard]] auto getPostfixes() noexcept -> const std::vector<string_view> &
        {
            return postfixes;
        }

        [[nodiscard]] auto getTabsAndSpaces() const noexcept -> const std::string &
        {
            return attributes.tabs_and_spaces;
        }

        auto setEnd(typename string_view::iterator end_) noexcept -> void
        {
            repr = { repr.begin(), end_ };
        }

        auto finishInitialization(const text::TextIterator &text_iterator) -> void
        {
            repr = text_iterator.getRemaining();
            attributes = TokenAttributes{ text_iterator };
        }

        auto setReprLength(size_t length) noexcept -> void
        {
            repr = { repr.begin(), length };
        }

        auto addPrefix(string_view prefix) -> void
        {
            prefixes.push_back(prefix);
        }

        auto addPostfix(string_view postfix) -> void
        {
            postfixes.push_back(postfix);
        }

        auto clear(size_t new_id)
        {
            id = new_id;
            repr = {};
            prefixes.clear();
            postfixes.clear();
            attributes.tabs_and_spaces.clear();
            attributes.location = text::Location{};
        }

    private:
        std::vector<string_view> prefixes{};
        std::vector<string_view> postfixes{};
        TokenAttributes attributes{};
        string_view repr{};
        size_t id{};
    };
}// namespace ccl::lex

#endif /* CCL_PROJECT_TOKEN_HPP */
