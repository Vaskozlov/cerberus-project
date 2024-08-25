#ifndef CCL_PROJECT_LL_PARSER_GENERATOR_HPP
#define CCL_PROJECT_LL_PARSER_GENERATOR_HPP

#include <ccl/parser/first_set.hpp>
#include <ccl/parser/follow_set.hpp>
#include <ccl/parser/grammar_rules_storage.hpp>
#include <ccl/parser/table_entry.hpp>

namespace ccl::parser::ll
{
    using Ll1Table = ankerl::unordered_dense::map<TableEntry, const Rule *>;
    using GllTable = ankerl::unordered_dense::map<TableEntry, std::vector<const Rule *>>;

    class LlParserGenerator
    {
    private:
        std::unordered_map<TableEntry, std::vector<const Rule *>> table;
        FirstSetEvaluator firstSetEvaluator;
        FollowSetEvaluator followSetEvaluator;
        std::function<std::string(SmallId)> idToStringConverter;
        const GrammarStorage &storage;

    public:
        LlParserGenerator(
            SmallId start_symbol, const GrammarStorage &grammar_storage,
            const std::function<std::string(SmallId)> &id_to_string_converter);

        [[nodiscard]] auto createLl1Table() const -> Ll1Table;

        [[nodiscard]] auto createGllTable() const -> GllTable;

    private:
        auto insertIntoTable(TableEntry entry, const Rule *rule) -> void;

        auto generateUsingRule(Symbol production, const Rule &rule) -> void;
    };
}// namespace ccl::parser::ll

#endif /* CCL_PROJECT_LL_PARSER_GENERATOR_HPP */
