#include <QThread>
#include "nsimulator.h"
#include "./Network/routingtable.h"

#include <QDebug>
#include <QTextStream>

NSimulator::NSimulator()
{
}

void NSimulator::initiate(int mode){

    interface = new Interface();
    interface->show();
    interface->log("Welcome to NSimulator...");

    network = new Network();
    network->setInterfaceObj(interface);

    if(mode == NORMAL_RUN_MODE)
    {
        network->initiate();
        connect(interface->pb_run,SIGNAL(released()),network,SLOT(run()));
        connect(interface->pb_stop,SIGNAL(released()),network,SLOT(stop()));
    }
    else
    {
        connect(interface->pb_run,SIGNAL(released()),this,SLOT(test()));
    }

    connect(interface->pb_plots,SIGNAL(released()),this,SLOT(showPlots()));
    connect(interface->pb_quit,SIGNAL(released()),this,SLOT(closeApp()));
}

void NSimulator::showPlots()
{
    interface->showPlots(network);
}

void NSimulator::test()
{
    RoutingTable *rt = new RoutingTable("./config/p4/Ph4ForwardingTable");
    /*unsigned char d[4] = {0,0,0,0};
    int p,q;
    rt->lookUp(d,p,q);

    interface->log(QString("Port : %1").arg(p));
    delete rt;*/

    rt->printRoutingTable();

    /*generateFTR2();
    generateFTR3();
    generateFTR4();
    generateFTR5();
    generateFTR6();
    generateFTR7();*/
    //verifyPackets();
}

void NSimulator::verifyPackets()
{
    writeR2OutContent();
    writeR4OutContent();
    writeR5OutContent();
    writeR6OutContent();
    writeR7OutContent();
}

void NSimulator::writeR2OutContent()
{
    QFile f("./data/p4/r2Out");
    QFile of("./data/p4/r2Out.txt");

    if(!f.open(QFile::ReadOnly))
        return;

    if(!of.open(QFile::WriteOnly))
        return;

    QTextStream outStream(&of);

    int pSize = 500;

    QByteArray ba;

    ba = f.readAll();
    char *buffer = new char[ba.size()];
    buffer = ba.data();


    int j = 0;

    while(j < ba.size())
    {
        packet p;
        memcpy(&p.packetv4, buffer, pSize);

        outStream << p.packetv4.destination_addr[0] << " " << p.packetv4.destination_addr[1] << " "
                << p.packetv4.destination_addr[2] << " " << p.packetv4.destination_addr[3] << "\n";
        buffer += pSize;
        j += pSize;
    }

    of.close();

    qDebug() << "Done writing r2 contents";
}

void NSimulator::writeR4OutContent()
{
    QFile f("./data/p4/r4Out");
    QFile of("./data/p4/r4Out.txt");

    if(!f.open(QFile::ReadOnly))
        return;

    if(!of.open(QFile::WriteOnly))
        return;

    QTextStream outStream(&of);

    int pSize = 500;

    QByteArray ba;

    ba = f.readAll();
    char *buffer = new char[ba.size()];
    buffer = ba.data();


    int j = 0;

    while(j < ba.size())
    {
        packet p;
        memcpy(&p.packetv4, buffer, pSize);

        outStream << p.packetv4.destination_addr[0] << " " << p.packetv4.destination_addr[1] << " "
                << p.packetv4.destination_addr[2] << " " << p.packetv4.destination_addr[3] << "\n";
        buffer += pSize;
        j += pSize;
    }

    of.close();

    qDebug() << "Done writing r4 contents";
}

