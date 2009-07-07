#include "MovieMeanAlgo.h"
#include "RatingsManager.h"
#include "Rating.h"

void NFP::algos::MovieMeanAlgo::init(){
    LOG(INFO) << "Init...";
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
    
    ushort current_movie_id = (*ratings_begin)->movie_id();
    
    LOG(INFO) << "Starting loop over all the ratings...";
    for (std::list<NFP::model::Rating*>::iterator it = ratings_begin; it != ratings_end; it++) {
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
            LOG(INFO) << "Finished processing movie "
                       << current_movie_id << " mean_rate=" << mean_rates_[current_movie_id];
            sum_rates = (*it)->rate();
            nb_rates = 1;
            current_movie_id = movie_id;
        }
    }
    mean_rates_[current_movie_id] = (float)sum_rates / (float)nb_rates;
    LOG(INFO) << "Finished processing movie "
               << current_movie_id << " mean_rate=" << mean_rates_[current_movie_id];
    
    logEnd();
    return 0;
}

float NFP::algos::MovieMeanAlgo::get_predicted_rate(uint u_id, ushort m_id, std::string date = "")
{
    //FIXME: warnings about u_id, date, unused.
    return mean_rates_.find(m_id) == mean_rates_.end() ? -1.0 : mean_rates_[m_id];
}
