//#include <tclap/CmdLine.h>
#include <glog/logging.h>

#include <dirent.h>
#include <errno.h>

#include <string>
#include <map>
#include <iostream>
#include <fstream>

#include <QDir>
#include <QString>
#include <QRegExp>
#include <QList>

#include "RatingsSS.h"


const std::string NFP_TRAINING_SET_DIR = getenv("NFP_TRAINING_SET_DIR");
const std::string NFP_SHM_FILES        = getenv("NFP_SHM_FILES");


int rm(std::string);
int load(std::string);
int infos(std::string);
void usage();
int ratings(std::string, int);

int movieYear(int);
std::string movieName(int);
void fillMoviesMaps();

std::map<int, int> movieYears;
std::map<int, std::string> movieNames;


int main (int argc, char* argv[])
{
    google::InitGoogleLogging(argv[0]);
    google::InstallFailureSignalHandler();
    
    if (argc > 1 && strcmp(argv[1], "remove") == 0)
        rm((argc > 2) ? argv[2] : "");
    else if (argc > 1 && strcmp(argv[1], "load") == 0)
        load((argc > 2) ? argv[2] : "");
    else if (argc > 1 && strcmp(argv[1], "infos") == 0)
        infos((argc > 2) ? argv[2] : "");
    else if (argc > 1 && strcmp(argv[1], "ratings") == 0)
        ratings((argc > 2) ? argv[2] : "", (argc > 3) ? atoi(argv[3]) : -1);
    else
        usage();
    
    return 0;
}


int rm(std::string movie_id = "")
{
    LOG(INFO) << "NFP_SHM_FILES = " << NFP_SHM_FILES;
    
    DIR *dp;
    struct dirent *dirp;
    if((dp  = opendir(NFP_SHM_FILES.c_str())) == NULL) {
        LOG(ERROR) << "Error(" << errno << ") opening " << NFP_SHM_FILES;
        return errno;
    }

    while ((dirp = readdir(dp)) != NULL) {
        std::string keyFileName = dirp->d_name;
        
        if ((int)keyFileName.find(".shmkey") == -1 ||
                (movie_id != "" && (int)keyFileName.find(movie_id) == -1))
            continue;
            
        std::string dataFileName = keyFileName;
        dataFileName.erase(dataFileName.end()-7, dataFileName.end());
        dataFileName = NFP_TRAINING_SET_DIR + std::string("/") + dataFileName;
        keyFileName = NFP_SHM_FILES + std::string("/") + keyFileName;
        
        NFP::RatingsSS mySSR(dataFileName, keyFileName);
        mySSR.create();
        mySSR.remove();
        std::cout << "Removed " << mySSR.keyFileName() << std::endl;
   }
   closedir(dp);
   return 0;
}


int load(std::string movie_id = "")
{
    QDir training_set_dir(QString::fromStdString(NFP_TRAINING_SET_DIR), "mv_*.txt");
    if (!training_set_dir.exists()) {
        LOG(ERROR) << NFP_TRAINING_SET_DIR << " does not exist.";
        return 1;	    
	}
	
    QStringList files = training_set_dir.entryList();
    
    QString movie_id_QS;
    
    QRegExp mvFileLineRegExp("^(\\d+),([1-5]),(\\d{4}-\\d{2}-\\d{2})$");
    
    for (QList<QString>::iterator it = files.begin(); it != files.end(); ++it)
    {
        LOG(INFO) << "Processing file " << (*it).toStdString();
        std::string dataFilePath = QString(training_set_dir.absolutePath()
                                        + QString("/")
                                        + (*it)).toStdString();
        std::string shmKeyFilePath = NFP_SHM_FILES;
        shmKeyFilePath += "/";
        shmKeyFilePath += (*it).toStdString();
        shmKeyFilePath += ".shmkey";
        
        if ((int)shmKeyFilePath.find(".shmkey") == -1 ||
                (movie_id != "" && (int)shmKeyFilePath.find(movie_id) == -1))
            continue;
        
        NFP::RatingsSS mySR(dataFilePath, shmKeyFilePath);
        if (mySR.create()) {
            LOG(ERROR) << "Pfiout!";
            exit(1);
        }
        
        std::cout << dataFilePath << " loaded as " << mySR.key() << std::endl;
    }
    
    return 0;
}


