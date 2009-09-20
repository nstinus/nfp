#include <glog/logging.h>

#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>

#include "RatingsShmSegment.h"
#include "Rating.h"

NFP::shm::RatingsShmSegment::RatingsShmSegment(std::string dataFileName, std::string keyFileName) {
    struct stat stFileInfo;
    if (stat(dataFileName.c_str(), &stFileInfo) == 0) {
        dataFileName_ = dataFileName;
            keyFileName_ = keyFileName;
            if (stat(keyFileName.c_str(), &stFileInfo) == 0) {
            std::ifstream in(keyFileName_.c_str());
            std::string line;
            getline(in, line);
            int s = atoi(line.c_str());
            if (s > 0) {
                size(s * RATING_DATA_SIZE);
                DLOG(INFO) << "Found a shmkey file. size = " << s;
                create();
                attach();
            }
        }
        else {
            load();
        }
    }
    else
        LOG(ERROR) << "Data file " << dataFileName << " does not exist."; 
}

int NFP::shm::RatingsShmSegment::load()
{
    std::ifstream in(dataFileName_.c_str());
    
    if (in.is_open())
    {
        LOG(INFO) << "Parsing file " << dataFileName_;
        std::string line;
        int movie_id = -1;
        int user_id = -1;
        int rate = -1;
        char date[11];
        std::list<NFP::model::Rating*> ratings;

        while (!in.eof()) {
            getline(in, line);
            if (movie_id == -1) {
	        sscanf(line.c_str(), "%d", &movie_id);
            }
            else {
	        if (sscanf(line.c_str(), "%d,%d,%s", &user_id, &rate, (char*)&date) == 3) {
                    NFP::model::Rating* r = new NFP::model::Rating(movie_id, user_id, rate, std::string(date));
                    ratings.push_back(r);
	        }
            }
        }
        
        in.close();
        
        LOG(INFO) << "File read for movie " << movie_id << ", "
            << ratings.size() << " ratings found.";

        std::ofstream out(keyFileName_.c_str());
        out << ratings.size() << std::endl;
        out.close();

        size(ratings.size() * RATING_DATA_SIZE);
        
        int err = 0;
        
        if (create()) {
            LOG(ERROR) << "An error occured while creating shm segment";
            err++;
        }
        if (attach()) {
            LOG(ERROR) << "An error occured while attaching shm segment";
            err++;
        }
        
        if (err == 0) {
            LOG(INFO) << "Loading ShmSegment (size " << size() << ") ...";
            int i = 0;
            std::list<NFP::model::Rating*>::const_iterator it;
            char* data = new char[RATING_DATA_SIZE];
            for (it = ratings.begin(); it != ratings.end(); it++) {
                (*it)->data(data);
                memcpy((NFP::model::Rating*)(ptr()) + i, data, RATING_DATA_SIZE);
                i++;
            }
            LOG(INFO) << "... done";
        }
        return 0;
    }
    else {
        LOG(ERROR) << "Unable to open file \"" << keyFileName_ << "\" for reading.";
        return -1;
    }
}

NFP::model::Rating* NFP::shm::RatingsShmSegment::ptr()
{
    if (ptr_) {
        return (NFP::model::Rating*)ptr_;
    } else {
        DLOG(ERROR) << "ptr_ is null. " << info();
    }
    return NULL;
}

int NFP::shm::RatingsShmSegment::remove()
{
    int ret = NFP::base::ShmSegment::remove();
    if (ret == 0) {
        LOG(INFO) << "Removing " << keyFileName_;
        std::remove(keyFileName_.c_str());
    } else {
        LOG(ERROR) << "remove failed";
    }
    return ret;
}
