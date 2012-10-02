include_directories(${NFP_CORE_DIR})
include_directories(${NFP_ALGOS_DIR})

include(${QT_USE_FILE})

link_directories(${GLOG_LIBRARY_PATH})

add_executable(nfpshm nfpshm.cpp)


target_link_libraries(nfpshm
        nfpcore
        nfpalgos
        ${GLOG_LIBRARY}
        ${QT_LIBRARIES}
) 
