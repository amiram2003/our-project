#include "SerialCommunication.h"

SerialCommunication::SerialCommunication(QObject *parent) : QObject(parent), serial(new QSerialPort(nullptr)) 
{
}

SerialCommunication::~SerialCommunication() {
    if (serial->isOpen()) {
        serial->close();
    }
    delete serial; 
    serial = nullptr;
    qDebug() << "Master Serial Port Closed and Memory Cleared.";
}

bool SerialCommunication::connectPort(const QString& name) {
    if(name.isEmpty()) {
        qInfo() << "[UART] Port name is empty!";
        return false;
    }

    //< Set Configuration !! 
    serial->setPortName(name);
    serial->setBaudRate(QSerialPort::Baud9600);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);

    if (serial->open(QIODevice::ReadWrite)) {
        qInfo() << "UART: serial port has been opened!";
        connect(serial, &QSerialPort::readyRead, this, &SerialCommunication::onDataReceived, Qt::UniqueConnection);
        return true;
    }
    return false;
}

void SerialCommunication::onDataReceived() {
    QByteArray data = serial->readAll();
    if(data.isEmpty()) {
        return;
    } else {
        //< emit signal here to DBus !! 
    }
}

void SerialCommunication::send(const QString& data) const {
    if(serial && serial->isOpen()) {
        serial->write(data.toLatin1());
        serial->flush(); 
        qDebug() << "UART: sent:" << data;
    } else {
        qInfo() << "UART: Trying to send data while serial port is not opened!";
    }
}