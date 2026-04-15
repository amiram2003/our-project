#include "controller.h"
#include <QDebug>
#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusMessage>

// مكتبات Linux CAN
#include <linux/can.h>
#include <linux/can/raw.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <unistd.h>
#include <cstring>

Controller::Controller(QObject *parent) : QObject(parent), m_canSocket(-1), m_canNotifier(nullptr)
{

    m_systemdInterface = new QDBusInterface(
        "org.freedesktop.login1",
        "/org/freedesktop/login1",
        "org.freedesktop.login1.Manager",
        QDBusConnection::systemBus(),
        this
    );
      // بنفتح الـ vcan0 أول ما الكنترولر يبدأ
      setupCanInterface("vcan0");
}

Controller::~Controller() {
    if (m_canSocket != -1) ::close(m_canSocket);
    delete m_systemdInterface;
}

bool Controller::setupCanInterface(const QString &ifaceName)
{
    m_canSocket = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if (m_canSocket < 0) return false;

    struct ifreq ifr;
    std::strncpy(ifr.ifr_name, ifaceName.toStdString().c_str(), IFNAMSIZ);
    ioctl(m_canSocket, SIOCGIFINDEX, &ifr);

    struct sockaddr_can addr;
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    if (bind(m_canSocket, (struct sockaddr *)&addr, sizeof(addr)) < 0) return false;

    // بنربط السوكت بـ QSocketNotifier عشان ينبهنا لما داتا تيجي
    m_canNotifier = new QSocketNotifier(m_canSocket, QSocketNotifier::Read, this);
    connect(m_canNotifier, &QSocketNotifier::activated, this, &Controller::readCanFrame);

    qDebug() << "Controller: Listening on" << ifaceName;
    return true;
}

void Controller::readCanFrame()
{
    struct can_frame frame;
    if (read(m_canSocket, &frame, sizeof(struct can_frame)) > 0) {
        // لو الـ ID هو اللي باعتة السكربت (0x123)
        if (frame.can_id == 0x123) {
            int status = frame.data[0];
            qDebug() << "Controller: Received Fingerprint Status via CAN:" << status;

            // هنا بقى بنبعت الخبر للـ DBusReader اللي في الـ UI
            QDBusMessage msg = QDBusMessage::createSignal("/Controller", "com.project.system.Controller", "FingerprintUpdate");
            msg << status;
            QDBusConnection::sessionBus().send(msg);
        }
    }
}

void Controller::requestSystemReboot()
{
    qDebug() << "Controller: Received Reboot request from UI via D-Bus...";
    
    if (m_systemdInterface && m_systemdInterface->isValid()) {
        m_systemdInterface->call("Reboot", true);
    } else {
        qCritical() << "Controller: Cannot reach systemd-logind";
    }
}