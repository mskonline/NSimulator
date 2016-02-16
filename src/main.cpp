#include <QApplication>
#include "nsimulator.h"


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    NSimulator *ns = new NSimulator();
    ns->initiate();

    ns->interface->log("Network Initiated...");
    ns->network->setRoutingSrc("W:/Test/rtable.bin");
    ns->network->initiateRoutingTable();


    return app.exec();
}
