cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

project(BaseGraph VERSION 1.0.0 LANGUAGES CXX
        DESCRIPTION "A minimalist library that facilitates the manipulation, the reading and writing and the computation of metrics of graphs.")

set(CXX_STANDARD 14)
set(CXX_STANDARD_REQUIRED on)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)


option(BUILD_TESTS "build gtest unit tests" off)
option(BUILD_DOCS "build doxygen documentation" off)
option(BUILD_BINDINGS "build python bindings" off)
option(BUILD_EXTENSIONS "build extensions " on)

include(packageutils)


if (BUILD_TESTS)
    enable_testing()
    set(allGTests "")
endif()

include_directories(include)
include_directories(extensions)

add_subdirectory(src)

set(BaseGraphExtensions "")

if (BUILD_EXTENSIONS)
    add_subdirectory(include/BaseGraph/extensions)
endif()

if (BUILD_TESTS)
    add_subdirectory(tests)
endif()

if (BUILD_DOCS)
    add_subdirectory(docs)
endif()

if (BUILD_BINDINGS)
    find_package(pybind11 REQUIRED)

    file(GLOB_RECURSE BaseGraphCore_bind "${PROJECT_SOURCE_DIR}/pybind_wrapper/*.cpp")

    pybind11_add_module(basegraph ${BaseGraphCore_bind})
    target_link_libraries(basegraph PRIVATE BaseGraph ${BaseGraphExtensions})
endif()


#export_target(${PROJECT_NAME} ${PROJECT_SOURCE_DIR}/include)
set(BaseGraphAllTargets "BaseGraph;${BaseGraphExtensions}")

foreach (target ${BaseGraphAllTargets})
    target_include_directories(
        ${target} PUBLIC
       "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
       "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
    )
endforeach()


# install header
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/BaseGraph DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# install libaries
install(
    TARGETS ${BaseGraphAllTargets}
    EXPORT ${PROJECT_NAME}-targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# install export target and config for find_package
set(BASEGRAPH_EXPORT_DIR ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME})
install(EXPORT ${PROJECT_NAME}-targets DESTINATION ${BASEGRAPH_EXPORT_DIR})

include(CMakePackageConfigHelpers)
configure_package_config_file(
    "${PROJECT_NAME}Config.cmake.in" "${CMAKE_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
    INSTALL_DESTINATION ${BASEGRAPH_EXPORT_DIR}
)
install(FILES "${CMAKE_BINARY_DIR}/${PROJECT_NAME}Config.cmake" DESTINATION ${BASEGRAPH_EXPORT_DIR})
