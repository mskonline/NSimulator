struct ipv4 {
    int addr [5]; // 20 bytes
    int opt_addr[5]; // 20 bytes
    int header[5]; // 20 bytes
    int data[110]; // 440 bytes
};

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    return a.exec();
}

void writeBin()
{
    ipv4 packet = {{192,1,1,0,0}};
    QFile f("W:/Test/ipv4.bin");

    if (f.open(QFile::WriteOnly | QFile::Truncate)) {
       f.write(reinterpret_cast<char*>(&packet), sizeof(packet));
       f.close();
    }
}