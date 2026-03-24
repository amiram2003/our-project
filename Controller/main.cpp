#include <QCoreApplication>
#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusError>
#include "controller.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    Controller controller;

    QDBusConnection bus = QDBusConnection::sessionBus();
    
    if (!bus.registerService("com.project.system")) {
        qCritical() << "Failed to register D-Bus service:" << bus.lastError().message();
        return 1;
    }

    if (!bus.registerObject("/Controller", &controller, QDBusConnection::ExportAllSlots)) {
        qCritical() << "Failed to register D-Bus object:" << bus.lastError().message();
        return 1;
    }

    qDebug() << "Controller Service is running and waiting for UI signals...";
    return app.exec(); 
}