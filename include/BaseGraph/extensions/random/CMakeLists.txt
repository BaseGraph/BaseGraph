add_library(BaseGraph_random rng.cpp randomgraphs.cpp)
target_link_libraries(BaseGraph_random BaseGraph_core)
add_extension(BaseGraph_random)

if (BUILD_TESTS)
    file(GLOB RANDOM_TEST_SRC "${CMAKE_CURRENT_SOURCE_DIR}/test_random/*.cpp")

    add_executable(test_random ${RANDOM_TEST_SRC})
    target_link_libraries(test_random BaseGraph_random)
    add_gtest(test_random)
endif()
