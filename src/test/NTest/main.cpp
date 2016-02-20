#include <QCoreApplication>
#include <QFile>
#include <iostream>
using namespace std;
struct ipv4 {
    unsigned char addr [20]; // 20 bytes
    int opt_addr[5]; // 20 bytes
    int header[5]; // 20 bytes
    int data[110]; // 440 bytes
};

struct test {
    unsigned char a:4,b:4;
};

void writeBin();
void bitTest();

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    bitTest();
    return a.exec();
}

void bitTest()
{
    test t;
    t.a = 2;
    t.b = 2;

    cout << sizeof(t);
}

void writeBin()
{
    ipv4 packet = {{192},{1},{1},{0}};
    QFile f("W:/Test/ipv4_1.bin");

    if (f.open(QFile::WriteOnly | QFile::Truncate)) {
       f.write(reinterpret_cast<char*>(&packet), sizeof(packet));
       f.close();
    }
}
