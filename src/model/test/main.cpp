#include <glog/logging.h>

#include "Rating.h"

//#include <list>
#include <string>


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
    Ratings d(100480507);
    */
    
    Rating a(17770, 480189, (uchar)4, 1234);
    
    LOG_VAR(a.movie_id());
    LOG_VAR(a.user_id());
    LOG_VAR((uint)a.rate());
    
    ushort mov;
    uint us;
    uchar r;
    
    #define NB_ITER 40000000
    
    LOG(INFO) << "Reying to allocate " << NB_ITER << " longs";
    long int** longs = (long**)malloc(NB_ITER * sizeof(long));
    for (int i = 0; i < NB_ITER; ++i)
    {
        longs[i] = new long(i);
    }
    sleep(5);
    LOG(INFO) << "Ok, that's done. Cleaning up";
    for (int i = 0; i < NB_ITER; ++i)
    {
        //printf("%d\n", ints[i]);
        delete longs[i];
    }
    free(longs);
    LOG(INFO) << "Done messing with those shitty longs";
    
    sleep(10);
        
    
    /*LOG(INFO) << "Starting " << NB_ITER << " gets";
    for (int i = 0; i< NB_ITER; ++i)
    {
        mov = a.movie_id();
        us = a.user_id();
        r = a.rate();        
    }
    LOG(INFO) << "End of " << NB_ITER << " gets";
    */
    
    //std::list<Rating*> L;
    LOG(INFO) << "sizeof(Rating) = " << sizeof(Rating);
    Rating** L;
    L = (Rating**)malloc(NB_ITER * sizeof(Rating*));
    
    LOG(INFO) << "Amount of memory needed " << sizeof(Rating) * NB_ITER / 1024 / 1024 << " Mo";
    
    LOG(INFO) << "Creating " << NB_ITER << " Rating in a std::list";
    for (int i = 0; i < NB_ITER; ++i)
    {
        DLOG(INFO) << "Created item " << i;
        /*L.push_back(new Rating((ushort)(i % 177701),
                               (uint)(i % 480190),
                               (uchar)(i % 5),
                               (ushort)(i % 1000)));
        */
        L[i] = new Rating((ushort)(i % 177701),
            (uint)(i % 480190),
            (uchar)(i % 5),
            (ushort)(i % 1000));
    }
    LOG(INFO) << "End of creation";
    
    //LOG(INFO) << (int)L[NB_ITER-10]->movie_id();
    
    sleep(5);
    
    LOG(INFO) << "Deletion...";
    /*
    std::list<Rating*>::iterator iter = L.begin();
    while(iter != L.end())
    {
        mov = (*iter)->movie_id();
        us = (*iter)->user_id();
        r = (*iter)->rate();
        
        DLOG(INFO) << "Deleted " << mov << " " << us << " " << (uint)r;
        delete *iter;
        ++iter;
    }
    */
    for (int i = 0; i < NB_ITER; ++i)
    {
        mov = L[i]->movie_id();
        us = L[i]->user_id();
        r = L[i]->rate();
        
        DLOG(INFO) << "Deleting " << mov << " " << us << " " << (uint)r;
        delete L[i];
    }
    LOG(INFO) << "Finished!";
    free(L);
    
    return 0;
}