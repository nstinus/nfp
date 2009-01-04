#include "Rating.h"
#include "macros.h"

/*Rating::Rating()
{
    Rating(0, 0, 0, 0);
}*/

Rating::Rating(const ushort &movie_id, const uint &user_id, const uchar &r, const ushort &d)
{
    DLOG(INFO) << "Rating::Rating()";
    
    set_movie_id(movie_id);
    set_user_id(user_id);
    set_rate(r);
    set_date(d);
}

Rating::~Rating()
{

}


/*************/
/*  Getters  */
/*************/

ushort Rating::movie_id()
{
    return *(ushort*)&_data[RATING__MOVIE_ID_POS];
}

uint Rating::user_id()
{
    return *((uint*)&_data[RATING__USER_ID_POS]) & (uint)RATING__USER_ID_MASK;
}

uchar Rating::rate()
{
    return (uchar)_data[RATING__USER_ID_POS];
}

ushort Rating::date()
{
    return _data[RATING__DATE_POS];
}


/*************/
/*  Setters  */
/*************/

void Rating::set_movie_id(const ushort &movie_id)
{
    DLOG(INFO) << "Movie_id = " << (movie_id & (ushort)RATING__MOVIE_ID_MASK);
    *(ushort*)&_data[RATING__MOVIE_ID_POS] = (movie_id & (ushort)RATING__MOVIE_ID_MASK);
}

void Rating::set_user_id(const uint &u_id)
{
    uint toto = u_id;
    toto = 0;
}

void Rating::set_rate(const uchar &r)
{
    _data[RATING__RATE_POS] = (char)(r & (uchar)RATING__RATE_MASK); 
}

void Rating::set_date(const ushort &d)
{
    ushort toto = d;
    toto = 0;
}


