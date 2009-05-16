#include "User.h"

//#include <numeric>

NFP::User::User(uint user_id) : _user_id(user_id)
{
    _avg_rating = -1;
}

void NFP::User::add_rating(NFP::Rating* r)
{
    _ratings.push_back(r);
}

uint NFP::User::nb_ratings()
{
    return _ratings.size();
}

// long NFP::sum_Rating_rates(NFP::Rating* r1, NFP::Rating* r2)
// {
//     return r1->rate() + r2->rate();
// }

float NFP::User::avg_rating()
{
    if (_avg_rating >= 0 && _avg_rating <= 5)
    {
        return _avg_rating;
    }
    else {
        long sRates = 0;
        for (std::list<NFP::Rating*>::iterator i = _ratings.begin(); i != _ratings.end(); i++) {
            sRates += (*i)->rate();
        }
        return (float)sRates / (float)nb_ratings();
    }
    return -1.0;
}