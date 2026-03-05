#include <QCoreApplication>
#include "controller.h"
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    Controller controller;
    
    qDebug() << "IP:" << controller.getIpAddress();
    qDebug() << "MAC:" << controller.getMacAddress();
    
    controller.requestRestart();

    return app.exec();
}