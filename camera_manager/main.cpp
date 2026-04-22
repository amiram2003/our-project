#include <QCoreApplication>
#include <QDebug>
#include <QtDBus/QDBusConnection>
#include "cameramanager.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    CameraManager manager;

    QDBusConnection bus = QDBusConnection::sessionBus();
    if (!bus.isConnected()) {
        qCritical() << "CameraManager: Cannot connect to D-Bus";
        return 1;
    }

   bool connected = bus.connect(
    "com.project.system",             // 1. Service Name
    "/Controller",                    // 2. Object Path
    "com.project.system.Controller",  // 3. Interface Name
    "CaptureIntruder",                // 4. Signal Name
    &manager, 
    SLOT(onCaptureRequested())
);

    if (!connected) {
        qCritical() << "CameraManager: Failed to connect to 'CaptureIntruder' signal";
    }

    qDebug() << "Camera Service is ready. Waiting for CaptureIntruder signals...";
    return app.exec();
}