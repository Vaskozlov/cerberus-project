#ifndef CCL_PROJECT_ARGUMENT_HPP
#define CCL_PROJECT_ARGUMENT_HPP

#include <ast-lang/ast/node_sequence.hpp>

namespace astlang::ast::function::decl
{
    class Argument
      : public ccl::parser::ast::UnNodeSequence
      , public NodeSequence
    {
    public:
        using ccl::parser::ast::UnNodeSequence::NodeSequence;

        auto compute(interpreter::Interpreter &interpreter)
            -> interpreter::EvaluationResult override;
    };
}// namespace astlang::ast::function::decl

#endif /* CCL_PROJECT_ARGUMENT_HPP */