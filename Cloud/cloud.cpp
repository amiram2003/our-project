#include "cloud.h"
#include <QNetworkInterface>
#include <QProcess>
#include <QDebug>

Cloud::Cloud(QObject *parent) : QObject(parent)
{
    m_mac = getMacAddress(); 
}

QString Cloud::getInterfaceIP(const QString &interfaceName)
{
    const auto interfaces = QNetworkInterface::allInterfaces();
    for (const QNetworkInterface &iface : interfaces) {
        if (iface.name().contains(interfaceName) && iface.flags().testFlag(QNetworkInterface::IsUp)) {
            const auto entries = iface.addressEntries();
            for (const QNetworkAddressEntry &entry : entries) {
                if (entry.ip().protocol() == QAbstractSocket::IPv4Protocol && !entry.ip().isLoopback()) {
                    return entry.ip().toString();
                }
            }
        }
    }
    return "";
}

QString Cloud::getIP() {
    QString currentIp = getInterfaceIP("wlan");
    if (currentIp != m_ip) {
        m_ip = currentIp;
        emit sigWifiIPChanged(m_ip);
    }
    return m_ip;
}

QString Cloud::getEthernetIP() {
    QString currentIp = getInterfaceIP("eth");
    if (currentIp != m_ethIp) {
        m_ethIp = currentIp;
        emit sigEthernetIPChanged(m_ethIp);
    }
    return m_ethIp;
}

QString Cloud::getMacAddress()
{
    if (m_mac.isEmpty() || m_mac == "00:00:00:00:00:00") {
        for (const QNetworkInterface &iface : QNetworkInterface::allInterfaces()) {
            if (iface.name().contains("wlan") || iface.name().contains("eth")) {
                m_mac = iface.hardwareAddress();
                if (!m_mac.isEmpty()) break;
            }
        }
    }
    return m_mac.isEmpty() ? "00:00:00:00:00:00" : m_mac;
}

double Cloud::getFrequencyBands()
{
    QProcess proc;
    proc.start("iwgetid", QStringList() << "-f"); 
    if (!proc.waitForFinished()) return 0.0;
    
    QString output = QString(proc.readAllStandardOutput()).trimmed();
    
    if (output.contains("5.")) return 5.0;
    if (output.contains("2.4")) return 2.4;
    
    return 0.0;
}

void Cloud::connectToWifi(const QString& ssid, const QString& pass)
{
    QString command = QString("sudo nmcli dev wifi connect '%1' password '%2'").arg(ssid, pass);
    QProcess *proc = new QProcess(this);
    
    connect(proc, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(onConnectionFinished()));
    
    proc->start("sh", QStringList() << "-c" << command);
}

void Cloud::onConnectionFinished()
{
    this->getIP();
    sender()->deleteLater();
}