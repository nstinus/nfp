#include "RatingDate.h"

#include <QDate>
#include <QString>

void RatingDate::set(const usint &v)
{
    _value = v - (usint)RATINGDATE__OFFSET;
}

void RatingDate::set(const QDate &v)
{
    set(v.toJulianDay());
}

void RatingDate::set(const std::string &v)
{
    set(QDate::fromString(QString::fromStdString(v), "yyyy-MM-dd"));
}