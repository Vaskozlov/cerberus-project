#ifndef AST_LANG_2_CALL_ARGUMENTS_HPP
#define AST_LANG_2_CALL_ARGUMENTS_HPP

#include <ast-lang-2/ast/core/node.hpp>

namespace astlang2::ast::function::call
{
    class CallArguments final : public core::AstlangNode
    {
    public:
        using AstlangNode::AstlangNode;

        auto compute(interpreter::Interpreter &interpreter) const
            -> core::ComputationResult override;
    };
}// namespace astlang2::ast::function::call

#endif /*AST_LANG_2_CALL_ARGUMENTS_HPP */
