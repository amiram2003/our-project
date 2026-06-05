#include "vibrationsimulator.h"
#include <sys/ioctl.h>
#include <net/if.h>
#include <unistd.h>
#include <cstring>
#include <QDebug>

VibrationSimulator::VibrationSimulator(QObject *parent)
    : QObject(parent)
{
}

VibrationSimulator::~VibrationSimulator()
{
    if (m_canSocket != -1) {
        ::close(m_canSocket);
    }
}

bool VibrationSimulator::initCanInterface(const QString &interfaceName)
{
    struct sockaddr_can addr;
    struct ifreq ifr;

    m_canSocket = ::socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if (m_canSocket < 0) {
        qCritical() << "VibrationSimulator: Error opening socket";
        return false;
    }

    std::strncpy(ifr.ifr_name, interfaceName.toUtf8().constData(), IFNAMSIZ - 1);
    if (::ioctl(m_canSocket, SIOCGIFINDEX, &ifr) < 0) {
        qCritical() << "VibrationSimulator: Error getting interface index for" << interfaceName;
        ::close(m_canSocket);
        m_canSocket = -1;
        return false;
    }

    std::memset(&addr, 0, sizeof(addr));
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    if (::bind(m_canSocket, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        qCritical() << "VibrationSimulator: Error binding socket";
        ::close(m_canSocket);
        m_canSocket = -1;
        return false;
    }

    qInfo() << "VibrationSimulator: Successfully bound to" << interfaceName;
    return true;
}

void VibrationSimulator::sendVibrationFrame()
{
    if (m_canSocket == -1) {
        qWarning() << "VibrationSimulator: CAN socket not initialized!";
        return;
    }

    struct can_frame frame;
    frame.can_id = 0x456; // الـ ID المطلوب في الكنترولر عندك
    frame.can_dlc = 1;    // هنبعت Byte واحد
    frame.data[0] = 0xFF; // القيمة تعبر عن وجود اهتزاز (Vibration Detected)

    int nbytes = ::write(m_canSocket, &frame, sizeof(struct can_frame));
    if (nbytes == sizeof(struct can_frame)) {
        qInfo() << "VibrationSimulator: Sent CAN frame (0x456) -> Pothole Detected!";
    } else {
        qCritical() << "VibrationSimulator: Failed to send CAN frame";
    }
}