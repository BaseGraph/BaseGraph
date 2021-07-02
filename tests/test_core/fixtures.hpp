#ifndef CORE_FIXTURES
#define CORE_FIXTURES


#include <stdexcept>
#include <vector>
#include <type_traits>


#include "gtest/gtest.h"
#include "BaseGraph/vertexlabeled_graph.hpp"


struct CustomNonHashableType {
    std::string label;

    CustomNonHashableType(const std::string& label): label(label) {}

    bool operator==(const CustomNonHashableType& other) const { return label == other.label; }
    friend std::ostream& operator <<(std::ostream &stream, const CustomNonHashableType& object) {
        return stream;
    }
};


struct CustomHashableType: public CustomNonHashableType {
    CustomHashableType(const std::string& label): CustomNonHashableType(label) {}
};
namespace std {

    template <> struct hash<CustomHashableType> {
        size_t operator()(const CustomHashableType& object) const {
            return hash<string>()(object.label);
        }
    };
}; // namespace std


template<typename T>
std::vector<T> getVertices() {
    return {};
};
template<>
inline std::vector<char> getVertices() {
    return {'a', 'b', 'c', 'd'};
};
template<>
inline std::vector<std::string> getVertices() {
    return {"A", "B", "C", "D"};
};
template<>
inline std::vector<CustomHashableType> getVertices() {
    using T = CustomHashableType;
    return {T("A"), T("B"), T("C"), T("D")};
};
template<>
inline std::vector<CustomNonHashableType> getVertices() {
    using T = CustomNonHashableType;
    return {T("A"), T("B"), T("C"), T("D")};
};


template<typename T>
std::vector<T> getOtherVertices() {
    return {};
};
template<>
inline std::vector<char> getOtherVertices() {
    return {'z', 'y', 'x', 'w'};
};
template<>
inline std::vector<std::string> getOtherVertices() {
    return {"Z", "Y", "X", "W"};
};
template<>
inline std::vector<CustomHashableType> getOtherVertices() {
    using T = CustomHashableType;
    return {T("Z"), T("Y"), T("X"), T("W")};
};
template<>
inline std::vector<CustomNonHashableType> getOtherVertices() {
    using T = CustomNonHashableType;
    return {T("Z"), T("Y"), T("X"), T("W")};
};


template<typename Label_hashable>
class VertexLabeledGraph : public testing::Test {
    using Label = typename Label_hashable::first_type;
    using hashable = typename Label_hashable::second_type;

    public:
        std::vector<Label> labels;
        std::vector<Label> unusedLabels;

        BaseGraph::VertexLabeledDirectedGraph<Label, hashable::value> directedGraph;
        BaseGraph::VertexLabeledUndirectedGraph<Label, hashable::value> undirectedGraph;

        void SetUp() {
            labels = getVertices<Label>();
            unusedLabels = getOtherVertices<Label>();

            for (auto& vertex: labels) {
                directedGraph.addVertex(vertex);
                undirectedGraph.addVertex(vertex);
            }
        }
};

#endif
