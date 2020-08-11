#include <iostream>
#include <string>

#include "nka_graph.h"

int main() {
    std::cout << "Введите регулярное выражение:" << std::endl;
    std::string input;
    std::cin >> input;
    graph::Nka_graph nka(input);
    return 0;
}