#include <core/logging.h>

#define LOG_VAR(x) LOG(INFO) << "" #x "" << " = " << x;
#define DLOG_VAR(x) DLOG(INFO) << "" #x "" << " = " << x;

#define STR(s) _STR(s)
#define _STR(s) #s

#define INIT_LOG() google::InitGoogleLogging(argv[0]); \
    google::InstallFailureSignalHandler();

#define LOG_BUILD_INFO  LOG(INFO) << "Compilation infos: GIT_COMMIT=" STR(BUILD__GIT) \
      ", MODE=" STR(BUILD__MODE) \
      ", PROF=" STR(BUILD__PROF) \
      ", ARCH=" STR(BUILD__ARCH);
