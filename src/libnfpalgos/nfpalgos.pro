QT -= gui

CONFIG += debug_and_release

TEMPLATE = lib

CONFIG += stl
CONFIG += x86
CONFIG += staticlib
CONFIG -= app_bundle

macx {
LIBS += /usr/local/lib/libglog.dylib
}
linux-g++ {
LIBS += /usr/local/lib/libglog.so
}

CONFIG(release, debug|release) {
	message(Release build!)
	LIBS += $(NFP_ROOT)/bin/libnfp.a
	TARGET = nfpalgos
}

CONFIG(debug, debug|release) {
	message(Debug build!)
	DEFINES += NDEBUG
	LIBS += $(NFP_ROOT)/bin/libnfpd.a
	TARGET = nfpalgosd
	# Profiling
	QMAKE_CFLAGS+=-pg
	QMAKE_CXXFLAGS+=-pg
	QMAKE_LFLAGS+=-pg
}

HEADERS += BaseAlgo.h
HEADERS += MovieMeanAlgo.h
HEADERS += UserMeanAlgo.h

SOURCES += MovieMeanAlgo.cpp
SOURCES += UserMeanAlgo.cpp

INCLUDEPATH += $(NFP_ROOT)/src/libnfp

