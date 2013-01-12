#include "algos/UserMeanAlgo.hpp"
#include "core/RatingsManager.hpp"
#include "core/Rating.hpp"


int NFP::algos::UserMeanAlgo::run()
{
    clock_t start = clock();
    logStart();

    LOG(INFO) << "Looping over all the ratings...";

    NFP::shm::RatingsIterator it = NFP::shm::RatingsManager::instance()->begin();
    for (; it != NFP::shm::RatingsManager::instance()->end(); it++) {
        if (*it == NULL) {
            LOG(ERROR) << "ptr is null!";
            return -1;
        }

        uint32_t user_id = (*it)->user_id();

        std::map<uint32_t, User*>::iterator u_it = users_.find(user_id);
        if (u_it == users_.end())
          users_[user_id] = new User(user_id);
        users_[user_id]->add_rating((*it)->rate());

        DLOG(INFO) << "Added rate to " << user_id;
    }
    LOG(INFO) << "Finished processing ratings in " << clock() - start << " cpu cycles.";

    start = clock();
    LOG(INFO) << "Calculating average rate per user...";
    for (std::map<uint32_t, User*>::iterator u_it = users_.begin(); u_it != users_.end(); u_it++) {
      u_it->second->compute();
      DLOG(INFO) << "user_id = " << u_it->first
                 << ", nb_ratings = " << u_it->second->nb_ratings
                 << ", mean_rating = " << u_it->second->mean_rating;
    }
    LOG(INFO) << "Computation(!) done in " << clock() - start << " cpu cycles.";
    logEnd();
    return 0;
}
