#include "slave.h"

SlaveManager::SlaveManager(QObject *parent) : QObject(parent) {
    serial = new QSerialPort(this);
}

SlaveManager::~SlaveManager() {
    if (serial->isOpen()) {
        serial->close();
    }
    delete serial;
    qDebug() << "Slave Serial Port Closed and Memory Cleared.";
}

bool SlaveManager::listenOnPort(QString name) {
    serial->setPortName(name);
    serial->setBaudRate(QSerialPort::Baud9600);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);

    if (serial->open(QIODevice::ReadWrite)) {
        connect(serial, &QSerialPort::readyRead, this, &SlaveManager::onDataReceived);
        return true;
    }
    return false;
}

void SlaveManager::onDataReceived() {
    QByteArray incomingData = serial->readAll();
    qDebug() << "Slave received from Master:" << incomingData;
    
    if(serial->isOpen()) {
        serial->write("SLAVE_ACK: Data Received OK!");
        serial->flush();
        qDebug() << "Slave sent acknowledgment.";
    }
}