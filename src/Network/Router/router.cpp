#include "router.h"
#include "Commons/commons.h"
#include <QSettings>
#include <QFile>
#include <QByteArray>
#include <QQueue>
#include <ctime>


#include <QDebug>

Router::Router(QString name)
{
    this->name = name;
    this->allSet = false;
}

void Router::initiate()
{
    QString settingsFile = "./config/" + this->name + ".ini";
    QSettings *settings = new QSettings(settingsFile,QSettings::IniFormat);

    if(settings->allKeys().count() == 0)
        return;

    settings->beginGroup("input");
        this->inputFiles = settings->value("values").toString().split(":");
        this->numInputs = settings->value("values").toString().split(":").count();
        QStringList ratesList = settings->value("rates").toString().split(":");

        input_rates = new int[ratesList.size()];
        for(int i = 0; i < ratesList.size(); ++i){
            input_rates[i] = ratesList.at(i).toInt();
        }

    settings->endGroup();

    settings->beginGroup("output");
        this->outputFiles = settings->value("values").toString().split(":");
        this->numOutputs = settings->value("values").toString().split(":").count();
        QStringList outRatesList = settings->value("rates").toString().split(":");
        output_rates = new int[outRatesList.size()];
        for(int i = 0; i < outRatesList.size(); ++i){
            output_rates[i] = outRatesList.at(i).toInt();
        }
    settings->endGroup();

    delete settings;

    allSet = true;
    interface->log("Router " + name + " initiated");
}

void Router::run()
{
    if(allSet){
        interface->log("Router " + name + " running");

        inpQueues = new QQueue<packet>*[numInputs];
        num_input_packets = 0;

        for(int i = 0; i < numInputs; ++i){
            inpQueues[i] = new QQueue<packet>();

            QFile f(this->inputFiles.at(i));

            if(!f.open(QFile::ReadOnly))
            {
                qDebug() << "No Such File";
                return;
            }

            char *buffer;
            QByteArray ba;

            ba = f.readAll();
            buffer = ba.data();
            num_input_packets += (ba.size()/PACKET_SIZE);
            int j = 0;

            while(j < ba.size())
            {
                packet p;
                memcpy(&p.packetv4, buffer, PACKET_SIZE);
                p.arrivalTime = std::time(0);
                inpQueues[i]->enqueue(p);

                buffer += PACKET_SIZE;
                j += PACKET_SIZE;
            }

            delete buffer;
        }

        outQueues = new QQueue<packet>*[numOutputs];
        for(int i = 0; i < numOutputs; ++i)
            outQueues[i] = new QQueue<packet>();

        // For all packets on input side
        int pk_count = 0;
        int iP, nQ = 0;

        while(pk_count < num_input_packets)
        {
            for(iP = 0; iP <= input_rates[nQ]; ++iP)
            {
                if(inpQueues[nQ]->count() == 0)
                    break;

                packet p = inpQueues[nQ]->dequeue();

                int oPort = routingTable->lookUp(p.packetv4.destination_addr);
                --oPort;
                outQueues[oPort]->enqueue(p);
            }
            ++nQ;
            nQ = nQ % numInputs;
            pk_count += iP;
        }

        oFiles = new QFile*[numOutputs];

        for(int i = 0; i < numOutputs; ++i){
            oFiles[i] = new QFile(this->outputFiles.at(i));

            if(!oFiles[i]->open(QFile::WriteOnly | QFile::Truncate))
            {
                qDebug() << "No Such File";
                return;
            }
        }

        pk_count = 0;
        nQ = 0;
        int oP;

        // For all packets on output side
        while(pk_count < num_input_packets)
        {
            for(oP = 0; oP <= output_rates[nQ]; ++oP)
            {
                if(outQueues[nQ]->count() == 0)
                    break;


                //R=(pk_count+num_input_packets);
                //sum+=R


                packet p = outQueues[nQ]->dequeue();



                /*p.depTime= std::time(0);
                resTime=p.depTime-p.arrivalTime; //residence time of packet
                totalTime=p.totalTime+p.resTime // p.totalTime is initially NULL
                count++ // no of packets
                 if(maxTR<resTime)                //maxTR is initially NULL
                    {
                        maxTR=resTime;

                    }



                    */
                oFiles[nQ]->write(reinterpret_cast<char*>(&p.packetv4), PACKET_SIZE);
            }
            ++nQ;
            nQ = nQ % numOutputs;
            pk_count += oP;
        }
        //r=R/sum;



        // meanTr= totalTime/count;
        // cout << meanTr;
        // cout << maxTR;

        for(int i = 0; i < numOutputs; ++i){
            oFiles[i]->flush();
            oFiles[i]->close();
        }
    }

    interface->log("Router " + name + " finished processing.");
}

void Router::fabric(packet p,int pNo,int qNo)
{

}

void Router::setInterfaceObj(Interface *interface)
{
    this->interface = interface;
}

void Router::setRoutingTable(RoutingTable *rTable){
    this->routingTable = rTable;
}

Router::~Router()
{
}
