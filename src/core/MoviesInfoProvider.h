#ifndef __NFP__MOVIESINFOPROVIDER_H__
#define __NFP__MOVIESINFOPROVIDER_H__

#include "typedefs.h"
#include "Singleton.hxx"

#include <string>
#include <map>


const uint MOVIE__YEAR_OFFSET = 1896;

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
    virtual ~Movie() {}

    void year(const int y) { year_ = (ushort)(y - MOVIE__YEAR_OFFSET); }
    int year() const { return (int)(year_ + MOVIE__YEAR_OFFSET); }
    void name(std::string const& n) { name_ = n; }
    std::string const * name() const { return &name_; }

private:
    ushort year_;
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
    NFP::model::Movie const * getInfos(const uint) const;
    int getMovieYear(const uint movie_id) const { return getInfos(movie_id)->year(); }
    std::string const * getMovieName(const uint movie_id) const { return getInfos(movie_id)->name(); }
    int nbMovies() { return movies_.size(); }

private:
    void init();
    std::map<uint, NFP::model::Movie const *> movies_;
    bool initialized_;
};

} // utils
} // NFP


#endif // __NFP__MOVIESINFOPROVIDER_H__