void NSimulator::writeR5OutContent()
{
    QFile f("./data/p4/r5Out");
    QFile of("./data/p4/r5Out.txt");

    if(!f.open(QFile::ReadOnly))
        return;

    if(!of.open(QFile::WriteOnly))
        return;

    QTextStream outStream(&of);

    int pSize = 500;

    QByteArray ba;

    ba = f.readAll();
    char *buffer = new char[ba.size()];
    buffer = ba.data();


    int j = 0;

    while(j < ba.size())
    {
        packet p;
        memcpy(&p.packetv4, buffer, pSize);

        outStream << p.packetv4.destination_addr[0] << " " << p.packetv4.destination_addr[1] << " "
                << p.packetv4.destination_addr[2] << " " << p.packetv4.destination_addr[3] << "\n";
        buffer += pSize;
        j += pSize;
    }

    of.close();

    qDebug() << "Done writing r5 contents";
}

void NSimulator::writeR6OutContent()
{
    QFile f("./data/p4/r6Out");
    QFile of("./data/p4/r6Out.txt");

    if(!f.open(QFile::ReadOnly))
        return;

    if(!of.open(QFile::WriteOnly))
        return;

    QTextStream outStream(&of);

    int pSize = 500;

    QByteArray ba;

    ba = f.readAll();
    char *buffer = new char[ba.size()];
    buffer = ba.data();


    int j = 0;

    while(j < ba.size())
    {
        packet p;
        memcpy(&p.packetv4, buffer, pSize);

        outStream << p.packetv4.destination_addr[0] << " " << p.packetv4.destination_addr[1] << " "
                << p.packetv4.destination_addr[2] << " " << p.packetv4.destination_addr[3] << "\n";
        buffer += pSize;
        j += pSize;
    }

    of.close();

    qDebug() << "Done writing r6 contents";
}

void NSimulator::writeR7OutContent()
{
    QFile f("./data/p4/r7Out");
    QFile of("./data/p4/r7Out.txt");

    if(!f.open(QFile::ReadOnly))
        return;

    if(!of.open(QFile::WriteOnly))
        return;

    QTextStream outStream(&of);

    int pSize = 500;

    QByteArray ba;

    ba = f.readAll();
    char *buffer = new char[ba.size()];
    buffer = ba.data();


    int j = 0;

    while(j < ba.size())
    {
        packet p;
        memcpy(&p.packetv4, buffer, pSize);

        outStream << p.packetv4.destination_addr[0] << " " << p.packetv4.destination_addr[1] << " "
                << p.packetv4.destination_addr[2] << " " << p.packetv4.destination_addr[3] << "\n";
        buffer += pSize;
        j += pSize;
    }

    of.close();

    qDebug() << "Done writing r7 contents";
}

