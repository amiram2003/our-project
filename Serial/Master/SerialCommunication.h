/**
 * @file SerialCommunication.h
 * @author your name (you@domain.com)
 * @brief This is class open a serial communication over UART0 to send and recive data 
 *          This class is try to connect on the serial peridically as long as no connection established!
 * @version 0.1
 * @date 2026-03-05
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#ifndef MASTER_MANAGER_H
#define MASTER_MANAGER_H

#include <QObject>
#include <QSerialPort>
#include <QDebug>

class SerialCommunication : public QObject {
    Q_OBJECT
public:
    explicit SerialCommunication(QObject *parent = nullptr);
    ~SerialCommunication(); 
    bool connectPort(const QString& name = QString());

    void send(const QString& data) const;
public slots:
    void onDataReceived();

private:
    QSerialPort *serial;
};

#endif