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

    if (connection.interface()->isServiceRegistered("local.automotive.CloudService")) {
        qWarning() << "Service already running!";
    }

    if (!connection.registerService("local.automotive.CloudService")) {
        return 1;
    }

    if (!connection.registerObject("/", &cloud, QDBusConnection::ExportAllContents)) {
        return 1;
    }

    qDebug() << "=========================================";
    qDebug() << "🚀 Automotive Cloud Service is Active";
    qDebug() << "📡 Interface: local.automotive.Cloud";
    qDebug() << "💻 PID:" << app.applicationPid();
    qDebug() << "=========================================";

    return app.exec();
}