#include "vehicledata.h"

VehicleData::VehicleData(QObject *parent) : QObject(parent) {
  
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [this]() {
        static double s = 0;
        s = (s > 240) ? 0 : s + 1;
        updateSensorData(s, s * 30); 
    });
    timer->start(50);
}

void VehicleData::updateSensorData(double newSpeed, double newRpm) {
    if (m_speed != newSpeed) {
        m_speed = newSpeed;
        emit speedChanged();
    }
    if (m_rpm != newRpm) {
        m_rpm = newRpm;
        emit rpmChanged();
    }
}