#include <glog/logging.h>

#include "Rating.h"
#include "RatingsShmSegment.h"
#include "RatingsManager.h"

//#include <list>
#include <string>

#include <QDate>
#include <QString>


int main (int argc, char const *argv[])
{
    google::InitGoogleLogging(argv[0]);
    google::InstallFailureSignalHandler();
    
    LOG(INFO) << "Starting tests. Hello!"; 
    
    NFP::shm::RatingsManager* R1 = NFP::shm::RatingsManager::instance();
    LOG(INFO) << "R1 = " << R1;
    LOG(INFO) << R1->nbSegments() << " segments loaded";
    if (R1->nbSegments() < 17700) R1->load("", true);
    
    LOG(INFO) << "Finished testing. Bye! Bye!";
    return 0;
}