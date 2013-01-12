#ifndef __NFP__BASEALGO_H__
#define __NFP__BASEALGO_H__

#include <string>

#include "core/logging.hpp"
#include "core/Singleton.hxx"

namespace NFP
{

namespace algos
{

class BaseAlgo : public NFP::base::Singleton<BaseAlgo>
{
    friend class base::Singleton<BaseAlgo>;
    
public:
    BaseAlgo(std::string name, std::string version) :
        name_(name),
        version_(version) {}
    virtual ~BaseAlgo() {}
    
    virtual int run() = 0;
    void logStart() { LOG(INFO) << "Starting " << name_ << " - " << version_; }
    void logEnd() { LOG(INFO) << "Done " << name_ << " - " << version_; }
    virtual float get_predicted_rate(uint32_t /* user_id */, uint16_t /* movie_id */, std::string /* date */) = 0;
        
private:
    std::string name_;
    std::string version_;
    time_t start_time_;
    time_t stop_time_;
};

    
}
}

#endif // __NFP__BASEALGO_H__
