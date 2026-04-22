#include <QCoreApplication>
#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusError>
#include <QDebug>
#include "controller.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    Controller controller;

    if (!controller.setupCanInterface("vcan0")) {
        qCritical() << "Controller: Failed to setup CAN interface (vcan0)!";
        return 1; 
    } 

    QDBusConnection bus = QDBusConnection::sessionBus();
    
    if (!bus.registerService("com.project.system")) {
        qCritical() << "Failed to register D-Bus service:" << bus.lastError().message();
        return 1;
    }

    if (!bus.registerObject("/Controller", &controller, 
                           QDBusConnection::ExportAllSlots | QDBusConnection::ExportAllSignals)) {
        qCritical() << "Failed to register D-Bus object:" << bus.lastError().message();
        return 1;
    }

    qDebug() << "Controller Service is running...";
    qDebug() << "Monitoring vcan0 for Fingerprint and D-Bus for UI commands.";

    return app.exec(); 
}