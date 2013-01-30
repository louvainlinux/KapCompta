TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS += KCCore\
           KCApp\
           KCPlugins

OTHER_FILES += \
    LICENSE.txt \
    BUILDING.txt \
    osx_shared_lib.sh

KCApp.depends = KCCore
