#ifndef DBUSREADER_H
#define DBUSREADER_H

#include <QObject>
#include <QString>
#include <QVariantList>
#include <QtDBus/QDBusMessage>
#include <QtDBus/QDBusConnection>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

class DBusReader : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString wifiIP READ wifiIP NOTIFY wifiIPChanged)
    Q_PROPERTY(QVariantList wifiList READ wifiList NOTIFY wifiListChanged)
    Q_PROPERTY(QString authStatus READ authStatus NOTIFY authStatusChanged)
    Q_PROPERTY(QString intruderImagePath READ intruderImagePath NOTIFY intruderImagePathChanged)

public:
    explicit DBusReader(QObject *parent = nullptr);
    
    QString wifiIP() const { return m_wifiIP; }
    QVariantList wifiList() const { return m_wifiList; }
    QString authStatus() const { return m_authStatus; }
    QString intruderImagePath() const { return m_intruderImagePath; }

public slots:
    void handleWifiIPChanged(const QString &newIp);
    void scanAvailableWifi(); 
    void enableDisableWifi(bool enable);
    void connectToWifi(const QString &ssid, const QString &password);
    void requestReboot();
    void handleFingerprintUpdate(int status);

signals:
    void wifiIPChanged();
    void wifiListChanged();
    void authStatusChanged();
    void triggerAIServiceRequested(bool active);
    void intruderImagePathChanged();

private:
    QString m_wifiIP;
    QVariantList m_wifiList;
    QString m_authStatus;
    QString m_intruderImagePath;
};

#endif