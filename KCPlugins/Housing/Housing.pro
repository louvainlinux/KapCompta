include(../kcplugin.prf)
TARGET  = $$qtLibraryTarget(kc_housing)

OTHER_FILES += \
    housing.json

HEADERS += \
    housing.h \
    mealpanel.h \
    mealcalendar.h

SOURCES += \
    housing.cpp \
    mealpanel.cpp \
    mealcalendar.cpp

FORMS += \
    mealsubscription.ui \
    meal.ui

RESOURCES += \
    rsrc.qrc

