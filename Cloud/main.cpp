#include <QCoreApplication>
#include <QDBusConnection>
#include "cloud.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    Cloud cloud;
    //< Create separate socket for DBus - do not use session bus !! 
    QDBusConnection connection = QDBusConnection::sessionBus();
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
