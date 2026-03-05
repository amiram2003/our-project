/**
 * @file controller.h
 * @author your name (you@domain.com)
 * @brief This class handle differnet interfaces and get all network related informations
 *          communicate with network manager to get data from
 *          expose different data iver dbus 
 * @version 0.1
 * @date 2026-03-05
 * 
 * @copyright Copyright (c) 2026
 * 
 */
#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QtDBus/QDBusInterface>

class Controller : public QObject
{
    Q_OBJECT

public:
    explicit Controller(QObject *parent = nullptr);
    ~Controller();
    QString getIpAddress();
    QString getMacAddress();
    void requestRestart();
    
    /**
     * @brief connect to wifi 
     * 
     * @param ssid 
     * @param pass 
     */
    void connectToWifi(const QString& ssid, const QString& pass);

private:
    QDBusInterface *m_interface;
};

#endif