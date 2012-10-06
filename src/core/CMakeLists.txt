file(GLOB core_src "*.cpp")

add_library(core_obj OBJECT ${core_src})
set_target_properties(core_obj PROPERTIES COMPILE_FLAGS "-fno-strict-aliasing")

# install(TARGETS nfpcore DESTINATION lib)

if(${BUILD_TESTS})
        add_subdirectory(test)
endif(${BUILD_TESTS})
