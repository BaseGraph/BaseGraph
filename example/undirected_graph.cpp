#include <iostream>
#include "BaseGraph/undirectedgraph.h"


int main() {
    BaseGraph::UndirectedGraph graph(10);
    graph.addEdgeIdx(0, 1);
    graph.addEdgeIdx(2, 3);
    graph.addEdgeIdx(2, 4);
    graph.addEdgeIdx(3, 4);

    std::cout << graph << std::endl;
    std::cout << "The degree of vertex 2 is: "
        << graph.getDegreeOfIdx(2)
        << std::endl;
    return 0;
}
