find_package(GTest REQUIRED)
enable_testing()

set(TEST_FILES_NAMES 
    test_directedgraph
    test_directed_multigraph
    test_undirectedgraph
    test_undirected_multigraph
    test_edgelabeled_directedgraph
    test_edgelabeled_undirectedgraph
    test_graph_fileIO
)

foreach(TEST_NAME ${TEST_FILES_NAMES})
    add_executable(${TEST_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/${TEST_NAME}.cpp")
    target_link_libraries(${TEST_NAME} core gtest gtest_main)
    add_test(${TEST_NAME} ${TEST_NAME})
endforeach()
