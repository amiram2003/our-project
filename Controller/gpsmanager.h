#ifndef GPSMANAGER_H
#define GPSMANAGER_H

#include <QObject>
#include <QSerialPort>

class GPSManager : public QObject
{
    Q_OBJECT
public:
    explicit GPSManager(QObject *parent = nullptr);
    bool openPort(const QString &portName);

signals:
// ببعت الاشاره دي لما بلاقي احداثيات جديدة
    void locationUpdated(double latitude, double longitude);

private slots:
    void readData();

private:
    QSerialPort *m_serial;
};

#endif