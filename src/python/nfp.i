%module nfp
%{
#include "Rating.h"
#include "RatingsManager.h"
#include "Singleton.hxx"
#include "log.h"
%}

%include "std_string.i"
%include "std_list.i"

%template(RatingsList) std::list<NFP::model::Rating*>;

void initLogging(std::string);

namespace NFP
{
namespace model
{

class Rating
{
public:
    Rating();
    Rating(int const&, int const&, int const&, std::string const&);
    
    unsigned short movie_id() const;
    unsigned int user_id() const;
    unsigned short rate() const;
    std::string date() const;
    std::string to_string();
    
    void set_movie_id(unsigned short const&);
    void set_user_id(unsigned int const&);
    void set_rate(unsigned char const&);
    void set_date(std::string const&);

};

} // NFP::model


namespace shm
{
class RatingsManager
{
public:
    //RatingsManager();
    static RatingsManager* instance();
    void kill();
    //int load(std::string, bool);
    //int remove(std::string, bool);
    //int removeAll();
    //int save(std::string arg_movie_id = "", bool feedback = false);
    int nbSegments();
    void refreshRatingsList();
    unsigned int nb_ratings();
    std::list<NFP::model::Rating*>::const_iterator ratings_begin();
    std::list<NFP::model::Rating*>::const_iterator ratings_end();
};

} // NFP::shm


}
