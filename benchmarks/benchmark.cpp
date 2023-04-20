#include <BaseGraph/types.h>
#include <BaseGraph/undirected_weighted_graph.hpp>
#include <bits/chrono.h>
#include <chrono>
#include <random>
#include <string>

#include "BaseGraph/algorithms/paths.hpp"
#include "BaseGraph/extensions/random/randomgraphs.h"
#include "BaseGraph/fileio.hpp"
#include "BaseGraph/undirected_graph.hpp"

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/breadth_first_search.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <igraph/igraph.h>
#include <igraph/igraph_interface.h>

class Timer {
    std::chrono::time_point<std::chrono::high_resolution_clock> startPoint,
        endPoint;

  public:
    void start() { startPoint = std::chrono::high_resolution_clock::now(); }
    void stop() { endPoint = std::chrono::high_resolution_clock::now(); }
    double durationMs() {
        return (
            std::chrono::time_point_cast<std::chrono::milliseconds>(endPoint)
                .time_since_epoch()
                .count() -
            std::chrono::time_point_cast<std::chrono::milliseconds>(startPoint)
                .time_since_epoch()
                .count());
    }
};

// https://stackoverflow.com/questions/17052395/calculate-the-running-standard-deviation/17053010
std::pair<double, double> getAverageStd(const std::vector<double> &vec) {
    double mean = 0, M2 = 0, variance = 0;

    size_t n = vec.size();
    for (size_t i = 0; i < n; ++i) {
        double delta = vec[i] - mean;
        mean += delta / (i + 1);
        M2 += delta * (vec[i] - mean);
        variance = M2 / (i + 1);
    }
    return std::make_pair(mean, std::sqrt(variance));
}

void benchmark(const std::function<void(Timer &)> &func, const std::string &lib,
               size_t n) {

    std::vector<double> times(n);
    for (size_t i = 0; i < n; i++) {
        Timer timer;
        func(timer);
        times[i] = timer.durationMs();
    }
    auto average_std = getAverageStd(times);
    printf("%s:\t%.4f±%.4f ms\n", lib.c_str(), average_std.first,
           average_std.second / std::sqrt(n));
}

typedef boost::adjacency_list<boost::listS, boost::vecS, boost::no_property>
    BoostUndirectedGraph;
typedef boost::property<boost::edge_weight_t, double> EdgeWeightProperty;
typedef boost::adjacency_list<boost::listS, boost::vecS, boost::undirectedS,
                              boost::no_property, EdgeWeightProperty>
    BoostWeightedGraph;

