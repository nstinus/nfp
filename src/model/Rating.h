#ifndef __RATING_H__
#define __RATING_H__

#include "macros.h"

#define RATING__MOVIE_ID_SIZE 15  // [0 -> 17770], card = 17770 < 2**15
#define RATING__USER_ID_SIZE  19  // [6 -> 2649429], card = 480189 < 2**19
#define RATING__RATE_SIZE      3  // [1 -> 5], card = 5 < 2**3
#define RATING__DATE_SIZE     12  // [0 -> 2181], card = 2182 < 2**12
#define RATING__SIZE RATINGS__MOVIE_ID_SIZE \
                    + RATINGS__USER_ID_SIZE \
                    + RATINGS__RATE_SIZE \
                    + RATINGS__DATE_SIZE)
#define RATING__ROUND_SIZE RATING__SIZE / 8 + 1 // Align the shit on a char
//#define RATINGS__DATE_ORIGIN

class Rating
{
private:
    char* _data[RATING__ROUND_SIZE];

public:
    Rating();
    Rating(const ushort &, const uint &, const rate &, const ushort &);
    ~Rating();
    
    int getMovie_id();
    int getUser_id();
    rate getRate();
    //date getDate();
}

#endif // __RATING_H__