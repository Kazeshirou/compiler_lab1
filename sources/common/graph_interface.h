#pragma once

#include <functional>
#include <map>
#include <string>

namespace graph {

constexpr const char* epsilon_c = "<&#949;>";

class Node {
public:
    explicit Node(const std::string_view& label) : label_{label} {}

    std::string print() const {
        std::string temp;
        for (const auto& [symbol, link]: linked_) {
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

    void add_linked_node(const std::string_view& symbol, Node* node) {
        linked_[symbol] = node;
    }

private:
    std::string                  label_;
    std::map<std::string_view, Node*> linked_;
};

class Graph_interface {
public:
    virtual ~Graph_interface()        = default;
    virtual std::string print() const = 0;
};

}  // namespace graph