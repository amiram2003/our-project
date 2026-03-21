#include <QCoreApplication>
#include "controller.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    Controller controller;
    
    /* Execute the system reboot via D-Bus */
    controller.requestSystemReboot();

    return app.exec();
}