int main(int argc, char *argv[]) {
    // if (argc < 2) {
    // fprintf(stderr, "Missing program argument: edge list file name.");
    // return 1;
    //}
    // std::string edgeListFileName = argv[1];
    std::string edgeListFileName = "assets/undirected_graph.txt";
    size_t vertexNumber = 198;
    size_t sourceVertex = 50;

    size_t benchmarkSampleSize = 100;
    std::string basegraphName = "BaseGraph";
    std::string igraphName = "igraph\t";
    std::string boostName = "Boost Graph";

    printf("Benchmarking BaseGraph with %lu iterations.\n\n",
           benchmarkSampleSize);

    printf("Benchmark - Parse text file\n");
    benchmark(
        [&](Timer &timer) {
            timer.start();
            BaseGraph::io::loadTextEdgeList<BaseGraph::LabeledUndirectedGraph,
                                            BaseGraph::NoLabel>(
                edgeListFileName);
            timer.stop();
        },
        basegraphName, benchmarkSampleSize);
    benchmark(
        [&](Timer &timer) {
            timer.start();

            igraph_integer_t v = vertexNumber;
            igraph_t graph;
            auto fileStream = fopen(edgeListFileName.c_str(), "r");
            igraph_read_graph_edgelist(&graph, fileStream, v, false);
            fclose(fileStream);

            timer.stop();

            igraph_destroy(&graph);
        },
        igraphName, benchmarkSampleSize);

    // Graphs used to compare libraries
    // BaseGraph
    vertexNumber = 10000;
    BaseGraph::random::rng.seed(420);
    auto basegraphGraph = BaseGraph::random::generateSmallWorldRandomGraph(
        vertexNumber, 3, 0.0001);

    // igraph
    igraph_t igraphGraph;
    igraph_empty(&igraphGraph, vertexNumber, false);
    // boost::graph
    boost::adjacency_list<boost::listS, boost::vecS, boost::undirectedS>
        boostGraph;

    for (auto edge : basegraphGraph.edges()) {
        igraph_add_edge(&igraphGraph, edge.first, edge.second);
        boost::add_edge(edge.first, edge.second, boostGraph);
    }

    printf("\nBenchmark - Undirected shortest paths\n");
    benchmark(
        [&](Timer &timer) {
            timer.start();
            BaseGraph::algorithms::findVertexPredecessors(basegraphGraph,
                                                          sourceVertex);
            timer.stop();
        },
        basegraphName, benchmarkSampleSize);
    benchmark(
        [&](Timer &timer) {
            timer.start();

            igraph_matrix_t shortestPaths;
            igraph_vector_init(&shortestPaths.data, vertexNumber);
            shortestPaths.nrow = 1;
            shortestPaths.ncol = vertexNumber;

            igraph_vs_t source, destination;
            source.type = IGRAPH_VS_1;
            source.data.vid = sourceVertex;
            destination.type = IGRAPH_VS_RANGE;
            destination.data.range.start = 0;
            destination.data.range.end = vertexNumber;

            igraph_distances(&igraphGraph, &shortestPaths, source, destination,
                             IGRAPH_ALL);

            timer.stop();

            igraph_vector_destroy(&shortestPaths.data);
        },
        igraphName, benchmarkSampleSize);
    benchmark(
        [&](Timer &timer) {
            timer.start();
            std::vector<size_t> distances(num_vertices(boostGraph));
            auto recorder =
                record_distances(distances.data(), boost::on_tree_edge{});

            boost::breadth_first_search(boostGraph,
                                        boost::vertex(sourceVertex, boostGraph),
                                        visitor(make_bfs_visitor(recorder)));
            timer.stop();
        },
        boostName, benchmarkSampleSize);

    std::mt19937 rng(420);
    BaseGraph::UndirectedWeightedGraph basegraphWeightedGraph(
        basegraphGraph.getSize());

    igraph_vector_t igraphWeights;
    igraph_vector_init(&igraphWeights, basegraphGraph.getEdgeNumber());

    BoostWeightedGraph boostWeightedGraph(basegraphGraph.getSize());
    boost::property_map<BoostWeightedGraph, boost::edge_weight_t>::type
        weightmap = get(boost::edge_weight, boostWeightedGraph);

    size_t edgeCount = 0;
    for (auto edge : basegraphGraph.edges()) {
        auto weight = std::abs(std::normal_distribution<double>(10, 2)(rng));

        basegraphWeightedGraph.addEdge(edge.first, edge.second, weight);

        igraph_vector_set(&igraphWeights, edgeCount++, weight);

        bool inserted;
        BoostWeightedGraph::edge_descriptor e;
        boost::tie(e, inserted) =
            boost::add_edge(edge.first, edge.second, boostWeightedGraph);
        weightmap[e] = weight;
    }

    printf("\nBenchmark - Dijkstra\n");
    benchmark(
        [&](Timer &timer) {
            timer.start();
            auto res = BaseGraph::algorithms::findGeodesicsDijkstra(
                           basegraphWeightedGraph, sourceVertex)
                           .first;
            timer.stop();
        },
        basegraphName, benchmarkSampleSize);
    benchmark(
        [&](Timer &timer) {
            timer.start();

            igraph_matrix_t shortestPaths;
            igraph_vector_init(&shortestPaths.data, vertexNumber);
            shortestPaths.nrow = 1;
            shortestPaths.ncol = vertexNumber;

            igraph_vs_t source, destination;
            source.type = IGRAPH_VS_1;
            source.data.vid = sourceVertex;
            destination.type = IGRAPH_VS_RANGE;
            destination.data.range.start = 0;
            destination.data.range.end = vertexNumber;

            igraph_distances_dijkstra(&igraphGraph, &shortestPaths, source,
                                      destination, &igraphWeights, IGRAPH_OUT);
            timer.stop();

            igraph_vector_destroy(&shortestPaths.data);
        },
        igraphName, benchmarkSampleSize);
    benchmark(
        [&](Timer &timer) {
            timer.start();
            int n = boost::num_vertices(boostWeightedGraph);
            std::vector<double> dist_map(n);
            std::vector<BoostWeightedGraph::vertex_descriptor> pred(n);
            auto s = boost::vertex(sourceVertex, boostWeightedGraph);

            boost::dijkstra_shortest_paths(
                boostWeightedGraph, s,
                boost::predecessor_map(&pred[0]).distance_map(&dist_map[0]));
            timer.stop();
        },
        boostName, benchmarkSampleSize);

    igraph_destroy(&igraphGraph);
    igraph_vector_destroy(&igraphWeights);
    return 0;
}
