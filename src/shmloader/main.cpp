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
//#include <QSharedMemory>

// STL
#include <string>
#include <list>

#include <Rating.h>

typedef unsigned char uchar;
//typedef std::list<std::string> stringList;
//typedef std::list<std::string>::iterator stringListIterator;


int main (int argc, char** argv)
{
    
    // Initializing the logging facility
    google::InitGoogleLogging(argv[0]);
    google::InstallFailureSignalHandler();
    
    // Commandline handling
    TCLAP::CmdLine cmd("Loads `training_set'-like files in shared memory.", ' ', "" __DATE__ " " __TIME__ "");
    char* defPath = getenv("NFP_TRAINING_SET");
    if (defPath == NULL)
        defPath = "";
    TCLAP::ValueArg<std::string> pathArg("i",
        "input_data",
	    std::string("Path where the data files are located.")
            + std::string("These files should have the same")
            + std::string("format as the original training_set. ")
            + std::string("Will try to read the environment variable NFP_TRAINING_SET <")
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

	std::string path = pathArg.getValue();
    std::string filter;
	if (movieIdArg.getValue() > -1) {
        char f[12];
        sprintf(f, "mv_%07d.txt", movieIdArg.getValue());
        filter = f;
	}
	else if (!filterArg.getValue().empty()) {
	    filter = filterArg.getValue();
	}
	//else {
    //    filter = "*";
	//}
    
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