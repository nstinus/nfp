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
    QList<NFP::Rating*> ratings;

    QString movie_id_QS;
    ushort movie_id = 0;
    uint user_id = 0;
    uchar rate = 0;
    
    QRegExp mvFileLineRegExp("^(\\d+),([1-5]),([12]\\d{3})-(0[1-9]|1[0-2])-(\\d{2})$");
    
    QMap<uint, uint> ratesByMovie;
    
    
    
    
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
        QTextStream stream(&inFile);
        movie_id_QS = stream.readLine();
        movie_id_QS.chop(1);
        movie_id = movie_id_QS.toUShort();
        //LOG(INFO) << "Movie_id: <" << (int)movie_id << ">";
        
        QString line;
        NFP::Rating rating;
        
        do {
            line = stream.readLine();
            if (mvFileLineRegExp.indexIn(line) > -1) {
                user_id = mvFileLineRegExp.cap(1).toUInt();
                rate = (uchar)mvFileLineRegExp.cap(2).toUShort();
                NFP::Rating* r = new NFP::Rating(movie_id, user_id, rate, 0);
                ratings.push_back(r);
                DLOG(INFO) << "Created Rating in shm [" << r << "]: "
                    << (int)r->movie_id() << ", "
                    << (int)r->user_id() << ", "
                    << (int)r->rate() << "---";
            }
        } while (!line.isNull());
        
        //LOG(INFO) << "Movie " << movie_id << " has " << ratings.size() << " records";
        ratesByMovie[movie_id] = (uint)ratings.size();
        ratings.clear();
        inFile.close();
    }
    
    QMapIterator<uint, uint> rBmIt(ratesByMovie);
    QByteArray ratesByMovieAsByteArray;
    while(rBmIt.hasNext()) {
        rBmIt.next();
        ratesByMovieAsByteArray.append(QString::number(rBmIt.key())
            + QString::number(rBmIt.value()));
        LOG(INFO) << "Movie " << rBmIt.key() << " has " << rBmIt.value() << " records";
    }
    
    QByteArray mapHash = QCryptographicHash::hash(ratesByMovieAsByteArray,
        QCryptographicHash::Sha1).toHex();
        
    LOG(INFO) << "mapHash.size() = " << mapHash.size();        
    LOG(INFO) << "mapHash = " << QString(mapHash).toStdString();
    
    //LOG(INFO) "Writing output file ";
    
    LOG(INFO) << "Bye-bye!";
    
    return EXIT_SUCCESS;
}