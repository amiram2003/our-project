#include "dbusreader.h"
#include <QDBusConnection>
#include <QDebug>

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
    qDebug() << "--- Signal Received! New IP is:" << newIp << " ---";
    if (m_wifiIP != newIp) {
        m_wifiIP = newIp;
        emit wifiIPChanged();
    }
}