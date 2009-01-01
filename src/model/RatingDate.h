#ifndef __RATINGDATE_H__
#define __RATINGDATE_H__

#include <string>

#include "typedefs.h"

#define RATINGDATE__VALUE_STORAGE_TYPE usint;
#define DATE_OFFSET 0

class QDate;

class RatingDate
{
private:
    usint _value;
    
public:
    RatingDate(const usint &v) {set(v);};
    RatingDate(const std::string &v) {set(v);};
    RatingDate(const QDate &v) {set(v);};
    
    usint getUSInt();
    QDate getQDate();
    std::string getStdString();
    
    void set(const usint &);
    void set(const QDate &);
    void set(const std::string &);
    
    static const RatingDate MIN() { return RatingDate(DATE_OFFSET); };
    static const RatingDate MAX() { return RatingDate(DATE_OFFSET); };
};

#endif // __RATINGDATE_H__