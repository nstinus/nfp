QT -= gui

CONFIG += debug_and_release

TEMPLATE = app

CONFIG += qt
CONFIG += stl
CONFIG += x86
CONFIG -= app_bundle

macx {
	LIBS += /usr/local/lib/libglog.dylib
}

linux-g++ {
	LIBS += /usr/local/lib/libglog.so
}

CONFIG(release, debug|release) {
	message(Release build!)
	LIBS += ~/projects/dev/nfp/bin/libnfp.a
	LIBS += ~/projects/dev/nfp/bin/libnfpalgos.a
	TARGET = nfpshm
}

CONFIG(debug, debug|release) {
	message(Debug build!)
	DEFINES += NDEBUG
	LIBS += ~/projects/dev/nfp/bin/libnfpd.a
	LIBS += ~/projects/dev/nfp/bin/libnfpalgosd.a
	TARGET = nfpshm.debug
	# Profiling
	QMAKE_CFLAGS+=-pg
	QMAKE_CXXFLAGS+=-pg
	QMAKE_LFLAGS+=-pg
}

INCLUDEPATH += $(NFP_ROOT)/src/libnfp
INCLUDEPATH += $(NFP_ROOT)/src/libnfpalgos/

SOURCES += main.cpp
