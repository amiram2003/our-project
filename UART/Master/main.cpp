#include <QCoreApplication>
#include <QTimer>
#include "master.h"

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);
    MasterManager master;

    if (master.connectPort("/dev/pts/7")) {
        QTimer *timer = new QTimer(&a);
        QObject::connect(timer, &QTimer::timeout, &master, &MasterManager::sendPing);
        timer->start(3000); 
    } else {
        return -1;
    }

    return a.exec();
}