#include "interface.h"
#include "widgets/nplotter.h"
#include <QDateTime>

Interface::Interface(QWidget *parent) : QMainWindow(parent)
{
    this->setupUi(this);
}

void Interface::log(QString msg)
{
    this->LogViewer->append("[" + QDateTime::currentDateTime()
                            .toString("hh:mm:ss") + "] " + msg);
}

void Interface::mlog(QString msg)
{
    this->LogViewer->append(msg);
}

void Interface::showPlots(Network *network)
{
    NPlotter *np = new NPlotter(this,network);
    np->plot();
}

Interface::~Interface()
{

}
