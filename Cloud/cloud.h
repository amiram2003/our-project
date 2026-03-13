#ifndef CLOUD_H
#define CLOUD_H

#include <QObject>
#include <QString>

class Cloud : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "local.automotive.Cloud")

public:
    explicit Cloud(QObject *parent = nullptr);

public slots:
    QString getIP();
    QString getMacAddress();
    QString getFrequencyBands();

    void connectToWifi(const QString& ssid, const QString& pass);

signals:
    void sigWifiIPChanged(const QString &newIp);

private:
    QString m_ip;
    QString m_mac; 
};

#endif