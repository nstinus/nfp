#include <iostream>
#include <string>
#include <dirent.h>
#include <stdlib.h>
#include <list>
#include <vector>
#include <algorithm>

#include "core/RatingsManager.hpp"
#include "core/logging.hpp"

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
    init("", false);
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

    std::vector<std::string> files;
    files.reserve(17770);

    while ((dirp = readdir(dp)) != NULL) {
        files.push_back(dirp->d_name);
    }

    std::sort(files.begin(), files.end());
    std::vector<std::string>::const_iterator it;
    for (it = files.begin(); it != files.end(); ++it) {
        int local_err = 0;
        std::string dataFileName, keyFileName;
        dataFileName = keyFileName = *it;

        if (((arg_movie_id != "" && (int)dataFileName.find(arg_movie_id) == -1)
	        || dataFileName == ".")
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
    return ret;
}

int NFP::shm::RatingsManager::remove(std::string const& arg_movie_id, bool feedback)
{
    std::string msg;
    std::list<RatingsSegments::iterator> erasionList;

    bool fundCandidates = false;
    RatingsSegments::iterator it;
    for (it = segments_.begin(); it != segments_.end(); ++it) {
        if ((int)(*it)->keyFileName().find(arg_movie_id) != -1) {
            (*it)->remove();
            msg = "Removed " + (*it)->info();
            erasionList.push_back(it);
            LOG(INFO) << msg;
            if (feedback)
              std::cout << msg << std::endl;
            fundCandidates = true;
        }
    }
    if (fundCandidates) {
        rebuildLoadedSegments();
    } else {
        msg = "No segments fund for deletion with pattern '" + arg_movie_id + "'.";
        LOG(WARNING) << msg;
        if (feedback)
            std::cout << msg << std::endl;
    }
    return 0;
}

void NFP::shm::RatingsManager::rebuildLoadedSegments()
{
    loadedSegments_.clear();
    RatingsSegments::iterator it = segments_.begin();
    for (it = segments_.begin(); it != segments_.end(); it++) {
        loadedSegments_.insert((*it)->keyFileName());
    }
}

void NFP::shm::RatingsManager::addSegment(RatingsShmSegment* rss)
{
    segments_.push_back(rss);
    loadedSegments_.insert(rss->keyFileName());
}


int NFP::shm::RatingsManager::init(std::string arg_movie_id, bool feedback)
{
    DIR *dp;
    struct dirent *dirp;
    if((dp  = opendir(NFP_SHM_FILES.c_str())) == NULL) {
     LOG(ERROR) << "Error(" << errno << ") opening " << NFP_SHM_FILES;
     return errno;
    }

    LOG(INFO) << "Looking for shmsegments...";
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
        addSegment(mySSR);
        if (feedback) {
            std::string msg("Found   " + mySSR->info());
            LOG(INFO) << msg;
            std::cout << msg << std::endl;
        }
    }
    closedir(dp);
    LOG(INFO) << "Done finding shmsegments.";

    rebuildLoadedSegments();

    LOG(INFO) << "Init done";
    return 0;
}

void str_replace(std::string& str, const std::string& oldStr, const std::string& newStr)
{
  size_t pos = 0;
  while((pos = str.find(oldStr, pos)) != std::string::npos)
  {
     str.replace(pos, oldStr.length(), newStr);
     pos += newStr.length();
  }
}

int NFP::shm::RatingsManager::save(std::string arg_movie_id, bool /*feedback*/)
{
    int ret = 0;
    std::cout << std::endl << "Saving segments...";
    RatingsSegments::const_iterator it = segments_.begin();
    for (it = segments_.begin(); it != segments_.end(); it++) {
        if ((*it)->keyFileName().find(arg_movie_id, 0) != std::string::npos) {
          std::string filename((*it)->keyFileName());
          str_replace(filename, ".txt.shmkey", ".bin");
          ret += (*it)->save_raw(filename.c_str());
        }
    }
    return ret;
}

int NFP::shm::RatingsManager::removeAll()
{
    int ret = 0;
    RatingsSegments::iterator it;
    for (it = segments_.begin(); it != segments_.end(); ++it) {
            std::string msg("Removed " + (*it)->info());
            (*it)->remove();
            LOG(INFO) << msg;
    }
    segments_.clear();
    LOG(INFO) << "Segments list cleared.";
    return ret;
}
