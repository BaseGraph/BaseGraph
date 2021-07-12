#ifndef CORE_FIXTURES
#define CORE_FIXTURES


#include <stdexcept>
#include <vector>
#include <type_traits>


#include "gtest/gtest.h"
#include "BaseGraph/vertexlabeled_graph.hpp"
#include "BaseGraph/edgelabeled_directedgraph.hpp"
#include "BaseGraph/edgelabeled_undirectedgraph.hpp"


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
std::vector<T> getLabels() {
    return {};
};
template<>
inline std::vector<char> getLabels() {
    return {'a', 'b', 'c', 'd', 'e'};
};
template<>
inline std::vector<std::string> getLabels() {
    return {"A", "B", "C", "D", "E"};
};
template<>
inline std::vector<int> getLabels() {
    return {-10, 0, 1, 10, 100};
};
template<>
inline std::vector<CustomHashableType> getLabels() {
    using T = CustomHashableType;
    return {T("A"), T("B"), T("C"), T("D"), T("E")};
};
template<>
inline std::vector<CustomNonHashableType> getLabels() {
    using T = CustomNonHashableType;
    return {T("A"), T("B"), T("C"), T("D"), T("E")};
};


template<typename T>
std::vector<T> getOtherLabels() {
    return {};
};
template<>
inline std::vector<char> getOtherLabels() {
    return {'z', 'y', 'x', 'w', 'v'};
};
template<>
inline std::vector<int> getOtherLabels() {
    return {-5, -1, 2, 11, 21};
};
template<>
inline std::vector<std::string> getOtherLabels() {
    return {"Z", "Y", "X", "W", "V"};
};
template<>
inline std::vector<CustomHashableType> getOtherLabels() {
    using T = CustomHashableType;
    return {T("Z"), T("Y"), T("X"), T("W"), T("V")};
};
template<>
inline std::vector<CustomNonHashableType> getOtherLabels() {
    using T = CustomNonHashableType;
    return {T("Z"), T("Y"), T("X"), T("W"), T("V")};
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
            labels = getLabels<Label>();
            unusedLabels = getOtherLabels<Label>();

            for (auto& vertex: labels) {
                directedGraph.addVertex(vertex);
                undirectedGraph.addVertex(vertex);
            }
        }
};


template<typename Label_integral>
class EdgeLabeledGraph : public testing::Test {
    using Label = typename Label_integral::first_type;
    using isIntegral = typename Label_integral::second_type;

    public:
        std::vector<Label> labels;
        std::vector<Label> unusedLabels;

        BaseGraph::EdgeLabeledDirectedGraph<Label> directedGraph;
        BaseGraph::EdgeLabeledUndirectedGraph<Label> undirectedGraph;

        void SetUp() {
            labels = getLabels<Label>();
            unusedLabels = getOtherLabels<Label>();
            directedGraph.resize(4);
            undirectedGraph.resize(4);
        }
};

template<typename Label>
class EdgeLabeledGraph_integral: public EdgeLabeledGraph<std::pair<Label, std::true_type>> {
    static_assert(std::is_integral<Label>::value, "Type must be integral");
};

#endif
