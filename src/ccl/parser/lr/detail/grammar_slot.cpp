#include <ccl/parser/lr_item.hpp>

auto fmt::formatter<ccl::parser::LrItemPrintWrapper>::format(
    const ccl::parser::LrItemPrintWrapper &item_print_wrapper,
    format_context &ctx) -> format_context::iterator
{
    const auto &item = item_print_wrapper.item;
    fmt::format_to(ctx.out(), "{} -> [", item_print_wrapper.idToStr(item.getProductionType()));

    for (std::size_t i = 0; i != item.dottedRule.size(); ++i) {
        if (i == item.dottedRule.dotPosition) {
            fmt::format_to(ctx.out(), "\u2022");
        }

        fmt::format_to(ctx.out(), "{} ", item_print_wrapper.idToStr(item.dottedRule.at(i)));
    }

    if (item.dottedRule.isDotInTheEnd()) {
        fmt::format_to(ctx.out(), "\u2022");
    }

    return fmt::format_to(ctx.out(), ", {}]", item_print_wrapper.idToStr(item.getLookAhead()));
}