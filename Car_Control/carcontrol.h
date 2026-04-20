#ifndef CARCONTROL_H
#define CARCONTROL_H

#include <avr/io.h>
#include <avr/interrupt.h>

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

class CarControl {
public:
    static void init(uint32_t baud);
    static void setSpeed(uint8_t speed);
    static void moveForward();
    static void moveBackward();
    static void turnRight();
    static void turnLeft();
    static void stop();
    static void processCommand(char cmd);
    
    // عدادات منفصلة لكل موتور
    static volatile long pulseCountRight;
    static volatile long pulseCountLeft;
};

#endif