#ifndef __NFP__PYTHON__LOG_H__
#define __NFP__PYTHON__LOG_H__

#include <glog/logging.h>
#include <string>

inline void initLogging(std::string const& appli_name = "")
{
    static const std::string default_name("pynfp");
    google::InitGoogleLogging(((appli_name != "") ? appli_name : default_name).c_str());
    google::InstallFailureSignalHandler();    
}

//TODO: use an enum and a switch
//TODO: find a way to convey, python scripts line in logging. That's probably hard...
inline void LOG_(std::string const& message, int const& level = 1)
{
    if (level < 2 || level > 4) {
        LOG(INFO) << message;
    } else if (level == 2) {
        LOG(WARNING) << message;
    } else if (level == 3) {
        LOG(ERROR) << message;
    } else if (level == 4) {
        LOG(FATAL) << message;
    }
}

inline void LOG_INFO (std::string const& message) {LOG_(message, 1);}
inline void LOG_WARN (std::string const& message) {LOG_(message, 2);}
inline void LOG_ERROR(std::string const& message) {LOG_(message, 3);}
inline void LOG_FATAL(std::string const& message) {LOG_(message, 4);}

#endif // __NFP__PYTHON__LOG_H__
