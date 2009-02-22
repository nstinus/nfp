QT -= gui

CONFIG += debug_and_release

CONFIG(release, debug|release):message(Release build!)
CONFIG(debug, debug|release):message(Debug build!)
CONFIG(release, debug|release) {
	DEFINES += NDEBUG
}

TEMPLATE = lib

CONFIG += stl
CONFIG += x86
CONFIG += staticlib
CONFIG -= app_bundle

LIBS += /usr/local/lib/libglog.a

HEADERS += Rating.h
HEADERS += ShmSegment.h
HEADERS += RatingsSS.h

SOURCES += Rating.cpp
SOURCES += ShmSegment.cpp
SOURCES += RatingsSS.cpp

CONFIG(release, debug|release) {
	TARGET = nfp
}
CONFIG(debug, debug|release) {
	TARGET = nfpd
}