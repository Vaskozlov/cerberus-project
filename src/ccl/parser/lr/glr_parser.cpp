#include "ccl/parser/lr/glr_parser.hpp"

#include "ccl/lexer/tokenizer.hpp"
#include "ccl/parser/ast/terminal.hpp"
#include "ccl/parser/dot/dot_repr.hpp"
#include "ccl/parser/lr/detail/lr_parser_generator.hpp"
#include "ccl/parser/lr/gss/gss.hpp"

#include <isl/dot_repr.hpp>
#include <isl/io.hpp>

namespace ccl::parser
{
    using enum ParsingAction;

    static constexpr bool DebugGLR = false;

    static auto saveGlrState(const lr::GSS &gss, const auto &function) -> void
    {
        auto result = std::vector<ast::Node *>{};

        for (const auto &[nonTerminals, terminal] : gss.getLevels()) {
            result.emplace_back(terminal->value.get());

            for (const auto &node : nonTerminals) {
                result.emplace_back(node->value.get());
            }
        }

        auto tree_repr = dot::createDotRepresentation(result, function);
        isl::io::write(std::filesystem::current_path().append("glr.dot"), tree_repr);
    }

    static auto debugGlr(const lr::GSS &gss, const auto &function) -> void
    {
        if constexpr (DebugGLR) {
            saveGlrState(gss, function);
        }
    }

    GlrParser::GlrParser(
        const LrItem &start_item,
        const Symbol epsilon_symbol,
        const GrammarStorage &parser_rules,
        const std::function<std::string(SmallId)> &id_to_string_converter)
      : idToStringConverter{id_to_string_converter}
    {
        auto parser_generator =
            lr::LrParserGenerator(start_item, epsilon_symbol, parser_rules, idToStringConverter);

        gotoTable = std::move(parser_generator.getGotoTable());
        actionTable = parser_generator.getGlrActionTable();
    }

    auto GlrParser::parse(lexer::LexicalAnalyzer::Tokenizer &tokenizer) const
        -> AmbiguousParsingResult
    {
        using enum ParsingAction;

        auto parsing_result = AmbiguousParsingResult{.algorithmName = "GLR"};
        auto token = ast::SharedNode<ast::Terminal>(tokenizer.yield());

        auto gss = lr::GSS{};
        gss.nextWord();

        auto *start_node = gss.pushTerminal(nullptr, 0, 0, token);

        gss.addDescriptor({
            .stack = start_node,
            .inputPosition = 0,
            .parserState = 0,
        });

        while (gss.hasDescriptors()) {
            auto descriptor = gss.getDescriptor();
            const auto parser_state = descriptor.parserState;

            if (descriptor.inputPosition == gss.getGlobalInputPosition()) {
                token = ast::SharedNode<ast::Terminal>(tokenizer.yield());
                gss.nextWord();
            }

            auto possible_actions_it = actionTable.find({
                .state = parser_state,
                .symbol = token->getType(),
            });

            if (possible_actions_it == actionTable.end()) {
                continue;
            }

            for (const Action &action : possible_actions_it->second) {
                switch (action.getActionType()) {
                case SHIFT:
                    gss.addDescriptor(
                        lr::Descriptor{
                            .stack = gss.pushTerminal(
                                descriptor.stack,
                                descriptor.inputPosition,
                                action.getShiftingState(),
                                token),
                            .inputPosition = descriptor.inputPosition + 1,
                            .parserState = action.getShiftingState(),
                        });
                    break;

                case REDUCE:
                    gss.reduce(
                        action.getNumberOfElementsInProduction(),
                        std::addressof(gotoTable),
                        action.getProductionType(),
                        descriptor);
                    break;

                case ACCEPT:
                    parsing_result.roots.emplace_back(descriptor.stack->value);
                    break;

                default:
                    isl::unreachable();
                }
            }
        }

        return parsing_result;
    }
} // namespace ccl::parser
