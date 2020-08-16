#include <fstream>
#include <iostream>
#include <string>

#include "dfa_graph.h"
#include "nfa_graph.h"

int main() {
    std::cout << "Введите регулярное выражение:" << std::endl;
    std::string input;
    std::cin >> input;
    graph::Nfa_graph nfa(input);
    std::fstream     file("bin/nfa.dot", file.out);
    if (!file.is_open()) {
        std::cout << "failed to open "
                  << "bin/nfa.dot" << '\n';
    }
    file << nfa.print();
    file.close();

    graph::Dfa_graph dfa(nfa);
    std::fstream     dfa_file("bin/dfa.dot", dfa_file.out);
    if (!dfa_file.is_open()) {
        std::cout << "failed to open "
                  << "bin/dfa.dot" << '\n';
    }
    dfa_file << dfa.print();
    dfa_file.close();
    return 0;
}