QT += core sql gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

macx {
    DESTDIR = $$OUT_PWD/../KCApp/KapCompta.app/Contents/MacOS/
} else {
    DESTDIR = $$OUT_PWD/../KCApp/lib
}

TARGET = $$qtLibraryTarget(KCCore)
TEMPLATE = lib

DEFINES += KCCORE_LIBRARY
DEFINES *= QT_USE_QSTRINGBUILDER

SOURCES += kccore.cpp \
    kcaccountfile.cpp \
    kcdatabase.cpp \
    kcdatedelegate.cpp \
    kcspinnerdelegate.cpp \
    kcbooleandelegate.cpp \
    kccomboboxdelegate.cpp \
    kcplaintextdelegate.cpp \
    mustache.cpp

HEADERS += kccore_global.h \
       kccore.h \
    interfaces/kcplugin.h \
    kcaccountfile.h \
    interfaces/kcpanel.h \
    kcdatabase.h \
    kcglobals.h \
    kcdatedelegate.h \
    kcspinnerdelegate.h \
    kcbooleandelegate.h \
    kccomboboxdelegate.h \
    kcplaintextdelegate.h \
    mustache.h
