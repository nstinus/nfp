QT -= gui

CONFIG += debug_and_release

TEMPLATE = app

CONFIG += qt
CONFIG += stl
CONFIG += x86
CONFIG -= app_bundle

LIBS += /usr/local/lib/libglog.dylib

CONFIG(release, debug|release) {
	message(Release build!)
	LIBS += /Users/nico/projects/dev/nfp/bin/libnfp.a
	TARGET = nfpshm
}

CONFIG(debug, debug|release) {
	message(Debug build!)
	DEFINES += NDEBUG
	LIBS += /Users/nico/projects/dev/nfp/bin/libnfpd.a
	TARGET = nfpshm.debug
	# Profiling
	QMAKE_CFLAGS+=-pg
	QMAKE_CXXFLAGS+=-pg
	QMAKE_LFLAGS+=-pg
}

INCLUDEPATH += /Users/nico/projects/dev/nfp/src/libnfp/

SOURCES += main.cpp
