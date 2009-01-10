#include <tclap/CmdLine.h>
#include <glog/logging.h>

// Qt
#include <QDir>
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QRegExp>
#include <QMap>
//#include <QSharedMemory>

// STL
#include <string>
#include <list>

#include <Rating.h>

typedef unsigned char uchar;
//typedef std::list<std::string> stringList;
//typedef std::list<std::string>::iterator stringListIterator;

//#define SHM_START_KEY 0
//#define SHM_NB_SEGMENTS = 1000;

int main (int argc, char** argv)
{
    
    // Initializing the logging facility
    google::InitGoogleLogging(argv[0]);
    google::InstallFailureSignalHandler();
    
    // Commandline handling
    TCLAP::CmdLine cmd("Loads a training_set in shared memory.", ' ', "");
    TCLAP::ValueArg<std::string> pathArg("p",
                                         "path",
	                                     "Path where the training_set files are located.",
	                                     true,
	                                     "training_set",
                                         "string");
    TCLAP::ValueArg<std::string> filterArg("f",
                                           "filter",
                                 	       "Filename filter.",
                                 	       false,
                                 	       "mv_*.txt",
                                           "string");
    
    cmd.add(pathArg);
    cmd.add(filterArg);
    cmd.parse(argc, argv);

	std::string path = pathArg.getValue();
    std::string filter = filterArg.getValue();
    
	LOG(INFO) << "path = " << path;
	LOG(INFO) << "filter = " << filter;
    
	QDir training_set_dir(QString::fromStdString(path), QString::fromStdString(filter));
	if (!training_set_dir.exists()) {
        LOG(ERROR) << path << " does not exist.";
        exit(1);	    
	}
	
    std::list<QString> files = training_set_dir.entryList().toStdList();
    std::list<Rating*> ratings;
    //string_list files;

    QString movie_id_QS;
    ushort movie_id = 0;
    uint user_id = 0;
    uchar rate = 0;
    
    QRegExp mvFileLineRegExp("^(\\d+),([1-5]),([12]\\d{3})-(0[1-9]|1[0-2])-(\\d{2})$");
    
    for (std::list<QString>::iterator it = files.begin(); it != files.end(); ++it)
    {
        LOG(INFO) << "Proceeding file " << (*it).toStdString();
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
        movie_id = movie_id_QS.toUShort();
        //LOG(INFO) << "Movie_id: <" << (int)movie_id << ">";
        
        QString line;
        Rating rating;
        do {
            line = stream.readLine();
            if (mvFileLineRegExp.indexIn(line) > -1) {
                user_id = mvFileLineRegExp.cap(1).toUInt();
                rate = (uchar)mvFileLineRegExp.cap(2).toUShort();
                //LOG(INFO) << (int)movie_id << " " << (int)user_id << " " << (int)rate;
                ratings.push_back(new Rating(movie_id, user_id, rate, 0));
            }
        } while (!line.isNull());
        
        file.close();
    }
    
   
    
    LOG(INFO) << ratings.size() << " ratings found";
    
    QMap<ushort, ulong> ratingsSummedByMovie;
    QMap<ushort, uint> nbRatingsByMovie;
    
    for (std::list<Rating*>::iterator it = ratings.begin();
            it != ratings.end(); ++it) {
            
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
        LOG(INFO) << rsum.key() << " " << (double)rsum.value() / nbRatingsByMovie[rsum.key()];
    }
    
    
    
    /*
    std::list<QSharedMemory*> shm_segments;
    for (int i = 0; i < 100; ++i)
    {
        QSharedMemory* shm = new QSharedMemory(QString::number((int)SHM_START_KEY + i));
        //const int shm_size = 1000000;
        if (!shm->create(sizeof(Rating))){
            LOG(ERROR) << "Shm creation failed for " << sizeof(Rating) << " bytes";
            LOG(ERROR) << "Error code: " << shm->error();
            LOG(ERROR) << shm->errorString().toStdString();
            
            if(shm->error() == 4) {
                if (!shm->attach()) {
                    LOG(INFO) << "Unable to attach to shm";
                }
            }
            //exit(10 + shm->error());
            break;
        }
        else {
            shm_segments.push_back(shm);
        }
    }
    LOG(INFO) << "Shm created for " << shm_segments.size() << " segments";
    
    //sleep(300);
    
    for (std::list<QSharedMemory*>::iterator it = shm_segments.begin();
        it != shm_segments.end();
        ++it) {
            if(!(*it)->detach()) {
                LOG(ERROR) << "Unable to detach shm.";
            }
            delete *it;
        }
    LOG(INFO) << "Shm detached.";
    */
    
    for (std::list<Rating*>::iterator it = ratings.begin();
            it != ratings.end(); ++it) {
            /*LOG(INFO) << "Rating: "
                << (int)(*it)->movie_id() << " "
                << (int)(*it)->user_id() << " "
                << (int)(*it)->rate();*/
            delete (*it);
    }
    
    return EXIT_SUCCESS;
}