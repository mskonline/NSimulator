#ifndef NSIMULATOR_H
#define NSIMULATOR_H

#include "Interface/interface.h"
#include "Network/network.h"

class NSimulator: public QObject
{
    Q_OBJECT        

    public:
        Interface *interface;
        Network *network;

        NSimulator();
        ~NSimulator();
        void initiate(int);

    public slots:

        void closeApp();
        void test();

        void showPlots();
};

#endif // NSIMULATOR_H
