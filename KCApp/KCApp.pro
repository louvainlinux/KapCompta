QT       += core gui sql
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = KapCompta
TEMPLATE = app
ICON = icons/logo.png

SOURCES += main.cpp\
        kcmain.cpp \
    kcwelcomescreen.cpp \
    kcnewaccount.cpp \
    kcbasicaccountproperties.cpp \
    kcticketpanel.cpp \
    kcpeoplepanel.cpp \
    kcspendingpanel.cpp \
    kcpropertiespanel.cpp \
    kcsummarypanel.cpp \
    kcbasicsummaryview.cpp \
    kcpdfformat.cpp

HEADERS  += kcmain.h \
    kcwelcomescreen.h \
    kcnewaccount.h \
    kcbasicaccountproperties.h \
    kcticketpanel.h \
    kcpeoplepanel.h \
    kcspendingpanel.h \
    kcpropertiespanel.h \
    kcsummarypanel.h \
    kcbasicsummaryview.h \
    kcpdfformat.h

RESOURCES += \
    icons/icons.qrc \

TRANSLATIONS += kapcompta_fr.ts

DEFINES *= QT_USE_QSTRINGBUILDER

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../KCCore/release/ -lKCCore
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../KCCore/debug/ -lKCCore
else:unix: LIBS += -L$$OUT_PWD/../KCCore/ -lKCCore

INCLUDEPATH += $$PWD/../KCCore
DEPENDPATH += $$PWD/../KCCore
# Might not be useful and caused bugs
#PRE_TARGETDEPS += $$OUT_PWD/../KCCore/KCCore.1.dylib
