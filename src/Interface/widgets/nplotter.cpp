#include "../../Network/network.h"
#include "nplotter.h"

#include <QVector>
#include <QDebug>

NPlotter::NPlotter(QWidget *parent, Network *n) :
    QWidget(parent,Qt::Window)
{
    setupUi(this);
    this->network = n;
    this->setAttribute(Qt::WA_QuitOnClose);

    this->setInputs();
    connect(cb_router,SIGNAL(currentIndexChanged(int)),this,SLOT(setOAdaptorsCombo(int)));
    connect(pb_plot,SIGNAL(released()),this,SLOT(plot()));
    show();
}

void NPlotter::setInputs()
{
    for(int i = 0; i < network->numRouters; ++i)
        cb_router->addItem(network->routers[i]->name);

    for(int j = 0; j < network->routers[0]->numOutputs; ++j)
       cb_oAdaptor->addItem(QString("%1").arg(j + 1));

    for(int k = 0; k < network->routers[0]->numQueues; ++k)
        cb_queue->addItem(QString("%1").arg(k + 1));

}

void NPlotter::plot()
{
    w_plot->clearGraphs();

    int index = cb_parameter->currentIndex();
    int rIndex = cb_router->currentIndex();
    int qIndex = cb_queue->currentIndex();
    int oAdaptor = cb_oAdaptor->currentIndex();
    QString xLabel, yLabel;

    QVector<double> y;

    if(index == 0)
    {
       y = QVector<double>::fromStdVector(network->routers[rIndex]
                          ->outAdaptors[oAdaptor]->queues[qIndex]->itemsInQ);
       xLabel = "After nth packet removed";
       yLabel = "Queue Size";
    }
    else
    {
       y = QVector<double>::fromStdVector(network->routers[rIndex]
                          ->outAdaptors[oAdaptor]->queues[qIndex]->residenceTime);
       xLabel = "Packet";
       yLabel = "Residence Time";
    }


    w_plot->addGraph();

    QVector<double> x;

    for(int i = 1; i <= y.size(); ++i)
        x.push_back(i);

    w_plot->graph(0)->setData(x, y);
    w_plot->xAxis->setLabel(xLabel);
    w_plot->yAxis->setLabel(yLabel);
    w_plot->rescaleAxes();
    w_plot->replot(QCustomPlot::rpImmediate);

    //for(int i = 0; i < x.size(); ++i)
        //qDebug() << x.at(i);
}

void NPlotter::setOAdaptorsCombo(int index)
{
    qDebug() << index;
    for(int i = 0; i < network->routers[index]->numOutputs; ++i)
       cb_oAdaptor->addItem(QString("%1").arg(i + 1));
}

NPlotter::~NPlotter()
{
    delete ui;
}
