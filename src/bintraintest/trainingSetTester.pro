QT -= gui

CONFIG += debug

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
LIBS += $(NFP_BIN_DIR)/libnfpmodel.a

INCLUDEPATH += /Users/nico/projects/dev/nfp/src/nfpmodel/

SOURCES += main.cpp

TARGET = tsbt



