#ifndef __RATINGDATE_H__
#define __RATINGDATE_H__

#include <string>
#include <math.h>
#include <QDate>
#include <QString>

#include "typedefs.h"

#define RATINGDATE__OFFSET 730069 // MIN from src/scrips/dates_spread.py
#define RATINGDATE__MIN_DATE RATINGDATE__OFFSET
#define RATINGDATE__MAX_DATE RATINGDATE__OFFSET + pow(2, sizeof(usint) * 7) - 1


class RatingDate
{
private:
    usint _value;
    
public:
    RatingDate(const usint &v) {set(v);};
    RatingDate(const std::string &v) {set(v);};
    RatingDate(const QDate &v) {set(v);};
    
    usint getUSInt() { return _value + (usint)RATINGDATE__OFFSET; };
    QDate getQDate() { return QDate::fromJulianDay(getUSInt()); };
    std::string getStdString() { return getQDate().toString().toStdString(); };
    
    void set(const usint &v) { _value = v - (usint)RATINGDATE__OFFSET; };
    void set(const QDate &v); { set(v.toJulianDay()); };
    void set(const std::string &v) {
        set(QDate::fromString(QString::fromStdString(v), "yyyy-MM-dd"));
    };
    
    static const RatingDate MIN() { return RatingDate(RATINGDATE__MIN_DATE); };
    static const RatingDate MAX() { return RatingDate(RATINGDATE__MAX_DATE); };
};

#endif // __RATINGDATE_H__