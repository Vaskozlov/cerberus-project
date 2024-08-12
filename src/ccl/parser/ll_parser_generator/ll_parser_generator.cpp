#include <ccl/parser/ll/ll_parser_generator.hpp>

namespace ccl::parser::ll
{
    LlParserGenerator::LlParserGenerator(
        SmallId start_symbol,
        const GrammarStorage &grammar_storage,
        std::function<std::string(SmallId)>
            id_to_string_converter)
      : firstSetEvaluator{grammar_storage.getEpsilon(), grammar_storage}
      , followSetEvaluator(
            start_symbol,
            std::to_underlying(lexer::ReservedTokenType::EOI),
            grammar_storage.getEpsilon(),
            grammar_storage,
            firstSetEvaluator.getFirstSet())
      , idToStringConverter{std::move(id_to_string_converter)}
      , storage{grammar_storage}
    {
        for (auto &[key, rule] : storage.rulesIterator()) {
            generateUsingRule(key, rule);
        }
    }

    [[nodiscard]] auto LlParserGenerator::createLl1Table() const
        -> ankerl::unordered_dense::map<TableEntry, const Rule *>
    {
        auto ll_1_table = ankerl::unordered_dense::map<TableEntry, const Rule *>{};

        for (const auto &[entry, rules] : table) {
            if (rules.size() > 1) {
                throw std::runtime_error{"Grammar is not LL(1) backtrack free."};
            }

            ll_1_table.emplace(entry, rules.front());
        }

        return ll_1_table;
    }

    [[nodiscard]] auto LlParserGenerator::createGllTable() const
        -> ankerl::unordered_dense::map<TableEntry, std::vector<const Rule *>>
    {
        return ankerl::unordered_dense::map<TableEntry, std::vector<const Rule *>>{
            table.begin(), table.end()};
    }

    auto LlParserGenerator::generateUsingRule(
        ccl::parser::Symbol production,
        const ccl::parser::Rule &rule) -> void
    {
        fmt::println("{}", std::views::transform(rule, idToStringConverter));
        const auto &rule_first_set =
            firstSetEvaluator.getFirstSetOfRules().at(std::addressof(rule));

        auto set = rule_first_set;

        if (set.contains(storage.getEpsilon())) {
            set.insert_range(followSetEvaluator.getFollowSet().at(production));
        }

        fmt::println("{}", std::views::transform(set, idToStringConverter));

        for (auto symbol : set) {
            if (!storage.isTerminal(symbol) || symbol == storage.getEpsilon()) {
                continue;
            }

            insertIntoTable(
                {
                    .state = production,
                    .symbol = symbol,
                },
                std::addressof(rule));

            fmt::println(
                "[{},{}] = {}",
                idToStringConverter(production),
                idToStringConverter(symbol),
                std::views::transform(rule, idToStringConverter));
        }
    }

    auto LlParserGenerator::insertIntoTable(TableEntry entry, const Rule *rule) -> void
    {
        auto [it, inserted] = table.try_emplace(entry);
        auto &vec = it->second;

        if (std::ranges::find(vec, rule) == vec.end()) {
            vec.emplace_back(rule);
        }
    }
}// namespace ccl::parser::ll
