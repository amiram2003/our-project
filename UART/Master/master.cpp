#include "master.h"

MasterManager::MasterManager(QObject *parent) : QObject(parent) {
    serial = new QSerialPort(this);
}

MasterManager::~MasterManager() {
    if (serial->isOpen()) {
        serial->close();

    }
    delete serial; 
    qDebug() << "Master Serial Port Closed and Memory Cleared.";
}

bool MasterManager::connectPort(QString name) {
    serial->setPortName(name);
    serial->setBaudRate(QSerialPort::Baud9600);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);

    if (serial->open(QIODevice::ReadWrite)) {
        connect(serial, &QSerialPort::readyRead, this, &MasterManager::onDataReceived);
        return true;
    }
    return false;
}

void MasterManager::sendPing() {
    if(serial && serial->isOpen()) {
        serial->write("Amira");
        serial->flush(); 
        qDebug() << "Master sent: Amira";
    }
}

void MasterManager::onDataReceived() {
    QByteArray response = serial->readAll();
    qDebug() << "Master received response from Slave:" << response;
}