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
    void requestReboot();

signals:
    void wifiIPChanged();

private:
    QString m_wifiIP; 
};

#endif