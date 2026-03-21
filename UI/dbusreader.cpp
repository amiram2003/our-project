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
    qDebug() << "UI: Reboot requested. Calling SystemController directly...";

    // المسار ده لازم يكون هو المسار الحقيقي لملف حسناء بعد ما تعمليله build
    QString program = "/home/pi/our-project/Controller/build/SystemController";

    // السطر ده هو اللي بيقوم ملف حسناء "يخطف" ريستارت ويقفل
    bool success = QProcess::startDetached(program);

    if (success) {
        qDebug() << "UI: SystemController started successfully.";
    } else {
        qCritical() << "UI: Could not start SystemController. Check the path!";
    }
}