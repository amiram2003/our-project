#include "dbusreader.h"
#include <QDebug>
#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusMessage>

DBusReader::DBusReader(QObject *parent) : QObject(parent) {
    QDBusConnection::sessionBus().connect(
        "local.automotive.CloudService",
        "/",
        "local.automotive.Cloud",
        "sigWifiIPChanged",
        this,
        SLOT(handleWifiIPChanged(QString))
    );
}

void DBusReader::handleWifiIPChanged(const QString &newIp) {
    if (m_wifiIP != newIp) {
        m_wifiIP = newIp;
        emit wifiIPChanged();
    }
}

void DBusReader::connectToWifi(const QString &username, const QString &password) {
    QDBusMessage msg = QDBusMessage::createMethodCall(
        "local.automotive.CloudService",
        "/",
        "local.automotive.Cloud",
        "connectToWifi"
    );

    msg << username << password;
    QDBusConnection::sessionBus().send(msg);
}

void DBusReader::requestReboot() {
    qDebug() << "UI: Sending Reboot request to Controller via D-Bus...";

    QDBusMessage msg = QDBusMessage::createMethodCall(
        "com.project.system",
        "/Controller",
        "com.project.system.Controller",
        "requestSystemReboot"
    );

    QDBusMessage reply = QDBusConnection::sessionBus().call(msg);

    if (reply.type() == QDBusMessage::ErrorMessage) {
        qCritical() << "UI: D-Bus Error:" << reply.errorMessage();
    } else {
        qDebug() << "UI: Reboot request processed by Controller.";
    }
}