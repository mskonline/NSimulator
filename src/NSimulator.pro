QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = NSimulator
TEMPLATE = app
INCLUDEPATH += C:/Boost/include/boost-1_60 \
               ./Plotting
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
    Utils/nutils.cpp \
    Interface/widgets/nplotter.cpp \
    Network/Router/enqueueprocessor.cpp \
    Network/Router/dequeueprocessor.cpp \
    Network/Router/Queue/queue.cpp


win32:UI_DIR = temp/ui
win32:MOC_DIR = temp/moc
win32:OBJECTS_DIR = temp/obj

FORMS += \
    Interface/interface.ui \
    Interface/widgets/nplotter.ui

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
    Utils/nutils.h \
    Interface/widgets/nplotter.h \
    Network/Router/enqueueprocessor.h \
    Network/Router/dequeueprocessor.h \
    Network/Router/Queue/queue.h


RESOURCES += \
    resources/resources.qrc

DEFINES += QCUSTOMPLOT_USE_LIBRARY

CONFIG(debug, release|debug) {
  win32:QCPLIB = qcustomplotd1
  else: QCPLIB = qcustomplotd
} else {
  win32:QCPLIB = qcustomplot1
  else: QCPLIB = qcustomplot
}

LIBS += -L../src/lib/ -l$$QCPLIB

