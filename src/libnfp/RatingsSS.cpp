#include <glog/logging.h>

#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <sys/stat.h>

#include <QRegExp>

#include "RatingsSS.h"
#include "Rating.h"

NFP::RatingsSS::RatingsSS(std::string keyFile){
    struct stat stFileInfo;
    if (stat(keyFile.c_str(), &stFileInfo))
        keyFile_ = keyFile;
    else
        LOG(ERROR) << "File " << keyFile << " does not exist."; 
}

void NFP::RatingsSS::load()
{
    std::ifstream in(keyFile_.c_str());
    
    if (in.is_open())
    {
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
            if (movie_id == -1)
                movie_id = atoi(line.c_str());
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

        size(ratings.size() * RATING_DATA_SIZE);        
        create();
        attach();
        
        int i = 0;
        std::list<NFP::Rating*>::const_iterator it;
        char* data = new char[RATING_DATA_SIZE];
        for (it = ratings.begin(); it != ratings.end(); it++) {
            (*it)->data(data);
            memcpy((NFP::Rating*)(ptr()) + i, data, RATING_DATA_SIZE);
            i++;
        }
    }
    else
        LOG(ERROR) << "Unable to open file \"" << keyFile_ << "\" for reading.";    
}