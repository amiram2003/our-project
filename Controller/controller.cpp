#include "controller.h"
#include <cmath>
#include <QDebug>
#include <QDateTime>
#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusMessage>

// مكتبات Linux CAN
#include <fcntl.h>
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

      m_gps = new GPSManager(this);
      m_gps->openPort("serial0"); 
      connect(m_gps, &GPSManager::locationUpdated, this, &Controller::onLocationUpdated);
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
    if (ioctl(m_canSocket, SIOCGIFINDEX, &ifr) < 0) return false;

    struct sockaddr_can addr;
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    if (bind(m_canSocket, (struct sockaddr *)&addr, sizeof(addr)) < 0) return false;

    // --- التعديل المهم هنا ---
    // بنخلي السوكت Non-blocking عشان الـ read ميعملش Freezing للبرنامج
    int flags = fcntl(m_canSocket, F_GETFL, 0);
    fcntl(m_canSocket, F_SETFL, flags | O_NONBLOCK);

    m_canNotifier = new QSocketNotifier(m_canSocket, QSocketNotifier::Read, this);
    connect(m_canNotifier, &QSocketNotifier::activated, this, &Controller::readCanFrame);
    
    qDebug() << "Controller: Successfully bound to" << ifaceName << "and monitoring...";
    return true;
}
void Controller::readCanFrame()
{
    struct can_frame frame;
    // بنقرأ الداتا ونشوف رجع لنا كام Byte
    int nbytes = read(m_canSocket, &frame, sizeof(struct can_frame));
    
    if (nbytes > 0) {
        // السطر ده هو "الكشاف" بتاعنا.. هيطبع أي ID يمر على الـ Bus
        qDebug() << "CAN Frame Received! ID: 0x" << QString::number(frame.can_id, 16);

        if (frame.can_id == 0x123) {
            int status = frame.data[0];
            qDebug() << "Controller: Fingerprint Status Match! Status:" << status;

            // إرسال إشارة الـ UI
            QDBusMessage msg = QDBusMessage::createSignal("/Controller", "com.project.system.Controller", "FingerprintUpdate");
            msg << status;
            QDBusConnection::sessionBus().send(msg);

            // إرسال إشارة الكاميرا لو status == 0
            if (status == 0) {
                QDBusMessage cameraMsg = QDBusMessage::createSignal("/Controller", "com.project.system.Controller", "CaptureIntruder");
                QDBusConnection::sessionBus().send(cameraMsg);
            }
        }

            else if (frame.can_id == 0x456) {
                 qDebug() << "New Pothole Detected! Logging to SD Card...";

                 QFile file("potholes_log.csv");
                 if (file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
                      QTextStream stream(&file);
                 if (file.size() == 0) {
                      stream << "Timestamp,Latitude,Longitude\n";
                 }
                 QString currentTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
                  stream << currentTime << "," 
                  << QString::number(m_currentLat, 'f', 6) << "," 
                  << QString::number(m_currentLon, 'f', 6) << "\n";
                file.close();
                qDebug() << "Logged successfully.";
            }
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

void Controller::onLocationUpdated(double lat, double lon)
{
    m_currentLat = lat;
    m_currentLon = lon;

    
    QDBusMessage gpsMsg = QDBusMessage::createSignal("/Controller", "com.project.system.Controller", "GPSLocationChanged");
    gpsMsg << lat << lon;
    QDBusConnection::sessionBus().send(gpsMsg);

    // 2. ثانياً: مقارنة الموقع الحالي بالمطبات المخزنة
    QFile file("potholes_log.csv");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream stream(&file);
        QString header = stream.readLine(); // تخطي السطر الأول (العناوين)

        while (!stream.atEnd()) {
            QString line = stream.readLine();
            QStringList fields = line.split(",");
            if (fields.size() >= 3) {
                double savedLat = fields[1].toDouble();
                double savedLon = fields[2].toDouble();

                // حساب المسافة بالمتر
                double distance = calculateDistance(m_currentLat, m_currentLon, savedLat, savedLon);

                // لو المسافة أقل من أو تساوي 50 متر
                if (distance <= 50.0) {
                    qDebug() << "Warning! Saved pothole is close! Distance:" << distance << "meters";

                    QDBusMessage warningMsg = QDBusMessage::createSignal("/Controller", "com.project.system.Controller", "PotholeAlert");
                    warningMsg << distance; 
                    QDBusConnection::sessionBus().send(warningMsg);
                    
                    break; // إيقاف اللوب لمنع تكرار التنبيه في نفس اللحظة
                }
            }
        }
        file.close();
    }
}

double Controller::calculateDistance(double lat1, double lon1, double lat2, double lon2)
{
    double r = 6371000; // نصف قطر الأرض بالمتر
    double dLat = (lat2 - lat1) * M_PI / 180.0;
    double dLon = (lon2 - lon1) * M_PI / 180.0;
    
    double a = std::sin(dLat / 2) * std::sin(dLat / 2) +
               std::cos(lat1 * M_PI / 180.0) * std::cos(lat2 * M_PI / 180.0) *
               std::sin(dLon / 2) * std::sin(dLon / 2);
    double c = 2 * std::atan2(std::sqrt(a), std::sqrt(1 - a));
    
    return r * c;
}