#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "vehicledata.h"
#include "dbusreader.h"

int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);

    VehicleData carData;
    DBusReader dbusReader;

    QQmlApplicationEngine engine;

    engine.rootContext()->setContextProperty("vData", &carData);
    engine.rootContext()->setContextProperty("dbusData", &dbusReader);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}