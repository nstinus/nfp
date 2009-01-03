#ifndef __RATINGS_H__
#define __RATINGS_H__

#include "Rating.h"
#include "macros.h"


class Rating;

class Ratings
{
private:
    char* _data;
    
protected:
    uint getItemPos(const uint &) const;
    uint getItemOffset(const uint &) const;
    
public:
    Ratings(const uint &);
    ~Ratings();
    
    //Rating get(const uint &) const;
    //void set(const Rating &);
        
};


#endif // __RATINGS_H__