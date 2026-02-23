#include <QCoreApplication>
#include <QTimer>
#include "master.h"

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);
    MasterManager master;

    if (master.connectPort("/dev/pts/7")) { 
        qDebug() << "Master is UP and Running...";
        QTimer *timer = new QTimer();
        QObject::connect(timer, &QTimer::timeout, &master, &MasterManager::sendPing);
        timer->start(3000); 
    } else {
        qDebug() << "Could not start Master. Check port!";
    }

    return a.exec();
}