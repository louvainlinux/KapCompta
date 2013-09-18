cache()
TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS += KCCore\
           KCApp\
           KCPlugins

OTHER_FILES += \
    LICENSE.txt \
    BUILDING.txt \
    README.md

KCApp.depends = KCCore
KCPlugins.depends = KCCore \
    KCApp
