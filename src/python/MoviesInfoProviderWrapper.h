#ifndef __NFP__PYTHON__MOVIESINFOPROVIDERWRAPPER_H__
#define __NFP__PYTHON__MOVIESINFOPROVIDERWRAPPER_H__

#include "MoviesInfoProvider.h"
#include <string>

namespace NFP
{

namespace utils
{

int movieYear(int m) {
    return NFP::utils::MoviesInfoProvider::instance()->getMovieYear((uint)m);
}

std::string movieName(int m) {
    return *(NFP::utils::MoviesInfoProvider::instance()->getMovieName((uint)m));
}

int MoviesInfoProvider_nbMovies() {
    return NFP::utils::MoviesInfoProvider::instance()->nbMovies();
}

} // utils
} // NFP

#endif // __NFP__PYTHON__MOVIESINFOPROVIDERWRAPPER_H__
