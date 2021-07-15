#include <stdexcept>
#include <vector>
#include <type_traits>


#include "gtest/gtest.h"
#include "BaseGraph/vertexlabeled_graph.hpp"

#include "fixtures.hpp"


using TestTypes = ::testing::Types<
                        std::tuple<BaseGraph::DirectedGraph, char,                  std::true_type>,
                        std::tuple<BaseGraph::DirectedGraph, char,                  std::false_type>,
                        std::tuple<BaseGraph::DirectedGraph, std::string,           std::true_type>,
                        std::tuple<BaseGraph::DirectedGraph, std::string,           std::false_type>,
                        std::tuple<BaseGraph::DirectedGraph, CustomHashableType,    std::true_type>,
                        std::tuple<BaseGraph::DirectedGraph, CustomHashableType,    std::false_type>,
                        std::tuple<BaseGraph::DirectedGraph, CustomNonHashableType, std::false_type>,

                        std::tuple<BaseGraph::UndirectedGraph, char,                  std::true_type>,
                        std::tuple<BaseGraph::UndirectedGraph, char,                  std::false_type>,
                        std::tuple<BaseGraph::UndirectedGraph, std::string,           std::true_type>,
                        std::tuple<BaseGraph::UndirectedGraph, std::string,           std::false_type>,
                        std::tuple<BaseGraph::UndirectedGraph, CustomHashableType,    std::true_type>,
                        std::tuple<BaseGraph::UndirectedGraph, CustomHashableType,    std::false_type>,
                        std::tuple<BaseGraph::UndirectedGraph, CustomNonHashableType, std::false_type>
                    >;

#define GET_OTHER_LABELS getOtherLabels<typename std::tuple_element<1, TypeParam>::type>()

TYPED_TEST_SUITE(VertexLabeledGraph, TestTypes);
// This test suite assumes that base classes work properly.


TYPED_TEST(VertexLabeledGraph, isVertex_existentLabel_returnTrue) {
    for (auto vertex: this->labels)
        EXPECT_TRUE(this->graph.isVertex(vertex));
}

TYPED_TEST(VertexLabeledGraph, isVertex_inexistentLabel_returnFalse) {
    auto inexistentVertices = GET_OTHER_LABELS;

    for (auto vertex: inexistentVertices)
        EXPECT_FALSE(this->graph.isVertex(vertex));
}


TYPED_TEST(VertexLabeledGraph, getLabelFromIndex_validVertex_returnCorrectLabel) {
    for (BaseGraph::VertexIndex i: this->graph)
        EXPECT_EQ(this->labels[i], this->graph.getLabelFromIndex(i));
}

TYPED_TEST(VertexLabeledGraph, getLabelFromIndex_vertexOutOfRange_throwOutOfRange) {
    EXPECT_THROW(this->graph.getLabelFromIndex(this->labels.size()),
                 std::out_of_range);
}


TYPED_TEST(VertexLabeledGraph, findVertexIndex_existentLabels_returnCorrectIndex) {
    BaseGraph::VertexIndex i=0;
    for (auto vertex: this->labels) {
        EXPECT_EQ(this->graph.findVertexIndex(vertex), i);
        i++;
    }
}

TYPED_TEST(VertexLabeledGraph, findVertexIndex_inexistentLabels_throwInvalidArgument) {
    auto inexistentVertices = GET_OTHER_LABELS;

    for (auto vertex: inexistentVertices)
        EXPECT_THROW(this->graph.findVertexIndex(vertex), std::invalid_argument);
}


TYPED_TEST(VertexLabeledGraph, changeVertexLabelTo_inexistentLabel_onlyNewLabelExists) {
    this->graph.changeVertexLabelTo(this->labels[0], this->unusedLabels[0]);

    EXPECT_TRUE (this->graph.isVertex(this->unusedLabels[0]));
    EXPECT_FALSE(this->graph.isVertex(this->labels[0]));
}

TYPED_TEST(VertexLabeledGraph, changeVertexLabelTo_backAndForthChange_onlyOriginalLabelExists) {
    this->graph.changeVertexLabelTo(this->labels[0], this->unusedLabels[0]);
    this->graph.changeVertexLabelTo(this->unusedLabels[0], this->labels[0]);

    EXPECT_TRUE (this->graph.isVertex(this->labels[0]));
    EXPECT_FALSE(this->graph.isVertex(this->unusedLabels[0]));
}

TYPED_TEST(VertexLabeledGraph, changeVertexLabelTo_toExistentLabel_throwInvalidArgument) {
    EXPECT_THROW(
        this->graph.changeVertexLabelTo(this->labels[0], this->labels[1]),
        std::invalid_argument);
    EXPECT_THROW(
        this->graph.changeVertexLabelTo(this->labels[0], this->labels[0]),
        std::invalid_argument);
}

TYPED_TEST(VertexLabeledGraph, changeVertexLabelTo_frominexistentLabel_throwInvalidArgument) {
    EXPECT_THROW(
        this->graph.changeVertexLabelTo(this->unusedLabels[0], this->labels[0]),
        std::invalid_argument);
}
