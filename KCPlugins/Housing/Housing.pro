QT           += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE      = lib
CONFIG       += plugin
#HEADERS       =
#SOURCES       =
TARGET        = $$qtLibraryTarget(kc_housing)

DESTDIR       = $$OUT_PWD/../../KCApp/plugins

#RESOURCES +=

macx {
    KCCORE_LIB = $$OUT_PWD/../../KCApp/KapCompta.app/Contents/MacOS/
} else {
    KCCORE_LIB = $$OUT_PWD/../../KCApp/lib
}

CONFIG( debug, debug|release ) {
    LIBS += -L$$KCCORE_LIB -lKCCore_debug
} else {
    LIBS += -L$$KCCORE_LIB -lKCCore
}

INCLUDEPATH += $$PWD/../../KCCore
DEPENDPATH += $$PWD/../../KCCore
