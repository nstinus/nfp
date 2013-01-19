#ifndef __NFP__PYTHON__MOVIESINFOPROVIDERWRAPPER_H__
#define __NFP__PYTHON__MOVIESINFOPROVIDERWRAPPER_H__

#include "core/MoviesInfoProvider.hpp"

#include <string>

namespace NFP
{

namespace utils
{

int movieYear(int m) {
    return NFP::utils::MoviesInfoProvider::instance()->getMovieYear((uint32_t)m);
}

std::string const& movieName(int m) {
    return NFP::utils::MoviesInfoProvider::instance()->getMovieName((uint32_t)m);
}

} // utils
} // NFP

#endif // __NFP__PYTHON__MOVIESINFOPROVIDERWRAPPER_H__
