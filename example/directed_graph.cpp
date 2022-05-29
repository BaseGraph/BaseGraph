#include <iostream>
#include "BaseGraph/directedgraph.h"


int main() {
    BaseGraph::DirectedGraph graph(5);
    graph.addEdgeIdx(0, 1);
    graph.addEdgeIdx(0, 3);
    graph.addEdgeIdx(1, 0);

    std::cout << graph << std::endl;
    std::cout << "The out degree of vertex 0 is "
        << graph.getOutDegreeOfIdx(0)
        << std::endl;
    return 0;
}
