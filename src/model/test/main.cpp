#include <glog/logging.h>

//#include ".RatingDate.h"
#include "Rating.h"


int main (int argc, char const *argv[])
{
    google::InitGoogleLogging(argv[0]);
    google::InstallFailureSignalHandler();
    
    /*RatingDate d_1("2001-01-01");
    DLOG(INFO) << "2001-01-01" << ": usint " << d_1.getUSInt()
        << " string " << d_1.getStdString() << std::endl;
    DLOG(INFO) << "";*/
    
    /*RatingDate d_2((usint)730069);
    DLOG(INFO) << "730069" << ": usint " << d_2.getUSInt()
        << " string " << d_2.getStdString() << std::endl;
    DLOG(INFO) << "";
    
    RatingDate d_3((usint)0);
    DLOG(INFO) << "0" << ": usint " << d_3.getUSInt()
        << " string " << d_3.getStdString() << std::endl;
    DLOG(INFO) << "";
    
    RatingDate d_5((usint)1);
    DLOG(INFO) << "0" << ": usint " << d_5.getUSInt()
        << " string " << d_5.getStdString() << std::endl;
    DLOG(INFO) << "";
    
    RatingDate d_4((usint)32767);
    DLOG(INFO) << "32767" << ": usint " << d_4.getUSInt()
        << " string " << d_4.getStdString() << std::endl;
    DLOG(INFO) << "";
    */ 
    
    /*   
    DLOG(INFO) << "MIN: " << RatingDate::MIN().getUSInt()
        << " MAX: " << RatingDate::MAX().getUSInt() << std::endl;
    DLOG(INFO) << "";*/
    
    /*Ratings a(1);
    Ratings b(2);
    Ratings c(10);
    Ratings d(100480507);*/
    
    Rating a(17770, 480189, 5, 1234);
    DLOG_VAR(a.movie_id());
    DLOG_VAR(a.user_id());
    
    return 0;
}