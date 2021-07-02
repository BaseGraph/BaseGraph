#include <stdexcept>
#include <vector>
#include <type_traits>


#include "gtest/gtest.h"
#include "BaseGraph/vertexlabeled_graph.hpp"

#include "fixtures.hpp"


using TestTypes = ::testing::Types<
                        std::pair<char, std::true_type>,
                        std::pair<char, std::false_type>,
                        std::pair<std::string, std::true_type>,
                        std::pair<std::string, std::false_type>,
                        std::pair<CustomHashableType, std::true_type>,
                        std::pair<CustomHashableType, std::false_type>,
                        std::pair<CustomNonHashableType, std::false_type>
                    >;
TYPED_TEST_SUITE(VertexLabeledGraph, TestTypes);
// This test suite assumes that base classes DirectedGraph and UndirectedGraph work
// properly.


TYPED_TEST(VertexLabeledGraph, isVertex_existentLabel_returnTrue) {
    for (auto vertex: this->labels)
        EXPECT_TRUE(this->directedGraph.isVertex(vertex));
}

TYPED_TEST(VertexLabeledGraph, isVertex_inexistentLabel_returnFalse) {
    auto inexistentVertices = getOtherVertices<typename TypeParam::first_type>();

    for (auto vertex: inexistentVertices)
        EXPECT_FALSE(this->directedGraph.isVertex(vertex));
}


TYPED_TEST(VertexLabeledGraph, getLabelFromIndex_validVertex_returnCorrectLabel) {
    for (BaseGraph::VertexIndex i: this->directedGraph)
        EXPECT_EQ(this->labels[i], this->directedGraph.getLabelFromIndex(i));
}

TYPED_TEST(VertexLabeledGraph, getLabelFromIndex_vertexOutOfRange_throwOutOfRange) {
    EXPECT_THROW(this->directedGraph.getLabelFromIndex(this->labels.size()),
                 std::out_of_range);
}


TYPED_TEST(VertexLabeledGraph, findVertexIndex_existentLabels_returnCorrectIndex) {
    BaseGraph::VertexIndex i=0;
    for (auto vertex: this->labels) {
        EXPECT_EQ(this->directedGraph.findVertexIndex(vertex), i);
        i++;
    }
}

TYPED_TEST(VertexLabeledGraph, findVertexIndex_inexistentLabels_throwInvalidArgument) {
    auto inexistentVertices = getOtherVertices<typename TypeParam::first_type>();

    for (auto vertex: inexistentVertices)
        EXPECT_THROW(this->directedGraph.findVertexIndex(vertex), std::invalid_argument);
}


TYPED_TEST(VertexLabeledGraph, changeVertexLabelTo_inexistentLabel_onlyNewLabelExists) {
    this->directedGraph.changeVertexLabelTo(this->labels[0], this->unusedLabels[0]);

    EXPECT_TRUE (this->directedGraph.isVertex(this->unusedLabels[0]));
    EXPECT_FALSE(this->directedGraph.isVertex(this->labels[0]));
}

TYPED_TEST(VertexLabeledGraph, changeVertexLabelTo_backAndForthChange_onlyOriginalLabelExists) {
    this->directedGraph.changeVertexLabelTo(this->labels[0], this->unusedLabels[0]);
    this->directedGraph.changeVertexLabelTo(this->unusedLabels[0], this->labels[0]);

    EXPECT_TRUE (this->directedGraph.isVertex(this->labels[0]));
    EXPECT_FALSE(this->directedGraph.isVertex(this->unusedLabels[0]));
}

TYPED_TEST(VertexLabeledGraph, changeVertexLabelTo_toExistentLabel_throwInvalidArgument) {
    EXPECT_THROW(
        this->directedGraph.changeVertexLabelTo(this->labels[0], this->labels[1]),
        std::invalid_argument);
    EXPECT_THROW(
        this->directedGraph.changeVertexLabelTo(this->labels[0], this->labels[0]),
        std::invalid_argument);
}

TYPED_TEST(VertexLabeledGraph, changeVertexLabelTo_frominexistentLabel_throwInvalidArgument) {
    EXPECT_THROW(
        this->directedGraph.changeVertexLabelTo(this->unusedLabels[0], this->labels[0]),
        std::invalid_argument);
}


TYPED_TEST(VertexLabeledGraph, addEdge_existentLabels_edgeExists) {
this->directedGraph.addEdge(this->labels[0], this->labels[1]);
    this->directedGraph.addEdge(this->labels[0], this->labels[2]);
    this->directedGraph.addEdge(this->labels[1], this->labels[1]);

    EXPECT_TRUE(this->directedGraph.isEdgeIdx(0, 1));
    EXPECT_TRUE(this->directedGraph.isEdgeIdx(0, 2));
    EXPECT_TRUE(this->directedGraph.isEdgeIdx(1, 1));
}

