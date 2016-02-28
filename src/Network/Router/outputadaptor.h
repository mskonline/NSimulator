#ifndef OUTPUTADAPTOR_H
#define OUTPUTADAPTOR_H
#include <QThread>
#include "../link.h"

class OutputAdaptor: public QThread
{
    public:

        QQueue<packet> **outQueues;
        Link *l;
        OutputAdaptor();
        ~OutputAdaptor();

        void run();
};

#endif // OUTPUTADAPTOR_H
