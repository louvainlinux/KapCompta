include(../kcplugin.prf)
TARGET  = $$qtLibraryTarget(kc_housing)

OTHER_FILES += \
    housing.json

HEADERS += \
    housing.h \
    housingpanel.h

SOURCES += \
    housing.cpp \
    housingpanel.cpp

FORMS += \
    housing.ui

