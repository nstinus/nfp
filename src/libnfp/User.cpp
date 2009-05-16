#include "User.h"

//#include <numeric>

NFP::base::User::User(uint user_id) : _user_id(user_id)
{
    _avg_rating = -1;
}

void NFP::base::User::add_rating(NFP::model::Rating* r)
{
    _ratings.push_back(r);
}

uint NFP::base::User::nb_ratings()
{
    return _ratings.size();
}

// long NFP::sum_Rating_rates(NFP::model::Rating* r1, NFP::model::Rating* r2)
// {
//     return r1->rate() + r2->rate();
// }

float NFP::base::User::avg_rating()
{
    if (_avg_rating >= 0 && _avg_rating <= 5)
    {
        return _avg_rating;
    }
    else {
        long sRates = 0;
        for (std::list<NFP::model::Rating*>::iterator i = _ratings.begin(); i != _ratings.end(); i++) {
            sRates += (*i)->rate();
        }
        return (float)sRates / (float)nb_ratings();
    }
    return -1.0;
}