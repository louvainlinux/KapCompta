QT += core sql gui

TARGET = $$qtLibraryTarget(KCCore)
TEMPLATE = lib

DEFINES += KCCORE_LIBRARY
DEFINES += QT_USE_QSTRINGBUILDER

SOURCES +=\
           kcdatabasehelper.cpp\
           kcfilemanager.cpp \
           kcsettings.cpp \
    kccore.cpp \
    kcdateedit.cpp \
    kcspinboxdelegate.cpp

HEADERS +=\
           KCCore_global.h\
           kcdatabasehelper.h\
           kcfilemanager.h \
           kcsettings.h\
    kccore.h \
    kcpanel.h \
    kcsummaryview.h \
    kcaccountsetting.h \
    kcfileformat.h \
    kcplugin.h \
    kcdateedit.h \
    kcspinboxdelegate.h
