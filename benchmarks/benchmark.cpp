#include "BaseGraph/algorithms/graphpaths.h"
#include <BaseGraph/fileio.h>
#include <BaseGraph/undirectedgraph.h>
#include <igraph/igraph.h>

#include <bits/chrono.h>
#include <chrono>

class Timer {
    std::chrono::time_point<std::chrono::high_resolution_clock> startPoint,
        endPoint;

  public:
    void start() { startPoint = std::chrono::high_resolution_clock::now(); }
    void stop() { endPoint = std::chrono::high_resolution_clock::now(); }
    double durationMs() {
        return .0001 * (std::chrono::time_point_cast<std::chrono::microseconds>(
                            endPoint)
                            .time_since_epoch()
                            .count() -
                        std::chrono::time_point_cast<std::chrono::microseconds>(
                            startPoint)
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
    for (size_t i=0; i<n; i++) {
        Timer timer;
        func(timer);
        times[i] = timer.durationMs();
    }
    auto average_std = getAverageStd(times);
    printf("%s:\t%f±%f ms\n", lib.c_str(), average_std.first, average_std.second);
}

int main(int argc, char *argv[]) {
    // if (argc < 2) {
    // fprintf(stderr, "Missing program argument: edge list file name.");
    // return 1;
    //}
    // std::string edgeListFileName = argv[1];
    std::string edgeListFileName = "../assets/undirected_graph.txt";
    size_t vertexNumber = 198;
    size_t sourceVertex = 50;

    size_t benchmarkSampleSize = 500;
    std::string basegraphName = "BaseGraph";
    std::string igraphName = "igraph\t";

    printf("Benchmark - Construct graph from text file\n");
    benchmark(
        [&](Timer &timer) {
            timer.start();
            BaseGraph::io::loadUndirectedTextEdgeList(edgeListFileName,
                                                      vertexNumber);
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


    printf("\nBenchmark - Shortest paths\n");
    benchmark(
        [&](Timer &timer) {
            auto graph = BaseGraph::io::loadUndirectedTextEdgeList(
                             edgeListFileName, vertexNumber)
                             .first;
            timer.start();
            BaseGraph::algorithms::findGeodesicsFromVertex(graph, sourceVertex);
            timer.stop();
        },
        basegraphName, benchmarkSampleSize);
    benchmark(
        [&](Timer &timer) {
            igraph_integer_t v = vertexNumber;
            igraph_t graph;
            auto fileStream = fopen(edgeListFileName.c_str(), "r");
            igraph_read_graph_edgelist(&graph, fileStream, v, false);
            fclose(fileStream);

            timer.start();

            igraph_matrix_t matrix;
            igraph_vector_t matrixValues;
            igraph_vector_init(&matrix.data, vertexNumber);
            matrix.nrow = 1;
            matrix.ncol = vertexNumber;

            igraph_vs_t source, destination;
            source.type = IGRAPH_VS_1;
            source.data.vid = sourceVertex;
            destination.type = IGRAPH_VS_RANGE;
            destination.data.range.start = 0;
            destination.data.range.end = vertexNumber;

            igraph_distances(&graph, &matrix, source, destination, IGRAPH_ALL);

            timer.stop();

            igraph_destroy(&graph);
            igraph_vector_destroy(&matrix.data);
        },
        igraphName, benchmarkSampleSize);

    return 0;
}
