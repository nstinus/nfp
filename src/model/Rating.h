#ifndef __RATING_H__
#define __RATING_H__

/*************************************************************************************************/
/*                                            RATING                                             */
/*************************************************************************************************/
/*                                                                                               */
/*  Abstract:                                                                                    */
/*    Low-level representation for a rating from the training_set.                               */
/*    The fields are: movie_id [0, 17770],   card = 17770  < 2**15 -> 15 bits                    */
/*                    user_id  [6, 2649429], card = 480189 < 2**19 -> 19 bits                    */
/*                    rate     [1, 5],       card = 5      < 2**3  ->  3 bits                    */
/*                    date     [0, 2182],    card = 2182   < 2**12 -> 12 bits                    */
/*                                                                                               */
/*  Visual representation of the data:                                                           */
/*                                                                                               */
/*      Bytes :      |       |       |       |       |       |       |       |       |           */
/*      char[i]:     -       7       6       5       4       3       2       1       0           */
/*      Fields:      |X--------------X    X------------------X    X--X   X-----------X           */
/*      Name:        |    Movie_id   |        User_id        |  Rate |      Date     |           */
/*      Size:        |       15      |           19          |    3  |       12      |           */
/*                                                                                               */
/*  General considerations:                                                                      */
/*                                                                                               */
/*      - the data integrity check is made at the insertion. (masks).                            */
/*      - the getters just do silly casts.                                                       */
/*                                                                                               */
/*************************************************************************************************/


#include "macros.h"
#include "typedefs.h"

const uint RATING__DATE_POS      = 0;
const uint RATING__RATE_POS      = 2;
const uint RATING__USER_ID_POS   = 3;
const uint RATING__MOVIE_ID_POS  = 6;

const uint RATING__DATE_MASK     = 0x0FFF;
const uint RATING__RATE_MASK     = 0x0007;
const uint RATING__USER_ID_MASK  = 0x0007FFFF;
const uint RATING__MOVIE_ID_MASK = 0x7FFF;

const uint RATING_DATA_SIZE      = 8;


class Rating
{
private:
    char _data[RATING_DATA_SIZE];

public:
    Rating(const ushort &, const uint &, const uchar &, const ushort &);
    ~Rating();
    
    ushort movie_id();
    uint user_id();
    uchar rate();
    ushort date();
    
    void set_movie_id(const ushort &);
    void set_user_id(const uint &);
    void set_rate(const uchar &);
    void set_date(const ushort &);
};

#endif // __RATING_H__