#ifndef CCL_PROJECT_LL_GSS_HPP
#define CCL_PROJECT_LL_GSS_HPP

#include <ccl/parser/ast/terminal.hpp>
#include <ccl/parser/grammar_rules_storage.hpp>
#include <ccl/parser/ll/gss/descriptor.hpp>
#include <ccl/parser/ll/gss/level.hpp>
#include <deque>

namespace ccl::parser
{
    class GrammarStorage;
}

namespace ccl::parser::ll
{
    class GSS
    {
    public:
        using Levels = std::vector<Level>;

    private:
        Levels levels;
        std::deque<Descriptor> terminalDescriptors;
        std::deque<Descriptor> nonTerminalDescriptors;
        std::array<ankerl::unordered_dense::set<const Node *>, 2> passed;
        const GrammarStorage *storage{};
        SmallId globalInputPosition{};

    public:
        explicit GSS(const GrammarStorage *grammar_storage)
          : storage{grammar_storage}
        {
        }

        [[nodiscard]] auto getGlobalInputPosition() const noexcept -> SmallId
        {
            return globalInputPosition;
        }

        [[nodiscard]] auto getLevels() const noexcept CCL_LIFETIMEBOUND -> const Levels &
        {
            return levels;
        }

        [[nodiscard]] auto hasDescriptors() const noexcept -> bool
        {
            return !terminalDescriptors.empty() || !nonTerminalDescriptors.empty();
        }

        auto nextWord() -> void
        {
            ++globalInputPosition;
            passed.at(globalInputPosition % 2).clear();
        }

        auto pop(const Descriptor &descriptor) -> void;

        auto addDescriptor(Descriptor descriptor) -> void;

        auto getDescriptor() -> Descriptor;

        auto pushNode(Node *parent, SPPFNode sppf_node, SmallId input_position) CCL_LIFETIMEBOUND
            -> Node *;

        auto pushNode(Node *parent, RuleWithDot rule, Symbol production, SmallId input_position)
            CCL_LIFETIMEBOUND -> Node *;

        auto pushNode(const Node::Vector &parents, SPPFNode sppf_node, SmallId input_position)
            CCL_LIFETIMEBOUND -> Node *;

    private:
        auto getLevel(SmallId input_position) -> Level &;
    };
}// namespace ccl::parser::ll

#endif /* CCL_PROJECT_LL_GSS_HPP */
