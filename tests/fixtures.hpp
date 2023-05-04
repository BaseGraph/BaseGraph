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
        EXPECT_EQ(graph.getOutNeighbours(vertex), neighbours);
    }
    void EXPECT_LABEL(BaseGraph::Edge edge, size_t labelIndex) {
        ASSERT_EQ(graph.getEdgeLabel(edge.first, edge.second),
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
        EXPECT_EQ(graph.getNeighbours(vertex), neighbours);
    }
    void EXPECT_LABEL(BaseGraph::Edge edge, size_t labelIndex) {
        ASSERT_EQ(graph.getEdgeLabel(edge.first, edge.second),
                  this->labels[labelIndex]);
        ASSERT_EQ(graph.getEdgeLabel(edge.second, edge.first),
                  this->labels[labelIndex]);
    }
};

class UndirectedHouseGraph : public ::testing::Test {
    /*
     * (0)     (1)
     *  | \   / | \
     *  |  \ /  |  \
     *  |   X   |  (4)
     *  |  / \  |  /
     *  | /   \ | /
     * (2)-----(3)-----(5)
     *
     *      (6)
     */
  public:
    BaseGraph::UndirectedGraph graph = BaseGraph::UndirectedGraph(7);
    void SetUp() {
        graph.addEdge(0, 2);
        graph.addEdge(0, 3);
        graph.addEdge(1, 2);
        graph.addEdge(1, 3);
        graph.addEdge(1, 4);
        graph.addEdge(2, 3);
        graph.addEdge(3, 4);
        graph.addEdge(3, 5);
    }
};

class DirectedHouseGraph : public ::testing::Test {
    /*
     * (0)_    (1)
     *  ||\   / | \
     *  |  \ /  |  V
     *  |   X   |  (4)
     *  |  / \  |  /
     *  V V   \ | V
     *  (2)---->(3)---->(5)
     *     <----
     *
     *      (6)
     */
  public:
    BaseGraph::DirectedGraph graph = BaseGraph::DirectedGraph(7);
    void SetUp() {
        graph.addEdge(0, 2);
        graph.addEdge(3, 0);
        graph.addEdge(1, 2);
        graph.addEdge(3, 1);
        graph.addEdge(1, 4);
        graph.addReciprocalEdge(2, 3);
        graph.addEdge(4, 3);
        graph.addEdge(3, 5);
    }
};

class TreeLikeGraph : public ::testing::Test {
    /*
     *        (0)
     *       /   \
     *     (1)   (2)
     *    /   \ /   \
     *   (3)  (4)  (5)
     *      \  |  /
     *        (6)
     *         |
     *        (7)
     */
  public:
    BaseGraph::UndirectedGraph graph = BaseGraph::UndirectedGraph(8);
    void SetUp() {
        graph.addEdge(0, 1);
        graph.addEdge(0, 2);
        graph.addEdge(1, 3);
        graph.addEdge(1, 4);
        graph.addEdge(2, 4);
        graph.addEdge(2, 5);
        graph.addEdge(3, 6);
        graph.addEdge(4, 6);
        graph.addEdge(5, 6);
        graph.addEdge(6, 7);
    }
};

class ThreeComponentsGraph : public ::testing::Test {
    /*
     *        (0)--(1)--(2)--(3)
     *
     *           (7)--(8)
     *           /  \
     *         (6)  (9)
     *        /   \
     *      (4)---(5)     (10)
     */
  public:
    BaseGraph::UndirectedGraph graph = BaseGraph::UndirectedGraph(11);
    void SetUp() {
        graph.addEdge(0, 1);
        graph.addEdge(1, 2);
        graph.addEdge(2, 3);

        graph.addEdge(4, 5);
        graph.addEdge(5, 6);
        graph.addEdge(6, 4);
        graph.addEdge(6, 7);
        graph.addEdge(7, 8);
        graph.addEdge(7, 9);
    }
};

#endif
