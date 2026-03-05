#include "controller.h"
#include <QDebug>

Controller::Controller(QObject *parent) : QObject(parent)
{
    m_interface = new QDBusInterface(
        "com.raspberry.cloud",
        "/",
        "com.raspberry.network",
        QDBusConnection::sessionBus(),
        this
    );
}

Controller::~Controller() {
    if (m_interface) {
        delete m_interface;
        m_interface = nullptr;
    }
}

QString Controller::getIpAddress()
{
    if (m_interface->isValid()) {
        return m_interface->property("ipAddress").toString();
    }
    return QString();
}

QString Controller::getMacAddress()
{
    if (m_interface->isValid()) {
        return m_interface->property("macAddress").toString();
    }
    return QString();
}

void Controller::requestRestart()
{
    //< Is this really work ?! 
    if (m_interface->isValid()) {
        m_interface->call("requestRestart");
    }
}

void connectToWifi(const QString& ssid, const QString& pass) {
    //< To be implemented !!    
}