#include <QCoreApplication>
#include <QDBusConnection>
#include <QDBusError>
#include <QDebug>
#include <QDBusConnectionInterface>
#include "cloud.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    Cloud cloud;

    QDBusConnection connection = QDBusConnection::sessionBus();

    if (!connection.isConnected()) {
        return 1;
    }

    if (connection.interface()->isServiceRegistered("local.automotive.CloudService2")) {
        qWarning() << "Service already running! Replacing old instance...";
    }

    if (!connection.registerService("local.automotive.CloudService2")) {
        qCritical() << "Failed to register service:" << connection.lastError().message();
        return 1;
    }

    if (!connection.registerObject("/", &cloud, QDBusConnection::ExportChildObjects | QDBusConnection::ExportAllSlots | QDBusConnection::ExportAllSignals)) {
        qCritical() << "Failed to register object:" << connection.lastError().message();
        return 1;
    }

    qDebug() << "=========================================";
    qDebug() << "🚀 Automotive Cloud Service is Active";
    qDebug() << "📡 Path: /";
    qDebug() << "📡 Interface: local.automotive.Cloud";
    qDebug() << "💻 PID:" << app.applicationPid();
    qDebug() << "=========================================";

    return app.exec();
}