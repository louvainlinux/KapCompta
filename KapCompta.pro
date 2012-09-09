QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = KapCompta
TEMPLATE = app
ICON = :/icons/logo.png

SOURCES += main.cpp\
        kcmain.cpp \
    kcwelcomescreen.cpp \
    kcnewaccount.cpp \
    kcsettings.cpp \
    kcbasicaccountproperties.cpp \
    kcticketpanel.cpp \
    kcpeoplepanel.cpp \
    kcspendingpanel.cpp \
    kcpropertiespanel.cpp \
    kcsummarypanel.cpp \
    kcbasicsummaryview.cpp \
    kcdatabasehelper.cpp \
    kcfilemanager.cpp

HEADERS  += kcmain.h \
    kcwelcomescreen.h \
    kcnewaccount.h \
    interfaces.h \
    kcsettings.h \
    kcbasicaccountproperties.h \
    kcticketpanel.h \
    kcpeoplepanel.h \
    kcspendingpanel.h \
    kcpropertiespanel.h \
    kcsummarypanel.h \
    kcbasicsummaryview.h \
    kcdatabasehelper.h \
    kcfilemanager.h

RESOURCES += \
    icons/icons.qrc \

TRANSLATIONS += kapcompta_fr.ts

DEFINES *= QT_USE_QSTRINGBUILDER

OTHER_FILES += \
    LICENSE.txt