int infos(std::string movie_id = "")
{
    LOG(INFO) << "NFP_SHM_FILES = " << NFP_SHM_FILES;

    DIR *dp;
    struct dirent *dirp;
    if((dp  = opendir(NFP_SHM_FILES.c_str())) == NULL) {
        LOG(ERROR) << "Error(" << errno << ") opening " << NFP_SHM_FILES;
        return errno;
    }

    char* msg = new char[256];
    int total_ratings = 0;
    
    sprintf(msg, "%7s  %9s  %9s  %5s  %4s  %s",
        "#    id",
        "Nb. Rat.",
        "Mem.",
        "R ArM",
        "Year",
        "Name");
    std::cout << msg << std::endl;
    
    while ((dirp = readdir(dp)) != NULL) {
        std::string keyFileName = dirp->d_name;

        if ((int)keyFileName.find(".shmkey") == -1 ||
                (movie_id != "" && (int)keyFileName.find(movie_id) == -1))
            continue;

        std::string dataFileName = keyFileName;
        dataFileName.erase(dataFileName.end()-7, dataFileName.end());
        dataFileName = NFP_TRAINING_SET_DIR + std::string("/") + dataFileName;
        keyFileName = NFP_SHM_FILES + std::string("/") + keyFileName;

        NFP::RatingsSS mySSR(dataFileName, keyFileName);
        
        if (mySSR.create()) {
            LOG(ERROR) << "An error occured creating shm segment. ";
            continue;
        }
        if (mySSR.attach()) {
            LOG(ERROR)<< "An error occured attaching to shm segment. ";
            continue;
        }
        
        // Here is the shit that changes...
        
        int m_id = mySSR.ptr()->movie_id();
        int nb_ratings = mySSR.nb_ratings();
        
        double armean_rate = 0;
        int myRate = 0;
        for (int i = 0; i < nb_ratings; i++) {
            myRate = ((NFP::Rating*)(mySSR.ptr() + i))->rate();
            armean_rate += (double)myRate;
        }
        armean_rate /= nb_ratings;
        
        mySSR.detach();
        
        total_ratings += nb_ratings;
        sprintf(msg, "%07d  %9d  %9d  %5.3f  %4d  %s",
            m_id,
            nb_ratings,
            nb_ratings * RATING_DATA_SIZE,
            armean_rate,
            movieYear(m_id),
            movieName(m_id).c_str());
        std::cout << msg << std::endl;
        LOG(INFO) << msg;  
           
    }
    closedir(dp);
    
    sprintf(msg, "%7s  %9d  %9d",
        "#   All", total_ratings, total_ratings * RATING_DATA_SIZE);
    std::cout << std::endl << msg << std::endl;
    
    delete[] msg;
    
    return 0;
}


