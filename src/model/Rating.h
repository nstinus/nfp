#ifndef __RATING_H__
#define __RATING_H__


#include "macros.h"
#include "typedefs.h"


#define RATING__MOVIE_ID_SIZE 15  // [0 -> 17770], card = 17770 < 2**15
#define RATING__USER_ID_SIZE  19  // [6 -> 2649429], card = 480189 < 2**19
#define RATING__RATE_SIZE      3  // [1 -> 5], card = 5 < 2**3
#define RATING__DATE_SIZE     12  // [0 -> 2181], card = 2182 < 2**12
#define RATING__SIZE RATING__MOVIE_ID_SIZE \
                   + RATING__USER_ID_SIZE \
                   + RATING__RATE_SIZE \
                   + RATING__DATE_SIZE
#define RATING__ROUND_SIZE (int)RATING__SIZE / (int)8 + (int)1 // Align the shit on a char
//#define RATINGS__DATE_ORIGIN


typedef unsigned char rate;

class Rating
{
private:
    char* _data;

public:
    //Rating();
    Rating(const ushort &, const uint &, const rate &, const ushort &);
    ~Rating();
    
    ushort movie_id();
    uint user_id();
    //ate rate();
    //date date();
    
    void set_movie_id(const ushort &);
    void set_user_id(const uint &);
    //void set_rate(const rate &);
    //void set_date(const date &);
};

#endif // __RATING_H__