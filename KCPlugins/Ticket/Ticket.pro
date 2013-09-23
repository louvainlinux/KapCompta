include(../kcplugin.prf)
TARGET  = $$qtLibraryTarget(kc_ticket)

OTHER_FILES += \
    ticket.json

HEADERS += \
    ticket.h \
    ticketpanel.h

SOURCES += \
    ticket.cpp \
    ticketpanel.cpp

FORMS += \
    ticket.ui \
    addticket.ui

RESOURCES += \
    rsrc.qrc

