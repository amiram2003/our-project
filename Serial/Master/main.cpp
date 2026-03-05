#include <QCoreApplication>
#include <QTimer>
#include "SerialCommunication.h"

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);
    SerialCommunication master;

    //< Replace all of this here !! 
    if (master.connectPort("/dev/ttyS0")) {
        // QTimer *timer = new QTimer(&a);
        // QObject::connect(timer, &QTimer::timeout, &master, &SerialCommunication::sendPing);
        // timer->start(3000); 
    }

    return a.exec();
}