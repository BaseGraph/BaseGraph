#ifndef CORE_FIXTURES
#define CORE_FIXTURES

#include <stdexcept>
#include <type_traits>
#include <typeinfo>
#include <vector>

#include "BaseGraph/directed_graph.hpp"
#include "BaseGraph/undirected_graph.hpp"
#include "gtest/gtest.h"

template <typename T> std::vector<T> getLabels() {
    std::string error = "Test fixture: getLabels() not implemented for type" +
                        std::string(typeid(T).name());
    throw std::logic_error(error);
};
template <> inline std::vector<char> getLabels() {
    return {'a', 'b', 'c', 'd', 'e'};
};
template <> inline std::vector<std::string> getLabels() {
    return {"A", "B", "C", "D", "E"};
};
template <> inline std::vector<int> getLabels() {
    return {-10, 0, 1, 10, 100};
};

template <typename T> std::vector<T> getOtherLabels() {
    std::string error =
        "Test fixture: getOtherLabels() not implemented for type" +
        std::string(typeid(T).name());
    throw std::logic_error(error);
};
template <> inline std::vector<char> getOtherLabels() {
    return {'z', 'y', 'x', 'w', 'v'};
};
template <> inline std::vector<int> getOtherLabels() {
    return {-5, -1, 2, 11, 21};
};
template <> inline std::vector<std::string> getOtherLabels() {
    return {"Z", "Y", "X", "W", "V"};
};

template <typename EdgeLabel>
class LabeledDirectedGraph_ : public testing::Test {
  public:
    std::vector<EdgeLabel> labels = getLabels<EdgeLabel>();
    std::vector<EdgeLabel> unusedLabels = getOtherLabels<EdgeLabel>();

    BaseGraph::LabeledDirectedGraph<EdgeLabel> graph;

    void SetUp() {
        labels = getLabels<EdgeLabel>();
        unusedLabels = getOtherLabels<EdgeLabel>();
        graph.resize(4);
    }

    void EXPECT_NEIGHBOURS(BaseGraph::VertexIndex vertex,
                           const BaseGraph::Successors &neighbours) {
        EXPECT_EQ(graph.getOutEdgesOf(vertex), neighbours);
    }
    void EXPECT_LABEL(BaseGraph::Edge edge, size_t labelIndex) {
        ASSERT_EQ(graph.getEdgeLabelOf(edge.first, edge.second),
                  this->labels[labelIndex]);
    }
};

template <typename EdgeLabel>
class EdgeLabeledUndirectedGraph_ : public testing::Test {
  public:
    std::vector<EdgeLabel> labels = getLabels<EdgeLabel>();
    std::vector<EdgeLabel> unusedLabels = getOtherLabels<EdgeLabel>();

    BaseGraph::LabeledUndirectedGraph<EdgeLabel> graph;

    void SetUp() { graph.resize(4); }

    void EXPECT_NEIGHBOURS(BaseGraph::VertexIndex vertex,
                           const BaseGraph::Successors &neighbours) {
        EXPECT_EQ(graph.getOutEdgesOf(vertex), neighbours);
    }
    void EXPECT_LABEL(BaseGraph::Edge edge, size_t labelIndex) {
        ASSERT_EQ(graph.getEdgeLabelOf(edge.first, edge.second),
                  this->labels[labelIndex]);
        ASSERT_EQ(graph.getEdgeLabelOf(edge.second, edge.first),
                  this->labels[labelIndex]);
    }
};

#endif
