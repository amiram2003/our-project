
#include "cloud.h"
#include <QNetworkInterface>
#include <QProcess>
#include <QDebug>

Cloud::Cloud(QObject *parent) : QObject(parent)
{
    m_mac = getMacAddress(); 
}

QString Cloud::getIP()
{
    const auto interfaces = QNetworkInterface::allInterfaces();
    for (const QNetworkInterface &iface : interfaces) {
        if (iface.flags().testFlag(QNetworkInterface::IsUp) && 
            !iface.flags().testFlag(QNetworkInterface::IsLoopBack)) {
            const auto entries = iface.addressEntries();
            for (const QNetworkAddressEntry &entry : entries) {
                if (entry.ip().protocol() == QAbstractSocket::IPv4Protocol) {
                    QString currentIp = entry.ip().toString();
                    if (currentIp != m_ip) {
                        m_ip = currentIp;
                        emit sigWifiIPChanged(m_ip);
                    }
                    return m_ip;
                }
            }
        }
    }
    return "Disconnected";
}

QString Cloud::getMacAddress()
{
    if (m_mac.isEmpty() || m_mac == "00:00:00:00:00:00") {
        for (const QNetworkInterface &iface : QNetworkInterface::allInterfaces()) {
            if (iface.name().contains("wlan") || iface.name().contains("wifi")) {
                m_mac = iface.hardwareAddress();
                break;
            }
        }
    }
    return m_mac.isEmpty() ? "00:00:00:00:00:00" : m_mac;
}

QString Cloud::getFrequencyBands()
{
   QProcess proc;
   proc.start("iwgetid", QStringList() << "-f");
   if (!proc.waitForFinished()) return "Unknown";
   return QString(proc.readAllStandardOutput()).trimmed();
}

void Cloud::connectToWifi(const QString& ssid, const QString& pass)
{
    qDebug() << "Attempting to connect to:" << ssid;
    QString command = QString("sudo nmcli dev wifi connect '%1' password '%2' name '%1' ifname wlan0").arg(ssid, pass);
    qDebug() << "Executing command:" << command;
    QProcess::startDetached("sh", QStringList() << "-c" << command);
}