#ifndef __BASICRATING_H__
#define __BASICRATING_H__

/*************************************************************************************************/
/*                                          BasicRating                                          */
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
/*      There's no build! That shit is header only. Just include this file.                      */
/*                                                                                               */
/*************************************************************************************************/

typedef unsigned short int usint;
typedef unsigned long int ulong;
typedef unsigned char uchar;

const uint BASICRATING__DATE_POS      = 0;
const uint BASICRATING__RATE_POS      = 2;
const uint BASICRATING__USER_ID_POS   = 3;
const uint BASICRATING__MOVIE_ID_POS  = 6;

const uint BASICRATING__DATE_MASK     = 0x0FFF;
const uint BASICRATING__RATE_MASK     = 0x0007;
const uint BASICRATING__USER_ID_MASK  = 0x0007FFFF;
const uint BASICRATING__MOVIE_ID_MASK = 0x7FFF;

const uint RATING_DATA_SIZE      = 8;

namespace NFP
{

class BasicRating
{
private:
    char _data[RATING_DATA_SIZE];

public:
    BasicRating::BasicRating()
    {
        BasicRating(0, 0, 0, 0);
    }
    
    BasicRating::BasicRating(ushort const& movie_id,
                             uint const& user_id,
                             uchar const& r,
                             ushort const& d)
    {
        set_movie_id(movie_id);
        set_user_id(user_id);
        set_rate(r);
        set_date(d);
    }


    /*************/
    /*  Getters  */
    /*************/

    ushort BasicRating::movie_id() { return *(ushort*)&_data[BASICRATING__MOVIE_ID_POS]; };

    uint BasicRating::user_id()
    { 
        return *((uint*)&_data[BASICRATING__USER_ID_POS]) & (uint)BASICRATING__USER_ID_MASK;
    };

    uchar BasicRating::rate() { return _data[BASICRATING__RATE_POS]; };

    ushort BasicRating::date()
    {
        return *(ushort*)&_data[BASICRATING__DATE_POS];
    }

    char const* BasicRating::getData() {return _data; };


    /*************/
    /*  Setters  */
    /*************/

    void BasicRating::set_movie_id(ushort const& movie_id)
    {
        *(ushort*)&_data[BASICRATING__MOVIE_ID_POS] = (movie_id & (ushort)BASICRATING__MOVIE_ID_MASK);
    };

    void BasicRating::set_user_id(uint const& u_id)
    {
        *(uint*)&_data[BASICRATING__USER_ID_POS] =
            *(uint*)&_data[BASICRATING__USER_ID_POS] & (uint)0xFFFFFFFF - (uint)BASICRATING__USER_ID_MASK;
        *(uint*)&_data[BASICRATING__USER_ID_POS] |= u_id & (uint)BASICRATING__USER_ID_MASK;
    }

    void BasicRating::set_rate(uchar const& r)
    {
        _data[BASICRATING__RATE_POS] = (char)(r & (uchar)BASICRATING__RATE_MASK);
    };

    void BasicRating::set_date(ushort const& d)
    {
        *(ushort*)&_data[BASICRATING__DATE_POS] = (d & (ushort)BASICRATING__DATE_MASK);
    }

};

} // NFP

#endif // __BASICRATING_H__