//#include <tclap/CmdLine.h>
#include <glog/logging.h>

#include <dirent.h>
#include <errno.h>
#include <math.h>
#include <stdlib.h>

#include <string>
#include <map>
#include <vector>
#include <iostream>
#include <fstream>

#include <QString>
#include <QRegExp>

#include "RatingsShmSegment.h"
#include "RatingsManager.h"
//#include "User.h"
#include "MovieMeanAlgo.h"
#include "UserMeanAlgo.h"

const std::string NFP_TRAINING_SET_DIR = getenv("NFP_TRAINING_SET_DIR");
const std::string NFP_SHM_FILES        = getenv("NFP_SHM_FILES");


int rm(std::string);
int load(std::string);
int reload(std::string);
int save(std::string);
int infos(std::string);
int infos2(/*std::string*/);
int users();
int users2();
void usage();
int ratings(const std::string, const std::string);
int ratings2();

int movieYear(int);
std::string movieName(int);
void fillMoviesMaps();
std::string hr_size(int);

std::map<int, int> movieYears;
std::map<int, std::string> movieNames;


int main (int argc, char* argv[])
{
    google::InitGoogleLogging(argv[0]);
    google::InstallFailureSignalHandler();
    
    int ret = 0;
    
    if (argc > 1 && strcmp(argv[1], "remove") == 0)
        ret += rm((argc > 2) ? argv[2] : "");
    else if (argc > 1 && strcmp(argv[1], "load") == 0)
        ret += load((argc > 2) ? argv[2] : "");
    else if (argc > 1 && strcmp(argv[1], "reload") == 0)
        ret += reload((argc > 2) ? argv[2] : "");
    else if (argc > 1 && strcmp(argv[1], "save") == 0)
        ret += save((argc > 2) ? argv[2] : "");
    else if (argc > 1 && strcmp(argv[1], "infos") == 0)
        ret += infos((argc > 2) ? argv[2] : "");
    else if (argc > 1 && strcmp(argv[1], "infos2") == 0)
        ret += infos2(/*(argc > 2) ? argv[2] : ""*/);
    else if (argc > 1 && strcmp(argv[1], "ratings") == 0)
        ret += ratings((argc > 2) ? argv[2] : "", (argc > 3) ? argv[3] : "");
    else if (argc > 1 && strcmp(argv[1], "ratings2") == 0)
        ret += ratings2();
    else if (argc > 1 && strcmp(argv[1], "users") == 0)
        ret += users();
    else if (argc > 1 && strcmp(argv[1], "users2") == 0)
        ret += users2();
    else
        usage();
    
    return ret;
}


int rm(std::string arg_movie_id = "")
{
    return NFP::shm::RatingsManager::instance()->remove(arg_movie_id, true);
}


int load(std::string arg_movie_id = "")
{
    int ret = NFP::shm::RatingsManager::instance()->load(arg_movie_id, true);
    return ret;
}

int save(std::string arg_movie_id = "")
{
    int ret = NFP::shm::RatingsManager::instance()->save(arg_movie_id, true);
    return ret;
}

int reload(std::string arg_movie_id = "")
{
    return rm(arg_movie_id) + load(arg_movie_id);
}

