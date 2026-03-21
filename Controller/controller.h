#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QtDBus/QDBusInterface>

class Controller : public QObject
{
    Q_OBJECT

public:
    explicit Controller(QObject *parent = nullptr);
    ~Controller();

    /**
     * @brief Sends a Reboot request to the system-level D-Bus manager.
     * Targets the org.freedesktop.login1 interface.
     */
    void requestSystemReboot();

private:
    QDBusInterface *m_interface;
};

#endif