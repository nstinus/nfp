#ifndef __NFP__MOVIESINFOPROVIDER_H__
#define __NFP__MOVIESINFOPROVIDER_H__

#include "Defs.h"
#include "Singleton.hxx"

#include <string>

const uint32_t MOVIE__YEAR_OFFSET = 1896;

namespace NFP
{

namespace model
{

class Movie
{
public:
    Movie(int y, std::string const& n) {
        year(y);
        name(n);
    }
    Movie() : year_(0), name_("") {}
    virtual ~Movie() {}

    void year(const int y) { year_ = (uint16_t)(y - MOVIE__YEAR_OFFSET); }
    int year() const { return (int)(year_ + MOVIE__YEAR_OFFSET); }
    void name(std::string const& n) { name_ = n; }
    std::string const * name() const { return &name_; }

private:
    uint16_t year_;
    std::string name_;
};

} // model

namespace utils
{

class MoviesInfoProvider : public NFP::base::Singleton<MoviesInfoProvider> 
{
    friend class NFP::base::Singleton<MoviesInfoProvider>;

public:
    MoviesInfoProvider();
    virtual ~MoviesInfoProvider();
    NFP::model::Movie const * getInfos(const uint32_t) const;
    int getMovieYear(const uint32_t movie_id) const { return getInfos(movie_id)->year(); }
    std::string const * getMovieName(const uint32_t movie_id) const { return getInfos(movie_id)->name(); }

private:
    void init();
    NFP::model::Movie *movies_[MAX_NB_MOVIES];
    bool initialized_;
};

} // utils
} // NFP


#endif // __NFP__MOVIESINFOPROVIDER_H__
