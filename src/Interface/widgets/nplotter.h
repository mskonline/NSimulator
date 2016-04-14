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
        void setInputs();
        ~NPlotter();

    private:

        Ui::NPlotter *ui;

    public slots:

        void plot();
        void setOAdaptorsCombo(int);
};

#endif // NPLOTTER_H
