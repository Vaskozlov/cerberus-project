#ifndef CCL_PROJECT_NODE_HPP
#define CCL_PROJECT_NODE_HPP

#include <ccl/ccl.hpp>
#include <functional>
#include <isl/dynamic_forward_list.hpp>

namespace ccl::parser
{
    class NodesLifetimeManager;
}

namespace ccl::parser::ast
{
    class Node;

    struct LifetimeManagerNode
    {
    private:
        friend NodesLifetimeManager;
        Node *lifetimeManagerPrivateNextNode{};
        Node *lifetimeManagerPrivatePreviousNode{};
        NodesLifetimeManager *lifetimeManager{};

    public:
        [[nodiscard]] auto getLifetimeManager() const -> NodesLifetimeManager *
        {
            return lifetimeManager;
        }
    };

    class Node
      : public isl::DynamicForwardListNode
      , public LifetimeManagerNode
    {
    private:
        SmallId nodeType{};

    protected:
        static auto getPrintingPrefix(const std::string &prefix, bool is_left) -> std::string;

        static auto expandPrefix(
            const std::string &prefix, bool is_left, size_t extra_expansion = 1) -> std::string;

    public:
        Node() = default;

        explicit Node(SmallId node_type)
          : nodeType{node_type}
        {}

        Node(const Node &node) = default;
        Node(Node &&) noexcept = default;

        virtual ~Node() = default;

        auto operator=(const Node &node) -> Node & = default;
        auto operator=(Node &&) noexcept -> Node & = default;

        [[nodiscard]] auto getType() const noexcept -> SmallId
        {
            return nodeType;
        }

        virtual auto print(
            const std::string &prefix = "", bool is_left = false,
            std::function<std::string(SmallId)> id_converter = [](auto arg) {
                return fmt::to_string(arg);
            }) const -> void = 0;
    };
}// namespace ccl::parser::ast

#endif /* CCL_PROJECT_NODE_HPP */
