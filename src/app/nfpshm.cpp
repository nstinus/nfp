//#include <tclap/CmdLine.h>
#include <glog/logging.h>

#include <dirent.h>
#include <errno.h>
#include <math.h>
#include <stdlib.h>
#include <float.h>

#include <string>
#include <map>
#include <vector>
#include <iostream>
#include <fstream>

#include "core/RatingsShmSegment.h"
#include "core/RatingsManager.h"
//#include "User.h"
#include "algos/MovieMeanAlgo.h"
#include "algos/UserMeanAlgo.h"
#include "core/MoviesInfoProvider.h"
#include "core/Defs.h"
#include "core/macros.h"

const std::string NFP_TRAINING_SET_DIR = getenv("NFP_TRAINING_SET_DIR");
const std::string NFP_SHM_FILES        = getenv("NFP_SHM_FILES");


int rm(std::string);
int rmAll();
int load(std::string);
int reload(std::string);
int save(std::string);
int infos();
int users();
void usage();
int ratings(const std::string, const std::string);
int ratings2();

std::string hr_size(int);

int main (int argc, char* argv[])
{
    INIT_LOG();
    LOG(INFO) << "Starting main()";
    LOG_BUILD_INFO;

    int ret = 0;

    if (argc > 1 && strcmp(argv[1], "remove") == 0)
        ret += rm((argc > 2) ? argv[2] : "");
    else if (argc > 1 && strcmp(argv[1], "remove-all") == 0)
        ret += rmAll();
    else if (argc > 1 && strcmp(argv[1], "load") == 0)
        ret += load((argc > 2) ? argv[2] : "");
    else if (argc > 1 && strcmp(argv[1], "reload") == 0)
        ret += reload((argc > 2) ? argv[2] : "");
    else if (argc > 1 && strcmp(argv[1], "save") == 0)
        ret += save((argc > 2) ? argv[2] : "");
    else if (argc > 1 && strcmp(argv[1], "infos") == 0)
        ret += infos();
    else if (argc > 1 && strcmp(argv[1], "ratings") == 0)
        ret += ratings((argc > 2) ? argv[2] : "", (argc > 3) ? argv[3] : "");
    else if (argc > 1 && strcmp(argv[1], "ratings2") == 0)
      ret += ratings2();
    else if (argc > 1 && strcmp(argv[1], "users") == 0)
        ret += users();
    else
        usage();

    LOG(INFO) << "Done with main(). Bye!";

    return ret;
}


int rm(std::string arg_movie_id = "")
{
    return NFP::shm::RatingsManager::instance()->remove(arg_movie_id, true);
}

int rmAll()
{
    return NFP::shm::RatingsManager::instance()->removeAll();
}

int load(std::string arg_movie_id = "")
{
    //NFP::shm::RatingsManager::instance()->refreshRatingsList();
    int ret = NFP::shm::RatingsManager::instance()->load(arg_movie_id, true);
    return ret;
}

int save(std::string arg_movie_id = "")
{
    //NFP::shm::RatingsManager::instance()->refreshRatingsList();
    int ret = NFP::shm::RatingsManager::instance()->save(arg_movie_id, true);
    return ret;
}

int reload(std::string arg_movie_id = "")
{
    return rm(arg_movie_id) + load(arg_movie_id);
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

        char msg[50];
        sprintf(msg, "%5s  %8s  %1s  %10s", "#m_id", "user_id", "R", "Date");
        std::cout << msg << std::endl;

        char myRuId_s[10];
        std::string myRuIdS("");

        for (int i = 0; i < nb_ratings; i++) {
            myRating = (NFP::model::Rating*)(mySSR.ptr() + i);
            sprintf(myRuId_s, "%d", myRating->user_id());
            myRuIdS = myRuId_s;
            if (arg_user_id == "" || (int)myRuIdS.find(arg_user_id) != -1)
                std::cout << myRating->to_string() << std::endl;
        }

        mySSR.detach();
    }
    closedir(dp);

    return 0;
}

void usage()
{
    std::string desc;
    desc  = "nfpshm <load|remove[-all]|infos|ratings|users|save> [movie_id pattern]\n";
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

    char msg[10];
    sprintf(msg, "%d%-2s", bsize, units[iter].c_str());
    return std::string(msg);
}


int infos()
{
    NFP::algos::MovieMeanAlgo* m_mean_alg = new NFP::algos::MovieMeanAlgo();
    int ret = m_mean_alg->run();

    char msg[256];

    uint32_t skipped = 0;

    sprintf(msg, "%7s  %5s  %4s  %s",
            "#    id",
            "R ArM",
            "Year",
            "Name");
    DLOG(INFO) << "Printing header: " << msg;
    std::cout << msg << std::endl << std::endl;

    LOG(INFO) << "Printing results to stdout...";
    for (uint32_t m_id = 1; m_id <= MAX_NB_MOVIES; m_id++) {
        float res = m_mean_alg->get_predicted_rate(0, m_id, "");
        if (res > FLT_EPSILON) {
            sprintf(msg, "%07d  %5.3f  %4d  %s",
                m_id,
                res,
                NFP::utils::MoviesInfoProvider::instance()->getMovieYear(m_id),
                NFP::utils::MoviesInfoProvider::instance()->getMovieName(m_id).c_str());
            std::cout << msg << std::endl;
        } else {
             skipped++;
        }
    }
    LOG(INFO) << "Done printing.";

    return ret;
}



int users()
{
    NFP::algos::UserMeanAlgo* u_mean_alg = new NFP::algos::UserMeanAlgo();
    int ret = u_mean_alg->run();

    char msg[256];
    sprintf(msg, "%9s  %5s", "# user_id", "R ArM");
    std::cout << msg << std::endl << std::endl;
    for (auto it = u_mean_alg->get_users().begin(); it != u_mean_alg->get_users().end(); it++) {
        float mean = it->second->mean_rating;
        int nb = it->second->nb_ratings;
        if (mean != -1 && nb != -1) {
            sprintf(msg, "%09d  %6d  %5.3f", it->first, nb, mean);
            std::cout << msg << std::endl;
        }
    }
    return ret;
}



int ratings2()
{
    auto it = NFP::shm::RatingsManager::instance()->begin();
    for (; it != NFP::shm::RatingsManager::instance()->end(); it++)
        std::cout << (*it)->to_string() << std::endl;
    return 0;
}


