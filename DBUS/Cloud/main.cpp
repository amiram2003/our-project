#include <QCoreApplication>
#include <QDBusConnection>
#include "Cloud.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    Cloud cloud;

    QDBusConnection connection = QDBusConnection::systemBus();
    connection.registerService("com.raspberry.cloud");
    connection.registerObject(
        "/",
        &cloud,
        QDBusConnection::ExportAllSlots |
        QDBusConnection::ExportAllSignals |
        QDBusConnection::ExportAllProperties
    );

    return app.exec();
}
