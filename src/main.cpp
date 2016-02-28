#include <QApplication>
#include "nsimulator.h"
#include "Utils/nutils.h"

#include <iostream>
#include <QDebug>
using namespace std;

int main(int argc, char *argv[])
{

    QApplication app(argc, argv);
    NSimulator *ns = new NSimulator();
    ns->initiate();

    ns->interface->log("Network Initiated...");
    return app.exec();

    /*NUtils nU;

    if(argv[0] == "-cR")
        nU.createRoutingData();
    else if(argv[0] == "-a")
        nU.analyize();
    else if(argv[0] == "-t")
        nU.Test();

    cout << "done";*/

}
