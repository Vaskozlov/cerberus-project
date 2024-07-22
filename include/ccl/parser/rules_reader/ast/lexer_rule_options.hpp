#ifndef CCL_PROJECT_LEXER_RULE_OPTIONS_HPP
#define CCL_PROJECT_LEXER_RULE_OPTIONS_HPP

#include <ccl/parser/ast/string_node.hpp>
#include <ccl/parser/rules_reader/ast/rules_reader_node.hpp>
#include <ccl/parser/rules_reader/rules_lexer.hpp>

namespace ccl::parser::reader::ast
{
    class LexerRuleOptions
      : public parser::ast::StringNode
      , public RulesReaderNode
    {
    private:
        isl::Vector<RulesLexerToken> options;

    public:
        LexerRuleOptions(
            Id node_type_id, const std::initializer_list<RulesLexerToken> &rule_options);

        auto addOption(RulesLexerToken option) -> void;

        [[nodiscard]] auto getOptions() const noexcept -> const isl::Vector<RulesLexerToken> &
        {
            return options;
        }

        auto construct(lexer::LexicalAnalyzer &lexical_analyzer) const -> isl::UniqueAny override;
    };
}// namespace ccl::parser::reader::ast

#endif /* CCL_PROJECT_LEXER_RULE_OPTIONS_HPP */