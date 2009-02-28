#include <glog/logging.h>

#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <sys/stat.h>
#include <stdlib.h>

#include <QRegExp>

#include "RatingsSS.h"
#include "Rating.h"

NFP::RatingsSS::RatingsSS(std::string dataFileName, std::string keyFileName) {
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
                LOG(INFO) << "Found a shmkey file. size = " << s;
            }
        }
        else {
            load();
        }
    }
    else
        LOG(ERROR) << "Data file " << dataFileName << " does not exist."; 
}

void NFP::RatingsSS::load()
{
    std::ifstream in(dataFileName_.c_str());
    
    if (in.is_open())
    {
        LOG(INFO) << "Parsing file " << dataFileName_;
        std::string line;
        QRegExp mvFileLineRegExp("^(\\d+),([1-5]),(\\d{4}-\\d{2}-\\d{2})$");
        int movie_id = -1;
        int user_id = -1;
        int rate = -1;
        std::string date = "";
        std::list<NFP::Rating*> ratings;

        while (!in.eof()) {
            getline(in, line);
            DLOG(INFO) << "Read line: \"" << line << "\"";
            if (movie_id == -1) {
                QString l = QString::fromStdString(line);
                l.chop(1);
                movie_id = atoi(l.toStdString().c_str());
            }
            else {
                if (mvFileLineRegExp.indexIn(QString::fromStdString(line)) > -1) {
                    user_id = mvFileLineRegExp.cap(1).toInt();
                    rate = mvFileLineRegExp.cap(2).toInt();
                    date = mvFileLineRegExp.cap(3).toStdString();
                    NFP::Rating* r = new NFP::Rating(movie_id, user_id, rate, date);
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
        
        create();
        attach();
        
        LOG(INFO) << "Loading ShmSegment (size " << size() << ")...";
        int i = 0;
        std::list<NFP::Rating*>::const_iterator it;
        char* data = new char[RATING_DATA_SIZE];
        for (it = ratings.begin(); it != ratings.end(); it++) {
            (*it)->data(data);
            memcpy((NFP::Rating*)(ptr()) + i, data, RATING_DATA_SIZE);
            i++;
        }
        LOG(INFO) << "...done";
    }
    else
        LOG(ERROR) << "Unable to open file \"" << keyFileName_ << "\" for reading.";    
}

NFP::Rating* NFP::RatingsSS::ptr()
{
    if (ptr_) {
        return (NFP::Rating*)ptr_;
    } else {
        LOG(ERROR) << "ptr_ is null. " << info();
    }
}

int NFP::RatingsSS::remove()
{
    //Should remove shm keyfile
    NFP::ShmSegment::remove();
}