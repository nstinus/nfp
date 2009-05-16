#include <glog/logging.h>

#include <iostream>
#include <string>
#include <dirent.h>

#include "Ratings.h"

NFP::Ratings::Ratings()
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
}

NFP::Ratings::~Ratings()
{}


int NFP::Ratings::load(std::string arg_movie_id = "")
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
    
        NFP::RatingsShmSegment mySSR(dataFileName, keyFileName);
        local_err = mySSR.create();
        
        if (local_err == 0) 
            LOG(INFO) << "Loaded  " << mySSR.info();
        else
            LOG(WARNING) << "Unable to load " << dataFileName;
        ret += local_err;
    }
    closedir(dp);
    
    return ret;
}

