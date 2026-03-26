#include "dbusreader.h"
#include <QDebug>
#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusMessage>

DBusReader::DBusReader(QObject *parent) : QObject(parent) {
    QDBusConnection::sessionBus().connect(
        "local.automotive.CloudService2",
        "/",
        "local.automotive.Cloud",
        "sigWifiIPChanged",
        this,
        SLOT(handleWifiIPChanged(QString))
    );
}

void DBusReader::handleWifiIPChanged(const QString &newIp) {
    qDebug() << "UI Received IP:" << newIp;
    if (m_wifiIP != newIp) {
        m_wifiIP = newIp;
        emit wifiIPChanged();
    }
}

void DBusReader::connectToWifi(const QString &ssid, const QString &password) {
    qDebug() << "UI: Connecting to SSID:" << ssid;
    
    QDBusMessage msg = QDBusMessage::createMethodCall(
        "local.automotive.CloudService2",
        "/",
        "local.automotive.Cloud",
        "connectToWifi"
    );

    msg << ssid << password;
    QDBusConnection::sessionBus().send(msg);
}

void DBusReader::scanAvailableWifi() {
    qDebug() << "UI: Requesting Wifi Scan...";
    
    QDBusMessage msg = QDBusMessage::createMethodCall(
        "local.automotive.CloudService2",
        "/",
        "local.automotive.Cloud",
        "getAvailableWifi" 
    );

    QDBusMessage reply = QDBusConnection::sessionBus().call(msg);

    if (reply.type() != QDBusMessage::ErrorMessage) {
        QString jsonString = reply.arguments().at(0).toString();
        
        QJsonDocument doc = QJsonDocument::fromJson(jsonString.toUtf8());
        QJsonArray jsonArray = doc.array();
        
        m_wifiList.clear(); 
        for (const QJsonValue &value : jsonArray) {
            m_wifiList.append(value.toVariant()); 
        }

        emit wifiListChanged();
        qDebug() << "UI: Scan completed. Found" << m_wifiList.size() << "networks.";
    } else {
        qCritical() << "UI: D-Bus Scan Error:" << reply.errorMessage();
    }
}

void DBusReader::enableDisableWifi(bool enable) {
    qDebug() << "UI: Wifi Enable/Disable set to:" << enable;
    
    QDBusMessage msg = QDBusMessage::createMethodCall(
        "local.automotive.CloudService2",
        "/",
        "local.automotive.Cloud",
        "enableDisable"
    );

    msg << enable;
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