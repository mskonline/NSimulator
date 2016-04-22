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

        void generateFTR1();
        void generateFTR2();
        void generateFTR3();
        void generateFTR4();
        void generateFTR5();
        void generateFTR6();
        void generateFTR7();

        void verifyPackets();
        void writeR2OutContent();
        void writeR4OutContent();
        void writeR5OutContent();
        void writeR6OutContent();
        void writeR7OutContent();

    public slots:

        void closeApp();
        void test();

        void showPlots();
};

#endif // NSIMULATOR_H
