QT           += core gui sql
TEMPLATE      = lib
CONFIG       += plugin
HEADERS       = housing.h \
                meal.h
SOURCES       = housing.cpp \
                meal.cpp
TARGET        = $$qtLibraryTarget(kc_housing)

DESTDIR       = $$OUT_PWD/../../KCApp/plugins

RESOURCES += \
    icons.qrc

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../KCCore/release/ -lKCCore
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../KCCore/debug/ -lKCCore
else:unix: LIBS += -L$$OUT_PWD/../../KCCore/ -lKCCore

INCLUDEPATH += $$PWD/../../KCCore
DEPENDPATH += $$PWD/../../KCCore
