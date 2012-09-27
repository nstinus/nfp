include(${QT_USE_FILE})

add_library(nfpcore
        Rating.cpp
        User.cpp
        ShmSegment.cpp
        RatingsShmSegment.cpp
        RatingsManager.cpp
        MoviesInfoProvider.cpp
)
