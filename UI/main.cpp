#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "vehicledata.h"

int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);

    VehicleData carData;
    QQmlApplicationEngine engine;

    engine.rootContext()->setContextProperty("vData", &carData);

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    engine.load(url);

    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}