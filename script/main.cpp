#include <QCoreApplication>
#include <iostream>
#include "canmock.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    CanMock mock;
    if (!mock.initialize("vcan0")) {
        std::cerr << "Failed to initialize CAN interface" << std::endl;
        return 1;
    }

    return app.exec();
}