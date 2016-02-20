#ifndef ROUTER_H
#define ROUTER_H

#include <QString>

class Router
{
    public:
        QString name;
        int numInputs;


        Router(QString);
        void setUp();
        void run();
};

#endif // ROUTER_H
