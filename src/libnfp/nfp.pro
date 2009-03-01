QT -= gui

CONFIG += debug_and_release

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

