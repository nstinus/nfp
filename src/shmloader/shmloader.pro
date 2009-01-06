QT -= gui

CONFIG += debug

CONFIG(release, debug|release):message(Release build!)
CONFIG(debug, debug|release):message(Debug build!)
CONFIG(release, debug|release) {
	DEFINES += NDEBUG
}

TEMPLATE = app

#CONFIG -= qt
CONFIG += stl
CONFIG += x86
CONFIG -= app_bundle

LIBS += /usr/local/lib/libglog.dylib

#INCLUDEPATH += /Users/nico/projects/dev/nfp/src/model/

SOURCES += main.cpp

TARGET = shmloader



