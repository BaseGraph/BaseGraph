SUBDIRLIST(EXTENSIONS ${CMAKE_CURRENT_SOURCE_DIR})

foreach(subdir ${EXTENSIONS})
  add_subdirectory(${subdir})
endforeach()

set(allGTests ${allGTests} PARENT_SCOPE)
set(BaseGraphExtensions ${BaseGraphExtensions} PARENT_SCOPE)
