#ifndef __NFP__USERMEANALGO_H__
#define __NFP__USERMEANALGO_H__

#include <map>
#include <list>
#include <vector>
#include <string>

#include "BaseAlgo.h"
#include "RatingsManager.h"

namespace NFP
{

namespace algos
{

class UserMeanAlgo : public BaseAlgo
{

public:
    UserMeanAlgo() : BaseAlgo("User_Mean", "0.1") {}
    virtual int run();
    std::map<uint, float>* result() {return &user_mean_rates_;}
    void* get_results() {return &user_mean_rates_;}
    virtual float get_predicted_rate(uint, ushort, std::string);
    std::vector<uint>::const_iterator users_begin() {return users.begin();}
    std::vector<uint>::const_iterator users_end() {return users.end();}
    
private:
    std::map<uint, float> user_mean_rates_;
    std::list<NFP::model::Rating*>::const_iterator ratings_begin;
    std::list<NFP::model::Rating*>::const_iterator ratings_end;
    std::vector<uint> users;
    
    void init();
};

    
}
}

#endif // __NFP__USERMEANALGO_H__
