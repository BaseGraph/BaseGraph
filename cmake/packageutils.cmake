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
    list(APPEND allGTests ${_testname})
    set(allGTests ${allGTests} PARENT_SCOPE)
endmacro()

macro(add_extension _extension)
    set_target_properties(${_extension} PROPERTIES POSITION_INDEPENDENT_CODE TRUE)
    list(APPEND BaseGraphExtensions ${_extension})
    set(BaseGraphExtensions ${BaseGraphExtensions} PARENT_SCOPE)
endmacro()


include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

# Snippet taken from https://stackoverflow.com/questions/33462209/making-a-cmake-library-accessible-by-other-cmake-packages-automatically
function(export_target _target _include_dir)
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
endfunction(export_target)
