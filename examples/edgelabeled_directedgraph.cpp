#include <iostream>
#include <string>

#include "BaseGraph/directed_graph.hpp"

int main() {
    struct Flight {
        std::string company;
        double distance;
    };

    BaseGraph::LabeledDirectedGraph<Flight> graph(5);
    graph.addEdge(0, 1, {"Company A", 10.});
    graph.addEdge(4, 3, {"Company B", 2.2});

    Flight flightA = graph.getEdgeLabel(0, 1);
    Flight flightB = graph.getEdgeLabel(4, 3);

    std::cout << "Flight from 0 to 1, company: " << flightA.company
              << ", distance: " << flightA.distance << std::endl;
    std::cout << "Flight from 4 to 3, company: " << flightB.company
              << ", distance: " << flightB.distance << std::endl;

    graph.setEdgeLabel(0, 1, {"Company B", 10.});

    std::cout << "Flight from 0 to 1 is now by company: "
              << graph.getEdgeLabel(0, 1).company << std::endl;

    return 0;
}
