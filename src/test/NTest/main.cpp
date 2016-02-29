/*#include <QCoreApplication>
#include <QFile>

struct ipv4 {
    unsigned char addr [20]; // 20 bytes
    int opt_addr[5]; // 20 bytes
    int header[5]; // 20 bytes
    int data[110]; // 440 bytes
};

void writeBin();

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    writeBin();
    return a.exec();
}

void writeBin()
{
    ipv4 packet = {{192},{1},{1},{0}};
    QFile f("E:/ipv4_1.bin");

    if (f.open(QFile::WriteOnly | QFile::Truncate)) {
       f.write(reinterpret_cast<char*>(&packet), sizeof(packet));
       f.close();
    }
}
*/




#include <QCoreApplication>
#include<QFile>
#include<QString>
#include<QDebug>
#include<QTextStream>
#include<QBuffer>
#include<QQueue>
#include<QByteArray>
#include<iostream>
using namespace std;

struct ipv4 {
    unsigned char addr [20]; // 20 bytes
    int opt_addr[5]; // 20 bytes
    int header[5]; // 20 bytes
    int data[110]; // 440 bytes
};

void  Write(QString Filename) //writing into buffer
{

    QFile mfile1(Filename);
    QFile mfile2(Filename);
    QFile mfile3(Filename);

    QQueue<unsigned char> q1;
    QQueue<unsigned char> q2;
    QQueue<unsigned char> q3;

    q1 = new QList<ipv4>();


    if(!mfile1.open(QFile::ReadOnly))
    {
        qDebug() << "No Such File";
        return;
    }

    if(!mfile2.open(QFile::ReadOnly))
    {
        qDebug() << "No Such File";
        return;
    }
    if(!mfile3.open(QFile::ReadOnly))
    {
        qDebug() << "No Such File";
        return;
    }

    char *buffer1;
    char *buffer2;
    char *buffer3;


    QByteArray ba1;
    QByteArray ba2;
    QByteArray ba3;

     ba1 = mfile.readAll();//putting into bytearray
     ba2 = mfile.readAll();
     ba3 = mfile.readAll();


    int np1 = ba1.size()/500; // number of packets
    int np2 = ba2.size()/500;
    int np3 = ba3.size()/500;

    int tsize= ba1.size()+ ba2.size()+ ba3.size(); // total packets

    buffer1 = ba1.data();
    buffer2 = ba2.data();
    buffer3 = ba3.data();


    for(i=0;i<tsize;i++)
    {

        for(int i=0;i<buffer2.size();i=i+499)
             {
                 ipv4 ip;
                 memcpy(&ip,buffer1,i+500);
                 //QQueue q1;
                   q1.enqueue(ip);
                }

        for(int i=0;i<buffer2.size();i=i+499)
             {
                 ipv4 ip;
                 memcpy(&ip,buffer2,i+500);
                 //QQueue q2;
                   q2.enqueue(ip);
                }

        for(int i=0;i<buffer3.size();i=i+499)
             {
                 ipv4 ip;
                 memcpy(&ip,buffer3,i+500);
                 //QQueue q3;
                   q3.enqueue(ip);
                }


     }



   // QQueue queue;
    //queue.enqueue(mfile.readAll());


    mfile.flush();
    mfile.close();
}
/*void Read(QString Filename)
{
    QFile mfile(Filename);
    if(!mfile.open(QFile::ReadOnly | QFile::Truncate))
    {
        qDebug() << "No Such File";
        return;
    }
    QTextStream in(&mfile);
    QString mText = in.readAll();

    qDebug() << mText;

    mfile.flush();
    mfile.close();
}
 */
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QString mFilename = "E:/link1flow"; //path of file
    Write(mFilename);
   // Read(mFilename);

    return a.exec();
}
