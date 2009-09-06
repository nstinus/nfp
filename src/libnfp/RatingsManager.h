#ifndef __NFP__RATINGSMANAGER_H__
#define __NFP__RATINGSMANAGER_H__

#include "RatingsShmSegment.h"
#include "Rating.h"
#include "Singleton.hxx"

#include <vector>
#include <list>
#include <set>


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
    int remove(std::string arg_movie_id = "", bool feedback = false);
    int removeAll();
    int save(std::string arg_movie_id = "", bool feedback = false);
    int nbSegments() {return segments_.size();};
    void refreshRatingsList();
    uint nb_ratings() {return ratings_.size();};
    std::list<NFP::model::Rating*>::const_iterator ratings_begin() {return ratings_.begin();};
    std::list<NFP::model::Rating*>::const_iterator ratings_end() {return ratings_.end();};
    
private:
    RatingsManager();
    int init(std::string arg_movie_id = "", bool feedback = false);
    void rebuildLoadedSegments();
    void addSegment(RatingsShmSegment*);
    
    std::string NFP_TRAINING_SET_DIR;
    std::string NFP_SHM_FILES;
    
    std::vector<RatingsShmSegment*> segments_;
    std::set<std::string> loadedSegments_;
    
    std::list<NFP::model::Rating*> ratings_;
};

}
}

#endif // __NFP__RATINGSMANAGER_H__
