include_directories("../core")
include_directories("../algos")
include_directories(${QT_INCLUDE_DIR})

add_executable(nfpshm nfpshm.cpp)

target_link_libraries(nfpshm
        nfpcore
        nfpalgos
        glog
        QtCore) 
