#ifndef __NFP__RATING_H__
#define __NFP__RATING_H__

/*************************************************************************************************/
/*                                          Rating                                               */
/*************************************************************************************************/
/*                                                                                               */
/*  Abstract:                                                                                    */
/*    Low-level representation for a rating from the training_set.                               */
/*    The fields are: movie_id [1, 17770],   card = 17770  < 2**15 -> 15 bits                    */
/*                    user_id  [6, 2649429], card = 480189 < 2**24 -> 24 bits                    */
/*                    rate     [1, 5],       card = 5      < 2**3  ->  3 bits                    */
/*                    date     [0, 2182],    card = 2182   < 2**12 -> 12 bits                    */
/*                                                                                               */
/*************************************************************************************************/

#include <string>
#include <tr1/cstdint>

#include "core/Date.hpp"


const uint32_t BASICRATING__DATE_OFFSET = 2451494;

namespace NFP
{

namespace model
{

class Rating
{
private:
  uint64_t movie_id_:16;
  uint64_t user_id_:24;
  uint64_t rate_:4;
  uint64_t date_:12;

public:
  static std::string DateUS2S(uint16_t const& date) {
    return NFP::utils::fromJulianDay(date + BASICRATING__DATE_OFFSET);
  }
  static uint16_t DateS2US(std::string const& date) {
    return (uint16_t)(NFP::utils::toJulianDay(date) - BASICRATING__DATE_OFFSET);
  }

public:
  /*****************/
  /*  Constructor  */
  /*****************/ 
    
  Rating() :
    movie_id_(0),
    user_id_(0),
    rate_(0),
    date_(0)
  {}
  Rating(uint16_t movie_id, uint32_t user_id, uint8_t rate, uint16_t date) :
    movie_id_(movie_id),
    user_id_(user_id),
    rate_(rate),
    date_(date)
  {}
  Rating(int movie_id, int user_id, int rate, std::string const& date) :
    movie_id_(movie_id),
    user_id_(user_id),
    rate_(rate),
    date_(DateS2US(date))
  {}

  /*************/
  /*  Getters  */
  /*************/

  uint16_t movie_id() const { return movie_id_; }
  uint32_t user_id() const { return user_id_; }
  uint8_t raw_rate() const { return rate_; }
  uint8_t rate() const { return rate_; }
  uint16_t raw_date() const { return date_; }
  std::string const date() const { return DateUS2S(date_); }
  std::string const to_string() const {
    char ret[40];
    sprintf(ret, "%05d  %08d  %1d  %10s",
	    movie_id(),
	    user_id(),
	    rate(),
	    date().c_str());
    return (std::string)ret;
  }

  /*************/
  /*  Setters  */
  /*************/

  void set_movie_id(uint16_t movie_id) { movie_id_ = movie_id; }
  void set_user_id(uint32_t user_id) { user_id_ = user_id; }
  void set_rate(uint8_t rate) { rate_ = rate; }
  void set_date(uint16_t date) { date_ = date; }
  void set_date(std::string const& date) { date_ = DateS2US(date); }

} /*__attribute__((packed))*/;

}
} // NFP

const uint32_t RATING_DATA_SIZE = sizeof(NFP::model::Rating);


#endif // __NFP__RATING_H__
