#ifndef CANMOCK_H
#define CANMOCK_H

#include <QObject>
#include <QSocketNotifier>
#include <QTextStream>

class CanMock : public QObject
{
    Q_OBJECT
public:
    explicit CanMock(QObject *parent = nullptr);
    bool initialize(const QString &interfaceName);

private slots:
    void onUserInput();

private:
    void sendFrame(int id, int status);
    int m_socket;
    QSocketNotifier *m_notifier;
    QTextStream m_input;
};

#endif