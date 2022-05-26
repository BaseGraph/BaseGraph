macro(SUBDIRLIST result curdir)
  file(GLOB children RELATIVE ${curdir} ${curdir}/*)
  set(dirlist "")
  foreach(child ${children})
    if(IS_DIRECTORY ${curdir}/${child})
      LIST(APPEND dirlist ${child})
    endif()
  endforeach()
  set(${result} ${dirlist})
endmacro()

macro(add_gtest _testname)
    add_test(${_testname} ${_testname})
    target_link_libraries(${_testname} gtest gtest_main BaseGraph_core)
endmacro()

macro(add_extension _extension)
    list(APPEND BASEGRAPH_EXTENSIONS ${_extension})
    set(BASEGRAPH_EXTENSIONS ${BASEGRAPH_EXTENSIONS} PARENT_SCOPE)
endmacro()

macro(add_pybind_extension _module)
    list(APPEND PYBIND_MODULES ${_module})
    set(PYBIND_MODULES ${PYBIND_MODULES} PARENT_SCOPE)
endmacro()

macro(install_gtest)
    # Code from https://github.com/google/googletest/blob/master/googletest/README.md
    # Download and unpack googletest at configure time
    configure_file(${PROJECT_SOURCE_DIR}/cmake/GTestCMakeLists.txt.in googletest-download/CMakeLists.txt)
    execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
      RESULT_VARIABLE result
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
    if(result)
      message(FATAL_ERROR "CMake step for googletest failed: ${result}")
    endif()
    execute_process(COMMAND ${CMAKE_COMMAND} --build .
      RESULT_VARIABLE result
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
    if(result)
      message(FATAL_ERROR "Build step for googletest failed: ${result}")
    endif()

    # Prevent overriding the parent project's compiler/linker
    # settings on Windows
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

    # Add googletest directly to our build. This defines
    # the gtest and gtest_main targets.
    add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
                     ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
                     EXCLUDE_FROM_ALL)

    # The gtest/gtest_main targets carry header search path
    # dependencies automatically when using CMake 2.8.11 or
    # later. Otherwise we have to add them here ourselves.
    if (CMAKE_VERSION VERSION_LESS 2.8.11)
      include_directories("${gtest_SOURCE_DIR}/include")
    endif()
endmacro()
