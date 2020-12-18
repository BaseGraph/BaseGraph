#include <iostream>
#include "pgl/undirectedgraph.h"
#include "pgl/metrics/undirected.h"


int main() {
    PGL::UndirectedGraph graph(10);
    graph.addEdgeIdx(0, 1);
    graph.addEdgeIdx(2, 3);
    graph.addEdgeIdx(2, 4);
    graph.addEdgeIdx(3, 4);

    std::cout << graph << std::endl;
    std::cout << "Global clustering coefficient is: " << PGL::getGlobalClusteringCoefficient(graph) << std::endl;
    return 0;
}
