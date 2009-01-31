#include <tclap/CmdLine.h>
#include <glog/logging.h>

#include <stdio.h>
#include <stdlib.h>

// Qt
#include <QDir>
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QRegExp>
#include <QMap>
#include <QList>
#include <QDate>
//#include <QSharedMemory>

// STL
#include <string>
//#include <list>

// Custom
#include "BasicRating.h"

#include <sys/shm.h>

typedef unsigned char uchar;
//typedef std::list<std::string> stringList;
//typedef std::list<std::string>::iterator stringListIterator;

/*
void pushRatingToShm(key_t const& key, NFP::BasicRating* rating)
{
    int shmid;
    NFP::BasicRating* shmPtr;
    
    DLOG(INFO) << "Key = " << (long)key;
    if ((shmid = shmget(key, sizeof(NFP::BasicRating), 0600 | IPC_CREAT)) < 0) {
        LOG(ERROR) << "shmget";
        perror("shmget");
    };
    
    DLOG(INFO) << "shmid = " << shmid;
    if ((shmPtr = (NFP::BasicRating*)shmat(shmid, (void *)0, 0)) == (void*)-1) {
        LOG(ERROR) << "shmat";
        perror("shmat");
        exit(1);
    }
    
    rating = shmPtr;
    
    DLOG(INFO) << (int)rating->movie_id() << " "
        << (int)rating->user_id() << " "
        << (int)rating->rate();
}
*/

