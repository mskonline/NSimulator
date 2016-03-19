#ifndef INTERFACE_H
#define INTERFACE_H

#include <QMainWindow>
#include "ui_interface.h"

class Network;

class Interface : public QMainWindow, public Ui::Interface
{
    Q_OBJECT
    public:
        Interface(QWidget *parent = 0);

        void log(QString msg);
        ~Interface();

    public slots:

        void showPlots(Network *);
};

#endif // INTERFACE_H
