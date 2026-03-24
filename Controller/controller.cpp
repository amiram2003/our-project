#include "controller.h"
#include <QDebug>
#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusMessage>

Controller::Controller(QObject *parent) : QObject(parent)
{

    m_systemdInterface = new QDBusInterface(
        "org.freedesktop.login1",
        "/org/freedesktop/login1",
        "org.freedesktop.login1.Manager",
        QDBusConnection::systemBus(),
        this
    );
}

Controller::~Controller() {
    delete m_systemdInterface;
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