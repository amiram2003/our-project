#pragma once

#include <QObject>
#include <QTimer>
#include <sys/socket.h>
#include <linux/can.h>
#include <linux/can/raw.h>

class VibrationSimulator : public QObject
{
    Q_OBJECT

public:
    explicit VibrationSimulator(QObject *parent = nullptr);
    ~VibrationSimulator();

    bool initCanInterface(const QString &interfaceName);

public slots:
    void sendVibrationFrame();

private:
    int m_canSocket = -1;
};