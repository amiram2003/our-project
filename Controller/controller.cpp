#include "controller.h"
#include <QDebug>
#include <QtDBus/QDBusReply>

Controller::Controller(QObject *parent) : QObject(parent)
{
    /**
     * Initialize the D-Bus interface for systemd-logind.
     * Service: org.freedesktop.login1
     * Path: /org/freedesktop/login1
     * Interface: org.freedesktop.login1.Manager
     * Bus: System Bus (Required for power management)
     */
    m_interface = new QDBusInterface(
        "org.freedesktop.login1",
        "/org/freedesktop/login1",
        "org.freedesktop.login1.Manager",
        QDBusConnection::systemBus(),
        this
    );
}

Controller::~Controller() {
    if (m_interface) {
        delete m_interface;
    }
}

void Controller::requestSystemReboot()
{
    if (m_interface && m_interface->isValid()) {
        qDebug() << "Sending D-Bus Reboot signal...";
        
        /**
         * Call the 'Reboot' method. 
         * The 'true' parameter allows for interactive authorization if necessary.
         */
        QDBusMessage reply = m_interface->call("Reboot", true);
        
        if (reply.type() == QDBusMessage::ErrorMessage) {
            qCritical() << "D-Bus Operation Failed:" << reply.errorMessage();
        }
    } else {
        qCritical() << "Interface Invalid:" << m_interface->lastError().message();
    }
}