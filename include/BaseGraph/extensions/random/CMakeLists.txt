set(EXTENSION_NAME random)
add_library(${EXTENSION_NAME} rng.cpp randomgraphs.cpp)
target_link_libraries(${EXTENSION_NAME} core)
add_extension(${EXTENSION_NAME})


if (BUILD_BINDINGS)
    pybind11_add_module(_${EXTENSION_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/bind_random.cpp")
    target_link_libraries(_${EXTENSION_NAME} PRIVATE ${EXTENSION_NAME})
    add_pybind_extension(_${EXTENSION_NAME})
endif()

if (BUILD_TESTS)
    file(GLOB RANDOM_TEST_SRC "${CMAKE_CURRENT_SOURCE_DIR}/test_random/*.cpp")

    add_executable(test_random ${RANDOM_TEST_SRC})
    target_link_libraries(test_random ${EXTENSION_NAME})
    add_gtest(test_random)
endif()
