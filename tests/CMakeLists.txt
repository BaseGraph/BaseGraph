find_package(GTest)

if (NOT ${GTEST_FOUND})
    include(FetchContent)
    FetchContent_Declare(
      googletest
      # Specify the commit you depend on and update it regularly.
      URL https://github.com/google/googletest/archive/5376968f6948923e2411081fd9372e71a59d8e77.zip
    )
    # For Windows: Prevent overriding the parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
endif()

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