/*void NSimulator::generateFTR1()
{
    // TODO
    // For Router R1
    Routing r1 = {{187,210,0,0},{177,215,64,0},{255,255,192,0},{224,135,7,1},1,1,{0},{0,0,0}};
    Routing r2 = {{101,112,0,0},{183,216,160,0},{255,255,224,0},{224,135,7,1},1,2,{40},{0,0,0}};
    Routing r3 = {{188,0,0,0},{177,215,200,0},{255,255,248,0},{224,135,7,1},2,3,{80},{0,0,0}};
    Routing r4 = {{153,232,0,0},{216,15,0,0},{255,255,0,0},{224,135,7,1},2,1,{40},{0,0,0}};
    Routing r5 = {{122,34,0,0},{216,15,64,0},{255,255,192,0},{224,135,7,1},2,2,{72},{0,0,0}};
    Routing r6 = {{121,0,0,0},{200,177,144,0},{255,255,240,0},{224,135,7,1},2,3,{120},{0,0,0}};
    Routing r7 = {{197,225,0,0},{220,210,0,0},{255,255,0,0},{224,135,7,1},2,1,{0},{0,0,0}};
    Routing r8 = {{225,14,0,0},{212,18,32,0},{255,255,224,0},{224,135,7,1},2,2,{80},{0,0,0}};
    Routing r9 = {{226,0,0,0},{212,18,44,0},{255,255,252,0},{224,135,7,1},2,3,{184},{0,0,0}};
    Routing r10= {{121,18,0,0},{121,128,0,0},{255,255,0,0},{224,135,7,1},1,2,{72},{0,0,0}};
    Routing r11= {{155,43,0,0},{128,10,16,0},{255,255,240,0},{224,135,7,1},2,3,{120},{0,0,0}};
    Routing r12= {{0,0,0,0},{0,0,0,0},{0,0,0,0},{224,135,7,1},1,1,{0},{0,0,0}};

    QFile f("Ph4R1ForwardingTable");

    if (f.open(QFile::WriteOnly | QFile::Truncate)) {
       f.write(reinterpret_cast<char*>(&r1), sizeof(r1));
       f.write(reinterpret_cast<char*>(&r2), sizeof(r2));
       f.write(reinterpret_cast<char*>(&r3), sizeof(r3));
       f.write(reinterpret_cast<char*>(&r4), sizeof(r4));
       f.write(reinterpret_cast<char*>(&r5), sizeof(r5));
       f.write(reinterpret_cast<char*>(&r6), sizeof(r6));
       f.write(reinterpret_cast<char*>(&r7), sizeof(r7));
       f.write(reinterpret_cast<char*>(&r8), sizeof(r8));
       f.write(reinterpret_cast<char*>(&r9), sizeof(r9));
       f.write(reinterpret_cast<char*>(&r10), sizeof(r10));
       f.write(reinterpret_cast<char*>(&r11), sizeof(r11));
       f.write(reinterpret_cast<char*>(&r12), sizeof(r12));
       f.close();
    }
}

void NSimulator::generateFTR2()
{
    // For Router R2
    Routing r1 = {{187,210,0,0},{177,215,64,0},{255,255,192,0},{224,135,7,1},1,1,{0},{0,0,0}};
    Routing r2 = {{101,112,0,0},{183,216,160,0},{255,255,224,0},{224,135,7,1},2,2,{40},{0,0,0}};
    Routing r3 = {{188,0,0,0},{177,215,200,0},{255,255,248,0},{224,135,7,1},2,3,{80},{0,0,0}};
    Routing r4 = {{153,232,0,0},{216,15,0,0},{255,255,0,0},{224,135,7,1},2,1,{40},{0,0,0}};
    Routing r5 = {{122,34,0,0},{216,15,64,0},{255,255,192,0},{224,135,7,1},2,2,{72},{0,0,0}};
    Routing r6 = {{121,0,0,0},{200,177,144,0},{255,255,240,0},{224,135,7,1},2,3,{120},{0,0,0}};
    Routing r7 = {{197,225,0,0},{220,210,0,0},{255,255,0,0},{224,135,7,1},2,1,{0},{0,0,0}};
    Routing r8 = {{225,14,0,0},{212,18,32,0},{255,255,224,0},{224,135,7,1},2,2,{80},{0,0,0}};
    Routing r9 = {{226,0,0,0},{212,18,44,0},{255,255,252,0},{224,135,7,1},2,3,{184},{0,0,0}};
    Routing r10= {{121,18,0,0},{121,128,0,0},{255,255,0,0},{224,135,7,1},1,2,{72},{0,0,0}};
    Routing r11= {{155,43,0,0},{128,10,16,0},{255,255,240,0},{224,135,7,1},2,3,{120},{0,0,0}};
    Routing r12= {{0,0,0,0},{0,0,0,0},{0,0,0,0},{224,135,7,1},1,1,{0},{0,0,0}};

    QFile f("Ph4R2ForwardingTable");

    if (f.open(QFile::WriteOnly | QFile::Truncate)) {
       f.write(reinterpret_cast<char*>(&r1), sizeof(r1));
       f.write(reinterpret_cast<char*>(&r2), sizeof(r2));
       f.write(reinterpret_cast<char*>(&r3), sizeof(r3));
       f.write(reinterpret_cast<char*>(&r4), sizeof(r4));
       f.write(reinterpret_cast<char*>(&r5), sizeof(r5));
       f.write(reinterpret_cast<char*>(&r6), sizeof(r6));
       f.write(reinterpret_cast<char*>(&r7), sizeof(r7));
       f.write(reinterpret_cast<char*>(&r8), sizeof(r8));
       f.write(reinterpret_cast<char*>(&r9), sizeof(r9));
       f.write(reinterpret_cast<char*>(&r10), sizeof(r10));
       f.write(reinterpret_cast<char*>(&r11), sizeof(r11));
       f.write(reinterpret_cast<char*>(&r12), sizeof(r12));
       f.close();
    }
}

void NSimulator::generateFTR3()
{
    // For Router R3
    Routing r1 = {{187,210,0,0},{177,215,64,0},{255,255,192,0},{224,135,7,1},1,1,{0},{0,0,0}};
    Routing r2 = {{101,112,0,0},{183,216,160,0},{255,255,224,0},{224,135,7,1},1,2,{40},{0,0,0}};
    Routing r3 = {{188,0,0,0},{177,215,200,0},{255,255,248,0},{224,135,7,1},1,3,{80},{0,0,0}};
    Routing r4 = {{153,232,0,0},{216,15,0,0},{255,255,0,0},{224,135,7,1},1,1,{40},{0,0,0}};
    Routing r5 = {{122,34,0,0},{216,15,64,0},{255,255,192,0},{224,135,7,1},3,2,{72},{0,0,0}};
    Routing r6 = {{121,0,0,0},{200,177,144,0},{255,255,240,0},{224,135,7,1},3,3,{120},{0,0,0}};
    Routing r7 = {{197,225,0,0},{220,210,0,0},{255,255,0,0},{224,135,7,1},3,1,{0},{0,0,0}};
    Routing r8 = {{225,14,0,0},{212,18,32,0},{255,255,224,0},{224,135,7,1},3,2,{80},{0,0,0}};
    Routing r9 = {{226,0,0,0},{212,18,44,0},{255,255,252,0},{224,135,7,1},3,3,{184},{0,0,0}};
    Routing r10= {{121,18,0,0},{121,128,0,0},{255,255,0,0},{224,135,7,1},1,2,{72},{0,0,0}};
    Routing r11= {{155,43,0,0},{128,10,16,0},{255,255,240,0},{224,135,7,1},2,3,{120},{0,0,0}};
    Routing r12= {{0,0,0,0},{0,0,0,0},{0,0,0,0},{224,135,7,1},1,1,{0},{0,0,0}};

    QFile f("Ph4R3ForwardingTable");

    if (f.open(QFile::WriteOnly | QFile::Truncate)) {
       f.write(reinterpret_cast<char*>(&r1), sizeof(r1));
       f.write(reinterpret_cast<char*>(&r2), sizeof(r2));
       f.write(reinterpret_cast<char*>(&r3), sizeof(r3));
       f.write(reinterpret_cast<char*>(&r4), sizeof(r4));
       f.write(reinterpret_cast<char*>(&r5), sizeof(r5));
       f.write(reinterpret_cast<char*>(&r6), sizeof(r6));
       f.write(reinterpret_cast<char*>(&r7), sizeof(r7));
       f.write(reinterpret_cast<char*>(&r8), sizeof(r8));
       f.write(reinterpret_cast<char*>(&r9), sizeof(r9));
       f.write(reinterpret_cast<char*>(&r10), sizeof(r10));
       f.write(reinterpret_cast<char*>(&r11), sizeof(r11));
       f.write(reinterpret_cast<char*>(&r12), sizeof(r12));
       f.close();
    }
}

void NSimulator::generateFTR4()
{
    // For Router R4
    Routing r1 = {{187,210,0,0},{177,215,64,0},{255,255,192,0},{224,135,7,1},2,1,{0},{0,0,0}};
    Routing r2 = {{101,112,0,0},{183,216,160,0},{255,255,224,0},{224,135,7,1},2,2,{40},{0,0,0}};
    Routing r3 = {{188,0,0,0},{177,215,200,0},{255,255,248,0},{224,135,7,1},1,3,{80},{0,0,0}};
    Routing r4 = {{153,232,0,0},{216,15,0,0},{255,255,0,0},{224,135,7,1},1,1,{40},{0,0,0}};
    Routing r5 = {{122,34,0,0},{216,15,64,0},{255,255,192,0},{224,135,7,1},2,2,{72},{0,0,0}};
    Routing r6 = {{121,0,0,0},{200,177,144,0},{255,255,240,0},{224,135,7,1},2,3,{120},{0,0,0}};
    Routing r7 = {{197,225,0,0},{220,210,0,0},{255,255,0,0},{224,135,7,1},2,1,{0},{0,0,0}};
    Routing r8 = {{225,14,0,0},{212,18,32,0},{255,255,224,0},{224,135,7,1},2,2,{80},{0,0,0}};
    Routing r9 = {{226,0,0,0},{212,18,44,0},{255,255,252,0},{224,135,7,1},2,3,{184},{0,0,0}};
    Routing r10= {{121,18,0,0},{121,128,0,0},{255,255,0,0},{224,135,7,1},2,2,{72},{0,0,0}};
    Routing r11= {{155,43,0,0},{128,10,16,0},{255,255,240,0},{224,135,7,1},2,3,{120},{0,0,0}};
    Routing r12= {{0,0,0,0},{0,0,0,0},{0,0,0,0},{224,135,7,1},2,1,{0},{0,0,0}};

    QFile f("Ph4R4ForwardingTable");

    if (f.open(QFile::WriteOnly | QFile::Truncate)) {
       f.write(reinterpret_cast<char*>(&r1), sizeof(r1));
       f.write(reinterpret_cast<char*>(&r2), sizeof(r2));
       f.write(reinterpret_cast<char*>(&r3), sizeof(r3));
       f.write(reinterpret_cast<char*>(&r4), sizeof(r4));
       f.write(reinterpret_cast<char*>(&r5), sizeof(r5));
       f.write(reinterpret_cast<char*>(&r6), sizeof(r6));
       f.write(reinterpret_cast<char*>(&r7), sizeof(r7));
       f.write(reinterpret_cast<char*>(&r8), sizeof(r8));
       f.write(reinterpret_cast<char*>(&r9), sizeof(r9));
       f.write(reinterpret_cast<char*>(&r10), sizeof(r10));
       f.write(reinterpret_cast<char*>(&r11), sizeof(r11));
       f.write(reinterpret_cast<char*>(&r12), sizeof(r12));
       f.close();
    }
}

void NSimulator::generateFTR5()
{
    // For Router R5
    Routing r1 = {{187,210,0,0},{177,215,64,0},{255,255,192,0},{224,135,7,1},1,1,{0},{0,0,0}};
    Routing r2 = {{101,112,0,0},{183,216,160,0},{255,255,224,0},{224,135,7,1},1,2,{40},{0,0,0}};
    Routing r3 = {{188,0,0,0},{177,215,200,0},{255,255,248,0},{224,135,7,1},1,3,{80},{0,0,0}};
    Routing r4 = {{153,232,0,0},{216,15,0,0},{255,255,0,0},{224,135,7,1},1,1,{40},{0,0,0}};
    Routing r5 = {{122,34,0,0},{216,15,64,0},{255,255,192,0},{224,135,7,1},2,2,{72},{0,0,0}};
    Routing r6 = {{121,0,0,0},{200,177,144,0},{255,255,240,0},{224,135,7,1},1,3,{120},{0,0,0}};
    Routing r7 = {{197,225,0,0},{220,210,0,0},{255,255,0,0},{224,135,7,1},2,1,{0},{0,0,0}};
    Routing r8 = {{225,14,0,0},{212,18,32,0},{255,255,224,0},{224,135,7,1},1,2,{80},{0,0,0}};
    Routing r9 = {{226,0,0,0},{212,18,44,0},{255,255,252,0},{224,135,7,1},1,3,{184},{0,0,0}};
    Routing r10= {{121,18,0,0},{121,128,0,0},{255,255,0,0},{224,135,7,1},1,2,{72},{0,0,0}};
    Routing r11= {{155,43,0,0},{128,10,16,0},{255,255,240,0},{224,135,7,1},1,3,{120},{0,0,0}};
    Routing r12= {{0,0,0,0},{0,0,0,0},{0,0,0,0},{224,135,7,1},1,1,{0},{0,0,0}};

    QFile f("Ph4R5ForwardingTable");

    if (f.open(QFile::WriteOnly | QFile::Truncate)) {
       f.write(reinterpret_cast<char*>(&r1), sizeof(r1));
       f.write(reinterpret_cast<char*>(&r2), sizeof(r2));
       f.write(reinterpret_cast<char*>(&r3), sizeof(r3));
       f.write(reinterpret_cast<char*>(&r4), sizeof(r4));
       f.write(reinterpret_cast<char*>(&r5), sizeof(r5));
       f.write(reinterpret_cast<char*>(&r6), sizeof(r6));
       f.write(reinterpret_cast<char*>(&r7), sizeof(r7));
       f.write(reinterpret_cast<char*>(&r8), sizeof(r8));
       f.write(reinterpret_cast<char*>(&r9), sizeof(r9));
       f.write(reinterpret_cast<char*>(&r10), sizeof(r10));
       f.write(reinterpret_cast<char*>(&r11), sizeof(r11));
       f.write(reinterpret_cast<char*>(&r12), sizeof(r12));
       f.close();
    }
}

void NSimulator::generateFTR6()
{
    // For Router R6
    Routing r1 = {{187,210,0,0},{177,215,64,0},{255,255,192,0},{224,135,7,1},1,1,{0},{0,0,0}};
    Routing r2 = {{101,112,0,0},{183,216,160,0},{255,255,224,0},{224,135,7,1},1,2,{40},{0,0,0}};
    Routing r3 = {{188,0,0,0},{177,215,200,0},{255,255,248,0},{224,135,7,1},2,3,{80},{0,0,0}};
    Routing r4 = {{153,232,0,0},{216,15,0,0},{255,255,0,0},{224,135,7,1},2,1,{40},{0,0,0}};
    Routing r5 = {{122,34,0,0},{216,15,64,0},{255,255,192,0},{224,135,7,1},2,2,{72},{0,0,0}};
    Routing r6 = {{121,0,0,0},{200,177,144,0},{255,255,240,0},{224,135,7,1},2,3,{120},{0,0,0}};
    Routing r7 = {{197,225,0,0},{220,210,0,0},{255,255,0,0},{224,135,7,1},2,1,{0},{0,0,0}};
    Routing r8 = {{225,14,0,0},{212,18,32,0},{255,255,224,0},{224,135,7,1},2,2,{80},{0,0,0}};
    Routing r9 = {{226,0,0,0},{212,18,44,0},{255,255,252,0},{224,135,7,1},2,3,{184},{0,0,0}};
    Routing r10= {{121,18,0,0},{121,128,0,0},{255,255,0,0},{224,135,7,1},1,2,{72},{0,0,0}};
    Routing r11= {{155,43,0,0},{128,10,16,0},{255,255,240,0},{224,135,7,1},2,3,{120},{0,0,0}};
    Routing r12= {{0,0,0,0},{0,0,0,0},{0,0,0,0},{224,135,7,1},1,1,{0},{0,0,0}};

    QFile f("Ph4R6ForwardingTable");

    if (f.open(QFile::WriteOnly | QFile::Truncate)) {
       f.write(reinterpret_cast<char*>(&r1), sizeof(r1));
       f.write(reinterpret_cast<char*>(&r2), sizeof(r2));
       f.write(reinterpret_cast<char*>(&r3), sizeof(r3));
       f.write(reinterpret_cast<char*>(&r4), sizeof(r4));
       f.write(reinterpret_cast<char*>(&r5), sizeof(r5));
       f.write(reinterpret_cast<char*>(&r6), sizeof(r6));
       f.write(reinterpret_cast<char*>(&r7), sizeof(r7));
       f.write(reinterpret_cast<char*>(&r8), sizeof(r8));
       f.write(reinterpret_cast<char*>(&r9), sizeof(r9));
       f.write(reinterpret_cast<char*>(&r10), sizeof(r10));
       f.write(reinterpret_cast<char*>(&r11), sizeof(r11));
       f.write(reinterpret_cast<char*>(&r12), sizeof(r12));
       f.close();
    }
}

void NSimulator::generateFTR7()
{
    // For Router R7
    Routing r1 = {{187,210,0,0},{177,215,64,0},{255,255,192,0},{224,135,7,1},1,1,{0},{0,0,0}};
    Routing r2 = {{101,112,0,0},{183,216,160,0},{255,255,224,0},{224,135,7,1},1,2,{40},{0,0,0}};
    Routing r3 = {{188,0,0,0},{177,215,200,0},{255,255,248,0},{224,135,7,1},1,3,{80},{0,0,0}};
    Routing r4 = {{153,232,0,0},{216,15,0,0},{255,255,0,0},{224,135,7,1},1,1,{40},{0,0,0}};
    Routing r5 = {{122,34,0,0},{216,15,64,0},{255,255,192,0},{224,135,7,1},1,2,{72},{0,0,0}};
    Routing r6 = {{121,0,0,0},{200,177,144,0},{255,255,240,0},{224,135,7,1},1,3,{120},{0,0,0}};
    Routing r7 = {{197,225,0,0},{220,210,0,0},{255,255,0,0},{224,135,7,1},1,1,{0},{0,0,0}};
    Routing r8 = {{225,14,0,0},{212,18,32,0},{255,255,224,0},{224,135,7,1},1,2,{80},{0,0,0}};
    Routing r9 = {{226,0,0,0},{212,18,44,0},{255,255,252,0},{224,135,7,1},1,3,{184},{0,0,0}};
    Routing r10= {{121,18,0,0},{121,128,0,0},{255,255,0,0},{224,135,7,1},1,2,{72},{0,0,0}};
    Routing r11= {{155,43,0,0},{128,10,16,0},{255,255,240,0},{224,135,7,1},1,3,{120},{0,0,0}};
    Routing r12= {{0,0,0,0},{0,0,0,0},{0,0,0,0},{224,135,7,1},1,1,{0},{0,0,0}};

    QFile f("Ph4R7ForwardingTable");

    if (f.open(QFile::WriteOnly | QFile::Truncate)) {
       f.write(reinterpret_cast<char*>(&r1), sizeof(r1));
       f.write(reinterpret_cast<char*>(&r2), sizeof(r2));
       f.write(reinterpret_cast<char*>(&r3), sizeof(r3));
       f.write(reinterpret_cast<char*>(&r4), sizeof(r4));
       f.write(reinterpret_cast<char*>(&r5), sizeof(r5));
       f.write(reinterpret_cast<char*>(&r6), sizeof(r6));
       f.write(reinterpret_cast<char*>(&r7), sizeof(r7));
       f.write(reinterpret_cast<char*>(&r8), sizeof(r8));
       f.write(reinterpret_cast<char*>(&r9), sizeof(r9));
       f.write(reinterpret_cast<char*>(&r10), sizeof(r10));
       f.write(reinterpret_cast<char*>(&r11), sizeof(r11));
       f.write(reinterpret_cast<char*>(&r12), sizeof(r12));
       f.close();
    }
}
*/
void NSimulator::closeApp()
{
    // TODO :tear down network
    delete network;

    qApp->quit();
}

NSimulator::~NSimulator()
{
    delete interface;
}


