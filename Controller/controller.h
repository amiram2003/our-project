#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QtDBus/QDBusInterface>
#include <QtDBus/QDBusContext>

class Controller : public QObject, protected QDBusContext
{
    Q_OBJECT
    
    Q_CLASSINFO("D-Bus Interface", "com.project.system.Controller")

public:
    explicit Controller(QObject *parent = nullptr);
    ~Controller();

public slots:  
    void requestSystemReboot();

private:
    QDBusInterface *m_systemdInterface;
};

#endif