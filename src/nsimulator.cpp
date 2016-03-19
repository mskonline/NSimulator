#include <QThread>
#include "nsimulator.h"
#include "./Network/routingtable.h"


NSimulator::NSimulator()
{
}

void NSimulator::initiate(int mode){

    interface = new Interface();
    interface->show();
    interface->log("Welcome to NSimulator...");

    network = new Network();
    network->setInterfaceObj(interface);

    if(mode == NORMAL_RUN_MODE)
    {
        network->initiate();
        connect(interface->pb_run,SIGNAL(released()),network,SLOT(run()));
        connect(interface->pb_stop,SIGNAL(released()),network,SLOT(stop()));
    }
    else
    {
        connect(interface->pb_run,SIGNAL(released()),this,SLOT(test()));
    }

    connect(interface->pb_plots,SIGNAL(released()),this,SLOT(showPlots()));
    connect(interface->pb_quit,SIGNAL(released()),this,SLOT(closeApp()));
}

void NSimulator::showPlots()
{
    interface->showPlots(network);
}

void NSimulator::test()
{
    RoutingTable *rt = new RoutingTable("./config/forwardingtable1");
    unsigned char d[4] = {0,0,0,0};
    int p = rt->lookUp(d);

    interface->log(QString("Port : %1").arg(p));
    delete rt;
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


