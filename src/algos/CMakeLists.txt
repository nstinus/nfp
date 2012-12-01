file(GLOB algos_src "*.cpp")
add_library(${NFP_ALGOS_LIBRARY} ${algos_src})
install(TARGETS ${NFP_ALGOS_LIBRARY} DESTINATION lib)