TYPED_TEST(VertexLabeledGraph, addEdge_inexistentLabels_throwInvalidArgument) {
    EXPECT_THROW(
        this->directedGraph.addEdge(this->unusedLabels[0], this->labels[0]),
        std::invalid_argument);
    EXPECT_THROW(
        this->directedGraph.addEdge(this->labels[0], this->unusedLabels[0]),
        std::invalid_argument);
}


TYPED_TEST(VertexLabeledGraph, isEdge_existentLabelsAndEdge_returnTrue) {
    this->directedGraph.addEdgeIdx(0, 1);
    this->directedGraph.addEdgeIdx(0, 2);
    this->directedGraph.addEdgeIdx(1, 1);

    EXPECT_TRUE(this->directedGraph.isEdge(this->labels[0], this->labels[1]));
    EXPECT_TRUE(this->directedGraph.isEdge(this->labels[0], this->labels[2]));
    EXPECT_TRUE(this->directedGraph.isEdge(this->labels[1], this->labels[1]));
}

TYPED_TEST(VertexLabeledGraph, isEdge_inexistentLabelsAndEdge_returnFalse) {
    this->directedGraph.addEdgeIdx(0, 1);
    this->directedGraph.addEdgeIdx(0, 2);
    this->directedGraph.addEdgeIdx(1, 1);

    EXPECT_FALSE(this->directedGraph.isEdge(this->labels[0], this->labels[0]));
    EXPECT_FALSE(this->directedGraph.isEdge(this->labels[0], this->labels[3]));
}

TYPED_TEST(VertexLabeledGraph, isEdge_inexistentLabels_throwInvalidArgument) {
    EXPECT_THROW(
        this->directedGraph.isEdge(this->unusedLabels[0], this->labels[0]),
        std::invalid_argument);
    EXPECT_THROW(
        this->directedGraph.isEdge(this->labels[0], this->unusedLabels[0]),
        std::invalid_argument);
}


TYPED_TEST(VertexLabeledGraph, removeEdge_existentLabelsAndEdge_edgeDoesntExist) {
    this->directedGraph.addEdgeIdx(0, 1);
    this->directedGraph.addEdgeIdx(0, 2);

    this->directedGraph.removeEdge(this->labels[0], this->labels[2]);

    EXPECT_FALSE(this->directedGraph.isEdgeIdx(0, 2));
}

TYPED_TEST(VertexLabeledGraph, removeEdge_existentLabelsAndSelfLoop_selfLoopDoesntExist) {
    this->directedGraph.addEdgeIdx(0, 1);
    this->directedGraph.addEdgeIdx(0, 0);

    this->directedGraph.removeEdge(this->labels[0], this->labels[0]);

    EXPECT_FALSE(this->directedGraph.isEdgeIdx(0, 0));
}

TYPED_TEST(VertexLabeledGraph, removeEdge_inexistentLabels_throwInvalidArgument) {
    EXPECT_THROW(
        this->directedGraph.removeEdge(this->unusedLabels[0], this->labels[0]),
        std::invalid_argument);
    EXPECT_THROW(
        this->directedGraph.removeEdge(this->labels[0], this->unusedLabels[0]),
        std::invalid_argument);
}


TYPED_TEST(VertexLabeledGraph, removeVertexFromEdgeList_existentLabel_edgesWithLabelRemoved) {
    this->directedGraph.addEdgeIdx(0, 1);
    this->directedGraph.addEdgeIdx(0, 0);
    this->directedGraph.addEdgeIdx(1, 2);
    this->directedGraph.addEdgeIdx(1, 0);
    this->directedGraph.addEdgeIdx(1, 0, true);
    this->directedGraph.addEdgeIdx(1, 3);

    this->directedGraph.removeVertexFromEdgeList(this->labels[0]);

    EXPECT_EQ(this->directedGraph.getOutEdgesOfIdx(0), BaseGraph::Successors({}));
    EXPECT_EQ(this->directedGraph.getOutEdgesOfIdx(1), BaseGraph::Successors({2, 3}));
    EXPECT_EQ(this->directedGraph.getEdgeNumber(), 2);
}

TYPED_TEST(VertexLabeledGraph, removeVertexFromEdgeList_inexistentLabel_throwInvalidArgument) {
    EXPECT_THROW(this->directedGraph.removeVertexFromEdgeList(this->unusedLabels[0]),
            std::invalid_argument);
}


