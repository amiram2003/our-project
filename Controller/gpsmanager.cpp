#include "gpsmanager.h"
#include <QDebug>

GPSManager::GPSManager(QObject *parent) : QObject(parent), m_serial(new QSerialPort(this))
{
    connect(m_serial, &QSerialPort::readyRead, this, &GPSManager::readData);
}

bool GPSManager::openPort(const QString &portName)
{

    QString fullPath = "/dev/" + portName;
    
    m_serial->setPortName(fullPath);
    m_serial->setBaudRate(QSerialPort::Baud9600);
    m_serial->setDataBits(QSerialPort::Data8);
    m_serial->setParity(QSerialPort::NoParity);
    m_serial->setStopBits(QSerialPort::OneStop);
    m_serial->setFlowControl(QSerialPort::NoFlowControl);

    if (m_serial->open(QIODevice::ReadOnly)) {
        qDebug() << "GPS: Port opened successfully on" << fullPath;
        return true;
    }
    
    qDebug() << "GPS: Failed to open port" << fullPath;
    return false;
}

void GPSManager::readData()
{
    static QByteArray buffer;
    buffer.append(m_serial->readAll());

    while (buffer.contains('\n')) {
        int indexOfLineEnd = buffer.indexOf('\n');
        QByteArray lineData = buffer.left(indexOfLineEnd).trimmed();
        buffer.remove(0, indexOfLineEnd + 1);

        QString line = QString::fromLatin1(lineData);

        if (line.startsWith("$GPRMC")) {
            QStringList parts = line.split(",");
            if (parts.size() > 6 && parts[2] == "A") {
                
                double latRaw = parts[3].toDouble();
                double lonRaw = parts[5].toDouble();

            
                double lat = static_cast<int>(latRaw / 100) + ((latRaw - static_cast<int>(latRaw / 100) * 100) / 60.0);
                double lon = static_cast<int>(lonRaw / 100) + ((lonRaw - static_cast<int>(lonRaw / 100) * 100) / 60.0);

                qDebug() << "=== GPS CAPTURED == -> Lat:" << lat << "Lon:" << lon;

                emit locationUpdated(lat, lon);
            }
        }
    }
}