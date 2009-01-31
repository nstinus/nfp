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
	    std::string("Path where the data files are located. ")
            + std::string("Will try to read the environment variable NFP_TRAINING_SET_DIR <")
            + defPath
            + std::string("> if unspecified."),
        false,
        defPath,
        "string");
    TCLAP::ValueArg<std::string> filterArg("f",
        "filter",
        "Filename filter. Default is `mv_*.dat'",
        false,
        "mv_*.dat",
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
        sprintf(f, "mv_%07d.dat", movieIdArg.getValue());
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
    
    for (QList<QString>::iterator it = files.begin(); it != files.end(); ++it)
    {
        LOG(INFO) << "Processing file " << (*it).toStdString();
        QString filePath = training_set_dir.absolutePath() + QString("/") + (*it);
        std::ifstream inFile;
        inFile.open(filePath.toStdString().c_str(),
            std::ios::in | std::ios::binary | std::ios::ate);
        int size = inFile.tellg();
        if ((size % 8) != 0)
            LOG(ERROR) << "Data file seems corrupted! " << filePath.toStdString();
        inFile.seekg(0, std::ios::beg);
        
        /*if (!inFile.open(QIODevice::ReadOnly)) {
            LOG(ERROR) << "Unable to open " << inFile.fileName().toStdString()
                << " for reading. Skipping.";
            continue;
        }*/
        
        int i = 0;
        char* data;
        std::string hash;
        while(!inFile.eof() and inFile.good()) {
            if (!inFile.good())
                LOG(ERROR) << "Input stream is corrupted " << filePath.toStdString();
            data = new char[RATING_DATA_SIZE];
            inFile.read(data, RATING_DATA_SIZE);
            NFP::Rating* r = new NFP::Rating(data);
            hash = QString(QCryptographicHash::hash(data,
                QCryptographicHash::Sha1).toHex()).toStdString();
            DLOG(INFO) << "Read " << r->to_string() << " " << hash;
            delete r;
            delete[] data;
            ++i;
        }
        LOG(INFO) << "Read " << i << " ratings from file";
        
        /*
        QList<NFP::Rating*> ratings;

        QString line;
        
        do {
            line = stream.readLine();
            if (mvFileLineRegExp.indexIn(line) > -1) {
                user_id = mvFileLineRegExp.cap(1).toUInt();
                rate = (uchar)mvFileLineRegExp.cap(2).toUShort();
                NFP::Rating* r = new NFP::Rating(movie_id, user_id, rate, 0);
                ratings.push_back(r);
                DLOG(INFO) << "Created Rating [" << r << "]: " << r->to_string();
            }
        } while (!line.isNull());
        
        QDataStream outStream(&outFile);
        
        for (QList<NFP::Rating*>::iterator it = ratings.begin(); it != ratings.end(); ++it) {
            QByteArray data = QByteArray((*it)->getData(), sizeof(NFP::Rating));
            outStream << data;
            allOutStream << data;
        }
            
        ratings.clear();
        */
        inFile.close();
    }
    
    LOG(INFO) << "Bye-bye!";
    
    return EXIT_SUCCESS;
}