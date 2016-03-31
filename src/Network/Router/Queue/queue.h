#ifndef QUEUE_H
#define QUEUE_H

#include "Commons/commons.h"
#include <boost/lockfree/queue.hpp>
#include <vector>

class Queue
{
    private:
        boost::lockfree::queue<packet> *queue;

    public:
        std::vector<double> residenceTime, itemsInQ;

        bool push(packet);
        packet pop(bool &);
        bool empty();
        int size();
        void logQDepth();

        Queue(int);
};

#endif // QUEUE_H
