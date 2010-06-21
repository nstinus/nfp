#include "Rating.h"
#include <glog/logging.h>
#include <stdio.h>

#include <QDate>
#include <QString>


/**
 *  Converts a ushort value of a date stored in the a Rating
 *  to a formatted YYYY-MM-DD string.
 */
std::string NFP::utils::DateUS2S(ushort const& d)
{
    return QDate::fromJulianDay((int)d +
        (int)BASICRATING__DATE_OFFSET).toString("yyyy-MM-dd").toStdString();
}

/**
 *  Converts a formatted YYYY-MM-DD string
 *  to an offsetted ushort used by a Rating.
 */
ushort NFP::utils::DateS2US(std::string const& d)
{
   return (ushort)(QDate::fromString(QString::fromStdString(d),
       QString("yyyy-MM-dd")).toJulianDay() - (int)BASICRATING__DATE_OFFSET);
}

NFP::model::Rating::Rating()
{
    memset(data_, 0, RATING_DATA_SIZE);
}

NFP::model::Rating::Rating(ushort const& m, uint const& u, uchar const& r, ushort const& d)
{
    set_movie_id(m);
    set_user_id(u);
    set_rate(r);
    set_date(d);
}

NFP::model::Rating::Rating(int const& m, int const& u, int const& r, std::string const& d)
{
    set_movie_id((ushort)m);
    set_user_id((uint)u);
    set_rate((uchar)r);
    set_date(utils::DateS2US(d));
}


NFP::model::Rating::Rating(char const* data)
{
    memcpy(data_, data, RATING_DATA_SIZE);
}

ushort NFP::model::Rating::movie_id() const
{
    return *(ushort*)&data_[BASICRATING__MOVIE_ID_POS];
}

/*int NFP::model::Rating::movie_id() const
{
    ushort ret = movie_id();
    return (int)ret;
}*/

uint NFP::model::Rating::user_id() const
{
    return *((uint*)&data_[BASICRATING__USER_ID_POS]) & (uint)BASICRATING__USER_ID_MASK;
}

uchar NFP::model::Rating::raw_rate() const
{
    return data_[BASICRATING__RATE_POS];
}

ushort NFP::model::Rating::rate() const
{
    return (ushort)data_[BASICRATING__RATE_POS];
}

ushort NFP::model::Rating::raw_date() const
{
    return *(ushort*)&data_[BASICRATING__DATE_POS];
}

std::string NFP::model::Rating::date() const
{
    return utils::DateUS2S(raw_date());
}

void NFP::model::Rating::data(char dest[RATING_DATA_SIZE]) const
{
    memcpy(dest, data_, RATING_DATA_SIZE);
}

std::string NFP::model::Rating::to_string()
{
    char ret[40];
    sprintf(ret, "%05d  %08d  %1d  %10s",
        movie_id(),
        user_id(),
        rate(),
        date().c_str());
    return (std::string)ret;
}

void NFP::model::Rating::set_movie_id(ushort const& movie_id)
{
    *(ushort*)&data_[BASICRATING__MOVIE_ID_POS] =
        (movie_id & (ushort)BASICRATING__MOVIE_ID_MASK);
}

void NFP::model::Rating::set_user_id(uint const& u_id)
{
    *(uint*)&data_[BASICRATING__USER_ID_POS] =
        *(uint*)&data_[BASICRATING__USER_ID_POS] &
            ((uint)0xFFFFFFFF - (uint)BASICRATING__USER_ID_MASK);
    *(uint*)&data_[BASICRATING__USER_ID_POS] |= (u_id & (uint)BASICRATING__USER_ID_MASK);
}

void NFP::model::Rating::set_rate(uchar const& r)
{
    data_[BASICRATING__RATE_POS] = (char)(r & (uchar)BASICRATING__RATE_MASK);
}

void NFP::model::Rating::set_date(ushort const& d)
{
    *(ushort*)&data_[BASICRATING__DATE_POS] = (d & (ushort)BASICRATING__DATE_MASK);
}

void NFP::model::Rating::set_date(std::string const& d)
{
    set_date(utils::DateS2US(d));
}
