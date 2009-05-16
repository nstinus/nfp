#ifndef __NFP__RATING_H__
#define __NFP__RATING_H__

/*************************************************************************************************/
/*                                          Rating                                               */
/*************************************************************************************************/
/*                                                                                               */
/*  Abstract:                                                                                    */
/*    Low-level representation for a rating from the training_set.                               */
/*    The fields are: movie_id [0, 17770],   card = 17770  < 2**15 -> 15 bits                    */
/*                    user_id  [6, 2649429], card = 480189 < 2**24 -> 24 bits                    */
/*                    rate     [1, 5],       card = 5      < 2**3  ->  3 bits                    */
/*                    date     [0, 2182],    card = 2182   < 2**12 -> 12 bits                    */
/*                                                                                               */
/*  Visual representation of the data:                                                           */
/*                                                                                               */
/*      Bytes :      |       |       |       |       |       |       |       |       |           */
/*      char[i]:     -       7       6       5       4       3       2       1       0           */
/*      Fields:      |X--------------X-----------------------X    X--X   X-----------X           */
/*      Name:        |    Movie_id   |        User_id        |  Rate |      Date     |           */
/*      Size:        |       15      |           24          |    3  |       12      |           */
/*                                                                                               */
/*  General considerations:                                                                      */
/*                                                                                               */
/*      - the data integrity check is made at the insertion. (masks).                            */
/*      - the getters just do silly casts.                                                       */
/*                                                                                               */
/*************************************************************************************************/

#include <string>

typedef unsigned short int usint;
typedef unsigned long int ulong;
typedef unsigned char uchar;

typedef unsigned int uint;
typedef unsigned short ushort;

const uint BASICRATING__DATE_POS      = 0;
const uint BASICRATING__RATE_POS      = 2;
const uint BASICRATING__USER_ID_POS   = 3;
const uint BASICRATING__MOVIE_ID_POS  = 6;

const uint BASICRATING__DATE_MASK     = 0x0FFF;
const uint BASICRATING__RATE_MASK     = 0x0007;
const uint BASICRATING__USER_ID_MASK  = 0x00FFFFFF;
const uint BASICRATING__MOVIE_ID_MASK = 0x7FFF;

const uint RATING_DATA_SIZE = 8;

const uint BASICRATING__DATE_OFFSET = 2451494;

namespace NFP
{

namespace utils {
std::string DateUS2S(ushort const&);
ushort DateS2US(std::string const&);
}    

namespace model
{

class Rating
{
private:
    char data_[RATING_DATA_SIZE];

public:
    /*****************/
    /*  Constructor  */
    /*****************/    
    
    Rating();
    Rating(ushort const&, uint const&, uchar const&, ushort const&);
    Rating(int const&, int const&, int const&, std::string const&);
    Rating(char const*);
    
    //virtual ~Rating();

    /*************/
    /*  Getters  */
    /*************/

    ushort movie_id() const;
    //int movie_id() const;
    uint user_id() const;
    uchar raw_rate() const;
    ushort rate() const;
    ushort raw_date() const;
    std::string date() const;
    std::string to_string();
    void data(char[]) const;

    /*************/
    /*  Setters  */
    /*************/

public:
    
    void set_movie_id(ushort const&);
    void set_user_id(uint const&);
    void set_rate(uchar const&);
    void set_date(ushort const&);

};

}
} // NFP

#endif // __NFP__RATING_H__