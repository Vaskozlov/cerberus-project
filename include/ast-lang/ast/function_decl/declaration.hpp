#ifndef CCL_PROJECT_FUNCTION_DEFINITION_HPP
#define CCL_PROJECT_FUNCTION_DEFINITION_HPP

#include <ast-lang/ast/node.hpp>

namespace astlang::ast::function::decl
{
    class Definition : public Node
    {
    public:
        using Node::Node;

        auto compute(interpreter::Interpreter &interpreter)
            -> interpreter::EvaluationResult override;
    };
}// namespace astlang::ast::function::decl


#endif /* CCL_PROJECT_FUNCTION_DEFINITION_HPP */
