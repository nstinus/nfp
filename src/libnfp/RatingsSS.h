#ifndef __NFP__RATINGS_H__
#define __NFP__RATINGS_H__

#include "ShmSegment.h"

namespace NFP
{

class RatingsSS : public ShmSegment
{
public:
    RatingsSS(std::string);
    ~RatingsSS() {}
    
    void load();
};

}

#endif