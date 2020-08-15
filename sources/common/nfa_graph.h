#pragma once

#include "graph_interface.h"

#include <list>
#include <stack>
#include <string_view>

#include "tokenizer.h"
#include "tokens.h"

namespace graph {

using namespace ::tokens;

class Nfa_graph : public Graph_interface {
public:
    explicit Nfa_graph(const std::string_view& regex) {
        auto tokenize_result = tokens::Tokenizer::tokenize(regex);
        if (!tokenize_result) {
            return;
        }
        if (tokenize_result.value().empty()) {
            return;
        }

        auto tokens = tokenize_result.value();

        std::stack<std::pair<Node*, Node*>> stack;
        while (true) {
            if (tokens.empty()) {
                break;
            }
            const token_base_t& token = tokens.front().get();
            tokens.pop();
            if (token.is_operand()) {
                auto first = create_node();
                auto last  = create_node();
                first->add_linked_node(std::string{token.get_value()}, last);
                stack.push(std::make_pair(first, last));

                continue;
            }

            if (token.get_value() == operator_concat_t{}.get_value()) {
                auto second_operand = stack.top();
                stack.pop();
                auto first_operand = stack.top();
                stack.pop();


                auto will_be_deleted = second_operand.first;
                first_operand.second->add_linked_node(
                    will_be_deleted->get_linked());

                stack.push(
                    std::make_pair(first_operand.first, second_operand.second));

                nodes_.remove(will_be_deleted);
                delete will_be_deleted;

                continue;
            }

            if (token.get_value() == operator_or_t{}.get_value()) {
                auto second_operand = stack.top();
                stack.pop();
                auto first_operand = stack.top();
                stack.pop();

                auto new_first = create_node();
                auto new_last  = create_node();

                new_first->add_linked_node(first_operand.first);
                new_first->add_linked_node(second_operand.first);

                first_operand.second->add_linked_node(new_last);
                second_operand.second->add_linked_node(new_last);

                stack.push(std::make_pair(new_first, new_last));

                continue;
            }

            if (token.get_value() == operator_star_t{}.get_value()) {
                auto operand = stack.top();
                stack.pop();

                auto new_first = create_node();
                auto new_last  = create_node();

                new_first->add_linked_node(operand.first);
                new_first->add_linked_node(new_last);

                operand.second->add_linked_node(new_last);
                operand.second->add_linked_node(operand.first);

                stack.push(std::make_pair(new_first, new_last));

                continue;
            }

            if (token.get_value() == operator_plus_t{}.get_value()) {
                auto operand = stack.top();
                stack.pop();

                auto new_first = create_node();
                auto new_last  = create_node();

                new_first->add_linked_node(operand.first);

                operand.second->add_linked_node(new_last);
                operand.second->add_linked_node(operand.first);

                stack.push(std::make_pair(new_first, new_last));

                continue;
            }

            assert("unknown operator");
            break;
        }

        auto graph = stack.top();
        stack.pop();

        auto new_first = new Node;
        new_first->add_linked_node(graph.first);
        nodes_.push_front(new_first);
        new_first->set_double_circle(true);

        graph.second->add_linked_node(create_node());
        nodes_.back()->set_double_circle(true);

        size_t i = 0;
        for (auto& node : nodes_) {
            node->set_label(std::to_string(i++));
        }
    }

    ~Nfa_graph() {
        for (auto& node : nodes_) {
            delete node;
        }
    }

    std::string print() const override {
        std::string temp;
        temp += "digraph nfa{\n\trankdir = \"LR\";\n";
        for (const auto& node : nodes_) {
            temp += node->print();
        }
        temp += "}\n";
        return temp;
    }

private:
    Node* create_node() {
        auto node = new Node();
        nodes_.push_back(node);
        return node;
    }

private:
    std::list<Node*> nodes_;
};

}  // namespace graph