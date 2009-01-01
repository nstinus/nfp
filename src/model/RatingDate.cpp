#include "RatingDate.h"

#include <QDate>
#include <QString>

usint RatingDate::getUSInt()
{
    return _value;
}

QDate RatingDate::getQDate()
{
    return QDate::fromJulianDay(_value);
}

std::string RatingDate::getStdString()
{
    return getQDate().toString().toStdString();
}

void RatingDate::set(const usint &v)
{
    _value = v;
}

void RatingDate::set(const QDate &v)
{
    set(v.toJulianDay());
}

void RatingDate::set(const std::string &v)
{
    set(QDate::fromString(QString::fromStdString(v), "yyyy-MM-dd"));
}