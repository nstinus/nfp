#include <tclap/CmdLine.h>
#include <glog/logging.h>
#include <string>

#include <QDir>
#include <QString>
#include <QFile>
#include <QTextStream>

#include <list>

typedef std::list<std::string> string_list;
typedef std::list<std::string>::iterator string_list_iterator;


int main (int argc, char** argv)
{
    google::InitGoogleLogging(argv[0]);
    google::InstallFailureSignalHandler();
    // Wrap everything in a try block.  Do this every time, 
	// because exceptions will be thrown for problems.
	
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
    //string_list files;

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
        LOG(INFO) << "First line: <" << stream.readLine().toStdString() << ">";
        file.close();
    }
    
    
    

        
    
    
    return EXIT_SUCCESS;
}