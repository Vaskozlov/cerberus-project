#ifndef CCL_PROJECT_RETURN_STATEMENT_HPP
#define CCL_PROJECT_RETURN_STATEMENT_HPP

#include <ast-lang/ast/node.hpp>

namespace astlang::ast::statement
{
    class ReturnStatement : public Node
    {
    public:
        using Node::Node;

        auto compute(interpreter::Interpreter &interpreter)
            -> interpreter::EvaluationResult override;
    };
}// namespace astlang::ast::statement

#endif /* CCL_PROJECT_RETURN_STATEMENT_HPP */
