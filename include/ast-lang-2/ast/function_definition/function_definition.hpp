#ifndef AST_LANG_2_FUNCTION_DEFINITION_HPP
#define AST_LANG_2_FUNCTION_DEFINITION_HPP

#include <ast-lang-2/ast/core/node.hpp>

namespace astlang2::ast::function::def
{
    class FunctionDefinition final : public core::AstlangNode
    {
    public:
        using AstlangNode::AstlangNode;

        auto compute(interpreter::Interpreter &interpreter) const
            -> core::ComputationResult override;
    };
}// namespace astlang2::ast::function::def

#endif /* AST_LANG_2_FUNCTION_DEFINITION_HPP */
