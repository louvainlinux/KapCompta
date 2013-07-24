QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = KapCompta
TEMPLATE = app
ICON = icons/logo.png

SOURCES += main.cpp \
        kcmainwindow.cpp

HEADERS  += kcmainwindow.h

FORMS += kcmainwindow.ui

#RESOURCES += \

TRANSLATIONS += kapcompta_fr.ts

DEFINES *= QT_USE_QSTRINGBUILDER

LIBS += -L$$OUT_PWD/plugins -lKCCore

INCLUDEPATH += $$PWD/../KCCore
DEPENDPATH += $$PWD/../KCCore

