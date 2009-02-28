#include <glog/logging.h>

#include "Rating.h"
#include "RatingsSS.h"

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
    
    #if 0
    {
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
    }
    #endif
    
    std::string fileName = std::string(getenv("NFP_TRAINING_SET_DIR")) + "/"
        + std::string(argv[1]);
    std::string keyFileName = std::string(getenv("NFP_SHM_FILES")) + "/"
        + std::string(argv[1]) + std::string(".shmkey");
    NFP::RatingsSS ratings_1(fileName, keyFileName);
    //ratings_1.load();
    ratings_1.create();
    ratings_1.attach();
    
    int rates = 0;
    for (int i=0; i < ratings_1.nb_ratings(); i++)
        rates += ratings_1.ptr()[i].rate();
    LOG(INFO) << "Average rate " << float(rates) / ratings_1.nb_ratings();
    
    // NFP::RatingsSS ratings_1p(fileName);
    //     ratings_1.attach();
    //     
    //     for (int i=0; i < ratings_1p.nb_ratings(); i++)
    //         LOG(INFO) << ratings_1p.ptr()[i].to_string();
    
    
    //ratings_1.remove();
    
    return 0;
}