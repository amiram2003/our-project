#include "dbusreader.h"
#include <QDebug>
#include <QProcess>

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
    qDebug() << "UI: Sending Reboot signal via systemctl...";

    QString program = "systemctl";
    QStringList arguments;
    arguments << "start" << "reboot-trigger.service";

    bool success = QProcess::startDetached(program, arguments);

    if (success) {
        qDebug() << "UI: Reboot signal sent to systemd.";
    } else {
        qCritical() << "UI: Failed to trigger reboot-trigger.service.";
    }
}