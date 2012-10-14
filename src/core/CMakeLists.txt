configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/version.cpp.in
  ${CMAKE_CURRENT_BINARY_DIR}/version.cpp
  @ONLY
)

file(GLOB core_src "*.cpp")
list(APPEND core_src ${CMAKE_CURRENT_BINARY_DIR}/version.cpp)

add_library(core_obj OBJECT ${core_src})
set_target_properties(core_obj PROPERTIES COMPILE_FLAGS "-fno-strict-aliasing")

# install(TARGETS nfpcore DESTINATION lib)

if(${BUILD_TESTING})
        add_subdirectory(test)
endif()
