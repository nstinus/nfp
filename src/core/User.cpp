#include "User.h"

//#include <numeric>

NFP::model::User::User(uint32_t user_id) : _user_id(user_id)
{
    _avg_rating = -1;
}

void NFP::model::User::add_rating(NFP::model::Rating* r)
{
    _ratings.push_back(r);
}

uint32_t NFP::model::User::nb_ratings()
{
    return _ratings.size();
}

// long NFP::sum_Rating_rates(NFP::model::Rating* r1, NFP::model::Rating* r2)
// {
//     return r1->rate() + r2->rate();
// }

float NFP::model::User::avg_rating()
{
    if (_avg_rating >= 0 && _avg_rating <= 5)
    {
        return _avg_rating;
    }
    else {
        long sRates = 0;
        for (auto i = _ratings.begin(); i != _ratings.end(); i++) {
            sRates += (*i)->rate();
        }
        return (float)sRates / (float)nb_ratings();
    }
    return -1.0;
}