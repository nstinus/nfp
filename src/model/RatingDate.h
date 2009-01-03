#ifndef __RATINGDATE_H__
#define __RATINGDATE_H__

#include <glog/logging.h>

#include <string>
#include <math.h>
//#include <QDate>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <QString>

#include "typedefs.h"

#define RATINGDATE__OFFSET 0 // MIN from src/scrips/dates_spread.py
#define RATINGDATE__MIN_DATE RATINGDATE__OFFSET
#define RATINGDATE__MAX_DATE RATINGDATE__OFFSET + pow(2, sizeof(short) * 8 - 1) - 1
#define RATINGDATE__DATE_FORMAT "yyyy-MM-dd"

class RatingDate
{
private:
    usint _value;

    
public:
    RatingDate(const usint &v) { set(v); };
    RatingDate(const std::string &v) { set(v); };
    RatingDate(const date &v) { set(v); };
    
    usint getUSInt() {
        usint ret = _value;
        DLOG(INFO) << "Returning " << ret;
        return ret;
    };
    
    //int getJulianDay() { return _value + (usint)RATINGDATE__OFFSET; };
    
    //date getDate() { return date; };
    
    std::string getStdString() {
        return "";
        //return getQDate().toString(RATINGDATE__DATE_FORMAT).toStdString();
    };
    
    void set(const usint &v) {
        _value = v - (usint)RATINGDATE__OFFSET;
        DLOG(INFO) << "_value = " << _value;
    };
    
    void set(const date &v) { set((usint)v.julian_day()); };
    
    void set(const std::string &v) {
        set(boost::gregorian::from_simple_string(v));
    };
    
    static RatingDate MIN() { return RatingDate(RATINGDATE__MIN_DATE); };
    static RatingDate MAX() { return RatingDate(RATINGDATE__MAX_DATE); };
};

#endif // __RATINGDATE_H__