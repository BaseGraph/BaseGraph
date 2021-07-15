file(GLOB_RECURSE BaseGraph_SRC ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
add_library(BaseGraph ${BaseGraph_SRC})

set_target_properties(BaseGraph PROPERTIES LINKER_LANGUAGE CXX)
