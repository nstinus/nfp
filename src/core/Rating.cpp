#include "Rating.h"
#include "Date.h"

#include <stdio.h>
#include <string.h>


/**
 *  Converts a uint16_t value of a date stored in the a Rating
 *  to a formatted YYYY-MM-DD string.
 */
std::string NFP::utils::DateUS2S(uint16_t const& d)
{
  return NFP::utils::fromJulianDay(d + BASICRATING__DATE_OFFSET);
}

/**
 *  Converts a formatted YYYY-MM-DD string
 *  to an offsetted uint16_t used by a Rating.
 */
uint16_t NFP::utils::DateS2US(std::string const& d)
{
  return (uint16_t)(NFP::utils::toJulianDay(d) - BASICRATING__DATE_OFFSET);
}

NFP::model::Rating::Rating()
{
    memset(data_, 0, RATING_DATA_SIZE);
}

NFP::model::Rating::Rating(uint16_t const& m, uint32_t const& u, uint8_t const& r, uint16_t const& d)
{
    set_movie_id(m);
    set_user_id(u);
    set_rate(r);
    set_date(d);
}

NFP::model::Rating::Rating(int const& m, int const& u, int const& r, std::string const& d)
{
    set_movie_id((uint16_t)m);
    set_user_id((uint32_t)u);
    set_rate((uint8_t)r);
    set_date(utils::DateS2US(d));
}


NFP::model::Rating::Rating(char const* data)
{
    memcpy(data_, data, RATING_DATA_SIZE);
}

uint16_t NFP::model::Rating::movie_id() const
{
    return *(uint16_t*)&data_[BASICRATING__MOVIE_ID_POS];
}

/*int NFP::model::Rating::movie_id() const
{
    uint16_t ret = movie_id();
    return (int)ret;
}*/

uint32_t NFP::model::Rating::user_id() const
{
    return *((uint32_t*)&data_[BASICRATING__USER_ID_POS]) & (uint32_t)BASICRATING__USER_ID_MASK;
}

uint8_t NFP::model::Rating::raw_rate() const
{
    return data_[BASICRATING__RATE_POS];
}

uint16_t NFP::model::Rating::rate() const
{
    return (uint16_t)data_[BASICRATING__RATE_POS];
}

uint16_t NFP::model::Rating::raw_date() const
{
    return *(uint16_t*)&data_[BASICRATING__DATE_POS];
}

std::string const NFP::model::Rating::date() const
{
    return utils::DateUS2S(raw_date());
}

void NFP::model::Rating::data(char dest[RATING_DATA_SIZE]) const
{
    memcpy(dest, data_, RATING_DATA_SIZE);
}

std::string const NFP::model::Rating::to_string() const
{
    char ret[40];
    sprintf(ret, "%05d  %08d  %1d  %10s",
        movie_id(),
        user_id(),
        rate(),
        date().c_str());
    return (std::string)ret;
}

void NFP::model::Rating::set_movie_id(uint16_t const& movie_id)
{
    *(uint16_t*)&data_[BASICRATING__MOVIE_ID_POS] =
        (movie_id & (uint16_t)BASICRATING__MOVIE_ID_MASK);
}

void NFP::model::Rating::set_user_id(uint32_t const& u_id)
{
    *(uint32_t*)&data_[BASICRATING__USER_ID_POS] =
        *(uint32_t*)&data_[BASICRATING__USER_ID_POS] &
            ((uint32_t)0xFFFFFFFF - (uint32_t)BASICRATING__USER_ID_MASK);
    *(uint32_t*)&data_[BASICRATING__USER_ID_POS] |= (u_id & (uint32_t)BASICRATING__USER_ID_MASK);
}

void NFP::model::Rating::set_rate(uint8_t const& r)
{
    data_[BASICRATING__RATE_POS] = (char)(r & (uint8_t)BASICRATING__RATE_MASK);
}

void NFP::model::Rating::set_date(uint16_t const& d)
{
    *(uint16_t*)&data_[BASICRATING__DATE_POS] = (d & (uint16_t)BASICRATING__DATE_MASK);
}

void NFP::model::Rating::set_date(std::string const& d)
{
    set_date(utils::DateS2US(d));
}
