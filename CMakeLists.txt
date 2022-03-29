cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

project(BaseGraph VERSION 1.0.0 LANGUAGES CXX
        DESCRIPTION "A minimalist library that facilitates the manipulation, the reading and writing and the computation of metrics of graphs.")

set(CXX_STANDARD 11)
set(CXX_STANDARD_REQUIRED on)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})


option(BUILD_TESTS "build gtest unit tests" off)
option(BUILD_DOCS  "build doxygen documentation" off)


include_directories(include)
add_subdirectory(src)

if (BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

if (BUILD_DOCS)
    add_subdirectory(docs)
endif()

# Create package
include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

# Snippet taken from https://stackoverflow.com/questions/33462209/making-a-cmake-library-accessible-by-other-cmake-packages-automatically
function(my_export_target _target _include_dir)
    file(
        WRITE "${CMAKE_CURRENT_BINARY_DIR}/${_target}Config.cmake"
        "
            include(\"\$\{CMAKE_CURRENT_LIST_DIR\}/${_target}Targets.cmake\")
            set_property(
                TARGET ${_target}
                APPEND PROPERTY
                    INTERFACE_INCLUDE_DIRECTORIES \"${_include_dir}\"
            )
        "
    )

    export(TARGETS ${_target} FILE "${CMAKE_CURRENT_BINARY_DIR}/${_target}Targets.cmake")

    # NOTE: The following call can pollute your PC's CMake package registry
    #       See comments/alternatives below
    export(PACKAGE ${_target})
endfunction(my_export_target)

my_export_target(${PROJECT_NAME} ${PROJECT_SOURCE_DIR}/include)
