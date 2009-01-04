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
/*                                                                                               */
/*  Build:                                                                                       */
/*      There-s no build! That shit is header only. Just include this file.                      */
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
    Rating::Rating(const ushort &movie_id, const uint &user_id, const uchar &r, const ushort &d)
    {
        set_movie_id(movie_id);
        set_user_id(user_id);
        set_rate(r);
        set_date(d);
    }

    //Rating::~Rating();


    /*************/
    /*  Getters  */
    /*************/

    ushort Rating::movie_id() { return *(ushort*)&_data[RATING__MOVIE_ID_POS]; };

    uint Rating::user_id()
    { 
        return *((uint*)&_data[RATING__USER_ID_POS]) & (uint)RATING__USER_ID_MASK;
    };

    uchar Rating::rate() { return _data[RATING__RATE_POS]; };

    ushort Rating::date()
    {
        return *(ushort*)&_data[RATING__DATE_POS];
    }


    /*************/
    /*  Setters  */
    /*************/

    void Rating::set_movie_id(const ushort &movie_id)
    {
        *(ushort*)&_data[RATING__MOVIE_ID_POS] = (movie_id & (ushort)RATING__MOVIE_ID_MASK);
    };

    void Rating::set_user_id(const uint &u_id)
    {
        *(uint*)&_data[RATING__USER_ID_POS] =
            *(uint*)&_data[RATING__USER_ID_POS] & (uint)0xFFFFFFFF - (uint)RATING__USER_ID_MASK;
        *(uint*)&_data[RATING__USER_ID_POS] |= u_id & (uint)RATING__USER_ID_MASK;
    }

    void Rating::set_rate(const uchar &r)
    {
        _data[RATING__RATE_POS] = (char)(r & (uchar)RATING__RATE_MASK);
    };

    void Rating::set_date(const ushort &d)
    {
        *(ushort*)&_data[RATING__DATE_POS] = (d & (ushort)RATING__DATE_MASK);
    }
    
};

#endif // __RATING_H__