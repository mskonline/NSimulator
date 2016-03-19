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

    if(argc > 1){
        string arg = argv[1];
        if(arg == "-t")
            ns->initiate(TEST_MODE);
    } else
        ns->initiate(NORMAL_RUN_MODE);

    ns->interface->log("Network Initiated...");

    return app.exec();
}
