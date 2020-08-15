#pragma once

#include <functional>
#include <list>
#include <string>

namespace graph {

constexpr const char* epsilon_c = "<&#949;>";

class Node {
public:
    Node() : label_{""} {}
    Node(const std::string_view& label) : label_{label} {}

    std::string print() const {
        std::string temp;
        if (double_circle_) {
            temp += "\t";
            temp += label_;
            temp += "[shape=doublecircle]\n";
        }
        for (const auto& [symbol, link] : linked_) {
            temp += "\t";
            temp += label_;
            temp += " -> ";
            temp += link->get_label();
            temp += " [label=";
            temp += symbol;
            temp += "]";
            temp += "\n";
        }
        return temp;
    }

    std::string_view get_label() const {
        return label_;
    }

    void set_label(const std::string_view& label) {
        label_ = label;
    }

    void add_linked_node(Node* node) {
        linked_.push_back(std::make_pair(epsilon_c, node));
    }

    void add_linked_node(const std::string& symbol, Node* node) {
        linked_.push_back(std::make_pair(symbol, node));
    }

    void add_linked_node(
        const std::list<std::pair<std::string, Node*>>& links) {
        for (auto& link : links) {
            linked_.push_back(link);
        }
    }

    const std::list<std::pair<std::string, Node*>>& get_linked() const {
        return linked_;
    }

    void set_double_circle(bool flag) {
        double_circle_ = flag;
    }

private:
    std::string                              label_;
    bool                                     double_circle_{false};
    std::list<std::pair<std::string, Node*>> linked_;
};

class Graph_interface {
public:
    virtual ~Graph_interface()        = default;
    virtual std::string print() const = 0;
};

}  // namespace graph