int main (int argc, char** argv)
{
    
    // Initializing the logging facility
    google::InitGoogleLogging(argv[0]);
    google::InstallFailureSignalHandler();
    
    LOG(INFO) << "sizeof(NFP::BasicRating) = " << sizeof(NFP::BasicRating);
    //LOG(INFO) << "sizeof(NFP::Rating) = " << sizeof(NFP::Rating);
    
    
    // Commandline handling
    TCLAP::CmdLine cmd("Loads `training_set'-like files in shared memory.",
        ' ',
        "" __DATE__ " " __TIME__ "");
    char* defPath = getenv("NFP_TRAINING_SET_DIR");
    if (defPath == NULL)
        defPath = "";
    TCLAP::ValueArg<std::string> pathArg("i",
        "input_data",
	    std::string("Path where the data files are located.")
            + std::string("These files should have the same")
            + std::string("format as the original training_set. ")
            + std::string("Will try to read the environment variable NFP_TRAINING_SET_DIR <")
            + defPath
            + std::string("> if unspecified."),
        false,
        defPath,
        "string");
    TCLAP::ValueArg<std::string> filterArg("f",
        "filter",
        "Filename filter. Default is `mv_*.txt'",
        false,
        "mv_*.txt",
        "string");
    TCLAP::ValueArg<int> movieIdArg("m",
        "movie_id",
        std::string("Will be passed to a mv_%07d.txt filter. ")
            + std::string("Useful to load a specific movie_id from the training_set."),
        false,
        -1,
        "int");
    
    cmd.add(pathArg);
    cmd.add(filterArg);
    cmd.add(movieIdArg);
    cmd.parse(argc, argv);

	std::string path = pathArg.getValue().empty() ? std::string(".") : pathArg.getValue();
    std::string filter;
	if (movieIdArg.getValue() > -1) {
        char f[12];
        sprintf(f, "mv_%07d.txt", movieIdArg.getValue());
        filter = f;
	}
	else if (!filterArg.getValue().empty()) {
	    filter = filterArg.getValue();
	}
	
	LOG(INFO) << "path = " << path;
	LOG(INFO) << "filter = " << filter;
    
	QDir training_set_dir(QString::fromStdString(path), QString::fromStdString(filter));
	if (!training_set_dir.exists()) {
        LOG(ERROR) << path << " does not exist.";
        exit(1);	    
	}
	
    QStringList files = training_set_dir.entryList();
    //QList<NFP::Rating*> ratings;

    QString movie_id_QS;
    int movie_id = 0;
    int user_id = 0;
    int rate = 0;
    std::string date = "";
    
    QRegExp mvFileLineRegExp("^(\\d+),([1-5]),(\\d{4}-\\d{2}-\\d{2})$");
    
    //key_t key = ftok("/Users/nico/projects/dev/nfp/data/shm/training_set.shmkey", 'R');
    
    for (QList<QString>::iterator it = files.begin(); it != files.end(); ++it)
    {
        LOG(INFO) << "Processing file " << (*it).toStdString();
        QString filePath = training_set_dir.absolutePath() + QString("/") + (*it);
        QFile file(filePath);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            LOG(ERROR) << "Unable to open " << file.fileName().toStdString()
                << " for reading. Skipping.";
            continue;
        }
        QTextStream stream(&file);
        movie_id_QS = stream.readLine();
        movie_id_QS.chop(1);
        movie_id = movie_id_QS.toInt();
        //LOG(INFO) << "Movie_id: <" << (int)movie_id << ">";
        
        QString line;
    
        do {
            line = stream.readLine();
            if (mvFileLineRegExp.indexIn(line) > -1) {
                user_id = mvFileLineRegExp.cap(1).toInt();
                rate = mvFileLineRegExp.cap(2).toInt();
                date = mvFileLineRegExp.cap(3).toStdString();
                LOG(INFO) << "Data read: "
                    << movie_id << ", "
                    << user_id << ", "
                    << rate << ", "
                    << date;
                //NFP::Rating* r = new NFP::Rating(movie_id, user_id, rate, date);
                
                NFP::BasicRating* br = new NFP::BasicRating(movie_id, user_id, rate, date);
                
                //ratings.push_back(r);
                /*DLOG(INFO) << "Created Rating: "
                    << r->movie_id() << ", "
                    << r->user_id() << ", "
                    << r->rate() << ", "
                    << QDate::fromJulianDay(r->date()).toString("yyyy-MM-dd").toStdString();*/
                //LOG(INFO) << " R " << r->toStdString();
                LOG(INFO) << "BR " << br->toStdString() ;
                
            }
        } while (!line.isNull());
        
        file.close();
    }
    
    //LOG(INFO) << ratings.size() << " ratings found";
    
    //Means calculations
    //LOG(INFO) << ratings.size() << " ratings found";
    
    #if 0
    QMap<ushort, ulong> ratingsSummedByMovie;
    QMap<ushort, uint> nbRatingsByMovie;
    QMap<ushort, double> meanRateByMovie;
    
    for (QList<NFP::BasicRating*>::iterator it = ratings.begin(); it != ratings.end(); ++it) {
            
            if (!ratingsSummedByMovie.contains(movie_id))
                ratingsSummedByMovie[movie_id] = 0;
            if (!nbRatingsByMovie.contains(movie_id))
                nbRatingsByMovie[movie_id] = 0;
                
            ratingsSummedByMovie[(*it)->movie_id()] += (ulong)(*it)->rate();
            nbRatingsByMovie[(*it)->movie_id()]++;
    }
    
    QMapIterator<ushort, ulong> rsum(ratingsSummedByMovie);
    while(rsum.hasNext()) {
        rsum.next();
        meanRateByMovie[rsum.key()] = (double)rsum.value() / nbRatingsByMovie[rsum.key()];
        LOG(INFO) << rsum.key() << " " << meanRateByMovie[rsum.key()];
    }
    
    double rateMeansSum = 0;
    QMapIterator<ushort, double> rmean(meanRateByMovie);
    while(rmean.hasNext()) {
        rmean.next();
        rateMeansSum += rmean.value();
    }
    LOG(INFO) << "Mean = " << rateMeansSum / (float)meanRateByMovie.size();
    
    #if 0
    {
    key_t key;
    int shmid;
    NFP::BasicRating* shmPtrRating;

    key = ftok("/Users/nico/projects/dev/nfp/data/shm/training_set.shmkey", 'R');
    
    
    LOG(INFO) << "Key = " << (long)key;
    if ((shmid = shmget(key, sizeof(NFP::BasicRating), 0600 | IPC_CREAT)) < 0) {
        LOG(ERROR) << "shmget";
        perror("shmget");
    };
    LOG(INFO) << "shmid = " << shmid;
    if ((shmPtrRating = (NFP::BasicRating*)shmat(shmid, (void *)0, 0)) == (void*)-1) {
        LOG(ERROR) << "shmat";
        perror("shmat");
    }
    LOG(INFO) << "shmPtrRating = " << shmPtrRating;
    
    LOG(INFO) << "Un-initialized values: " << (int)shmPtrRating->movie_id() << " "
        << (int)shmPtrRating->user_id() << " "
        << (int)shmPtrRating->rate() << " ";
    
    LOG(INFO) << "Shm assignation";
    for (QList<NFP::BasicRating*>::iterator it = ratings.begin(); it != ratings.end(); ++it) {
        //memcpy(shmPtrRating, (*it), sizeof(Rating));
        *shmPtrRating = *(*it);
        LOG(INFO) << "Wrote: " << (int)shmPtrRating->movie_id() << " "
            << (int)shmPtrRating->user_id() << " "
            << (int)shmPtrRating->rate() << " ";
        //sleep(1);
    }
    
    sleep(1);
    
    shmdt(shmPtrRating);
    LOG(INFO) << "Detaching shm";
    
    shmctl(shmid, IPC_RMID, NULL);
    LOG(INFO) << "Deleting shm segment";
    
    #if 0
    {
    //Working shm part. needed memcopy
    key_t key = ftok("/Users/nico/projects/dev/nfp/data/shm/1", 'R');
    QSharedMemory shm(QString::number(key));
    if (shm.create(sizeof(NFP::BasicRating), QSharedMemory::ReadWrite)) {
        LOG(INFO) << "Shm segment created, size = " << shm.size();
    } else {
        LOG(ERROR) << "Shm segment creation failed: " << shm.errorString().toStdString();
        if (shm.attach(QSharedMemory::ReadWrite)) {
            LOG(INFO) << "Shm segment attached";
        } else {
            LOG(ERROR) << "Shm segment attach failed";
            exit(1);
        }
    }
    LOG(INFO) << "Shm key = " << shm.key().toStdString();
    
    NFP::BasicRating* shmRating = (NFP::BasicRating*)shm.data();
    
    LOG(INFO) << "Read: " << (int)shmRating->movie_id() << " "
        << (int)shmRating->user_id() << " "
        << (int)shmRating->rate() << " ";
    
    for (QList<NFP::BasicRating*>::iterator it = ratings.begin(); it != ratings.end(); ++it) {
        shm.lock();
        memcpy(shmRating, (*it), sizeof(NFP::BasicRating));
        //LOG(INFO) << "shm written";
        LOG(INFO) << "Wrote: " << (int)shmRating->movie_id() << " "
            << (int)shmRating->user_id() << " "
            << (int)shmRating->rate() << " ";
        //sleep(1);
    }
    
    shm.detach();
    }
    #endif
    }
    #endif
    
    /*
    int shmid;
    NFP::BasicRating* shmPtr;
    
    DLOG(INFO) << "Key = " << (long)key;
    if ((shmid = shmget(key, ratings.size() * sizeof(NFP::BasicRating), 0600 | IPC_CREAT)) < 0) {
        LOG(ERROR) << "shmget";
        perror("shmget");
    };
    
    DLOG(INFO) << "shmid = " << shmid;
    if ((shmPtr = (NFP::BasicRating*)shmat(shmid, (void *)0, 0)) == (void*)-1) {
        LOG(ERROR) << "shmat";
        perror("shmat");
        exit(1);
    }
    
    int i = 0;
    for (QList<NFP::BasicRating*>::iterator it = ratings.begin(); it != ratings.end(); ++it) {
        // *(shmPtr + i) = *(*it);
        //DLOG(INFO) << "(*it) " << (int)(*it)->movie_id() << " "
        //    << (int)(*it)->user_id() << " "
        //    << (int)(*it)->rate();
            
        memcpy(shmPtr+i, *it, sizeof(NFP::BasicRating));
        
        //DLOG(INFO) << (int)(shmPtr + i)->movie_id() << " "
        //    << (int)(shmPtr + i)->user_id() << " "
        //    << (int)(shmPtr + i)->rate();
        i++;
    }
    
    const int nb_ratings = ratings.size();
    
    LOG(INFO) << "Sleeping 1 sec...";
    sleep(1);

    for(int i = 0; i < nb_ratings; ++i) {
        LOG_EVERY_N(INFO, 1000) << (int)(shmPtr+i)->movie_id() << " "
            << (int)(shmPtr+i)->user_id() << " "
            << (int)(shmPtr+i)->rate();
    }
    
    LOG(INFO) << "Sleeping 1 sec...";
    sleep(1);
    
    shmdt(shmPtr);
    LOG(INFO) << "Detaching shm";
    
    shmctl(shmid, IPC_RMID, NULL);
    LOG(INFO) << "Deleting shm segment";
    */
    
    // Clean-up
    LOG(INFO) << "Cleaning up ... ";
    for (QList<NFP::BasicRating*>::iterator it = ratings.begin(); it != ratings.end(); ++it)
            delete (*it);
            
    #endif
    //ratings.clear();
    
    LOG(INFO) << "Bye-bye!";
    
    return EXIT_SUCCESS;
}