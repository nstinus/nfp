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

LIBS += /usr/local/lib/libglog.dylib

INCLUDEPATH += /Users/nico/projects/dev/nfp/src/model/

SOURCES += ../model/Rating.cpp
SOURCES += main.cpp

target.path += $(NFP_BIN_DIR)
INSTALLS += target

TARGET = trainingSetBinarizer



