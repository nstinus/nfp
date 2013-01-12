#ifndef __NFP__MOVIEMEANALGO_H__
#define __NFP__MOVIEMEANALGO_H__

#include <map>
#include <string>

#include "core/Defs.hpp"
#include "algos/BaseAlgo.hpp"

namespace NFP
{

namespace algos
{

class BaseAlgo;

class MovieMeanAlgo : public BaseAlgo
{

public:
    MovieMeanAlgo() : BaseAlgo("Movie_Mean", "0.1") {
        for (int i = 0; i < MAX_NB_MOVIES; ++i) {
            mean_rates_[i] = -1.0;
        }
    }
    virtual int run();
    virtual float get_predicted_rate(uint32_t, uint16_t, std::string);
    
    
private:
    float mean_rates_[MAX_NB_MOVIES];
};

    
}
}

#endif // __NFP__MOVIEMEANALGO_H__
