#ifndef NSIMULATOR_H
#define NSIMULATOR_H

#include <Interface/interface.h>
#include "Network/network.h"

class NSimulator: public QObject
{
    Q_OBJECT        

    public:
        Interface *interface;
        Network *network;

        NSimulator();
        ~NSimulator();
        void initiate();

    public slots:

        void closeApp();
};

#endif // NSIMULATOR_H
