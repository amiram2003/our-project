#include <QCoreApplication>
#include "slave.h"

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);
    SlaveManager slave;

    if (slave.listenOnPort("/dev/pts/8")) {
        qDebug() << "Slave is Listening and Ready...";
    } else {
        qDebug() << "Could not start Slave. Check port!";
    }

    return a.exec();
}