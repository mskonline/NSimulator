#ifndef NPLOTTER_H
#define NPLOTTER_H

#include <QWidget>
#include "ui_nplotter.h"

class Network;
class Router;

class NPlotter : public QWidget, public Ui::NPlotter
{
    Q_OBJECT

    public:
        Router **routers;
        Network *network;

        NPlotter(QWidget *parent,Network *);
        ~NPlotter();

    private:

        Ui::NPlotter *ui;

    public slots:

        void plot();
};

#endif // NPLOTTER_H
