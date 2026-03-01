#ifndef SLAVE_MANAGER_H
#define SLAVE_MANAGER_H

#include <QObject>
#include <QSerialPort>
#include <QDebug>

class SlaveManager : public QObject {
    Q_OBJECT
public:
    explicit SlaveManager(QObject *parent = nullptr);
    ~SlaveManager(); 
    bool listenOnPort(QString name);

public slots:
    void onDataReceived();

private:
    QSerialPort *serial;
};

#endif