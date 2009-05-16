#ifndef __NFP__RATINGSMANAGER_H__
#define __NFP__RATINGSMANAGER_H__

#include "RatingsShmSegment.h"
#include "Rating.h"
#include "Singleton.hxx"

#include <vector>


namespace NFP
{
    
namespace shm
{
    
typedef std::vector<NFP::shm::RatingsShmSegment*> RatingsSegments;

class RatingsManager : public base::Singleton<RatingsManager>
{
    friend class base::Singleton<RatingsManager>;

public:   
    ~RatingsManager();
    int load(std::string arg_movie_id = "", bool feedback = false);
    int remove(bool feedback = false);
    int nbSegments() { return segments_.size(); }
    
private:
    RatingsManager();
    int init(std::string arg_movie_id = "", bool feedback = false);
    
    std::string NFP_TRAINING_SET_DIR;
    std::string NFP_SHM_FILES;
    
    std::vector<RatingsShmSegment*> segments_;
    
};

}
}

#endif // __NFP__RATINGSMANAGER_H__