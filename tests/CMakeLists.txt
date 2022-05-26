file(GLOB_RECURSE CORE_TEST_SRC ${CMAKE_CURRENT_SOURCE_DIR}/test_core/*.cpp)

add_executable(test_core ${CORE_TEST_SRC})
add_gtest(test_core)
