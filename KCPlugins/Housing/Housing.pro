include(../kcplugin.prf)
TARGET  = $$qtLibraryTarget(kc_housing)

OTHER_FILES += \
    housing.json

HEADERS += \
    housing.h \
    mealpanel.h

SOURCES += \
    housing.cpp \
    mealpanel.cpp

FORMS += \
    mealsubscription.ui \
    meal.ui