TYPED_TEST(VertexLabeledGraph, convertIndicesListToLabels_validIndices_returnCorrectLabels) {
    std::list<BaseGraph::VertexIndex> indices = {1, 0, 3, 2};

    auto convertedIndices = this->directedGraph.convertIndicesListToLabels(indices);
    auto converted_it = convertedIndices.begin();

    for (BaseGraph::VertexIndex index: indices) {
        EXPECT_EQ(this->labels[index], *converted_it);
        converted_it++;
    }
}

TYPED_TEST(VertexLabeledGraph, convertIndicesListToLabels_vertexOutOfRange_throwOutOfRange) {
    EXPECT_THROW(auto convertedIndices = this->directedGraph.convertIndicesListToLabels({1, this->labels.size(), 0}),
            std::out_of_range);
}


TYPED_TEST(VertexLabeledGraph, convertIndicesVectorToLabels_validIndices_returnCorrectLabels) {
    std::vector<BaseGraph::VertexIndex> indices = {1, 0, 3, 2};

    auto convertedIndices = this->directedGraph.convertIndicesVectorToLabels(indices);
    auto converted_it = convertedIndices.begin();

    for (BaseGraph::VertexIndex index: indices) {
        EXPECT_EQ(this->labels[index], *converted_it);
        converted_it++;
    }
}

TYPED_TEST(VertexLabeledGraph, convertIndicesVectorToLabels_vertexOutOfRange_throwOutOfRange) {
    EXPECT_THROW(auto convertedIndices = this->directedGraph.convertIndicesVectorToLabels({1, this->labels.size(), 0}),
            std::out_of_range);
}


TYPED_TEST(VertexLabeledGraph, getOutEdgesOf_existentLabel_sameVertexIndices) {
    this->directedGraph.addEdgeIdx(0, 1);
    this->directedGraph.addEdgeIdx(0, 0);
    this->directedGraph.addEdgeIdx(0, 2);

    auto outEdges = this->directedGraph.getOutEdgesOf(this->labels[0]);
    auto outEdgesIdx = this->directedGraph.getOutEdgesOfIdx(0);
    ASSERT_EQ(outEdges.size(), outEdgesIdx.size());

    auto outEdgesIdx_it = outEdgesIdx.begin();
    for (auto& label: outEdges) {
        EXPECT_EQ(this->directedGraph.findVertexIndex(label), *outEdgesIdx_it);
        outEdgesIdx_it++;
    }
}

TYPED_TEST(VertexLabeledGraph, getOutEdgesOf_inexistentLabel_throwInvalidArgument) {
    EXPECT_THROW(this->directedGraph.getOutEdgesOf(this->unusedLabels[0]),
            std::invalid_argument);
}


TYPED_TEST(VertexLabeledGraph, dir_getDegree_existentLabel_degreeMatchWithCorrepondingIndex) {
    this->directedGraph.addEdgeIdx(0, 1);
    this->directedGraph.addEdgeIdx(0, 0);
    this->directedGraph.addEdgeIdx(0, 2);
    this->directedGraph.addEdgeIdx(1, 2);

    EXPECT_EQ(this->directedGraph.getOutDegree(this->labels[0]), this->directedGraph.getOutDegreeIdx(0));
    EXPECT_EQ(this->directedGraph.getInDegree(this->labels[2]), this->directedGraph.getInDegreeIdx(2));
}

TYPED_TEST(VertexLabeledGraph, dir_getDegree_inexistentLabel_throwInvalidArgument) {
    EXPECT_THROW(this->directedGraph.getOutDegree(this->unusedLabels[0]),
            std::invalid_argument);
    EXPECT_THROW(this->directedGraph.getInDegree(this->unusedLabels[0]),
            std::invalid_argument);
}

TYPED_TEST(VertexLabeledGraph, undir_getDegree_existentLabel_degreeMatchWithCorrepondingIndex) {
    this->undirectedGraph.addEdgeIdx(0, 1);
    this->undirectedGraph.addEdgeIdx(0, 0);
    this->undirectedGraph.addEdgeIdx(0, 2);

    EXPECT_EQ(this->undirectedGraph.getDegree(this->labels[0]), this->undirectedGraph.getDegreeIdx(0));
}

TYPED_TEST(VertexLabeledGraph, undir_getDegree_inexistentLabel_throwInvalidArgument) {
    EXPECT_THROW(this->undirectedGraph.getDegree(this->unusedLabels[0]),
            std::invalid_argument);
}
