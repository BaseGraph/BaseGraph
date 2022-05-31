#include <iostream>
#include <string>

#include "BaseGraph/edgelabeled_undirectedgraph.hpp"


struct Relationship {
    enum Status {
        MARRIED, DIVORCED
    } status;
    unsigned int durationInYears;
};

std::string strStatus(const Relationship::Status& status) {
    switch (status) {
        case Relationship::Status::MARRIED:
            return "Married";

        case Relationship::Status::DIVORCED:
            return "Divorced";

        default:
            return "Unknown status";
    };
}

int main() {

    BaseGraph::EdgeLabeledUndirectedGraph<Relationship> graph(5);
    graph.addEdgeIdx(0, 1, {Relationship::Status::MARRIED, 10});
    graph.addEdgeIdx(4, 3, {Relationship::Status::DIVORCED, 5});

    Relationship relationA = graph.getEdgeLabelOfIdx(0, 1);
    Relationship relationB = graph.getEdgeLabelOfIdx(4, 3);

    std::cout << "Relationship between 0 and 1: " << strStatus(relationA.status)
        << " for " << relationA.durationInYears << " years"
        << std::endl;
    std::cout << "Relationship between 3 and 4: " << strStatus(relationB.status)
        << " for " << relationA.durationInYears << " years"
        << std::endl;

    graph.setEdgeLabelIdx(0, 1, {Relationship::Status::DIVORCED, 10});

    std::cout << "0 and 1 are now " << strStatus(graph.getEdgeLabelOfIdx(0, 1).status)
        << std::endl;

    return 0;
}