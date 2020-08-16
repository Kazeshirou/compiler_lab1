#include <gtest/gtest.h>

#include "dfa_graph.h"

namespace lab1::test::graph {

using namespace ::graph;

TEST(Dfa_graph, graph) {
    Nfa_graph nfa{"ab"};
    Dfa_graph dfa{nfa};
}

}  // namespace lab1::test::graph