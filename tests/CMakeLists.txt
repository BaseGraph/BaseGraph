find_package(GTest)

if (NOT ${GTEST_FOUND})
    include(FetchContent)
    FetchContent_Declare(
      googletest
      # Specify the commit you depend on and update it regularly.
      URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
    )
    # For Windows: Prevent overriding the parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

    # Manually add subdirectory to avoid installing gtest with BaseGraph library (EXCLUDE_FROM_ALL option)
    FetchContent_GetProperties(googletest)
    if(NOT googletest_POPULATED)
      FetchContent_Populate(googletest)
      add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR} EXCLUDE_FROM_ALL)
    endif()
endif()

set(TEST_FILES_NAMES
    test_directed_labeled_graph
    test_directed_multigraph
    test_directed_weighted_graph
    test_directedgraph
    test_graph_fileIO
    test_paths
    test_undirected_labeled_graph
    test_undirected_multigraph
    test_undirected_weighted_graph
    test_undirectedgraph
)

foreach(TEST_NAME ${TEST_FILES_NAMES})
    add_executable(${TEST_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/${TEST_NAME}.cpp")
    target_link_libraries(${TEST_NAME} core gtest gtest_main)
    add_test(${TEST_NAME} ${TEST_NAME})
endforeach()
