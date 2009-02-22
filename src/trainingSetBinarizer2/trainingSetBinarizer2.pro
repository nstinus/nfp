QT -= gui

CONFIG += debug_and_release

CONFIG(release, debug|release):message(Release build!)
CONFIG(debug, debug|release):message(Debug build!)
CONFIG(release, debug|release) {
	DEFINES += NDEBUG
}

TEMPLATE = app

CONFIG += stl
CONFIG += x86
CONFIG -= app_bundle

LIBS += /usr/local/lib/libglog.a

CONFIG(release, debug|release) {
	LIBS += /Users/nico/projects/dev/nfp/src/libnfp/libnfp.a
}
CONFIG(debug, debug|release) {
	LIBS += /Users/nico/projects/dev/nfp/src/libnfp/libnfpd.a
}

INCLUDEPATH += /Users/nico/projects/dev/nfp/src/libnfp/

SOURCES += main.cpp

target.path = ${NFP_BIN_DIR}
INSTALLS += target

TARGET = tsb2



