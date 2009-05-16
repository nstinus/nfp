#ifndef __NFP__RATINGSSHMSEGMENT_H__
#define __NFP__RATINGSSHMSEGMENT_H__

#include <string>

#include "ShmSegment.h"
#include "Rating.h"


namespace NFP
{

class RatingsShmSegment : public ShmSegment
{
public:
    RatingsShmSegment(std::string, std::string);
    ~RatingsShmSegment() {}
    
    void load();
    NFP::Rating* ptr();
    int nb_ratings() {return size_ / RATING_DATA_SIZE;}
    int remove();
    
private:
    std::string dataFileName_;
};

}

#endif // __NFP__RATINGSSHMSEGMENT_H__