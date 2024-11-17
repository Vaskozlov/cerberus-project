#ifndef AST_LANG_2_IF_STATEMENT_HPP
#define AST_LANG_2_IF_STATEMENT_HPP

#include <ast-lang-2/ast/core/node.hpp>

namespace astlang2::ast::statement {
    class IfStatement final : public core::AstlangNode {
    private:
        isl::SmallVector<core::SharedNode<AstlangNode>, 3> nodes;

    public:
        IfStatement(SmallId id, const ccl::parser::ast::SmallVectorOfNodes&initial_nodes);

        auto compute(interpreter::Interpreter&interpreter) const
            -> core::ComputationResult override;

        auto castChildren(const ConversionTable&conversion_table) -> void override;

        auto optimize() -> core::SharedNode<> override;
    };
} // namespace astlang2::ast::statement

#endif /* AST_LANG_2_IF_STATEMENT_HPP */