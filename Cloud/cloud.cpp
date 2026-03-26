#include <QTimer>
#include "cloud.h"
#include <QNetworkInterface>
#include <QProcess>
#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

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
    return ""; // هنا بيرجع نص فاضي لو مفيش IP زي ما طلبتي
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
    QProcess::execute("nmcli connection delete \"" + ssid + "\"");
    QString command = QString("sudo nmcli dev wifi connect '%1' password '%2'").arg(ssid, pass);
    QProcess::startDetached("sh", QStringList() << "-c" << command);
}

void Cloud::onConnectionFinished()
{
    this->getIP();
    sender()->deleteLater();
}

// --- الدوال الجديدة المضافة ---

void Cloud::enableDisable(bool enable) {
    QString state = enable ? "on" : "off";
    QProcess::startDetached("nmcli", QStringList() << "radio" << "wifi" << state);
}

QString Cloud::getAvailableWifi() {
    QProcess proc;
    proc.start("nmcli", QStringList() << "-t" << "-f" << "SSID,SIGNAL,SECURITY" << "dev" << "wifi" << "list" << "--rescan" << "yes");
    
    if (!proc.waitForFinished()) return "[]";

    QString output = QString::fromUtf8(proc.readAllStandardOutput());
    QStringList lines = output.split('\n', Qt::SkipEmptyParts);

    QJsonArray wifiList;
    for (const QString &line : lines) {
        QStringList parts = line.split(':'); 
        if (parts.size() >= 2) {
            if (parts[0].isEmpty()) continue; 
            
            QJsonObject wifiObj;
            wifiObj["ssid"] = parts[0];
            wifiObj["signal"] = parts[1];
            wifiObj["security"] = (parts.size() > 2) ? parts[2] : "Open";
            wifiList.append(wifiObj);
        }
    }

    QJsonDocument doc(wifiList);
    return QString::fromUtf8(doc.toJson(QJsonDocument::Compact));
}