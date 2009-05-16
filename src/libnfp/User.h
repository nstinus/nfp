#ifndef __NFP__USER_H__
#define __NFP__USER_H__

#include <list>

#include "Rating.h"

namespace NFP
{
    
//long sum_Rating_rates(NFP::Rating*, NFP::Rating*);

class User
{
    private:
        uint _user_id;
        std::list<NFP::Rating*> _ratings;
        
        float _avg_rating;
        
    public:
        User(uint);
        uint nb_ratings();
        void add_rating(NFP::Rating*);
        float avg_rating();
        std::list<NFP::Rating*>* ratings() {return &_ratings;};
};
    
}

#endif