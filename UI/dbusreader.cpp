#include "dbusreader.h"
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
    QDBusConnection::sessionBus().connect(
        "local.automotive.CloudService",
        "/",
        "local.automotive.Cloud",
        "sigRebootRequested", // نفس الاسم اللي في كود جهاد
        this,
        SLOT(handleReboot())
    );
}

void DBusReader::requestReboot() {
    qDebug() << "UI: Sending reboot request to CloudService...";
    QDBusMessage msg = QDBusMessage::createMethodCall(
        "local.automotive.CloudService",
        "/",
        "local.automotive.Cloud",
        "requestReboot" // نفس اسم الـ Slot اللي عند جهاد
    );
    QDBusConnection::sessionBus().send(msg);
}

void DBusReader::handleReboot() {
    qDebug() << "Cloud accepted reboot! Executing Hasnaa's controller...";
    
    // تأكدي إن المسار ده هو اللي فيه ملف حسناء بعد الـ make
    QString program = "/home/pi/our-project/build/SystemController";
    
    // بنشغل الملف ونسيبه يكمل مهمته (الريبوت)
    QProcess::startDetached(program); 
    
    emit rebootStarted();
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