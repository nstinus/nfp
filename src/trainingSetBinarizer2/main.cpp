#include <tclap/CmdLine.h>
#include <glog/logging.h>

#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <fstream>

// Qt
#include <QDir>
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QDataStream>
#include <QRegExp>
#include <QMap>
#include <QList>
#include <QCryptographicHash>
//#include <QSharedMemory>

// STL
#include <string>
//#include <list>

#include "Rating.h"

typedef unsigned char uchar;

int main (int argc, char** argv)
{
    
    // Initializing the logging facility
    google::InitGoogleLogging(argv[0]);
    google::InstallFailureSignalHandler();
    
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
    
    QString movie_id_QS;
    int movie_id = 0;
    int user_id = 0;
    int rate = 0;
    std::string date = "";

    QRegExp mvFileLineRegExp("^(\\d+),([1-5]),(\\d{4}-\\d{2}-\\d{2})$");
    
    QMap<uint, uint> ratesByMovie;
    
    
    
    //std::ofstream allOutFile;
    //allOutFile.open("./out/all.dat", std::ios::out | std::ios::app | std::ios::binary);
    //allOutFile.fill(0);
    
    /*if (!allOutFile.open(QIODevice::WriteOnly)) {
        LOG(ERROR) << "Unable to open " << allOutFile.fileName().toStdString()
            << " for writing. Skipping.";
        //continue;
    }*/
    
    for (QList<QString>::iterator it = files.begin(); it != files.end(); ++it)
    {
        LOG(INFO) << "Processing file " << (*it).toStdString();
        QString filePath = training_set_dir.absolutePath() + QString("/") + (*it);
        QFile inFile(filePath);
        
        if (!inFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            LOG(ERROR) << "Unable to open " << inFile.fileName().toStdString()
                << " for reading. Skipping.";
            continue;
        }
        
        std::ofstream outFile;
        std::string outFileName;
        outFileName  = "./out/";
        outFileName += (*it).replace(".txt", "").toStdString();
        outFileName += ".dat";
        outFile.open(outFileName.c_str(), std::ios::out | std::ios::binary);
        outFile.fill(0);
        
        /*if (!outFile.open(QIODevice::WriteOnly)) {
            LOG(ERROR) << "Unable to open " << outFile.fileName().toStdString()
                << " for writing. Skipping.";
            continue;
        }*/
        
        QTextStream stream(&inFile);
        movie_id_QS = stream.readLine();
        movie_id_QS.chop(1);
        movie_id = movie_id_QS.toInt();
        //LOG(INFO) << "Movie_id: <" << (int)movie_id << ">";
        
        QList<NFP::Rating*> ratings;

        QString line;
        
        do {
            line = stream.readLine();
            if (mvFileLineRegExp.indexIn(line) > -1) {
                user_id = mvFileLineRegExp.cap(1).toInt();
                rate = mvFileLineRegExp.cap(2).toInt();
                date = mvFileLineRegExp.cap(3).toStdString();
                NFP::Rating* r = new NFP::Rating(movie_id, user_id, rate, date);
                ratings.push_back(r);
                //DLOG(INFO) << "Created Rating [" << r << "]: " << r->to_string();
            }
        } while (!line.isNull());
        
        //QDataStream outStream(&outFile);
        /* Almost working shit
        for (QList<NFP::Rating*>::iterator it = ratings.begin(); it != ratings.end(); ++it) {
            char data[RATING_DATA_SIZE];
            memset(data, 0, RATING_DATA_SIZE);
            (*it)->data(data);
            std::string hex = QString(QByteArray(data, sizeof(NFP::Rating)).toHex()).toStdString();
            std::string hash = QString(
                QCryptographicHash::hash(
                    QByteArray(data, sizeof(NFP::Rating)),
                    QCryptographicHash::Sha1).toHex()).toStdString();
            DLOG(INFO) << (*it)->to_string() << " " << hex << " " << hash;
            outFile.write(data, RATING_DATA_SIZE);
            //allOutFile.write(data, RATING_DATA_SIZE);
            //outFile << data;
            //allOutFile << data;
            outFile.flush();
        }
        */
        
        int ratings_size = ratings.size();
        char data[ratings_size * RATING_DATA_SIZE];
        memset(data, 0, ratings_size * RATING_DATA_SIZE);
        for (int i = 0; i < ratings_size; i++) {
            memcpy(&data[i * RATING_DATA_SIZE], ratings.at(i), RATING_DATA_SIZE);
            char* d = new char[RATING_DATA_SIZE];
            ratings.at(i)->data(d);
            DLOG(INFO) << ratings.at(i)->to_string() << " "
                << QString(QByteArray(d, RATING_DATA_SIZE).toHex()).toStdString();
        }
        outFile.write(data, ratings_size * RATING_DATA_SIZE);
        outFile.close();
        LOG(INFO) << "Wrote " << ratings.size() << " Ratings.";
        
        ratings.clear();
        inFile.close();
    }
    
    //allOutFile.close();
    LOG(INFO) << "Bye-bye!";
    
    return EXIT_SUCCESS;
}