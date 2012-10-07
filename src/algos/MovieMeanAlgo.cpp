#include "algos/MovieMeanAlgo.h"
#include "core/RatingsManager.h"
#include "core/Rating.h"

int NFP::algos::MovieMeanAlgo::run()
{
    clock_t start = clock();
    logStart();

    long sum_rates = 0;
    long nb_rates = 0;

    uint16_t current_movie_id = (*NFP::shm::RatingsManager::instance()->begin())->movie_id();

    LOG(INFO) << "Looping over all the ratings...";
    for (NFP::shm::RatingsIterator it = NFP::shm::RatingsManager::instance()->begin();
		 it != NFP::shm::RatingsManager::instance()->end();
		 it++) {
        if (*it == NULL) {
            LOG(ERROR) << "ptr is null!";
            return -1;
        }

        uint16_t movie_id = (*it)->movie_id();

        if (current_movie_id == movie_id) {
            sum_rates += (*it)->rate();
            nb_rates++;
        } else {
            mean_rates_[current_movie_id] = (float)sum_rates / (float)nb_rates;
            DLOG(INFO) << "movie_id=" << current_movie_id
                       << ", mean_rate=" << mean_rates_[current_movie_id];
            if ((*it)->movie_id() == 0) {
                continue;
            }
            sum_rates = (*it)->rate();
            nb_rates = 1;
            current_movie_id = movie_id;
        }
    }
    mean_rates_[current_movie_id] = (float)sum_rates / (float)nb_rates;
    LOG(INFO) << "Finished processing ratings in " << clock() - start << " cpu cycles.";
    logEnd();
    return 0;
}

float NFP::algos::MovieMeanAlgo::get_predicted_rate(uint32_t /*u_id*/, uint16_t m_id, std::string /*date = ""*/)
{
    return mean_rates_[m_id];
}
