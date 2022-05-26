#include <iostream>

#include "BaseGraph/directedgraph.h"
#include "BaseGraph/extensions/metrics/directed.h"


int main(int argc, char* argv[]) {
    BaseGraph::DirectedGraph graph(5);
    graph.addEdgeIdx(0, 1);
    graph.addEdgeIdx(0, 3);
    graph.addEdgeIdx(1, 0);

    size_t degree = graph.getOutDegreeOfIdx(0);
    double reciprocity = BaseGraph::metrics::getReciprocity(graph);

    std::cout << "Out degree of vertex 0 is " << degree
              << " and reciprocity is " << reciprocity
              << " for this graph:\n"
              << graph << std::endl;
    return 0;
}
