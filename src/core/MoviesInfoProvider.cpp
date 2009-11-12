#include <stdlib.h>

#include <iostream>
#include <fstream>

#include <QRegExp>
#include <QString>

#include <glog/logging.h>

#include "MoviesInfoProvider.h"

NFP::utils::MoviesInfoProvider::MoviesInfoProvider() : initialized_(FALSE)
{
    init();
}


NFP::utils::MoviesInfoProvider::~MoviesInfoProvider()
{
    std::map<uint, NFP::model::Movie const *>::iterator it = movies_.begin();
    for (; it != movies_.end(); ++it) {
        delete it->second;
    }
    movies_.empty();
}



NFP::model::Movie const * NFP::utils::MoviesInfoProvider::getInfos(const uint movie_id) const
{
    if (!initialized_) {
        LOG(WARNING) << "Try to fetch data before initialization!";
    }

    std::map<uint, NFP::model::Movie const *>::const_iterator it = movies_.find(movie_id);
    if (it == movies_.end()) {
        LOG(WARNING) << movie_id << " not found in movies map";
        return NULL;
    } else {
        return it->second;
    }
}


void NFP::utils::MoviesInfoProvider::init()
{
    std::string movie_titlesFilePath = getenv("NFP_MOVIE_TITLES_FILE");

    LOG(INFO) << "Parsing " << movie_titlesFilePath;

    std::ifstream in(movie_titlesFilePath.c_str());

    if (in.is_open())
    {
        std::string line;
        QRegExp mvFileLineRegExp("^(\\d+),(\\d+|NULL),(.*)$");

        int movie_id = -1;
        int year = -1;
        std::string name("");
        NFP::model::Movie const * movie;

        while (!in.eof()) {
            getline(in, line);
            DLOG(INFO) << "Read line: \"" << line << "\"";
            if (mvFileLineRegExp.indexIn(QString::fromStdString(line)) > -1) {
                movie_id = mvFileLineRegExp.cap(1).toInt();
                year = mvFileLineRegExp.cap(2).toInt();
                //year = year > 0 ? year : -1;
                name = mvFileLineRegExp.cap(3).toStdString();
                movie = new NFP::model::Movie(year, name);
                movies_[movie_id] = movie;
            }
        }
    } else LOG(ERROR) << "Unable to open " << movie_titlesFilePath;

    in.close();
    LOG(INFO) << "Done parsing. Found " << nbMovies() << " movies.";
    initialized_ = TRUE;
}
