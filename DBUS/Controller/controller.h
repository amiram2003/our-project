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
    QString getIpAddress();
    QString getMacAddress();
    void requestRestart();

private:
    QDBusInterface *m_interface;
};

#endif