include_directories(${NFP_CORE_DIR})
include_directories(${GTEST_DIR}/include)

add_executable(test_rating Rating_test.cpp)
target_link_libraries(test_rating
    ${GTEST_LIB}
    nfpcore
    ${GLOG_LIBRARY}
    ${QT_LIBRARIES}
) 
add_test(rating test_rating)

add_executable(test_date Date_test.cpp)
target_link_libraries(test_date
    ${GTEST_LIB}
) 
add_test(date test_date)
