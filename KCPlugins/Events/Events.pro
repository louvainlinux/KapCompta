include(../kcplugin.prf)
TARGET  = $$qtLibraryTarget(kc_events)

OTHER_FILES += \
    events.json

HEADERS += \
    events.h \
    eventspanel.h

SOURCES += \
    events.cpp \
    eventspanel.cpp

FORMS += \
    events.ui \
    addevents.ui

RESOURCES += \
    rsrc.qrc

