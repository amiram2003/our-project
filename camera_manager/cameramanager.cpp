#include "cameramanager.h"
#include <QDebug>
#include <opencv2/opencv.hpp>
#include <QFile>

CameraManager::CameraManager(QObject *parent) : QObject(parent)
{
}

void CameraManager::onCaptureRequested()
{
    qDebug() << "CameraManager: Received CaptureIntruder signal! Taking photo...";
    capture();
}

void CameraManager::capture() {
    qDebug() << "CameraManager: Capture triggered...";

    QString cmd = "rpicam-still -t 100 -o /tmp/intruder.jpg --width 640 --height 480 --immediate";
    
    int result = system(cmd.toStdString().c_str());

    if (result == 0) {
        QFile file("/tmp/intruder.jpg");
        if (file.exists() && file.size() > 0) {
            qDebug() << "✅ Photo saved to /tmp/intruder.jpg";
        } else {
            qCritical() << "❌ File not created or empty!";
        }
    } else {
        qCritical() << "❌ Command execution failed!";
    }
}