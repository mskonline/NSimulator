#include "queue.h"

Queue::Queue(int qSize)
{
    queue = new boost::lockfree::queue<packet>(qSize);
}

bool Queue::push(packet p)
{
    this->queue->push(p);
}

packet Queue::pop(bool &ok)
{
    packet p;
    ok = true;

    if(!this->queue->pop(p))
        ok = false;

    return p;
}

void Queue::logQDepth()
{
    itemsInQ.push_back(queue->qSize);
}

bool Queue::empty()
{
    return this->queue->empty();
}

int Queue::size()
{
    return this->queue->qSize;
}
