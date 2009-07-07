#include <glog/logging.h>

#include <iostream>
#include <string>
#include <dirent.h>
#include <stdlib.h>
#include <list>

#include "RatingsManager.h"

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
    
    segments_.reserve(17700); // Max segments in the original dataset.
    init("", true);
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
        
        if (loadedSegments_.find(keyFileName) == loadedSegments_.end()) {
            NFP::shm::RatingsShmSegment* mySSR;
            mySSR = new NFP::shm::RatingsShmSegment(dataFileName, keyFileName);
            local_err = mySSR->create();
            if (local_err == 0) {
                segments_.push_back(mySSR);
                LOG(INFO) << "Loading " << mySSR->nb_ratings() << " ratings...";
                for (int i = 0; i < mySSR->nb_ratings(); i++) {
                    NFP::model::Rating* r = (NFP::model::Rating*)(mySSR->ptr() + i);
                    if (r == NULL) {
                        LOG(ERROR) << "ptr==null";
                        continue;
                    }
                    else {
                        ratings_.push_back(r);
                    }
                }
                std::string msg("Loaded  " + mySSR->info());
                LOG(INFO) << msg;
                if (feedback) { std::cout << msg << std::endl; }
            } else { LOG(WARNING) << "Unable to load " << dataFileName; }
            ret += local_err;            
        }
        
        DLOG(INFO) << "NB segments " << nbSegments();
    }
    closedir(dp);
    LOG(INFO) << "Loading done.";
    //refreshRatingsList();
    return ret;
}

int NFP::shm::RatingsManager::remove(std::string arg_movie_id, bool feedback)
{
    std::list<RatingsSegments::iterator> erasionList;
    
    RatingsSegments::iterator it = segments_.begin();
    for (it = segments_.begin(); it != segments_.end(); it++) {
        if ((int)(*it)->keyFileName().find(arg_movie_id) != -1) {
            std::string msg("Removed " + (*it)->info());
            (*it)->remove();
            erasionList.push_back(it);
            LOG(INFO) << msg;
            if (feedback) { std::cout << msg << std::endl; }
        }
    }
    std::list<RatingsSegments::iterator>::iterator it2;
    for (it2 = erasionList.begin(); it2 != erasionList.end(); it2++) {
        segments_.erase(*it2);
    }
    rebuildLoadedSegments();
    refreshRatingsList();
    return 0;
}

void NFP::shm::RatingsManager::rebuildLoadedSegments()
{
    loadedSegments_.clear();
    RatingsSegments::iterator it = segments_.begin();
    for (it = segments_.begin(); it != segments_.end(); it++) {
        loadedSegments_.insert((*it)->keyFileName());
    }
    //LOG(INFO) << "Found " << loadedSegments_.size() << " segments.";
}

void NFP::shm::RatingsManager::addSegment(RatingsShmSegment* rss)
{
    segments_.push_back(rss);
    loadedSegments_.insert(rss->keyFileName());
}

// Should only find what's already loaded and what's not.
// This way we won't mess around with the filesystem when not necessary.
// This fonctionnality should go in a dedicated function, and called in the contructor.
// This function would thus be useless. How sad...
int NFP::shm::RatingsManager::init(std::string arg_movie_id, bool feedback)
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

        NFP::shm::RatingsShmSegment* mySSR;
        mySSR = new NFP::shm::RatingsShmSegment(dataFileName, keyFileName);
        std::string msg("Found   " + mySSR->info());
        addSegment(mySSR);
        LOG(INFO) << msg;
        if (feedback) { std::cout << msg << std::endl; }
        LOG(INFO) << "Loading " << mySSR->nb_ratings() << " ratings...";
        for (int i = 0; i < mySSR->nb_ratings(); i++) {
            NFP::model::Rating* r = (NFP::model::Rating*)(mySSR->ptr() + i);
            if (r == NULL) {
                LOG(ERROR) << "ptr==null";
                continue;
            }
            else {
                ratings_.push_back(r);
            }
        }
    }
    closedir(dp);
    
    rebuildLoadedSegments();
    //refreshRatingsList();
    
    LOG(INFO) << "Init done";
    return 0;
}

void NFP::shm::RatingsManager::refreshRatingsList() {
    LOG(INFO) << "Refreshing ratings list...";
    ratings_.clear();
    
    std::vector<RatingsShmSegment*>::iterator it;
    for (it = segments_.begin(); it != segments_.end(); it++) {
        for (int i = 0; i < (*it)->nb_ratings(); i++) {
            NFP::model::Rating* r = (NFP::model::Rating*)((*it)->ptr() + i);
            if (r == NULL) {
                LOG(ERROR) << "ptr==null";
                continue;
            }
            else {
                ratings_.push_back(r);
            }
        }
    }
    
    LOG(INFO) << "Done refreshing ratings list.";
}

