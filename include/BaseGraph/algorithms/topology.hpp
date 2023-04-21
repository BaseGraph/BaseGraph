#include <unordered_map>
#include <unordered_set>

#include "BaseGraph/directed_graph.hpp"
#include "BaseGraph/types.h"
#include "BaseGraph/undirected_graph.hpp"

namespace BaseGraph {
namespace algorithms {

/**
 * Construct a Graph that only contains the edges in \p vertices.
 * @param vertices Vertices to include in the subgraph.
 * @return Subgraph without vertex remapping. The subgraph has
 *         the same number of vertices than the original graph.
 */
template <template <class...> class Graph, typename EdgeLabel>
Graph<EdgeLabel>
getSubgraph(const Graph<EdgeLabel> &graph,
              const std::unordered_set<VertexIndex> &vertices) {
    Graph<EdgeLabel> subgraph(graph.getSize());

    for (VertexIndex i : vertices) {
        graph.assertVertexInRange(i);
        for (VertexIndex j : graph.getOutNeighbours(i))
            if (vertices.find(j) != vertices.end())
                subgraph.addEdge(i, j, graph.getEdgeLabel(i, j));
    }
    return subgraph;
}

/**
 * Construct a Graph that only contains the edges in \p vertices.
 * @param vertices Vertices to include in the subgraph.
 * @return Directed subgraph and mapping of the original vertex indices
 *         to the subgraph vertex indices.
 */
template <template <class...> class Graph, typename EdgeLabel>
std::pair<Graph<EdgeLabel>, std::unordered_map<VertexIndex, VertexIndex>>
getSubgraphWithRemap(const Graph<EdgeLabel> &graph,
                       const std::unordered_set<VertexIndex> &vertices) {

    Graph<EdgeLabel> subgraph(vertices.size());
    std::unordered_map<VertexIndex, VertexIndex> newMapping;

    VertexIndex position = 0;
    for (VertexIndex vertex : vertices) {
        newMapping[vertex] = position;
        ++position;
    }

    for (VertexIndex i : vertices) {
        graph.assertVertexInRange(i);
        for (VertexIndex j : graph.getOutNeighbours(i))
            if (i <= j && vertices.find(j) != vertices.end())
                subgraph.addEdge(newMapping[i], newMapping[j],
                                 graph.getEdgeLabel(i, j));
    }
    return {std::move(subgraph), std::move(newMapping)};
}

} // namespace algorithms
} // namespace BaseGraph
