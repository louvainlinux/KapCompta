QT           += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE      = lib
CONFIG       += plugin
#HEADERS       =
#SOURCES       =
TARGET        = $$qtLibraryTarget(kc_housing)

DESTDIR       = $$OUT_PWD/../../KCApp/plugins

#RESOURCES +=

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../KCCore/release/ -lKCCore
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../KCCore/debug/ -lKCCore
else:unix: LIBS += -L$$OUT_PWD/../../KCCore/ -lKCCore

INCLUDEPATH += $$PWD/../../KCCore
DEPENDPATH += $$PWD/../../KCCore
