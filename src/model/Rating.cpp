#include "Rating.h"
#include <glog/logging.h>

#include <QDate>
//#include <glog/logging.h>

std::string NFP::DateUS2S(ushort const& d){
    return QDate::fromJulianDay((int)d +
        (int)BASICRATING__DATE_OFFSET).toString("yyyy-MM-dd").toStdString();
}

ushort NFP::DateS2US(std::string const& d){
   return (ushort)(QDate::fromString(QString::fromStdString(d),
       QString("yyyy-MM-dd")).toJulianDay() - (int)BASICRATING__DATE_OFFSET);
}

NFP::Rating::Rating()
{}

NFP::Rating::Rating(ushort const& m, uint const& u, uchar const& r, ushort const& d)
{
    set_movie_id(m);
    set_user_id(u);
    set_rate(r);
    set_date(d);
}

NFP::Rating::Rating(int const& m, int const& u, int const& r, std::string const& d)
{
    set_movie_id((ushort)m);
    set_user_id((uint)u);
    set_rate((uchar)r);
    set_date(DateS2US(d));
}


NFP::Rating::Rating(char* data)
{
    memcpy(_data, data, RATING_DATA_SIZE);
    if (*data != *_data)
        LOG(ERROR) << "Ouch!";
    LOG(INFO) << to_string();
}

ushort NFP::Rating::movie_id() const
{
    return *(ushort*)&_data[BASICRATING__MOVIE_ID_POS];
}

/*int NFP::Rating::movie_id() const
{
    ushort ret = movie_id();
    return (int)ret;
}*/

uint NFP::Rating::user_id() const 
{ 
    return *((uint*)&_data[BASICRATING__USER_ID_POS]) & (uint)BASICRATING__USER_ID_MASK;
}

uchar NFP::Rating::raw_rate() const
{
    return _data[BASICRATING__RATE_POS];
}

ushort NFP::Rating::rate() const
{
    return (ushort)_data[BASICRATING__RATE_POS];
}

ushort NFP::Rating::raw_date() const
{
    return *(ushort*)&_data[BASICRATING__DATE_POS];
}

std::string NFP::Rating::date() const
{
    return DateUS2S(raw_date());
}

char* NFP::Rating::data() const
{
    char ret[RATING_DATA_SIZE];
    memcpy(ret, _data, RATING_DATA_SIZE);
    return ret;
}

std::string NFP::Rating::to_string()
{
    char ret[40];
    sprintf(ret, "%05d %010d %1d %10s",
        movie_id(),
        user_id(), rate(),
        date().c_str());
    return (std::string)ret;
}

void NFP::Rating::set_movie_id(ushort const& movie_id)
{
    *(ushort*)&_data[BASICRATING__MOVIE_ID_POS] =
        (movie_id & (ushort)BASICRATING__MOVIE_ID_MASK);
}

void NFP::Rating::set_user_id(uint const& u_id)
{
    *(uint*)&_data[BASICRATING__USER_ID_POS] =
        *(uint*)&_data[BASICRATING__USER_ID_POS] &
            (uint)0xFFFFFFFF - (uint)BASICRATING__USER_ID_MASK;
    *(uint*)&_data[BASICRATING__USER_ID_POS] |= u_id & (uint)BASICRATING__USER_ID_MASK;
}

void NFP::Rating::set_rate(uchar const& r)
{
    _data[BASICRATING__RATE_POS] = (char)(r & (uchar)BASICRATING__RATE_MASK);
}

void NFP::Rating::set_date(ushort const& d)
{
    *(ushort*)&_data[BASICRATING__DATE_POS] = (d & (ushort)BASICRATING__DATE_MASK);
}