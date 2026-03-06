#ifndef VEHICLEDATA_H
#define VEHICLEDATA_H

#include <QObject>
#include <QTimer>

class VehicleData : public QObject {
    Q_OBJECT
    
    Q_PROPERTY(double speed READ speed NOTIFY speedChanged)
    Q_PROPERTY(double rpm READ rpm NOTIFY rpmChanged)

public:
    explicit VehicleData(QObject *parent = nullptr);

    double speed() const { return m_speed; }
    double rpm() const { return m_rpm; }

signals:
    void speedChanged();
    void rpmChanged();

public slots:
    
    void updateSensorData(double newSpeed, double newRpm);

private:
    double m_speed = 0;
    double m_rpm = 0;
};

#endif // VEHICLEDATA_H