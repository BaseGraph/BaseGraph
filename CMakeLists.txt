cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

project(BaseGraph VERSION 0.4.1 LANGUAGES CXX
        DESCRIPTION "A extensible library that facilitates the reading, writing and manipulation of graphs.")

option(BUILD_TESTS "build gtest unit tests" off)
option(BUILD_DOCS "build doxygen documentation" off)
option(BUILD_BINDINGS "build python bindings" off)

if (BUILD_TESTS)
    set(CXX_STANDARD 14)
else()
    set(CXX_STANDARD 11)
endif()
set(CXX_STANDARD_REQUIRED)
set(CMAKE_CXX_EXTENSIONS OFF)
if (APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -stdlib=libc++")
endif()

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

add_library(core INTERFACE)
target_include_directories(core INTERFACE
    "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>"
    "$<INSTALL_INTERFACE:include>"
)

if (BUILD_BINDINGS OR SKBUILD)
    set(CMAKE_BUILD_TYPE Release)
    add_subdirectory(python)
endif()
if (BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
if (BUILD_DOCS)
    add_subdirectory(docs)
endif()

# Install BaseGraph on system
if (NOT SKBUILD)
    include(CMakePackageConfigHelpers)
    include(GNUInstallDirs)

    # Install headers
    install(
        DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include/BaseGraph"
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )

    # Install targets
    set(BASEGRAPH_EXPORT_DIR "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}")
    install(
        TARGETS core
        EXPORT ${PROJECT_NAME}-targets
    )

    # Install export target and config for find_package
    set(BASEGRAPH_EXPORT_DIR "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}")
    install(
        EXPORT ${PROJECT_NAME}-targets
        NAMESPACE ${PROJECT_NAME}::
        DESTINATION ${BASEGRAPH_EXPORT_DIR}
    )

    # Install version information
    configure_package_config_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in"
        "${CMAKE_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
        INSTALL_DESTINATION ${BASEGRAPH_EXPORT_DIR}
    )
    write_basic_package_version_file(
        "${CMAKE_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY SameMajorVersion
    )
    install(FILES
        "${CMAKE_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
        "${CMAKE_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
        DESTINATION ${BASEGRAPH_EXPORT_DIR}
    )
endif()
