#include <QCoreApplication>
#include "canmock.h"
#include <linux/can.h>
#include <linux/can/raw.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <unistd.h>
#include <iostream>
#include <cstring>

CanMock::CanMock(QObject *parent) 
    : QObject(parent), m_socket(-1), m_notifier(nullptr), m_input(stdin)
{
}

bool CanMock::initialize(const QString &interfaceName)
{
    m_socket = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if (m_socket < 0) return false;

    struct ifreq ifr;
    std::strncpy(ifr.ifr_name, interfaceName.toStdString().c_str(), IFNAMSIZ);
    ioctl(m_socket, SIOCGIFINDEX, &ifr);

    struct sockaddr_can addr;
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    if (bind(m_socket, (struct sockaddr *)&addr, sizeof(addr)) < 0) return false;

    m_notifier = new QSocketNotifier(STDIN_FILENO, QSocketNotifier::Read, this);
    connect(m_notifier, &QSocketNotifier::activated, this, &CanMock::onUserInput);

    std::cout << "--- Qt CAN Mock Tool Ready ---" << std::endl;
    std::cout << "Enter '0' (Fail), '1' (Success), or 'q' to quit" << std::endl;
    return true;
}

void CanMock::onUserInput()
{
    QString line = m_input.readLine();
    if (line == "0") sendFrame(0x123, 0);
    else if (line == "1") sendFrame(0x123, 1);
    else if (line == "q") qApp->quit();
    
    std::cout << "Enter command: " << std::flush;
}

void CanMock::sendFrame(int id, int status)
{
    struct can_frame frame;
    frame.can_id = id;
    frame.can_dlc = 1;
    frame.data[0] = static_cast<__u8>(status);

    if (write(m_socket, &frame, sizeof(struct can_frame)) == sizeof(struct can_frame)) {
        std::cout << "[Mock] Sent ID: 0x" << std::hex << id 
                  << " | Status: " << std::dec << status << std::endl;
    }
}