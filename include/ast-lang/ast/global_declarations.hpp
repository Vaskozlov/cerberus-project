#ifndef CCL_PROJECT_GLOBAL_DECLARATIONS_HPP
#define CCL_PROJECT_GLOBAL_DECLARATIONS_HPP

#include <ast-lang/ast/node.hpp>

namespace astlang::ast
{
    class GlobalDeclarations : public Node
    {
    public:
        using Node::Node;

        auto compute(interpreter::Interpreter &interpreter)
            -> interpreter::EvaluationResult override;
    };
}// namespace astlang::ast

#endif /* CCL_PROJECT_GLOBAL_DECLARATIONS_HPP */