int ratings(std::string arg_movie_id = "", int arg_user_id = -1)
{
    LOG(INFO) << "NFP_SHM_FILES = " << NFP_SHM_FILES;
    LOG(INFO) << "arg_movie_id = " << arg_movie_id << " arg_user_id = " << arg_user_id;

    DIR *dp;
    struct dirent *dirp;
    if((dp  = opendir(NFP_SHM_FILES.c_str())) == NULL) {
        LOG(ERROR) << "Error(" << errno << ") opening " << NFP_SHM_FILES;
        return errno;
    }

    while ((dirp = readdir(dp)) != NULL) {
        std::string keyFileName = dirp->d_name;

        if ((int)keyFileName.find(".shmkey") == -1 ||
                (arg_movie_id != "" && (int)keyFileName.find(arg_movie_id) == -1))
            continue;

        std::string dataFileName = keyFileName;
        dataFileName.erase(dataFileName.end()-7, dataFileName.end());
        dataFileName = NFP_TRAINING_SET_DIR + std::string("/") + dataFileName;
        keyFileName = NFP_SHM_FILES + std::string("/") + keyFileName;

        NFP::RatingsSS mySSR(dataFileName, keyFileName);
        
        if (mySSR.create()) {
            LOG(ERROR) << "An error occured creating shm segment. ";
            continue;
        }
        if (mySSR.attach()) {
            LOG(ERROR)<< "An error occured attaching to shm segment. ";
            continue;
        }
        
        int nb_ratings = mySSR.nb_ratings();
        LOG(INFO) << "Found " << nb_ratings;
        
        NFP::Rating* myRating = NULL;
        
        char* msg = new char[50];
        sprintf(msg, "%5s  %8s  %1s  %10s", "#m_id", "user_id", "R", "Date");
        std::cout << msg << std::endl;
        delete[] msg;
        
        for (int i = 0; i < nb_ratings; i++) {
            myRating = (NFP::Rating*)(mySSR.ptr() + i);
            // LOG(INFO) << "(" << arg_user_id << "!= -1) = " << (arg_user_id != -1);
            //             LOG(INFO) << "(" << arg_user_id
            //                       << " != " << (int)myRating->user_id()
            //                       << " = " << (arg_user_id != (int)myRating->user_id());
            if (arg_user_id == -1 || arg_user_id == (int)myRating->user_id())
                std::cout << myRating->to_string() << std::endl;
        }
        
        mySSR.detach();
    }
    closedir(dp);
    
    return 0;
}


int movieYear(int movie_id)
{
    int ret = 0;
    if (movieYears.empty())
        fillMoviesMaps();
    if (movieYears.find(movie_id) == movieYears.end())
        LOG(WARNING) << movie_id << " not found in movieYears map";
    else ret = movieYears[movie_id];    
    return ret;
}


std::string movieName(int movie_id)
{
    std::string ret("");
    if (movieNames.empty())
        fillMoviesMaps();
    if (movieNames.find(movie_id) == movieNames.end())
        LOG(WARNING) << movie_id << " not found in movieNames map";
    else ret = movieNames[movie_id];
    return ret;
}


void fillMoviesMaps()
{
    LOG(INFO) << "movieYears and/or movieNames maps are empty. Filling up ...";
    
    std::string movie_titlesFilePath = getenv("NFP_MOVIE_TITLES_FILE");
    // if (strcmp(movie_titlesFilePath.c_str(), "") == 0) {
    //         LOG(ERROR) << "env. var. NFP_MOVIE_TITLES_FILE not set.";
    //         return;
    //     }
    
    std::ifstream in(movie_titlesFilePath.c_str());
    
    if (in.is_open())
    {
        std::string line;
        QRegExp mvFileLineRegExp("^(\\d+),(\\d+),(.*)$");
        
        int movie_id = -1;
        int year = -1;
        std::string name("");
        
        while (!in.eof()) {
            getline(in, line);
            //DLOG(INFO) << "Read line: \"" << line << "\"";
            if (mvFileLineRegExp.indexIn(QString::fromStdString(line)) > -1) {
                movie_id = mvFileLineRegExp.cap(1).toInt();
                year = mvFileLineRegExp.cap(2).toInt();
                name = mvFileLineRegExp.cap(3).toStdString();
                movieYears[movie_id] = year;
                movieNames[movie_id] = name;
            }
        }    
    } else LOG(ERROR) << "Unable to open " << movie_titlesFilePath;
    
    in.close();
    LOG(INFO) << "... done";
}

void usage()
{
    std::cout << "Usage..." << std::endl;
}