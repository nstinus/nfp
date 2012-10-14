configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/version.cpp.in
  ${CMAKE_CURRENT_BINARY_DIR}/version.cpp
  @ONLY
)

file(GLOB core_src "*.cpp")
list(APPEND core_src ${CMAKE_CURRENT_BINARY_DIR}/version.cpp)

add_library(${NFP_CORE_LIBRARY} ${core_src})
set_target_properties(${NFP_CORE_LIBRARY} PROPERTIES COMPILE_FLAGS "-fno-strict-aliasing")

install(TARGETS ${NFP_CORE_LIBRARY} DESTINATION lib)

if(${BUILD_TESTING})
  add_subdirectory(test)
endif()
