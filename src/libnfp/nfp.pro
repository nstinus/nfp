QT -= gui

CONFIG += debug_and_release

TEMPLATE = lib

CONFIG += stl
CONFIG += x86
CONFIG += staticlib
CONFIG -= app_bundle

LIBS += /usr/local/lib/libglog.dylib

HEADERS += Rating.h
HEADERS += ShmSegment.h
HEADERS += RatingsShmSegment.h
HEADERS += User.h
HEADERS += RatingsManager.h
HEADERS += Singleton.hxx

SOURCES += Rating.cpp
SOURCES += ShmSegment.cpp
SOURCES += RatingsShmSegment.cpp
SOURCES += User.cpp
SOURCES += RatingsManager.cpp

# SOURCES += libnfp.cpp

CONFIG(release, debug|release) {
	message(Release build!)
	DEFINES += NDEBUG
	TARGET = nfp
	# profiling
	QMAKE_CFLAGS+=-pg
	QMAKE_CXXFLAGS+=-pg
	QMAKE_LFLAGS+=-pg
}
CONFIG(debug, debug|release) {
	message(Debug build!)
	TARGET = nfpd
}

