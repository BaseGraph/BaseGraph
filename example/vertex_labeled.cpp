#include <iostream>
#include "BaseGraph/vertexlabeled_graph.hpp"
#include "BaseGraph/metrics/undirected.h"


class CustomLabel {
    bool infected;
    std::string label;

    public:

        CustomLabel(const std::string& label, const bool& infected=false): label(label), infected(infected) {}

        void setInfected(const bool& p_infected) { infected = p_infected; }
        std::string inline getLabel() const { return label; }


        // Required implentations
        bool operator==(const CustomLabel& other) const { return label == other.label && infected == other.infected; }

        friend std::ostream& operator <<(std::ostream &stream, const CustomLabel& object) {
            stream << object.label
                << "(" << (object.infected ? "I" : "S") << ")";
            return stream;
        }
};

// Optional: Template specialization for custom type.
//           It allows to use the template parameter isHashable=true. This globally improve
//           efficiency because implementation of findVertexIndex is O(1) (amortized) instead of O(n)

namespace std {

    template <> struct hash<CustomLabel> {
        size_t operator()(const CustomLabel& object) const {
            return hash<string>()(object.getLabel());
        }
    };

}; // namespace std


int main() {
    BaseGraph::VertexLabeledUndirectedGraph<CustomLabel, true> graph;

    CustomLabel vertex1("v1");
    CustomLabel vertex2("v2");
    CustomLabel vertex3("v3");

    graph.addVertex(vertex1);
    graph.addVertex(vertex2);
    graph.addVertex(vertex3);
    graph.addEdge(vertex1, vertex2);
    graph.addEdge(vertex3, vertex1);


    std::cout << graph << std::endl;
    
    // Infect vertex "v1"
    graph.changeVertexLabelTo(vertex1, CustomLabel(vertex1.getLabel(), true));
    std::cout << vertex1.getLabel() << " is now infected.\n" << std::endl;
    std::cout << graph << std::endl;

    std::cout << "Global clustering coefficient is: " << BaseGraph::getGlobalClusteringCoefficient(graph) << std::endl;
    return 0;
}
