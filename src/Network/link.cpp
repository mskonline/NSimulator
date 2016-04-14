#include "link.h"
#include "Router/router.h"

Link::Link(Router *r)
{
    this->destRouter = r;
}

void Link::transfer(packet p)
{

}
