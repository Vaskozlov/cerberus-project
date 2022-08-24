#ifndef CERBERUS_PROJECT_FORMAT_HPP
#define CERBERUS_PROJECT_FORMAT_HPP

#include <array>
#include <cerberus/const_string.hpp>
#include <cerberus/format/core/string_splitter.hpp>
#include <cerberus/format/dumpers/int.hpp>
#include <cerberus/format/dumpers/location.hpp>
#include <cerberus/format/dumpers/string.hpp>
#include <string>

namespace cerb::fmt
{
    template<size_t BufferSize = 128>// NOLINT
    struct StaticFormatterFrame
    {
        StaticFormatterFrame() = default;

        constexpr StaticFormatterFrame(const std::u8string &string) : size(string.size())// NOLINT
        {
            if (string.size() > BufferSize) {
                throw InvalidArgument{ "string is too big" };
            }

            std::ranges::copy(string, buffer.begin());
        }

        CERBLIB_DECL auto begin() const noexcept
        {
            return buffer.begin();
        }

        CERBLIB_DECL auto end() const noexcept
        {
            return buffer.begin() + size;
        }

        CERBLIB_DECL operator u8string_view() const noexcept// NOLINT
        {
            return { buffer.data(), size };
        }

        std::array<char8_t, BufferSize> buffer{};
        size_t size{};
    };

    template<typename T, size_t N>
    struct ArrayConvertableToStrView : std::array<T, N>
    {
        using Base = std::array<T, N>;
        using Base::data;

        CERBLIB_DECL operator u8string_view() const noexcept// NOLINT
        {
            return { data(), N };
        }
    };

    template<ConstString String, bool ConstexprFormatting = false>
    class Formatter
    {
#ifdef __clang__
        static constexpr auto clang_compiler = true;
#else
        static constexpr auto clang_compiler = false;
#endif

    public:
        CERBLIB_DECL auto get() noexcept -> std::u8string &
        {
            return formatted_string;
        }

        template<typename... Args>
        constexpr explicit Formatter(Args &&...args) requires(
            not ConstexprFormatting || not clang_compiler)
        {
            static_assert(sizeof...(Args) == string_blocks.size() - 1, "Wrong number of arguments");

            formatted_string.reserve(approximate_length);
            addBlock<0>(std::forward<Args>(args)...);
        }

        template<typename... Args>
        constexpr explicit Formatter(Args &&...args) requires(ConstexprFormatting &&clang_compiler)
          : formatted_string{ u8"needs to be filled to use in constant expression" }
        {
            static_assert(sizeof...(Args) == string_blocks.size() - 1, "Wrong number of arguments");

            formatted_string.clear();
            formatted_string.reserve(approximate_length);
            addBlock<0>(std::forward<Args>(args)...);
        }

    private:
        template<size_t Index>
        constexpr auto addBlock() -> void
        {
            if constexpr (isNotEmptyBlock<Index>()) {
                append(string_blocks[Index]);
            }
        }

        template<size_t Index, typename Arg, typename... Args>
        constexpr auto addBlock(Arg &&value, Args &&...values) -> void
        {
            if constexpr (isNotEmptyBlock<Index>()) {
                append(string_blocks[Index]);
            }

            dump(formatted_string, value);

            if constexpr (Index + 1 != string_blocks.size()) {
                addBlock<Index + 1>(std::forward<Args>(values)...);
            }
        }

        template<size_t Index>
        CERBLIB_DECL static auto isNotEmptyBlock() noexcept -> bool
        {
            return not string_blocks[Index].empty();
        }

        constexpr auto append(const u8string_view &str) -> void
        {
            if CERBLIB_RUNTIME_BRANCH {// G++-12 doesn't allow to append string at compile time
                formatted_string.append(static_cast<std::u8string_view>(str));
            } else {
                for (auto &&chr : str) {
                    formatted_string.push_back(chr);
                }
            }
        }

        constexpr static auto countApproximateLength() noexcept -> unsigned long
        {
            constexpr auto string_size = std::accumulate(
                string_blocks.begin(), string_blocks.end(), static_cast<size_t>(0),
                [](auto acc, auto &block) { return acc + block.size(); });

            constexpr auto argument_count = string_blocks.size() - 1;
            constexpr auto arguments_approximate_length = argument_count * 8;

            return string_size + arguments_approximate_length;
        }

        constexpr static auto string_blocks = core::splitString<String>();
        constexpr static auto approximate_length = countApproximateLength();

        std::u8string formatted_string;
    };

    template<ConstString String, typename... Args>
    CERBLIB_DECL auto format(Args &&...args) -> std::u8string
    {
        if CERBLIB_COMPILE_TIME_BRANCH {
            auto formatter = Formatter<String, true>{ std::forward<Args>(args)... };
            return formatter.get();
        }

        auto formatter = Formatter<String>{ std::forward<Args>(args)... };
        return formatter.get();
    }

    template<ConstString String, auto... Args>
    struct StaticFormatter
    {
        constexpr static auto formatted_string =
            StaticFormatterFrame<1024>{ format<String>(Args...) };

        constexpr static auto get() -> ArrayConvertableToStrView<char8_t, formatted_string.size>
        {
            auto result = ArrayConvertableToStrView<char8_t, formatted_string.size>{};
            std::ranges::copy(formatted_string, result.begin());
            return result;
        }
    };

    template<ConstString String, auto... Args>
    consteval auto staticFormat()
    {
        return StaticFormatter<String, Args...>{}.get();
    }
}// namespace cerb::fmt

#endif /* CERBERUS_PROJECT_FORMAT_HPP */
