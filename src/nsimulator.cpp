#include <QThread>
#include "nsimulator.h"

NSimulator::NSimulator()
{
}

void NSimulator::initiate(){

    interface = new Interface();
    interface->show();
    interface->log("Welcome to NSimulator...");

    network = new Network();
    network->setInterfaceObj(interface);
    network->initiate();

    connect(interface->pb_quit,SIGNAL(released()),this,SLOT(closeApp()));
    connect(interface->pb_run,SIGNAL(released()),network,SLOT(run()));
    connect(interface->pb_stop,SIGNAL(released()),network,SLOT(stop()));
}

void NSimulator::closeApp()
{
    // TODO :tear down network
    delete network;

    qApp->quit();
}


NSimulator::~NSimulator()
{
    delete interface;
}


