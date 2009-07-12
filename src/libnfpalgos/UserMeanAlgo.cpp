#include "UserMeanAlgo.h"
#include "RatingsManager.h"
#include "Rating.h"
#include <boost/progress.hpp>

void NFP::algos::UserMeanAlgo::init(){
    LOG(INFO) << "Init...";
    ratings_begin = NFP::shm::RatingsManager::instance()->ratings_begin();
    ratings_end = NFP::shm::RatingsManager::instance()->ratings_end();
    LOG(INFO) << "...done";
}

int NFP::algos::UserMeanAlgo::run()
{
    logStart();
    
    init();
    
    std::map<uint, unsigned long long> user_summed_rates_;
    std::map<uint, uint> user_nb_rates_;
    
    
    LOG(INFO) << "Starting loop over all the ratings...";
    std::cout << std::endl << "Starting loop over all the ratings...";
    int nb_processed_ratings = 0;
    boost::progress_display show_progress_ratings(NFP::shm::RatingsManager::instance()->nb_ratings());
    for (std::list<NFP::model::Rating*>::const_iterator it = ratings_begin; it != ratings_end; it++) {
        if (*it == NULL) {
            LOG(ERROR) << "ptr is null!";
            return -1;
        }
        
        uint user_id = (*it)->user_id();
        ushort rate = (*it)->rate();
        
        if (user_summed_rates_.find(user_id) == user_summed_rates_.end()) {
            user_summed_rates_[user_id] = 0;
            users.push_back(user_id);
        }
        user_summed_rates_[user_id] += rate;
        if (user_nb_rates_.find(user_id) == user_nb_rates_.end())
            user_nb_rates_[user_id] = 0;
        user_nb_rates_[user_id]++;
        nb_processed_ratings++;
        LOG_EVERY_N(INFO, 1000000) << "Processed " << nb_processed_ratings << " ratings";
        ++show_progress_ratings;
    }
    
    LOG(INFO) << "Calculating average rate per user...";
    std::cout << std::endl << "Calculating average rate per user...";
    boost::progress_display show_progress_users(users.size());
    for (std::vector<uint>::iterator u_id = users.begin(); u_id != users.end(); u_id++) {
        user_mean_rates_[*u_id] = (float)user_summed_rates_[*u_id] / (float)user_nb_rates_[*u_id];
        LOG(INFO) << "user_id = " << *u_id << ", nb_ratings = " << user_nb_rates_[*u_id]
            << ", mean_rate = " << user_mean_rates_[*u_id];
        ++show_progress_users;
    }
    
    LOG(INFO) << "nb_users = " << users.size();
    
    logEnd();
    return 0;
}

float NFP::algos::UserMeanAlgo::get_predicted_rate(uint u_id, ushort /*m_id*/, std::string /*date = ""*/)
{
    //FIXME: warnings about u_id, date, unused.
    return user_mean_rates_.find(u_id) == user_mean_rates_.end() ? -1.0 : user_mean_rates_[u_id];
}
