#include "BasicRating.h"

#include <QDate>
#include <glog/logging.h>

std::string NFP::DateUS2S(ushort const& d){
    return QDate::fromJulianDay((int)d +
        (int)BASICRATING__DATE_OFFSET).toString("yyyy-MM-dd").toStdString();
}

ushort NFP::DateS2US(std::string const& d){
   return (ushort)(QDate::fromString(QString::fromStdString(d),
       QString("yyyy-MM-dd")).toJulianDay() - (int)BASICRATING__DATE_OFFSET);
}

NFP::BasicRating::BasicRating()
{}

NFP::BasicRating::BasicRating(ushort const& m, uint const& u, uchar const& r, ushort const& d)
{
    DLOG(INFO) << "BasicRating " << m << " " << u << " " << r << " " << d;
    set_movie_id(m);
    set_user_id(u);
    set_rate(r);
    set_date(d);
}

NFP::BasicRating::BasicRating(int const& m, int const& u, int const& r, std::string const& d)
{
    DLOG(INFO) << "BasicRating " << m << " " << u << " " << r << " " << d;
    set_movie_id((ushort)m);
    set_user_id((uint)u);
    set_rate((uchar)r);
    set_date(DateS2US(d));
}


NFP::BasicRating::BasicRating(char* const& data)
{
    memcpy(data, _data, (size_t)RATING_DATA_SIZE);
}

ushort NFP::BasicRating::movie_id() const
{
    ushort ret = *(ushort*)&_data[BASICRATING__MOVIE_ID_POS];
    //DLOG(INFO) << "ushort movie_id() const returns " << ret
    //    << "(" << (int)ret << ")";
    return ret;
}

/*int NFP::BasicRating::movie_id() const
{
    ushort ret = movie_id();
    return (int)ret;
}*/

uint NFP::BasicRating::user_id() const 
{ 
    return *((uint*)&_data[BASICRATING__USER_ID_POS]) & (uint)BASICRATING__USER_ID_MASK;
}

uchar NFP::BasicRating::rate() const
{
    return _data[BASICRATING__RATE_POS];
}

ushort NFP::BasicRating::date() const
{
    return *(ushort*)&_data[BASICRATING__DATE_POS];
}

/*std::string NFP::BasicRating::date() const
{
    std::string ret = "";
    return ret
}*/

/*char const* NFP::BasicRating::getData() {
    return _data;
};*/

std::string NFP::BasicRating::toStdString()
{
    char ret[40];
    sprintf(ret, "%5d %10d %1d %10s",
        movie_id(),
        user_id(), rate(),
        DateUS2S(date()).c_str());
    return (std::string)ret;
}

void NFP::BasicRating::set_movie_id(ushort const& movie_id)
{
    *(ushort*)&_data[BASICRATING__MOVIE_ID_POS] =
        (movie_id & (ushort)BASICRATING__MOVIE_ID_MASK);
}

void NFP::BasicRating::set_user_id(uint const& u_id)
{
    *(uint*)&_data[BASICRATING__USER_ID_POS] =
        *(uint*)&_data[BASICRATING__USER_ID_POS] &
            (uint)0xFFFFFFFF - (uint)BASICRATING__USER_ID_MASK;
    *(uint*)&_data[BASICRATING__USER_ID_POS] |= u_id & (uint)BASICRATING__USER_ID_MASK;
}

void NFP::BasicRating::set_rate(uchar const& r)
{
    _data[BASICRATING__RATE_POS] = (char)(r & (uchar)BASICRATING__RATE_MASK);
}

void NFP::BasicRating::set_date(ushort const& d)
{
    *(ushort*)&_data[BASICRATING__DATE_POS] = (d & (ushort)BASICRATING__DATE_MASK);
}