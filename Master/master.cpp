#include "master.h"

MasterManager::MasterManager(QObject *parent) : QObject(parent) {
    serial = new QSerialPort(this);
}

bool MasterManager::connectPort(QString name) {
    serial->setPortName(name);
    
    serial->setBaudRate(QSerialPort::Baud9600);
    serial->setDataBits(QSerialPort::Data8);      // 8 bits
    serial->setParity(QSerialPort::NoParity);    // No Parity
    serial->setStopBits(QSerialPort::OneStop);   // 1 Stop Bit
    serial->setFlowControl(QSerialPort::NoFlowControl);

    if (serial->open(QIODevice::ReadWrite)) {
        connect(serial, &QSerialPort::readyRead, this, &MasterManager::onDataReceived);
        return true;
    }
    return false;
}

void MasterManager::sendPing() {
    if(serial->isOpen()) {
        serial->write("Amira");
        qDebug() << "Master sent: Amira";
    }
}

void MasterManager::onDataReceived() {
    QByteArray response = serial->readAll();
    qDebug() << "Master received response from Slave:" << response;
}