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
  for (int i = 0; i < MAX_NB_MOVIES; ++i) {
    delete movies_[i];
  }
}



NFP::model::Movie const * NFP::utils::MoviesInfoProvider::getInfos(const uint32_t movie_id) const
{
    if (!initialized_) {
        LOG(WARNING) << "Try to fetch data before initialization!";
    }
    NFP::model::Movie* m = movies_[movie_id-1];
    if (m == NULL) {
      LOG(WARNING) << movie_id << " not found!";
    }
    return m;
}


void NFP::utils::MoviesInfoProvider::init()
{
    clock_t start = clock();

    std::string movie_titlesFilePath = getenv("NFP_MOVIE_TITLES_FILE");

    LOG(INFO) << "Parsing " << movie_titlesFilePath;

    std::ifstream in(movie_titlesFilePath.c_str());

    int nb_movies = 0;

    if (in.is_open())
    {
        char line[512] = "";
        int movie_id = -1;
        int year = -1;
        char* buf = NULL;

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
              movies_[movie_id-1] = new NFP::model::Movie(year, buf);
              ++nb_movies;
            }
        }
    } else LOG(ERROR) << "Unable to open " << movie_titlesFilePath;
    in.close();
    initialized_ = true;
    LOG(INFO) << "Done parsing in " << clock() - start << " cpu cycles. Found " << nb_movies << " movies.";
}
