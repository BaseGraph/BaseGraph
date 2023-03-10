#include <iostream>
#include "BaseGraph/undirectedgraph.h"


int main() {
    BaseGraph::UndirectedGraph graph(10);
    graph.addEdge(0, 1);
    graph.addEdge(2, 3);
    graph.addEdge(2, 4);
    graph.addEdge(3, 4);

    std::cout << graph << std::endl;
    std::cout << "The degree of vertex 2 is: "
        << graph.getDegreeOf(2)
        << std::endl;
    return 0;
}
