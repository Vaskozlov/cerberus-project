#ifndef CCL_PROJECT_FUNCTION_CALL_HPP
#define CCL_PROJECT_FUNCTION_CALL_HPP

#include <ast-lang/ast/node.hpp>

namespace astlang::ast::function::call
{
    class FunctionCall : public Node
    {
    public:
        using Node::Node;

        auto compute(interpreter::Interpreter &interpreter)
            -> interpreter::EvaluationResult override;
    };
}// namespace astlang::ast::function::call

#endif /* CCL_PROJECT_FUNCTION_CALL_HPP */
