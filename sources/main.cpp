#include <fstream>
#include <iostream>
#include <string>

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
    return 0;
}