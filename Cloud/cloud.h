#ifndef CLOUD_H
#define CLOUD_H

#include <QObject>

class Cloud : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString ipAddress READ ipAddress)
    Q_PROPERTY(QString macAddress READ macAddress)

public:
    explicit Cloud(QObject *parent = nullptr);

    QString ipAddress() const;
    QString macAddress() const;

public slots:
    void requestRestart();

signals:
    void dataChanged(const QString &newIp);

private:
    QString m_ip;
    QString m_mac;

    void updateNetworkData();
};

#endif
