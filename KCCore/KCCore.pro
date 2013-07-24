QT += core sql

macx {
    DESTDIR = $$OUT_PWD/../KCApp/KapCompta.app/Contents/MacOS/
} else {
    DESTDIR = $$OUT_PWD/../KCApp/lib
}

TARGET = $$qtLibraryTarget(KCCore)
TEMPLATE = lib

DEFINES += KCCORE_LIBRARY
DEFINES *= QT_USE_QSTRINGBUILDER

SOURCES += kccore.cpp

HEADERS += kccore_global.h \
       kccore.h
