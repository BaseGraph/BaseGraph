file(GLOB_RECURSE BASEGRAPH_SRC "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
add_library(BaseGraph_core ${BASEGRAPH_SRC})

set_target_properties(BaseGraph_core PROPERTIES LINKER_LANGUAGE CXX)
