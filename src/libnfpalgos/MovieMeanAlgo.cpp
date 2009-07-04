#include "MovieMeanAlgo.h"
#include "RatingsManager.h"
#include "Rating.h"

int NFP::algos::MovieMeanAlgo::run()
{
    logStart();
    
    std::list<NFP::model::Rating*> ratings = NFP::shm::RatingsManager::instance()->ratings();
    std::list<NFP::model::Rating*>::iterator it = ratings.begin();
    
    LOG(INFO) << ratings.size() << " ratings found";
    
    long sum_rates = 0;
    long nb_rates = 0;
    
    LOG(INFO) << (*it)->to_string();
    
    ushort current_movie_id = (*it)->movie_id();
    
    for (; it != ratings.end(); it++) {
        if (*it == NULL) {
            LOG(ERROR) << "ptr is null!";
            return -1;
        }
        ushort movie_id = (*it)->movie_id();
        
        if (current_movie_id == movie_id) {
            sum_rates += (*it)->rate();
            nb_rates++;
        } else {
            mean_rates_[movie_id] = (float)sum_rates / (float)nb_rates;
            sum_rates = (*it)->rate();
            nb_rates = 1;
            current_movie_id = movie_id;
        }
    }
    logEnd();
    
    return 0;
}
