#include <iostream>
#include "BaseGraph/undirectedgraph.h"
#include "BaseGraph/extensions/metrics/undirected.h"


int main() {
    BaseGraph::UndirectedGraph graph(10);
    graph.addEdgeIdx(0, 1);
    graph.addEdgeIdx(2, 3);
    graph.addEdgeIdx(2, 4);
    graph.addEdgeIdx(3, 4);

    std::cout << graph << std::endl;
    std::cout << "Global clustering coefficient is: " << BaseGraph::metrics::getGlobalClusteringCoefficient(graph) << std::endl;
    return 0;
}
