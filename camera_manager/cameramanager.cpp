#include "cameramanager.h"
#include <QDebug>
#include <opencv2/opencv.hpp>

CameraManager::CameraManager(QObject *parent) : QObject(parent)
{
}

void CameraManager::onCaptureRequested()
{
    qDebug() << "CameraManager: Received CaptureIntruder signal! Taking photo...";
    capture();
}

void CameraManager::capture()
{
    cv::VideoCapture cap(0, cv::CAP_V4L2);
    if (!cap.isOpened()) {
        qCritical() << "CameraManager: Could not open camera!";
        return;
    }

    cv::Mat frame;
    cap >> frame;
    if (!frame.empty()) {
        // بنسيف الصورة في مكان ثابت عشان الـ UI يقدر يقرأها ويعرضها بعدين
        cv::imwrite("/tmp/intruder.jpg", frame);
        qDebug() << "CameraManager: Intruder image captured and saved to /tmp/intruder.jpg";
    }
    cap.release();
}