cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

project(BaseGraph VERSION 0.1.0 LANGUAGES CXX
        DESCRIPTION "A extensible library that facilitates the reading, writing and manipulation of graphs.")

set(CXX_STANDARD 11)
set(CXX_STANDARD_REQUIRED on)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)


option(BUILD_TESTS "build gtest unit tests" off)
option(BUILD_DOCS "build doxygen documentation" off)
option(BUILD_BINDINGS "build python bindings" off)
option(BUILD_EXTENSIONS "build extensions " on)

include(packageutils)


if (BUILD_TESTS)
    find_package(GTest)
    if (NOT ${GTEST_FOUND})
        install_gtest()
    endif()
    enable_testing()
endif()

if (BUILD_BINDINGS)
    if(SKBUILD)
      # Scikit-Build does not add your site-packages to the search path
      # automatically, so we need to add it _or_ the pybind11 specific directory
      # here.
      execute_process(
        COMMAND "${PYTHON_EXECUTABLE}" -c
                "import pybind11; print(pybind11.get_cmake_dir())"
        OUTPUT_VARIABLE _tmp_dir
        OUTPUT_STRIP_TRAILING_WHITESPACE COMMAND_ECHO STDOUT)
      list(APPEND CMAKE_PREFIX_PATH "${_tmp_dir}")
    endif()

    find_package(pybind11 REQUIRED)
endif()


include_directories(include)
include_directories(extensions)

add_subdirectory(src)

if (BUILD_TESTS)
    add_subdirectory(tests)
endif()

set(BASEGRAPH_EXTENSIONS "")
set(PYBIND_MODULES "")
if (BUILD_EXTENSIONS)
    add_subdirectory(include/BaseGraph/extensions)
endif()

if (BUILD_DOCS)
    add_subdirectory(docs)
endif()

set(BASEGRAPH_ALL_TARGETS "BaseGraph_core;${BASEGRAPH_EXTENSIONS}")

if (BUILD_BINDINGS)
    file(GLOB_RECURSE CORE_BINDINGS_SRC "${PROJECT_SOURCE_DIR}/pybind_wrapper/*.cpp")
    pybind11_add_module(_core ${CORE_BINDINGS_SRC})
    list(PREPEND PYBIND_MODULES _core)

    if (SKBUILD)
        set(PYBIND_MODULES_OUTPUT_DIR "./basegraph")
    else
        set(PYBIND_MODULES_OUTPUT_DIR "${PROJECT_SOURCE_DIR}/basegraph")
    endif()

    foreach (pybind_module ${PYBIND_MODULES})
        target_link_libraries(${pybind_module} PRIVATE BaseGraph_core)
        set_target_properties(${pybind_module} PROPERTIES CXX_STANDARD 14)
        target_compile_definitions(${pybind_module} PRIVATE VERSION_INFO=${PROJECT_VERSION})

        set_target_properties(${pybind_module}
            PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${PYBIND_MODULES_OUTPUT_DIR}")
    endforeach()
    foreach (target ${BASEGRAPH_ALL_TARGETS})
        set_target_properties(${target} PROPERTIES POSITION_INDEPENDENT_CODE TRUE)
    endforeach()

    if (SKBUILD)
        install(TARGETS ${PYBIND_MODULES} DESTINATION ./basegraph)
    endif()
endif()


if (NOT SKBUILD)
    # Install BaseGraph on system
    include(CMakePackageConfigHelpers)
    include(GNUInstallDirs)

    foreach (target ${BASEGRAPH_ALL_TARGETS})
        target_include_directories(
            ${target} PUBLIC
           "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
           "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
        )
    endforeach()

    # Install headers
    install(
        DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include/BaseGraph"
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp"
        PATTERN "build" EXCLUDE
        PATTERN "test_*" EXCLUDE
        PATTERN "CMake*" EXCLUDE
    )

    # Install targets
    install(
        TARGETS ${BASEGRAPH_ALL_TARGETS}
        EXPORT ${PROJECT_NAME}-targets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )

    # Install export target and config for find_package
    set(BASEGRAPH_EXPORT_DIR "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}")
    install(EXPORT ${PROJECT_NAME}-targets DESTINATION ${BASEGRAPH_EXPORT_DIR})

    write_basic_package_version_file(
        "${CMAKE_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY SameMajorVersion
    )
    configure_package_config_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in"
        "${CMAKE_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
        INSTALL_DESTINATION ${BASEGRAPH_EXPORT_DIR}
    )
    install(FILES
        "${CMAKE_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
        "${CMAKE_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
        DESTINATION ${BASEGRAPH_EXPORT_DIR})
endif()
