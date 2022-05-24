#ifndef CORE_FIXTURES
#define CORE_FIXTURES


#include <stdexcept>
#include <vector>
#include <type_traits>
#include <typeinfo>


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
    std::string error = "Test fixture: getLabels() not implemented for type" + std::string(typeid(T).name());
    throw std::logic_error(error);
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
    std::string error = "Test fixture: getOtherLabels() not implemented for type" + std::string(typeid(T).name());
    throw std::logic_error(error);
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


template<typename Graph_Label_hashable>
class VertexLabeledGraph : public testing::Test {
    using Graph = typename std::tuple_element<0, Graph_Label_hashable>::type;
    using VertexLabel = typename std::tuple_element<1, Graph_Label_hashable>::type;
    using hashable = typename std::tuple_element<2, Graph_Label_hashable>::type;

    public:
        std::vector<VertexLabel> labels;
        std::vector<VertexLabel> unusedLabels;

        BaseGraph::VertexLabeledGraph<Graph, VertexLabel, hashable::value> graph;

        void SetUp() {
            labels = getLabels<VertexLabel>();
            unusedLabels = getOtherLabels<VertexLabel>();

            for (auto& vertex: labels)
                graph.addVertex(vertex);
        }
};


template<typename EdgeLabel>
class EdgeLabeledDirectedGraph_ : public testing::Test {
    public:
        std::vector<EdgeLabel> labels = getLabels<EdgeLabel>();
        std::vector<EdgeLabel> unusedLabels = getOtherLabels<EdgeLabel>();

        BaseGraph::EdgeLabeledDirectedGraph<EdgeLabel> graph;

        void SetUp() {
            labels = getLabels<EdgeLabel>();
            unusedLabels = getOtherLabels<EdgeLabel>();
            graph.resize(4);
        }

        void EXPECT_NEIGHBOURS(BaseGraph::VertexIndex vertex, const BaseGraph::Successors& neighbours) {
            EXPECT_EQ(graph.getOutEdgesOfIdx(vertex), neighbours);
        }
        void EXPECT_LABEL(BaseGraph::Edge edge, size_t labelIndex) {
            ASSERT_EQ(graph.getEdgeLabelOfIdx(edge.first, edge.second), this->labels[labelIndex]);
        }
};

template<typename EdgeLabel>
class EdgeLabeledUndirectedGraph_ : public testing::Test {
    public:
        std::vector<EdgeLabel> labels = getLabels<EdgeLabel>();
        std::vector<EdgeLabel> unusedLabels = getOtherLabels<EdgeLabel>();

        BaseGraph::EdgeLabeledUndirectedGraph<EdgeLabel> graph;

        void SetUp() {
            graph.resize(4);
        }

        void EXPECT_NEIGHBOURS(BaseGraph::VertexIndex vertex, const BaseGraph::Successors& neighbours) {
            EXPECT_EQ(graph.getOutEdgesOfIdx(vertex), neighbours);
        }
        void EXPECT_LABEL(BaseGraph::Edge edge, size_t labelIndex) {
            ASSERT_EQ(graph.getEdgeLabelOfIdx(edge.first, edge.second), this->labels[labelIndex]);
            ASSERT_EQ(graph.getEdgeLabelOfIdx(edge.second, edge.first), this->labels[labelIndex]);
        }
};

template<typename EdgeLabel>
class EdgeLabeledDirectedGraph_integral: public EdgeLabeledDirectedGraph_<EdgeLabel> {
    static_assert(std::is_integral<EdgeLabel>::value, "Type must be integral");
};

template<typename EdgeLabel>
class EdgeLabeledUndirectedGraph_integral: public EdgeLabeledUndirectedGraph_<EdgeLabel> {
    static_assert(std::is_integral<EdgeLabel>::value, "Type must be integral");
};


#endif
