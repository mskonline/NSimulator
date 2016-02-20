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


void  Write(QString Filename) //writing into buffer
{

    QFile mfile(Filename);
    if(!mfile.open(QFile::WriteOnly | QFile::Truncate))
    {
        qDebug() << "No Such File";
        return;
    }


    QQueue queue;
    queue.enqueue(mfile.readAll());


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
    QString mFilename = "E:/QT coding/Qt.bin"; //path of file
    Write(mFilename);
   // Read(mFilename);

    return a.exec();
}
