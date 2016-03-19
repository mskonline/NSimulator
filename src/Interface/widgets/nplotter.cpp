#include "../../Network/network.h"
#include "nplotter.h"

#include <QVector>

NPlotter::NPlotter(QWidget *parent, Network *n) :
    QWidget(parent,Qt::Window)
{
    setupUi(this);
    this->network = n;
    this->setAttribute(Qt::WA_QuitOnClose);
}

void NPlotter::plot()
{
    w_plot->addGraph();

    QVector<double> x = QVector<double>::fromStdVector(network->routers[0]->outAdaptors[0]->itemsInQ);
    QVector<double> y;

    for(int i = 1; i <= x.size(); ++i){
        y.push_back(i);
    }

    w_plot->graph(0)->setData(x, y);
    w_plot->xAxis->setLabel("x");
    w_plot->yAxis->setLabel("y");
    w_plot->rescaleAxes();

    show();
}

NPlotter::~NPlotter()
{
    delete ui;
}
