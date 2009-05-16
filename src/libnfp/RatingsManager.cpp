#include <glog/logging.h>

#include <iostream>
#include <string>
#include <dirent.h>

#include "RatingsManager.h"

/**
 * General accessor designed to setup the Ratings in shm (load), or remove them (remove).
 * Provides iterator on the loaded data aswell.
 */
NFP::shm::RatingsManager::RatingsManager()
{
    if (getenv("NFP_TRAINING_SET_DIR") == NULL) {
        LOG(ERROR) << "Unable to find NFP_TRAINING_SET_DIR env. variable. Exiting.";
        exit(1);
    } else {
        NFP_TRAINING_SET_DIR = getenv("NFP_TRAINING_SET_DIR");
        LOG(INFO) << "NFP_TRAINING_SET_DIR = " << NFP_TRAINING_SET_DIR;
    }
    
    if (getenv("NFP_SHM_FILES") == NULL) {
        LOG(FATAL) << "Unable to find NFP_SHM_FILES env. variable. Exiting";
        exit(1);
    } else {
        NFP_SHM_FILES = getenv("NFP_SHM_FILES");
        LOG(INFO) << "NFP_SHM_FILES = " << NFP_SHM_FILES;
    }
    
    //init();
}

NFP::shm::RatingsManager::~RatingsManager()
{}

int NFP::shm::RatingsManager::load(std::string arg_movie_id, bool feedback)
{
    int ret = 0;
    
    DIR *dp;
    struct dirent *dirp;
    if((dp  = opendir(NFP_TRAINING_SET_DIR.c_str())) == NULL) {
        LOG(ERROR) << "Error(" << errno << ") opening " << NFP_TRAINING_SET_DIR;
        return errno;
    }
    
    while ((dirp = readdir(dp)) != NULL) {
        int local_err = 0;
        std::string dataFileName, keyFileName;
        dataFileName = keyFileName = dirp->d_name;
    
        if (arg_movie_id != "" && (int)dataFileName.find(arg_movie_id) == -1
                || dataFileName == "."
                || dataFileName == "..")
            continue;
    
        dataFileName = NFP_TRAINING_SET_DIR + std::string("/") + dataFileName;
        keyFileName  = NFP_SHM_FILES + std::string("/") + keyFileName + std::string(".shmkey");
    
        NFP::shm::RatingsShmSegment* mySSR;
        mySSR = new NFP::shm::RatingsShmSegment(dataFileName, keyFileName);
        local_err = mySSR->create();
        
        //segments.push_back(mySSR);
        
        if (local_err == 0) {
            std::string msg("Loaded  " + mySSR->info());
            LOG(INFO) << msg;
            if (feedback) { std::cout << msg << std::endl; }
        } else { LOG(WARNING) << "Unable to load " << dataFileName; }
        ret += local_err;
    }
    closedir(dp);
    
    return ret;
}

int NFP::shm::RatingsManager::remove(std::string arg_movie_id, bool feedback)
{
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
     mySSR.create();
     std::string msg("Removed " + mySSR.info());
     mySSR.remove();
     LOG(INFO) << msg;
     if (feedback) { std::cout << msg << std::endl; }
    }
    closedir(dp);
    return 0;
}

/*int NFP::shm::RatingsManager::init(std::string arg_movie_id, bool feedback)
{
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
     mySSR.create();
     std::string msg("Found already loaded segment " + mySSR.info());
     segments_.push_back(mySSR);
     LOG(INFO) << msg;
     if (feedback) { std::cout << msg << std::endl; }
    }
    closedir(dp);
    return 0;
}*/

