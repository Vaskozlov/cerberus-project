#ifndef CCL_PROJECT_ACTION_HPP
#define CCL_PROJECT_ACTION_HPP

#include <ccl/parser/lr_item.hpp>
#include <ccl/parser/parsing_action.hpp>

namespace ccl::parser
{
    class Action
    {
    private:
        std::variant<std::monostate, Id, LrItem> data;
        ParsingAction parsingAction;

    public:
        explicit Action(Id state)
          : data{state}
          , parsingAction{ParsingAction::SHIFT}
        {}

        explicit Action(const LrItem &item)
          : data{item}
          , parsingAction{ParsingAction::REDUCE}
        {}

        explicit Action(std::monostate accept_state)
          : data{accept_state}
          , parsingAction{ParsingAction::ACCEPT}
        {}

        [[nodiscard]] auto operator==(const Action &other) const noexcept -> bool = default;

        [[nodiscard]] auto isShift() const noexcept -> bool
        {
            return parsingAction == ParsingAction::SHIFT;
        }

        [[nodiscard]] auto isReduce() const noexcept -> bool
        {
            return parsingAction == ParsingAction::REDUCE;
        }

        [[nodiscard]] auto isAccept() const noexcept -> bool
        {
            return parsingAction == ParsingAction::ACCEPT;
        }

        [[nodiscard]] auto getParsingAction() const noexcept -> ParsingAction
        {
            return parsingAction;
        }

        [[nodiscard]] auto
            getStoredData() const noexcept -> const std::variant<std::monostate, Id, LrItem> &
        {
            return data;
        }

        [[nodiscard]] auto getShiftingState() const noexcept -> Id
        {
            return std::get<Id>(data);
        }

        [[nodiscard]] auto getReducingItem() const noexcept -> const LrItem &
        {
            return std::get<LrItem>(data);
        }
    };

    template<typename T>
    struct ActionPrintWrapper
    {
        const Action &action;
    };
}// namespace ccl::parser

template<typename T>
class fmt::formatter<ccl::parser::ActionPrintWrapper<T>> : public fmt::formatter<std::string_view>
{
private:
    template<class... Ts>
    struct overloaded : Ts...
    {
        using Ts::operator()...;
    };

public:
    template<typename FmtContext>
    constexpr auto format(
        const ccl::parser::ActionPrintWrapper<T> &action_print_wrapper,
        FmtContext &ctx) const -> decltype(auto)
    {
        auto &action = action_print_wrapper.action;
        fmt::format_to(ctx.out(), "{} ", action.getParsingAction());

        return std::visit(
            overloaded{
                [&ctx](const ccl::parser::LrItem &arg) {
                    return fmt::format_to(ctx.out(), "{}", ccl::parser::LrItemPrintWrapper<T>(arg));
                },
                [&ctx](auto arg) {
                    return fmt::format_to(ctx.out(), "{}", arg);
                },
            },
            action.getStoredData());
    }
};

#endif /* CCL_PROJECT_ACTION_HPP */