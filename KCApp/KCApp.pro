QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = KapCompta
TEMPLATE = app
ICON = icons/logo.png

SOURCES += main.cpp \
        kcmainwindow.cpp

HEADERS  += kcmainwindow.h

FORMS += kcmainwindow.ui

RESOURCES += rsrc.qrc

TRANSLATIONS += kapcompta_fr.ts

DEFINES *= QT_USE_QSTRINGBUILDER

macx {
    KCCORE_LIB = $$OUT_PWD/../KCApp/KapCompta.app/Contents/MacOS/
} else {
    KCCORE_LIB = $$OUT_PWD/../KCApp/lib
}

CONFIG( debug, debug|release ) {
    LIBS += -L$$KCCORE_LIB -lKCCore_debug
} else {
    LIBS += -L$$KCCORE_LIB -lKCCore
}

INCLUDEPATH += $$PWD/../KCCore
DEPENDPATH += $$PWD/../KCCore
