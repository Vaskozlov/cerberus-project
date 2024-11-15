#ifndef AST_LANG_2_WHILE_LOOP_HPP
#define AST_LANG_2_WHILE_LOOP_HPP

#include <ast-lang-2/ast/core/node.hpp>

namespace astlang2::ast::statement
{
    class WhileLoop final : public core::AstlangNode
    {
    public:
        using AstlangNode::AstlangNode;

        auto compute(interpreter::Interpreter &interpreter) const
            -> core::ComputationResult override;
    };
}// namespace astlang2::ast::statement

#endif /* AST_LANG_2_WHILE_LOOP_HPP */
