include_directories(${NFP_CORE_DIR})
include_directories(${GTEST_DIR}/include)

add_executable(test_rating
        Rating_test.cpp
        $<TARGET_OBJECTS:core_obj>
)

target_link_libraries(test_rating
    ${GTEST_LIB}
    ${GLOG_LIBRARY}
    ${QT_LIBRARIES}
) 
add_test("core/rating" test_rating)

add_executable(test_date Date_test.cpp)
target_link_libraries(test_date
    ${GTEST_LIB}
) 
add_test("core/date" test_date)
