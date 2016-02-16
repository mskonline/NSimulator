QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = NSimulator
TEMPLATE = app

SOURCES += \
    main.cpp \
    Interface/interface.cpp \
    Network/Router/router.cpp \
    Network/network.cpp \
    nsimulator.cpp \
    Network/routingtable.cpp


win32:UI_DIR = temp/ui
win32:MOC_DIR = temp/moc
win32:OBJECTS_DIR = temp/obj

FORMS += \
    Interface/interface.ui

HEADERS += \
    Interface/interface.h \
    Commons/commons.h \
    Network/Router/router.h \
    Network/network.h \
    nsimulator.h \
    Network/routingtable.h

RESOURCES += \
    resources/resources.qrc


