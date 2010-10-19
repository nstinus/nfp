#ifndef __NFP__RATINGSMANAGER_H__
#define __NFP__RATINGSMANAGER_H__

#include "RatingsShmSegment.h"
#include "Rating.h"
#include "Singleton.hxx"

#include <vector>
#include <set>


namespace NFP
{
    
namespace shm
{
    
typedef std::vector<NFP::shm::RatingsShmSegment*> RatingsSegments;
typedef RatingsSegments::const_iterator RatingsSegmentsConstIterator;

class RatingsIterator
{
private:
  RatingsSegmentsConstIterator rs_it_;
  int r_idx_;

public:
 RatingsIterator(RatingsSegmentsConstIterator rs_it, int r_idx) :
    rs_it_(rs_it),
    r_idx_(r_idx) {}
  NFP::model::Rating* operator*();
  RatingsIterator& operator++(int);
  bool operator!=(const RatingsIterator&);
  //bool operator==(const RatingsIterator&);
};

class RatingsManager : public base::Singleton<RatingsManager>
{
    friend class base::Singleton<RatingsManager>;



public:
    // FIXME: needed to take RatingsManger() public for swig. Find a way to have it back in private.
    RatingsManager();
    ~RatingsManager();
    int load(std::string arg_movie_id = "", bool feedback = false);
    int remove(std::string arg_movie_id = "", bool feedback = false);
    int removeAll();
    int save(std::string arg_movie_id = "", bool feedback = false);
    int nbSegments() {return segments_.size();};
    RatingsSegments& segments() {return segments_;}

    RatingsIterator begin() {
      return RatingsIterator(segments_.begin(), 0);
    }
    RatingsIterator end() {
      return RatingsIterator(segments_.end(), 0);
    }

    
private:
    int init(std::string arg_movie_id = "", bool feedback = false, bool preloadSegments = false);
    void rebuildLoadedSegments();
    void addSegment(RatingsShmSegment*);
    
    std::string NFP_TRAINING_SET_DIR;
    std::string NFP_SHM_FILES;
    
    RatingsSegments segments_;
    std::set<std::string> loadedSegments_;
};

}
}

#endif // __NFP__RATINGSMANAGER_H__
