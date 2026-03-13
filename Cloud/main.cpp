#include <QCoreApplication>
#include <QDBusConnection>
#include <QDBusError>
#include <QDebug>
#include "cloud.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    Cloud cloud;
    
    QDBusConnection connection = QDBusConnection::sessionBus();

    if (!connection.isConnected()) {
        qCritical() << "Cannot connect to D-Bus:" << connection.lastError().message();
        return 1;
    }

    if (!connection.registerService("local.automotive.CloudService")) {
        qCritical() << "Service registration failed!" << connection.lastError().message();
        return 1;
    }

    if (!connection.registerObject("/", &cloud, 
                                   QDBusConnection::ExportAllSlots | 
                                   QDBusConnection::ExportAllSignals)) {
        qCritical() << "Object registration failed!";
        return 1;
    }

    qDebug() << "-----------------------------------------";
    qDebug() << "✅ Cloud Service is Online";
    qDebug() << "🏠 Service: local.automotive.CloudService";
    qDebug() << "🔍 Interface: local.automotive.Cloud";
    qDebug() << "-----------------------------------------";

    return app.exec();
}