int infos(std::string movie_id = "")
{
    LOG(INFO) << "NFP_SHM_FILES = " << NFP_SHM_FILES;

    DIR *dp;
    struct dirent *dirp;
    if((dp  = opendir(NFP_SHM_FILES.c_str())) == NULL) {
        LOG(ERROR) << "Error(" << errno << ") opening " << NFP_SHM_FILES;
        return errno;
    }

    char* msg = new char[256];
    int total_ratings = 0;
    double allR_mean = 0;
    int nb_movies = 0;
    
    sprintf(msg, "%7s  %9s  %9s  %5s  %4s  %s",
        "#    id",
        "Nb. Rat.",
        "Mem.",
        "R ArM",
        "Year",
        "Name");
    std::cout << msg << std::endl;
    
    while ((dirp = readdir(dp)) != NULL) {
        std::string keyFileName = dirp->d_name;

        if ((int)keyFileName.find(".shmkey") == -1 ||
                (movie_id != "" && (int)keyFileName.find(movie_id) == -1))
            continue;

        std::string dataFileName = keyFileName;
        dataFileName.erase(dataFileName.end()-7, dataFileName.end());
        dataFileName = NFP_TRAINING_SET_DIR + std::string("/") + dataFileName;
        keyFileName = NFP_SHM_FILES + std::string("/") + keyFileName;

        NFP::shm::RatingsShmSegment mySSR(dataFileName, keyFileName);
        
        if (mySSR.create()) {
            LOG(ERROR) << "An error occured creating shm segment. ";
            continue;
        }
        if (mySSR.attach()) {
            LOG(ERROR) << "An error occured attaching to shm segment. ";
            continue;
        }
        
        int m_id = mySSR.ptr()->movie_id();
        int nb_ratings = mySSR.nb_ratings();
        
        double armean_rate = 0;
        int myRate = 0;
        
        for (int i = 0; i < nb_ratings; i++) {
            myRate = ((NFP::model::Rating*)(mySSR.ptr() + i))->rate();
            armean_rate += (double)myRate;
        }
        mySSR.detach();
        
        armean_rate /= nb_ratings;
        
        sprintf(msg, "%07d  %9d  %9d  %5.3f  %4d  %s",
            m_id,
            nb_ratings,
            nb_ratings * RATING_DATA_SIZE,
            armean_rate,
            movieYear(m_id),
            movieName(m_id).c_str());
        std::cout << msg << std::endl;

        allR_mean = (allR_mean*total_ratings + armean_rate*nb_ratings);
        total_ratings += nb_ratings;
        allR_mean /= (double)(total_ratings);
        nb_movies++;
        
        LOG(INFO) << msg;
    }
    closedir(dp);
    
    sprintf(msg, "NB%5d  %9d  %9d  %11.9f  %s",
        nb_movies,
        total_ratings,
        total_ratings * RATING_DATA_SIZE,
        allR_mean,
        hr_size(total_ratings * RATING_DATA_SIZE * 4 / 8).c_str());
    std::cout << std::endl << msg << std::endl;
    
    delete[] msg;
    
    return 0;
}


int ratings(const std::string arg_movie_id = "", const std::string arg_user_id = "")
{
    LOG(INFO) << "NFP_SHM_FILES = " << NFP_SHM_FILES;
    
    DIR *dp;
    struct dirent *dirp;
    if((dp  = opendir(NFP_SHM_FILES.c_str())) == NULL) {
        LOG(ERROR) << "Error(" << errno << ") opening " << NFP_SHM_FILES;
        return errno;
    }

    while ((dirp = readdir(dp)) != NULL) {
        std::string keyFileName = dirp->d_name;

        if ((int)keyFileName.find(".shmkey") == -1 ||
                (arg_movie_id != "" && (int)keyFileName.find(arg_movie_id) == -1))
            continue;

        std::string dataFileName = keyFileName;
        dataFileName.erase(dataFileName.end()-7, dataFileName.end());
        dataFileName = NFP_TRAINING_SET_DIR + std::string("/") + dataFileName;
        keyFileName = NFP_SHM_FILES + std::string("/") + keyFileName;

        NFP::shm::RatingsShmSegment mySSR(dataFileName, keyFileName);
        
        if (mySSR.create()) {
            LOG(ERROR) << "An error occured creating shm segment. ";
            continue;
        }
        if (mySSR.attach()) {
            LOG(ERROR)<< "An error occured attaching to shm segment. ";
            continue;
        }
        
        int nb_ratings = mySSR.nb_ratings();
        LOG(INFO) << "Found " << nb_ratings;
        
        NFP::model::Rating* myRating = NULL;
        
        char* msg = new char[50];
        sprintf(msg, "%5s  %8s  %1s  %10s", "#m_id", "user_id", "R", "Date");
        std::cout << msg << std::endl;
        delete[] msg;
        
        char* myRuId_s = new char[10];
        std::string myRuIdS("");
        
        for (int i = 0; i < nb_ratings; i++) {
            myRating = (NFP::model::Rating*)(mySSR.ptr() + i);
            sprintf(myRuId_s, "%d", myRating->user_id());
            myRuIdS = myRuId_s;
            if (arg_user_id == "" || (int)myRuIdS.find(arg_user_id) != -1)
                std::cout << myRating->to_string() << std::endl;
        }
        delete[] myRuId_s;
        
        mySSR.detach();
    }
    closedir(dp);
    
    return 0;
}


