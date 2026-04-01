#ifndef CAMERAMANAGER_H
#define CAMERAMANAGER_H

#include <QObject>
#include <opencv2/opencv.hpp>

class CameraManager : public QObject
{
    Q_OBJECT
public:
    explicit CameraManager(QObject *parent = nullptr);

public slots:
    void onFingerprintReceived(int status);

private:
    void capture();
};

#endif