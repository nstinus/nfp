#ifndef __NFP__MOVIEMEANALGO_H__
#define __NFP__MOVIEMEANALGO_H__

#include <map>
#include <string>

#include "BaseAlgo.h"
#include "RatingsManager.h"

namespace NFP
{

namespace algos
{

class MovieMeanAlgo : public BaseAlgo
{

public:
    MovieMeanAlgo() : BaseAlgo("Movie_Mean", "0.1") {}
    virtual int run();
    std::map<ushort, float>* result() {return &mean_rates_;}
    void* get_results() {return &mean_rates_;}
    virtual float get_predicted_rate(uint, ushort, std::string);
    
    
private:
    std::map<ushort, float> mean_rates_;
};

    
}
}

#endif // __NFP__MOVIEMEANALGO_H__
