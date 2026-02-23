#include <QCoreApplication>
#include <QDBusInterface>
#include <QDBusReply>
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    QDBusInterface iface(
        "com.raspberry.cloud",
        "/",
        "com.raspberry.network",
        QDBusConnection::systemBus()
    );

    if (!iface.isValid())
        return -1;

    QString ip = iface.property("ipAddress").toString();
    QString mac = iface.property("macAddress").toString();

    qInfo() << ip;
    qInfo() << mac;

    iface.call("requestRestart");

    return app.exec();
}
