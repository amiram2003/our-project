#include "cameramanager.h"
#include <QDebug>
#include <opencv2/opencv.hpp>

CameraManager::CameraManager(QObject *parent) : QObject(parent)
{
}

void CameraManager::onFingerprintReceived(int status)
{
    if (status == 0) {
        qDebug() << "CameraManager: Unauthorized access! Capturing...";
        capture();
    }
}

void CameraManager::capture()
{
    cv::VideoCapture cap(0);
    if (!cap.isOpened()) {
        qCritical() << "CameraManager: Could not open camera!";
        return;
    }

    cv::Mat frame;
    cap >> frame;
    if (!frame.empty()) {
        cv::imwrite("/tmp/intruder.jpg", frame);
        qDebug() << "CameraManager: Image saved to /tmp/intruder.jpg";
    }
    cap.release();
}