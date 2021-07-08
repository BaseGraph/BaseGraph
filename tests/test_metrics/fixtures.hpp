#include "gtest/gtest.h"
#include "BaseGraph/directedgraph.h"
#include "BaseGraph/undirectedgraph.h"


class UndirectedHouseGraph: public::testing::Test{
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
            graph.addEdgeIdx(0, 2);
            graph.addEdgeIdx(0, 3);
            graph.addEdgeIdx(1, 2);
            graph.addEdgeIdx(1, 3);
            graph.addEdgeIdx(1, 4);
            graph.addEdgeIdx(2, 3);
            graph.addEdgeIdx(3, 4);
            graph.addEdgeIdx(3, 5);
        }
};


class DirectedHouseGraph: public::testing::Test{
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
            graph.addEdgeIdx(0, 2);
            graph.addEdgeIdx(3, 0);
            graph.addEdgeIdx(1, 2);
            graph.addEdgeIdx(3, 1);
            graph.addEdgeIdx(1, 4);
            graph.addReciprocalEdgeIdx(2, 3);
            graph.addEdgeIdx(4, 3);
            graph.addEdgeIdx(3, 5);
        }
};


class TreeLikeGraph: public::testing::Test{
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
        void SetUp(){
            graph.addEdgeIdx(0, 1);
            graph.addEdgeIdx(0, 2);
            graph.addEdgeIdx(1, 3);
            graph.addEdgeIdx(1, 4);
            graph.addEdgeIdx(2, 4);
            graph.addEdgeIdx(2, 5);
            graph.addEdgeIdx(3, 6);
            graph.addEdgeIdx(4, 6);
            graph.addEdgeIdx(5, 6);
            graph.addEdgeIdx(6, 7);
        }
};

class ThreeComponentsGraph: public::testing::Test{
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
        void SetUp(){
            graph.addEdgeIdx(0, 1);
            graph.addEdgeIdx(1, 2);
            graph.addEdgeIdx(2, 3);

            graph.addEdgeIdx(4, 5);
            graph.addEdgeIdx(5, 6);
            graph.addEdgeIdx(6, 4);
            graph.addEdgeIdx(6, 7);
            graph.addEdgeIdx(7, 8);
            graph.addEdgeIdx(7, 9);
        }
};
