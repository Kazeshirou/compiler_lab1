#pragma once

#include "graph_interface.h"

#include <list>
#include <set>
#include <stack>

#include "nfa_graph.h"

namespace graph {

using namespace ::tokens;

class Dfa_node : public Node {
public:
    explicit Dfa_node(const std::set<Node*>& nfa_states)
            : nfa_states_{nfa_states} {
        for (const auto& node : nfa_states) {
            if (node->get_double_circle()) {
                set_double_circle(true);
                break;
            }
        }
    }

    bool operator==(const Dfa_node& other) const {
        return nfa_states_ == other.nfa_states_;
    }

    const std::set<Node*>& get_nfa_states() const {
        return nfa_states_;
    }

private:
    const std::set<Node*> nfa_states_;
};

class Dfa_graph : public Graph_interface {
public:
    explicit Dfa_graph(const Nfa_graph& nfa) {
        const std::list<Node*>& nfa_states = nfa.get_nodes();

        Dfa_node* cur_state =
            new Dfa_node{get_eps_closure({nfa_states.front()})};
        nodes_.push_back(cur_state);

        std::stack<Dfa_node*> new_states;
        new_states.push(cur_state);

        while (!new_states.empty()) {
            cur_state = new_states.top();
            new_states.pop();
            for (auto c : all_operands) {
                std::string operand{c};
                Dfa_node    next_state(get_eps_closure(
                    get_move(cur_state->get_nfa_states(), operand)));

                if (!next_state.get_nfa_states().size()) {
                    continue;
                }
                Dfa_node* exist_state{nullptr};
                for (const auto& state : nodes_) {
                    if (*state == next_state) {
                        exist_state = state;
                        break;
                    }
                }
                if (!exist_state) {
                    exist_state = new Dfa_node{next_state.get_nfa_states()};
                    nodes_.push_back(exist_state);
                    new_states.push(exist_state);
                }
                cur_state->add_linked_node(operand, exist_state);
            }
        }

        size_t i = 0;
        for (auto& node : nodes_) {
            node->set_label(std::to_string(i++));
        }
    }

    ~Dfa_graph() {
        for (auto& node : nodes_) {
            delete node;
        }
    }

    std::string print() const override {
        std::string temp;
        temp += "digraph dfa{\n\trankdir = \"LR\";\n";
        for (const auto& node : nodes_) {
            temp += node->print();
        }
        temp += "}\n";
        return temp;
    }

    std::set<Node*> get_eps_closure(const std::set<Node*>& states) {
        std::set<Node*>   eps_closure{states};
        std::stack<Node*> stack{};

        for (const auto& state : states) {
            stack.push(state);
        }

        while (!stack.empty()) {
            Node* top = stack.top();
            stack.pop();
            for (const auto& [label, state] : top->get_linked()) {
                if (label != epsilon_c) {
                    continue;
                }
                if (eps_closure.find(state) == eps_closure.end()) {
                    eps_closure.insert(state);
                    stack.push(state);
                }
            }
        }
        return eps_closure;
    }

    std::set<Node*> get_move(const std::set<Node*>&  states,
                             const std::string_view& label) {
        std::set<Node*> move{};
        for (const auto& state : states) {
            for (const auto& [cur_label, next_state] : state->get_linked()) {
                if (cur_label != label) {
                    continue;
                }
                move.insert(next_state);
            }
        }
        return move;
    }

private:
    std::list<Dfa_node*> nodes_;
};

}  // namespace graph