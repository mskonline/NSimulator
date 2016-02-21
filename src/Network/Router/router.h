#ifndef ROUTER_H
#define ROUTER_H
#include "../../Interface/interface.h"
#include <QString>

class Router
{
    private:
        int numInputs;
        Interface *interface;
        bool allSet;

    public:
        QString name;

        Router(QString);
        ~Router();

        void setInterfaceObj(Interface *);
        void initiate();
        void run();
};

#endif // ROUTER_H
