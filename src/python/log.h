#include <glog/logging.h>
#include <string>

void initLogging(std::string appli_name = "")
{
    google::InitGoogleLogging(appli_name.c_str());
    google::InstallFailureSignalHandler();    
}
