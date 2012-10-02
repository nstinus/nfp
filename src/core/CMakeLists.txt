
add_library(RATING_OBJECT OBJECT Rating.cpp)
set_target_properties(RATING_OBJECT PROPERTIES COMPILE_FLAGS "-fno-strict-aliasing")

add_library(nfpcore
        User.cpp
        ShmSegment.cpp
        RatingsShmSegment.cpp
        RatingsManager.cpp
        MoviesInfoProvider.cpp
        $<TARGET_OBJECTS:RATING_OBJECT>
)

if(${BUILD_TESTS})
        add_subdirectory(test)
endif(${BUILD_TESTS})
