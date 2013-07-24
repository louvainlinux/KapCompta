QT += core sql

DESTDIR       = $$OUT_PWD/../KCApp/plugins

TARGET = $$qtLibraryTarget(KCCore)
TEMPLATE = lib

DEFINES += KCCORE_LIBRARY
DEFINES *= QT_USE_QSTRINGBUILDER

SOURCES += kccore.cpp

HEADERS += kccore_global.h \
       kccore.h
