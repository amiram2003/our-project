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
    
    // Movement methods
    static void moveForward();
    static void moveBackward();
    static void turnRight();
    static void turnLeft();
    static void stop();

    // Process commands received via Interrupt
    static void processCommand(char cmd);
};

#endif