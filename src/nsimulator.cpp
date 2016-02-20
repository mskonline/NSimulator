#include <QThread>
#include "nsimulator.h"

NSimulator::NSimulator()
{

}

void NSimulator::initiate(){

    interface = new Interface();
    interface->show();
    interface->log("Welcome to NSimulator");

    network = new Network();
    network->setInterfaceObj(interface);

    network->setUp();
    connect(interface->actionQuit,SIGNAL(triggered(bool)),this,SLOT(closeApp(bool)));
}

void NSimulator::closeApp(bool a)
{
    // TODO :tear down network
    delete network;

    qApp->quit();
}


NSimulator::~NSimulator()
{
    delete interface;
}


