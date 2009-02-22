#ifndef __NFP__RATINGS_H__
#define __NFP__RATINGS_H__

#include "ShmSegment.h"
#include "Rating.h"


namespace NFP
{

class RatingsSS : public ShmSegment
{
public:
    RatingsSS(std::string);
    ~RatingsSS() {}
    
    void load();
    NFP::Rating* ptr();
    int nb_ratings() {return size_ / RATING_DATA_SIZE;}
};

}

#endif