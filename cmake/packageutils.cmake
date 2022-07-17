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
    include(FetchContent)
    FetchContent_Declare(
      googletest
      # Specify the commit you depend on and update it regularly.
      URL https://github.com/google/googletest/archive/5376968f6948923e2411081fd9372e71a59d8e77.zip
    )
    # For Windows: Prevent overriding the parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
endmacro()
