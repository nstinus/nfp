#ifndef __NFP__RATINGSMANAGER_H__
#define __NFP__RATINGSMANAGER_H__

#include "RatingsShmSegment.h"
#include "Rating.h"
#include "Singleton.hxx"


namespace NFP
{
    
namespace shm
{

class RatingsManager : public base::Singleton<RatingsManager>
{
    friend class base::Singleton<RatingsManager>;

public:   
    ~RatingsManager();
    int load(std::string arg_movie_id = "", bool feedback = false);
    int remove(std::string arg_movie_id = "", bool feedback = false);
    
private:
    RatingsManager();
    
    std::string NFP_TRAINING_SET_DIR;
    std::string NFP_SHM_FILES;
    
    //std::list<RatingsShmSegment*> segments_;
    
};

}
}

#endif // __NFP__RATINGSMANAGER_H__