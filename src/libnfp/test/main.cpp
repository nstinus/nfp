#include <glog/logging.h>

//#include "Rating.h"

//#include <list>
#include <string>

#include <QDate>
#include <QString>


int main (int argc, char const *argv[])
{
    google::InitGoogleLogging(argv[0]);
    google::InstallFailureSignalHandler();
    
    #if 0
    {
    const int NB_RATINGS = 300000000;
    Rating* Ratings = new Rating[NB_RATINGS];
    
    LOG(INFO) << "Creating " << NB_RATINGS << " Rating s.";
    Rating tmpRating;
    for (int i = 0; i < NB_RATINGS; ++i)
    {
        Ratings[i] = Rating((ushort)(i % 177701),
                            (uint)  (i % 480190),
                            (uchar) (i % 5),
                            (ushort)(i % 1000));
    }
    LOG(INFO) << "Done creating.";
    
    LOG(INFO) << "Now trying to calculate the silly repartition of the rates...";
    unsigned long int sum_rate[5];
    for (int i = 0; i < 5; ++i)
        sum_rate[i] = 0;
    
    for (int i = 0; i < NB_RATINGS; ++i)
        sum_rate[(uint)Ratings[i].rate()]++;
    
    for (int i = 0; i < 5; ++i) 
        LOG(INFO) << "Rate " << i << " "
            << (float)sum_rate[i] / (float)NB_RATINGS * 100 << " %";
    
    LOG(INFO) << "Deleting...";
    
    delete[] Ratings;
    LOG(INFO) << "Done";
    }
    #endif
    
    if (QDate(2009, 11, 11).isValid())
        LOG(INFO) << "Date is valid...";
        
    LOG(INFO) << QDate(2009, 11, 11).toString(QString("yyyy-MM-dd")).toStdString();
    
    QDate min = QDate();
    if (min.isNull())
        LOG(INFO) << "Date is null and it's normal...";
        
    min = QDate::fromString(QString("1999-11-11"), QString("yyyy-MM-dd"));
    if (min.isNull())
        LOG(ERROR) << "Date shouldn't be null...";
        
    LOG(INFO) << "S " << min.toString(QString("yyyy-MM-dd")).toStdString() << " "
        << "J " << min.toJulianDay() << " "
        << "JS " << QDate::fromJulianDay(min.toJulianDay()).toString("yyyy-MM-dd").toStdString();
    
    
    
    return 0;
}