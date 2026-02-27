#include "cloud.h"
#include <QNetworkInterface>
#include <QProcess>

Cloud::Cloud(QObject *parent)
    : QObject(parent)
{
    updateNetworkData();
}

QString Cloud::ipAddress() const
{
    return m_ip;
}

QString Cloud::macAddress() const
{
    return m_mac;
}

void Cloud::updateNetworkData()
{
    const auto interfaces = QNetworkInterface::allInterfaces();
    for (const QNetworkInterface &iface : interfaces) {
        if (iface.flags().testFlag(QNetworkInterface::IsUp) &&
            !iface.flags().testFlag(QNetworkInterface::IsLoopBack)) {

            m_mac = iface.hardwareAddress();

            const auto entries = iface.addressEntries();
            for (const QNetworkAddressEntry &entry : entries) {
                if (entry.ip().protocol() == QAbstractSocket::IPv4Protocol) {
                    m_ip = entry.ip().toString();
                    emit dataChanged(m_ip);
                    return;
                }
            }
        }
    }
}

void Cloud::requestRestart()
{
    QProcess::startDetached("/sbin/reboot");
}
