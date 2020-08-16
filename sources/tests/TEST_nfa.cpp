#include <gtest/gtest.h>

#include "nfa_graph.h"

namespace lab1::test::graph {

using namespace ::graph;

TEST(Nfa_graph, concat) {
    Nfa_graph nfa{"ab"};
    ASSERT_EQ(
        nfa.print(),
        "digraph nfa{\n\trankdir = \"LR\";\n\t0[shape=doublecircle]\n\t0 -> 1 "
        "[label=a]\n\t1 -> 2 [label=b]\n\t2[shape=doublecircle]\n}\n");
}

TEST(Nfa_graph, or) {
    Nfa_graph nfa{"a|b"};
    ASSERT_EQ(nfa.print(),
              std::string() +
                  "digraph nfa{\n\trankdir = "
                  "\"LR\";\n\t0[shape=doublecircle]\n\t0 -> 1 [label=" +
                  epsilon_c + "]\n\t0 -> 3 [label=" + epsilon_c +
                  "]\n\t1 -> 2 [label=a]\n\t2 -> 5 [label=" + epsilon_c +
                  "]\n\t3 -> 4 [label=b]\n\t4 -> 5 [label=" + epsilon_c +
                  "]\n\t5[shape=doublecircle]\n}\n");
}

TEST(Nfa_graph, star) {
    Nfa_graph nfa{"a*"};
    ASSERT_EQ(nfa.print(),
              std::string() +
                  "digraph nfa{\n\trankdir = "
                  "\"LR\";\n\t0[shape=doublecircle]\n\t0 -> 1 [label=" +
                  epsilon_c + "]\n\t0 -> 3 [label=" + epsilon_c +
                  "]\n\t1 -> 2 [label=a]\n\t2 -> 1 [label=" + epsilon_c +
                  "]\n\t2 -> 3 [label=" + epsilon_c +
                  "]\n\t3[shape=doublecircle]\n}\n");
}

TEST(Nfa_graph, plus) {
    Nfa_graph nfa{"a+"};
    ASSERT_EQ(nfa.print(),
              std::string() + std::string() +
                  "digraph nfa{\n\trankdir = "
                  "\"LR\";\n\t0[shape=doublecircle]\n\t0 -> 1 [label=" +
                  epsilon_c + "]\n\t1 -> 2 [label=a]\n\t2 -> 1 [label=" +
                  epsilon_c + "]\n\t2 -> 3 [label=" + epsilon_c +
                  "]\n\t3[shape=doublecircle]\n}\n");
    ;
}

}  // namespace lab1::test::graph