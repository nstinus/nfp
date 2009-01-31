QT -= gui

CONFIG += debug

CONFIG(release, debug|release):message(Release build!)
CONFIG(debug, debug|release):message(Debug build!)
CONFIG(release, debug|release) {
	DEFINES += NDEBUG
}

TEMPLATE = app

CONFIG += qt
CONFIG += stl
CONFIG += x86
CONFIG -= app_bundle

LIBS += /usr/local/lib/libglog.dylib
#LIBS += /usr/local/lib/libboost_date_time-xgcc40-mt.dylib

INCLUDEPATH += /Users/nico/projects/dev/nfp/src/model/


HEADERS += ../Rating.h
HEADERS += ../BasicRating.h

SOURCES += main.cpp

TARGET = model_test



