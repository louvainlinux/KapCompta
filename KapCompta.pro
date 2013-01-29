TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS += KCCore\
           KCApp\
           KCPlugins

OTHER_FILES += \
    LICENSE.txt

KCApp.depends = KCCore
