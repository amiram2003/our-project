#ifndef CANHANDLER_H
#define CANHANDLER_H

#include <QObject>
#include <QCanBus>
#include <QCanBusDevice>
#include <QCanBusFrame>
#include <QDebug>

class CanHandler : public QObject
{
    Q_OBJECT
public:
    explicit CanHandler(QObject *parent = nullptr);
    ~CanHandler();

    bool connectToDevice(const QString &interfaceName);
    void disconnectDevice();
    void sendMessage(uint32_t id, const QByteArray &data);

signals:
    void messageReceived(const QCanBusFrame &frame);

private slots:
    void readPendingFrames();
    void handleError(QCanBusDevice::CanBusError error);

private:
    QCanBusDevice *m_canDevice = nullptr;
};

#endif // CANHANDLER_H