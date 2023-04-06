#include "BaseGraph/algorithms/graphpaths.h"
#include <BaseGraph/fileio.h>
#include <BaseGraph/undirectedgraph.h>
#include <igraph/igraph.h>

#include <bits/chrono.h>
#include <chrono>

class Timer {
  public:
    Timer() { start = std::chrono::high_resolution_clock::now(); }
    void stop() { end = std::chrono::high_resolution_clock::now(); }
    double durationMs() {
        return .0001 *
               (std::chrono::time_point_cast<std::chrono::microseconds>(end)
                    .time_since_epoch()
                    .count() -
                std::chrono::time_point_cast<std::chrono::microseconds>(start)
                    .time_since_epoch()
                    .count());
    }

  private:
    std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
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
    return std::make_pair(mean, variance);
}

int main(int argc, char *argv[]) {
    // if (argc < 2) {
    // fprintf(stderr, "Missing program argument: edge list file name.");
    // return 1;
    //}
    // std::string edgeListFileName = argv[1];
    std::string edgeListFileName =
        "/home/simon/dev/base_graph/benchmarks/assets/undirected_graph.txt";
    size_t vertexNumber = 198;
    size_t sourceVertex = 50;

    {
        Timer timer;

        BaseGraph::UndirectedGraph graph =
            BaseGraph::io::loadUndirectedTextEdgeList(edgeListFileName).first;

        auto res =
            BaseGraph::algorithms::findGeodesicsFromVertex(graph, sourceVertex);

        timer.stop();
        printf("BaseGraph time=%f ms\n", timer.durationMs());
    }

    {
        Timer timer;

        igraph_integer_t v = vertexNumber;
        igraph_t graph;
        auto fileStream = fopen(edgeListFileName.c_str(), "r");
        igraph_read_graph_edgelist(&graph, fileStream, v, false);
        fclose(fileStream);

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
        printf("igraph time=%f ms\n", timer.durationMs());

        igraph_destroy(&graph);
        igraph_vector_destroy(&matrix.data);
    }
    return 0;
}
