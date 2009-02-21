QT -= gui

CONFIG += release

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
LIBS += /Users/nico/projects/dev/nfp/src/libnfp/libnfp.a

INCLUDEPATH += /Users/nico/projects/dev/nfp/src/libnfp/

SOURCES += main.cpp

target.path += $(NFP_BIN_DIR)
INSTALLS += target

TARGET = ratesByMovie



