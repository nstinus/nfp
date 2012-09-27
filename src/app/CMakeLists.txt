include_directories("../core")
include_directories("../algos")
include(${QT_USE_FILE})

link_directories(${GLOG_LIBRARY_PATH})

add_executable(nfpshm nfpshm.cpp)


target_link_libraries(nfpshm
        nfpcore
        nfpalgos
        ${GLOG_LIBRARY}
        ${QT_LIBRARIES}) 
