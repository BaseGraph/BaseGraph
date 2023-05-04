#include <iostream>

#include "BaseGraph/directed_graph.hpp"

int main() {
    BaseGraph::DirectedGraph graph(5);
    graph.addEdge(0, 1);
    graph.addEdge(0, 3);
    graph.addEdge(1, 0);

    std::cout << graph << std::endl;
    std::cout << "The out degree of vertex 0 is " << graph.getOutDegree(0)
              << std::endl;
    return 0;
}
