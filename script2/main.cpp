#include <QCoreApplication>
#include <QTimer>
#include "vibrationsimulator.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    VibrationSimulator simulator;
    
    // ربط السكريبت بـ vcan0
    if (!simulator.initCanInterface("vcan0")) {
        return -1;
    }

    // تايمر يخليه يبعت إشارة اهتزاز أوتوماتيك كل 5 ثواني للتجربة
    QTimer timer;
    QObject::connect(&timer, &QTimer::timeout, &simulator, &VibrationSimulator::sendVibrationFrame);
    timer.start(5000); // 5000 ملي ثانية = 5 ثواني

    qInfo() << "Vibration Simulator is running... Press Ctrl+C to stop.";
    return app.exec();
}