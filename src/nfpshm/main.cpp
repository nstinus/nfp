//#include <tclap/CmdLine.h>
#include <glog/logging.h>

#include <dirent.h>
#include <errno.h>

#include <string>
#include <iostream>

#include "RatingsSS.h"

int removeSegmentsAndFiles();

int main (int argc, char* argv[])
{
    google::InitGoogleLogging(argv[0]);
    google::InstallFailureSignalHandler();
    
    if (argc == 2 && strcmp(argv[1], "remove") == 0)
            removeSegmentsAndFiles();
    
    return 0;
}

int removeSegmentsAndFiles()
{
    std::string shmKeyFilesDir = getenv("NFP_SHM_FILES");
    LOG(INFO) << "NFP_SHM_FILES = " << shmKeyFilesDir;
    
    DIR *dp;
    struct dirent *dirp;
    if((dp  = opendir(shmKeyFilesDir.c_str())) == NULL) {
        LOG(ERROR) << "Error(" << errno << ") opening " << shmKeyFilesDir;
        return errno;
    }

    while ((dirp = readdir(dp)) != NULL) {
        std::string keyFileName = dirp->d_name;
        if (keyFileName.find(".shmkey") == -1)
            continue;
        std::string dataFileName = keyFileName;
        dataFileName.erase(dataFileName.end()-7, dataFileName.end());
        dataFileName = getenv("NFP_TRAINING_SET_DIR") + std::string("/") + dataFileName;
        keyFileName = getenv("NFP_SHM_FILES") + std::string("/") + keyFileName;
        
        NFP::RatingsSS mySSR(dataFileName, keyFileName);
        mySSR.create();
        mySSR.remove();
        std::cout << "Removed " << mySSR.keyFileName() << std::endl;
   }
    closedir(dp);
    
    
}

