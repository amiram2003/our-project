#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QtDBus/QDBusInterface>
#include <QtDBus/QDBusContext>
#include <QSocketNotifier> // مكتبة مراقبة الـ Sockets

#include "gpsmanager.h" 
#include <QFile>
#include <QTextStream>

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
    double calculateDistance(double lat1, double lon1, double lat2, double lon2);
    void onLocationUpdated(double lat, double lon);
signals:
    void CaptureIntruder(); 
    void FingerprintUpdate(int status);
    

    void PotholeAlert(double distance);
    //علشان اعرض خطوط الطول والعرض على ال UI
    void GPSLocationChanged(double lat, double lon);

private:
    QDBusInterface *m_systemdInterface;
    int m_canSocket;
    QSocketNotifier *m_canNotifier;
    GPSManager *m_gps;
    double m_currentLat = 0.0;
    double m_currentLon = 0.0;
};

#endif