#ifndef MASTER_MANAGER_H
#define MASTER_MANAGER_H

#include <QObject>
#include <QSerialPort>
#include <QDebug>

class MasterManager : public QObject {
    Q_OBJECT
public:
    explicit MasterManager(QObject *parent = nullptr);
    bool connectPort(QString name);

public slots:
    void sendPing();
    void onDataReceived();

private:
    QSerialPort *serial;
};

#endif