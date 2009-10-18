#include "MovieMeanAlgo.h"
#include "RatingsManager.h"
#include "Rating.h"
#include <boost/progress.hpp>

void NFP::algos::MovieMeanAlgo::init(){
    LOG(INFO) << "Init...";
    NFP::shm::RatingsManager::instance()->refreshRatingsList();
    ratings_begin = NFP::shm::RatingsManager::instance()->ratings_begin();
    ratings_end = NFP::shm::RatingsManager::instance()->ratings_end();
    LOG(INFO) << "...done";
}

int NFP::algos::MovieMeanAlgo::run()
{
    logStart();
    
    init();
    
    long sum_rates = 0;
    long nb_rates = 0;
    
    if (NFP::shm::RatingsManager::instance()->nb_ratings() == 0) {
        LOG(WARNING) << "Maybe I was mistaken, by I was unable to find any rating to loop on!";
        return -1;
    }

    ushort current_movie_id = (*ratings_begin)->movie_id();
    
    std::string msg("Looping over all the ratings...");
    LOG(INFO) << msg;
    std::cout << std::endl << msg;
    boost::progress_display show_progress_ratings(NFP::shm::RatingsManager::instance()->nb_ratings());
    for (std::list<NFP::model::Rating*>::const_iterator it = ratings_begin; it != ratings_end; it++) {
        if (*it == NULL) {
            LOG(ERROR) << "ptr is null!";
            return -1;
        }
        
        ushort movie_id = (*it)->movie_id();
        
       if (current_movie_id == movie_id) {
            sum_rates += (*it)->rate();
            nb_rates++;
        } else {
            mean_rates_[current_movie_id] = (float)sum_rates / (float)nb_rates;
            #ifndef NDEBUG
            DLOG(INFO) << "Finished processing movie "
                       << current_movie_id << " mean_rate=" << mean_rates_[current_movie_id];
            #endif
            sum_rates = (*it)->rate();
            nb_rates = 1;
            current_movie_id = movie_id;
        }
        ++show_progress_ratings;
    }
    mean_rates_[current_movie_id] = (float)sum_rates / (float)nb_rates;
    #ifndef NDEBUG
    DLOG(INFO) << "Finished processing movie "
               << current_movie_id << " mean_rate=" << mean_rates_[current_movie_id];
    #endif
    logEnd();
    return 0;
}

float NFP::algos::MovieMeanAlgo::get_predicted_rate(uint /*u_id*/, ushort m_id, std::string /*date = ""*/)
{
    //FIXME: warnings about u_id, date, unused.
    return mean_rates_.find(m_id) == mean_rates_.end() ? -1.0 : mean_rates_[m_id];
}