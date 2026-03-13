#ifndef DBUSREADER_H
#define DBUSREADER_H

#include <QObject>
#include <QString>

class DBusReader : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString wifiIP READ wifiIP NOTIFY wifiIPChanged)

public:
    explicit DBusReader(QObject *parent = nullptr);
    QString wifiIP() const { return m_wifiIP; }

public slots:
    void handleWifiIPChanged(const QString &newIp);

signals:
    void wifiIPChanged();

private:
    QString m_wifiIP = "Disconnected";
};

#endif