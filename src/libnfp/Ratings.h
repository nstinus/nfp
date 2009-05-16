#ifndef __NFP__RATINGS_H__
#define __NFP__RATINGS_H__

#include "RatingsShmSegment.h"
#include "Rating.h"


namespace NFP
{

class Ratings
{
public:
    Ratings();
    ~Ratings();
    
    int load(std::string);
    
private:
    std::string NFP_TRAINING_SET_DIR;
    std::string NFP_SHM_FILES;
    
};

}

#endif // __NFP__RATINGSSHMSEGMENT_H__