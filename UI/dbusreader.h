#ifndef DBUSREADER_H
#define DBUSREADER_H

#include <QObject>
#include <QString>
#include <QtDBus/QDBusMessage>
#include <QtDBus/QDBusConnection>

class DBusReader : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString wifiIP READ wifiIP NOTIFY wifiIPChanged)

public:
    explicit DBusReader(QObject *parent = nullptr);
    QString wifiIP() const { return m_wifiIP; }

public slots:
    void handleWifiIPChanged(const QString &newIp);
    void connectToWifi(const QString &username, const QString &password);
    
    // الزيادة هنا:
    void requestReboot();      // دي اللي هتناديها لما تضغطي على أيقونة الـ UI
    void handleReboot();       // دي اللي هتتنفذ لما جهاد تبعت الإشارة وتشغل حسناء

signals:
    void wifiIPChanged();
    void rebootStarted();      // إشارة اختيارية لو عايزة تظهري "جاري إعادة التشغيل"

private:
    QString m_wifiIP; 
};

#endif