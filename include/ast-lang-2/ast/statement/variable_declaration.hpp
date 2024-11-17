#ifndef AST_LANG_2_VARIABLE_DECLARATION_HPP
#define AST_LANG_2_VARIABLE_DECLARATION_HPP

#include <ast-lang-2/ast/core/node.hpp>

namespace astlang2::ast::statement
{
    class VariableDeclaration final : public core::AstlangNode
    {
    private:
        std::string variableName;
        core::SharedNode<AstlangNode> initialValueNode;

    public:
        VariableDeclaration(SmallId id, const ccl::parser::ast::SmallVectorOfNodes &initial_nodes);

        auto compute(interpreter::Interpreter &interpreter) const
            -> core::ComputationResult override;

        auto castChildren(const ConversionTable &conversion_table) -> void override;

        auto optimize() -> core::SharedNode<> override;
    };
}// namespace astlang2::ast::statement

#endif /* AST_LANG_2_VARIABLE_DECLARATION_HPP */
