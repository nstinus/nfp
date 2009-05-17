#ifndef __NFP__USER_H__
#define __NFP__USER_H__

#include <list>

#include "Rating.h"

namespace NFP
{
namespace model
{
    
//long sum_Rating_rates(NFP::model::Rating*, NFP::model::Rating*);

class User
{
    private:
        uint _user_id;
        std::list<NFP::model::Rating*> _ratings;
        
        float _avg_rating;
        
    public:
        User(uint);
        uint nb_ratings();
        void add_rating(NFP::model::Rating*);
        float avg_rating();
        std::list<NFP::model::Rating*>* ratings() {return &_ratings;};
};

}   
}

#endif