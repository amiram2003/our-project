#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QtDBus/QDBusInterface>
#include <QtDBus/QDBusContext>
#include <QSocketNotifier> // مكتبة مراقبة الـ Sockets

class Controller : public QObject, protected QDBusContext
{
    Q_OBJECT
    
    Q_CLASSINFO("D-Bus Interface", "com.project.system.Controller")

public:
    explicit Controller(QObject *parent = nullptr);
    ~Controller();
    bool setupCanInterface(const QString &ifaceName);

public slots:  
    void requestSystemReboot();
private slots:
    void readCanFrame(); // الفانكشن اللي هتقرأ من vcan0
signals:
    void CaptureIntruder(); 
    void FingerprintUpdate(int status);

private:
    QDBusInterface *m_systemdInterface;
    int m_canSocket;
    QSocketNotifier *m_canNotifier;
};

#endif