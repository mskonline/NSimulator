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
    Network/routingtable.cpp \
    Network/Router/inputadaptor.cpp \
    Network/Router/outputadaptor.cpp \
    Network/link.cpp \
    Utils/nutils.cpp


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
    Network/routingtable.h \
    Network/Router/inputadaptor.h \
    Network/Router/outputadaptor.h \
    Network/link.h \
    Utils/nutils.h

RESOURCES += \
    resources/resources.qrc


