#include <gtest/gtest.h>

#include "graph_interface.h"

namespace lab1::tests::node {

using namespace graph;

TEST(Node_tests, print) {
    Node n1("1");
    Node n2("2");
    Node n3("3");
    n1.add_linked_node(epsilon_c, &n2);
    n1.add_linked_node("a", &n3);

    ASSERT_EQ(n1.print(), std::string("\t1 -> 2 [label=") + epsilon_c +
                              "]\n\t1 -> 3 [label=a]\n");
}

}  // namespace lab1::tests::node