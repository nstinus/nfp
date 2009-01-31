#ifndef __RATING_H__
#define __RATING_H__

/*************************************************************************************************/
/*                                            Rating                                             */
/*************************************************************************************************/
/*                                                                                               */
/*  Abstract:                                                                                    */
/*    Offers fancy/human-friendly getters and setters for BasicRating                            */
/*                                                                                               */
/*************************************************************************************************/
/*                                                                                               */
/*  Build:                                                                                       */
/*      There's no build! That shit is header only. Just include this file.                      */
/*                                                                                               */
/*************************************************************************************************/

#include <string>
#include <QDate>

#include "BasicRating.h"

// int value in JulianDay of the min value of the training set: 

const uint RATING__DATE_OFFSET = 2451494;

namespace NFP
{

class Rating
{
private:
    BasicRating _rating;
    
public:
    
    /******************/
    /*  Constructors  */
    /******************/
    
    Rating(int const& m, int const& u, int const& r, int const& d) {
        DLOG(INFO) << "Rating " << m << " " << u << " " << r << " " << d;
        _rating = BasicRating((ushort)m, (uint)u, (uchar)r, (ushort)d);
    }
    
    Rating(int const& m, int const& u, int const& r, std::string const& d)
    {
        Rating(m, u, r, QDate::fromString(QString::fromStdString(d), QString("yyyy-MM-dd")));
    }
    
    Rating(int const& m, int const& u, int const& r, QDate const& d)
    {
        Rating(m, u, r, d.toJulianDay() - (int)RATING__DATE_OFFSET);
    }
    
    //virtual ~Rating();
    

    /*************/
    /*  Getters  */
    /*************/

    int movie_id() const { return _rating.movie_id(); }

    int user_id() const { return (int)_rating.user_id(); }

    int rate() const { return (int)_rating.rate(); }

    int date() const
    {
        return (int)_rating.date() + (int)RATING__DATE_OFFSET;
    }

    /*QDate date() const
    {
        return QDate::fromJulianDay(date());
    }*/
    
    /*std::string date() const
    {
        return ((QDate::fromJulianDay(date())).toString("yyyy-MM-dd").toStdString();
    }*/
    
    std::string toStdString()
    {
        char ret[40];
        sprintf(ret, "%5d %10d %1d %10s",
            movie_id(),
            user_id(), rate(),
            QDate::fromJulianDay(date()).toString("yyyy-MM-dd").toStdString().c_str());
        return (std::string)ret;
        
    }

    
    /*************/
    /*  Setters  */
    /*************/

    /*
    void set_movie_id(int const& movie_id)
    {
        _rating.set_movie_id((ushort)movie_id);
    }

    void set_user_id(int const& u_id)
    {
        _rating.set_user_id((uint)u_id);
    }

    void set_rate(int const& r)
    {
        _rating.set_rate((uchar)r);
    }

    void set_date(int const& d)
    {
        _rating.set_date((ushort)d);
    }
    
    void set_date(QDate const& d)
    {
        set_date(d.toJulianDay() - (int)RATING__DATE_OFFSET);
    }
    
    void set_date(std::string const& d)
    {
        set_date(QDate::fromString(QString::fromStdString(d), "yyyy-MM-dd"));
    }
    */
};

}

#endif // __RATING_H__