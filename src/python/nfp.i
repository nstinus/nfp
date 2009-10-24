%module nfp
%{
#include "Rating.h"
%}

%include "std_string.i"

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

}
}
