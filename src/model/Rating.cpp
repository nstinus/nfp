#include "Rating.h"

Rating::Rating()
{
    this = new Rating(0, 0, 0, 0);
}

Rating::Rating(const ushort &movie_id, const uint &user, const rate &r, const ushort &d);
{
    DLOG(INFO) << "Rating::Rating()";
    DLOG_VAR(RATING__MOVIE_ID_SIZE);
    DLOG_VAR(RATING__USER_ID_SIZE);
    DLOG_VAR(RATING__RATE_SIZE);
    DLOG_VAR(RATING__DATE_SIZE);
    DLOG_VAR(RATING__SIZE);
    DLOG_VAR(RATING__ROUND_SIZE);
    
    const uint data_size = (uint)(RATING__ROUND_SIZE);
    
    DLOG(INFO) << "Now trying to allocate " << data_size
        << "B of memory (" << data_size * (uint)(8) << "b)";
    _data = (char*)malloc((int)data_size);
    if (_data == NULL) {
        LOG(FATAL) << "Unable to allocate memory";
    }
    else {
        DLOG(INFO) << "Memory successfully allocated " << _data;
    }
    
    setMovie_id(movie_id);
    setUser(user);
    setRate(r);
    setDate(d);
}

void Rating:: setMovie_id(const ushort &movie_id)
{
    
}

ushort Rating::getMovieId()
{
    return (ushort)(_data >> (RATINGS__USER_ID_SIZE + RATINGS__RATE_SIZE + RATINGS__DATE_SIZE));
}

uint Rating::getUser()
{
    return (uint)(_data >> (RATINGS__RATE_SIZE + RATINGS__DATE_SIZE) & RATING__USER_SIZE);
}

rate Rating::getRate()
{
    return (rate)((_data >> RATINGS__DATE_SIZE) & (char)RATINGS__RATE_SIZE);
}

ushort Rating::getDate()
{
    return (ushort)(_data & (ushort)RATING__DATE_SIZE)
}
