QT -= gui

CONFIG += debug_and_release
#CONFIG += release

CONFIG(release, debug|release):message(Release build!)
CONFIG(debug, debug|release):message(Debug build!)
CONFIG(release, debug|release) {
	DEFINES += NDEBUG
}


TEMPLATE = lib
#VERSION = 0.0.7

#CONFIG += exceptions
CONFIG += stl
CONFIG += x86
CONFIG += staticlib
#CONFIG += shared
CONFIG -= app_bundle

LIBS += /usr/local/lib/libglog.dylib

HEADERS += typedefs.h
HEADERS += MovieYear.h
HEADERS += RatingDate.h


CONFIG(release, debug|release) {
	TARGET = datamodel
}
CONFIG(debug, debug|release) {
	TARGET = datamodel.debug
}



