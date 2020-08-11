#pragma once

#include "graph_interface.h"

#include <list>
#include <stack>
#include <string_view>

namespace graph {

class Nka_graph : public Graph_interface {
public:
    explicit Nka_graph(const std::string_view& regex) {
        std::stack<std::pair<Node*, Node*>> stack_or;
        std::stack<Node*>                   stack_parentness;
        auto                                first = create_node();
        auto                                last  = first;
        while (true) {
            char c = regex.front();
            switch (c) {
                case '|': {
                    auto n0 = create_node();
                    auto nf = create_node();
                    n0->add_linked_node(epsilon_c, first);
                    last->add_linked_node(epsilon_c, nf);
                } break;
                case '*':
                    break;
                case '(': {
                    stack_parentness.push(std::make_pair(first, ));
                    auto first = create_node();
                    auto last  = first;
                } break;
                case ')':
                    break;
                default: {
                    auto node = create_node();
                    last->add_linked_node(std::string() + c, node);
                    last = node;
                } break;
            }
        }
    }

    ~Nka_graph() {
        for (auto& node : nodes_) {
            delete node;
        }
    }

    std::string print() const override {
        std::string temp;
        temp += "digraph NKA{\n";
        for (const auto& node : nodes_) {
            temp += node->print();
        }
        temp += "}\n";
        return temp;
    }

private:
    Node* create_node() {
        auto node = new Node(std::to_string(nodes_.size()));
        nodes_.push_back(node);
        return node;
    }

private:
    std::list<Node*> nodes_;
};

}  // namespace graph