#include "canhandler.h"
#include <QDebug>
#include <QtGlobal>

CanHandler::CanHandler(QObject *parent) : QObject(parent) {}

CanHandler::~CanHandler() {
    disconnectDevice();
}

bool CanHandler::connectToDevice(const QString &interfaceName) {
    QString errorString;

    m_canDevice = QCanBus::instance()->createDevice(QStringLiteral("socketcan"), interfaceName, &errorString);

    if (!m_canDevice) {
        qDebug() << "Error creating device:" << errorString;
        return false;
    }


    connect(m_canDevice, &QCanBusDevice::framesReceived, this, &CanHandler::readPendingFrames);
    connect(m_canDevice, &QCanBusDevice::errorOccurred, this, &CanHandler::handleError);

    if (!m_canDevice->connectDevice()) {
        qDebug() << "Connection error:" << m_canDevice->errorString();
        delete m_canDevice;
        m_canDevice = nullptr;
        return false;
    }

    qDebug() << "Connected to" << interfaceName;
    return true;
}

void CanHandler::disconnectDevice() {
    if (m_canDevice) {
        m_canDevice->disconnectDevice();
        delete m_canDevice;
        m_canDevice = nullptr;
    }
}

void CanHandler::sendMessage(uint32_t id, const QByteArray &data) {
    if (!m_canDevice) return;

    QCanBusFrame frame;
    frame.setFrameId(id);
    frame.setPayload(data);
    
    if (m_canDevice->writeFrame(frame)) {
       qDebug() << "Message sent ID:" << Qt::hex << id;
    } else {
        qDebug() << "Failed to send message";
    }
}

void CanHandler::readPendingFrames() {
    while (m_canDevice->framesAvailable() > 0) {
        QCanBusFrame frame = m_canDevice->readFrame();
       qDebug() << "Received ID:" << Qt::hex << frame.frameId() << "Data:" << frame.payload().toHex();
        emit messageReceived(frame);
    }
}

void CanHandler::handleError(QCanBusDevice::CanBusError error) {
    qDebug() << "CAN Error:" << error << m_canDevice->errorString();
}