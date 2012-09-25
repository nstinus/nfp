include_directories(${QT_INCLUDE_DIR})

add_library(nfpcore
        Rating.cpp
        User.cpp
        ShmSegment.cpp
        RatingsShmSegment.cpp
        RatingsManager.cpp
        MoviesInfoProvider.cpp
)
