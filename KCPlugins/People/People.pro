include(../kcplugin.prf)
TARGET  = $$qtLibraryTarget(kc_people)

OTHER_FILES += \
    people.json

HEADERS += \
    people.h \
    peoplepanel.h

SOURCES += \
    people.cpp \
    peoplepanel.cpp

FORMS += \
    people.ui

