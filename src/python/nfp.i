%module nfp
%{
#include "Rating.h"
#include "RatingsManager.h"
#include "MovieMeanAlgo.h"
#include "Singleton.hxx"
#include "log.h"
#include "MoviesInfoProviderWrapper.h"
%}

%include "std_string.i"
%include "std_list.i"
%include "std_map.i"

//%template(RatingsList) std::list<NFP::model::Rating*>;

void initLogging(std::string const& appli_name);
void LOG_INFO (std::string const& message);
void LOG_WARN (std::string const& message);
void LOG_ERROR(std::string const& message);
void LOG_FATAL(std::string const& message);

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

namespace algos
{
class MovieMeanAlgo
{
public:
    MovieMeanAlgo();
    virtual int run();
    //std::map<unsigned short, float>* result();
    virtual float get_predicted_rate(unsigned int, unsigned short, std::string);
};
} // NFP::algos


namespace utils
{

int movieYear(int);
std::string movieName(int);
int MoviesInfoProvider_nbMovies();

}


}
