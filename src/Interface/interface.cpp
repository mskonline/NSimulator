#include "interface.h"

#include <QDateTime>

Interface::Interface(QWidget *parent) : QMainWindow(parent)
{
    this->setupUi(this);
}

Interface::~Interface()
{

}

void Interface::log(QString msg)
{
    this->LogViewer->append("[" + QDateTime::currentDateTime()
                            .toString("hh:mm:ss") + "] " + msg);
}
