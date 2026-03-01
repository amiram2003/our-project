#include <QCoreApplication>
#include "slave.h"

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);
    SlaveManager slave;

    if (!slave.listenOnPort("/dev/pts/8")) {
        return -1;
    }

    return a.exec();
}