int movieYear(int movie_id)
{
    int ret = 0;
    if (movieYears.empty())
        fillMoviesMaps();
    if (movieYears.find(movie_id) == movieYears.end())
        LOG(WARNING) << movie_id << " not found in movieYears map";
    else ret = movieYears[movie_id];    
    return ret;
}


std::string movieName(int movie_id)
{
    std::string ret("");
    if (movieNames.empty())
        fillMoviesMaps();
    if (movieNames.find(movie_id) == movieNames.end())
        LOG(WARNING) << movie_id << " not found in movieNames map";
    else ret = movieNames[movie_id];
    return ret;
}


void fillMoviesMaps()
{
    LOG(INFO) << "movieYears and/or movieNames maps are empty. Filling up ...";
    
    std::string movie_titlesFilePath = getenv("NFP_MOVIE_TITLES_FILE");
    
    std::ifstream in(movie_titlesFilePath.c_str());
    
    if (in.is_open())
    {
        std::string line;
        QRegExp mvFileLineRegExp("^(\\d+),(\\d+|NULL),(.*)$");
        
        int movie_id = -1;
        int year = -1;
        std::string name("");
        
        while (!in.eof()) {
            getline(in, line);
            DLOG(INFO) << "Read line: \"" << line << "\"";
            if (mvFileLineRegExp.indexIn(QString::fromStdString(line)) > -1) {
                movie_id = mvFileLineRegExp.cap(1).toInt();
                year = mvFileLineRegExp.cap(2).toInt();
                year = year > 0 ? year : -1;
                name = mvFileLineRegExp.cap(3).toStdString();
                movieYears[movie_id] = year;
                movieNames[movie_id] = name;
            }
        }    
    } else LOG(ERROR) << "Unable to open " << movie_titlesFilePath;
    
    in.close();
    LOG(INFO) << "... done";
}

void usage()
{
    std::string desc;
    desc  = "nfpshm <load|remove|infos|users> [movie]\n";
    std::cout << desc << std::endl;
}

std::string hr_size(int bsize)
{
    std::vector<std::string> units;
    units.push_back("o");
    units.push_back("ko");
    units.push_back("Mo");
    
    int iter = 0;
    while (iter < 3 && bsize > 1024) {
        bsize /= 1024;
        iter++;
    }
    
    char* msg = new char[10];
    sprintf(msg, "%d%-2s", bsize, units[iter].c_str());
    return std::string(msg);
}

