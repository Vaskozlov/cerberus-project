#include "ccl/parser/detail/empty_moves_fixer.hpp"
#include <ccl/parser/grammar_rules_storage.hpp>

namespace ccl::parser
{
    GrammarRulesStorage::GrammarRulesStorage(Symbol epsilon)
      : grammarSymbols{0, epsilon}
      , possiblyEmptyRules{epsilon}
      , epsilonSymbol{epsilon}
    {}

    GrammarRulesStorage::GrammarRulesStorage(
        Symbol epsilon,
        const std::initializer_list<isl::Pair<Symbol, std::vector<Rule>>> &initial_data)
      : std::map<Symbol, std::vector<Rule>>{initial_data.begin(), initial_data.end()}
      , grammarSymbols{0, epsilon}
      , possiblyEmptyRules{epsilon}
      , epsilonSymbol{epsilon}
    {
        finishGrammar();
    }
    auto GrammarRulesStorage::getFollowSetLazily(Symbol start_symbol, Symbol end_symbol)
        -> std::map<Symbol, std::set<Symbol>>
    {
        if (!lazilyInitializedFollowSet.has_value()) {
            const auto first_set = evaluateFirstSet(epsilonSymbol, *this);
            auto follow_set =
                evaluateFollowSet(start_symbol, end_symbol, epsilonSymbol, *this, first_set);

            lazilyInitializedFollowSet = follow_set;
        }

        return lazilyInitializedFollowSet.value();
    }


    auto GrammarRulesStorage::getNotFilledHandlers(const Symbol start_symbol,
        const Symbol end_symbol) -> std::set<Symbol>
    {

        auto follow_set = getFollowSetLazily(start_symbol, end_symbol);

        auto result_set = std::set<Symbol>();

        for (const auto non_terminal_symbol : getNonTerminals()) {
            if (follow_set.at(non_terminal_symbol).size() == 0) {
                result_set.emplace(non_terminal_symbol);
            }
        }

        return result_set;
    }

    auto GrammarRulesStorage::registerAllRuleSymbols(const Rule &rule) -> void
    {
        for (const auto symbol : rule) {
            grammarSymbols.emplace(symbol);
        }
    }

    auto GrammarRulesStorage::finishGrammar() -> void
    {
        for (const auto &[key, rules] : *this) {
            nonTerminals.emplace(key);
            grammarSymbols.emplace(key);

            for (const auto &rule : rules) {
                registerAllRuleSymbols(rule);
            }
        }

        findAndFixEmptyRules();
    }

    auto GrammarRulesStorage::findAndFixEmptyRules() -> void
    {
        auto empty_moves_fixer = detail::EmptyRulesFixer{*this};
        empty_moves_fixer.apply();
    }
}// namespace ccl::parser