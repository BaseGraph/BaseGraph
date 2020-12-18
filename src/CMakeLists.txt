set(PGL_SRC
    directedgraph.cpp
    undirectedgraph.cpp
    metrics/general.cpp
    metrics/directed.cpp
    metrics/undirected.cpp
    algorithms/graphpaths.cpp
    algorithms/randomgraphs.cpp
    algorithms/percolation.cpp
)


add_library(PGL ${PGL_SRC})

set_target_properties(PGL PROPERTIES LINKER_LANGUAGE CXX)
