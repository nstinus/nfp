#include "Rating.h"
#include "macros.h"

/*Rating::Rating()
{
    Rating(0, 0, 0, 0);
}*/

Rating::Rating(const ushort &movie_id, const uint &user_id, const uchar &r, const ushort &d)
{
    DLOG(INFO) << "Rating::Rating()";
    DLOG_VAR(RATING__MOVIE_ID_SIZE);
    DLOG_VAR(RATING__USER_ID_SIZE);
    DLOG_VAR(RATING__RATE_SIZE);
    DLOG_VAR(RATING__DATE_SIZE);
    DLOG_VAR(RATING__SIZE);
    DLOG_VAR((int)RATING__ROUND_SIZE);
    
    _data = (char*) malloc( (int)RATING__ROUND_SIZE * sizeof(char) );
    if (_data == NULL) {
        LOG(FATAL) << "Unable to allocate memory";
    }
    else {
        DLOG(INFO) << "Memory successfully allocated " << _data;
        DLOG_VAR(sizeof(_data));
    }
    
    set_movie_id(movie_id);
    set_user_id(user_id);
    set_rate(r);
    //setDate(d);
}

Rating::~Rating()
{
    free(_data);
}

ushort Rating::movie_id()
{
    return (ushort)((uint) (*(uint*)&_data[4]) >> 2) & (uint)0x7FFF;
}

uint Rating::user_id()
{
    return ((uint) (*(uint*)&_data[1]) >> 7) & (uint)0x07FFFF;
}

uchar Rating::rate()
{
    return ((*(uchar*)&_data[1]) >> 4) & (uchar)0x7;
}

void Rating::set_movie_id(const ushort &movie_id)
{
    *(uint*)&_data[4] = (*(uint*)&_data[4] & (uint)0xFFFE0004)
        | (((uint)movie_id << 2) & (uint)0x001FFFB);
}

void Rating::set_user_id(const uint &u_id)
{
    *(uint*)&_data[1] = (*(uint*)&_data[1] & (uint)0xFA00007F)
        | (((uint)u_id << 7) & (uint)0x03FFFF80);
}

void Rating::set_rate(const uchar &u_id)
{
    *(uchar*)&_data[1] = (*(uchar*)&_data[1] & (uchar)0x8F) | (((uchar)u_id << 4) & (uchar)0x70);
}


