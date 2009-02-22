QT -= gui

CONFIG += debug_and_release

TEMPLATE = app

CONFIG += qt
CONFIG += stl
CONFIG += x86
CONFIG -= app_bundle

LIBS += /usr/local/lib/libglog.dylib
#LIBS += /usr/local/lib/libboost_date_time-xgcc40-mt.dylib

CONFIG(release, debug|release) {
	message(Release build!)
	LIBS += /Users/nico/projects/dev/nfp/src/libnfp/libnfp.a
	TARGET = model_test
}
CONFIG(debug, debug|release) {
	message(Debug build!)
	DEFINES += NDEBUG
	LIBS += /Users/nico/projects/dev/nfp/src/libnfp/libnfpd.a
	TARGET = model_test.debug
}


INCLUDEPATH += /Users/nico/projects/dev/nfp/src/libnfp/

SOURCES += main.cpp





