add_library(BaseGraph_metrics directed.cpp undirected.cpp general.cpp)
target_link_libraries(BaseGraph_metrics BaseGraph)
add_extension(BaseGraph_metrics)


if (BUILD_TESTS)
    file(GLOB_RECURSE METRICS_TEST_SRC ${CMAKE_CURRENT_SOURCE_DIR}/test_metrics/*.cpp)

    add_executable(test_metrics ${METRICS_TEST_SRC})
    target_link_libraries(test_metrics BaseGraph_metrics)
    add_gtest(test_metrics)
endif()
