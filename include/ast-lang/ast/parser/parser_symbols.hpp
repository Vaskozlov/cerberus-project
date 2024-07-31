#ifndef CCL_PROJECT_PARSER_SYMBOLS_HPP
#define CCL_PROJECT_PARSER_SYMBOLS_HPP

#include <ast-lang/ast/node_sequence.hpp>

namespace astlang::ast
{
    class ParserSymbols
      : public ccl::parser::ast::UnNodeSequence
      , public NodeSequence
    {
    public:
        using ccl::parser::ast::UnNodeSequence::NodeSequence;

        auto
            compute(interpreter::Interpreter &interpreter) -> interpreter::EvaluationResult override
        {
            return interpreter::EvaluationResult{
                .value = std::nullopt, .type = interpreter::Type::VOID};
        }
    };
}// namespace astlang::ast

#endif /* CCL_PROJECT_PARSER_SYMBOLS_HPP */