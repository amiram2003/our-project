#include <QCoreApplication>
#include "canhandler.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    CanHandler can;

    if (can.connectToDevice("vcan0")) {
        can.sendMessage(0x123, QByteArray::fromHex("AABBCCDD"));
    }

    return a.exec();
}