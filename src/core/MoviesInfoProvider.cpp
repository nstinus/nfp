#include <stdlib.h>
#include <string.h>

#include <iostream>
#include <fstream>

#include <glog/logging.h>

#include "MoviesInfoProvider.h"

NFP::utils::MoviesInfoProvider::MoviesInfoProvider() : initialized_(false)
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
        char line[512] = "";
        int movie_id = -1;
        int year = -1;
        char* buf = NULL;
        NFP::model::Movie const * movie;

        while (!in.eof()) {
            in.getline(line, 512);
            DLOG(INFO) << "Read line: \"" << line << "\"";
            buf = strtok(line, ",");
            if (buf != NULL) {
              movie_id = atoi(buf);
            }
            buf = strtok(NULL, ",");
            if (buf != NULL && strcmp(buf, "NULL") != 0) {
              year = atoi(buf);
            } else {
              year = 0;
            }
            buf = strtok(NULL, ",");
            if (buf != NULL) {
              movie = new NFP::model::Movie(year, buf);
              movies_[movie_id] = movie;
            }
        }
    } else LOG(ERROR) << "Unable to open " << movie_titlesFilePath;

    in.close();
    LOG(INFO) << "Done parsing. Found " << nbMovies() << " movies.";
    initialized_ = true;
}