int users()
{
    // LOG(INFO) << "NFP_SHM_FILES = " << NFP_SHM_FILES;
    // 
    // DIR *dp;
    // struct dirent *dirp;
    // if((dp  = opendir(NFP_SHM_FILES.c_str())) == NULL) {
    //     LOG(ERROR) << "Error(" << errno << ") opening " << NFP_SHM_FILES;
    //     return errno;
    // }
    // 
    // std::map<uint, NFP::model::User*> users;
    // 
    // while ((dirp = readdir(dp)) != NULL) {
    //     std::string keyFileName = dirp->d_name;
    // 
    //     if ((int)keyFileName.find(".shmkey") == -1)
    //         continue;
    // 
    //     std::string dataFileName = keyFileName;
    //     dataFileName.erase(dataFileName.end()-7, dataFileName.end());
    //     dataFileName = NFP_TRAINING_SET_DIR + std::string("/") + dataFileName;
    //     keyFileName = NFP_SHM_FILES + std::string("/") + keyFileName;
    // 
    //     NFP::shm::RatingsShmSegment mySSR(dataFileName, keyFileName);
    //     
    //     if (mySSR.create()) {
    //         LOG(ERROR) << "An error occured creating shm segment. ";
    //         continue;
    //     }
    //     if (mySSR.attach()) {
    //         LOG(ERROR)<< "An error occured attaching to shm segment. ";
    //         continue;
    //     }
    //     
    //     int nb_ratings = mySSR.nb_ratings();
    //     LOG(INFO) << "Found " << nb_ratings;
    //     
    //     NFP::model::Rating* myRating = NULL;
    //     
    //     char* msg = new char[50];
    //     sprintf(msg, "%5s  %8s  %1s  %10s", "#m_id", "user_id", "R", "Date");
    //     std::cout << msg << std::endl;
    //     delete[] msg;
    //     
    //     char* myRuId_s = new char[10];
    //     std::string myRuIdS("");
    //     
    //     for (int i = 0; i < nb_ratings; i++) {
    //         myRating = (NFP::model::Rating*)(mySSR.ptr() + i);
    //         if (not users.has_key(myRating->user_id())
    //             sprintf(myRuId_s, "%d", myRating->user_id());
    //         myRuIdS = myRuId_s;
    //         if (arg_user_id == "" || (int)myRuIdS.find(arg_user_id) != -1)
    //             std::cout << myRating->to_string() << std::endl;
    //     }
    //     delete[] myRuId_s;
    //     
    //     mySSR.detach();
    // }
    // closedir(dp);
    // 
    return 0;    
}

int infos2(/*std::string movie_id = ""*/)
{
    NFP::algos::MovieMeanAlgo* m_mean_alg = new NFP::algos::MovieMeanAlgo();
    int ret = m_mean_alg->run();
    
    char* msg = new char[256];
    
    sprintf(msg, "%7s  %5s  %4s  %s",
        "#    id",
        "R ArM",
        "Year",
        "Name");
    std::cout << msg << std::endl << std::endl;
    uint skipped = 0, nb_segments = (uint)NFP::shm::RatingsManager::instance()->nbSegments();
    for (uint m_id = 1; m_id <= nb_segments + (skipped == 0 ? 0 : skipped-1); m_id++) {
        float res = m_mean_alg->get_predicted_rate(0, m_id, "");
        if (res > 0) {
            sprintf(msg, "%07d  %5.3f  %4d  %s",
                m_id,
                res,
                movieYear(m_id),
                movieName(m_id).c_str());
            std::cout << msg << std::endl;
        } else {
             skipped++;
        }
    }
    
    return ret;
}


int users2()
{
    NFP::algos::UserMeanAlgo* u_mean_alg = new NFP::algos::UserMeanAlgo();
    int ret = u_mean_alg->run();
    
    char* msg = new char[256];
    sprintf(msg, "%9s  %5s", "# user_id", "R ArM");
    std::cout << msg << std::endl << std::endl;
    std::vector<uint>::const_iterator u;
    for (u = u_mean_alg->users_begin(); u != u_mean_alg->users_end(); u++) {
        float res = u_mean_alg->get_predicted_rate(*u, 0, "");
        if (res > 0) {
            sprintf(msg, "%09d  %5.3f", *u, res);
            std::cout << msg << std::endl;
        }
    }
    
    return ret;
}

int ratings2()
{
    std::list<NFP::model::Rating*>::const_iterator it, end;
    DLOG(INFO) << "Getting start begin() on ratings...";
    it = NFP::shm::RatingsManager::instance()->ratings_begin();
    DLOG(INFO) << "Getting start end() on ratings...";
    end = NFP::shm::RatingsManager::instance()->ratings_end();
    DLOG(INFO) << "Let's loop...";
    for (; it != end; it++)
        //LOG(INFO) << *it << " " << NULL;
        std::cout << (*it)->to_string() << std::endl;
    return 0;
}


