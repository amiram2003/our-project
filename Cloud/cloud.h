#ifndef CLOUD_H
#define CLOUD_H

#include <QObject>
#include <QString>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

class Cloud : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "local.automotive.Cloud")

public:
    explicit Cloud(QObject *parent = nullptr);

public slots:
    QString getIP();               
    QString getEthernetIP();       
    QString getMacAddress();
    double getFrequencyBands();    
    void connectToWifi(const QString& ssid, const QString& pass);
    
    
    QString getAvailableWifi(); 
    void enableDisable(bool enable); 

signals:
    void sigWifiIPChanged(const QString &newIp);
    void sigEthernetIPChanged(const QString &newIp); 


private slots:
    void onConnectionFinished();

private:
    QString getInterfaceIP(const QString &interfaceName);
    QString m_ip;
    QString m_ethIp;
    QString m_mac; 
};

#endif