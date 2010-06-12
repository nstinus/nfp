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

struct User
{
  uint user_id;
  uint nb_ratings;
  unsigned long long summed_ratings;
  float mean_rating;

  User(uint uid): user_id(uid), nb_ratings(0), summed_ratings(0), mean_rating(-1) {}
  User(): user_id(-1), nb_ratings(0), summed_ratings(0), mean_rating(-1) {}
  
  void add_rating(ushort r) {
    summed_ratings += r;
    nb_ratings++;
  }

  void compute() {
    mean_rating = (float)((double)summed_ratings/(double)nb_ratings);
  }
};

class UserMeanAlgo : public BaseAlgo
{

public:
    UserMeanAlgo() : BaseAlgo("User_Mean", "0.1") {}
    virtual int run();
    float get_mean_rating(uint uid) {
      User* u = users_.find(uid)->second;
      return (u != NULL) ? u->mean_rating : -1;
    }
    uint get_nb_ratings(uint uid) {
      User* u = users_.find(uid)->second;
      return (u != NULL) ? u->nb_ratings : 0;
    }
    virtual float get_predicted_rate(uint u_id, ushort /*m_id*/, std::string /*date*/) {
      return get_mean_rating(u_id);
    }
    std::map<uint, User*> const& get_users() const{
      return users_;
    }
    
private:
    std::map<uint, User*> users_;
};

    
}
}

#endif // __NFP__USERMEANALGO_H__
