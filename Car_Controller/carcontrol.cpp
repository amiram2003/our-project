#include "carcontrol.h"

void CarControl::init(uint32_t baud) {
    // Calculate UBRR
    uint16_t ubrr = F_CPU/16/baud - 1;
    UBRRH = (ubrr >> 8);
    UBRRL = ubrr;

    // Enable Receiver, Transmitter and RX Complete Interrupt
    UCSRB = (1 << RXEN) | (1 << TXEN) | (1 << RXCIE);
    
    // Set frame format: 8 data bits, 1 stop bit
    UCSRC = (1 << URSEL) | (1 << UCSZ1) | (1 << UCSZ0);

    // Configure Pins PD2, PD3, PD4, PD5 as Output for Motor Driver
    DDRD |= 0x3C; 

    // Enable Global Interrupts
    sei();
}

void CarControl::processCommand(char cmd) {
    switch (cmd) {
        case 'F': moveForward();  break;
        case 'B': moveBackward(); break;
        case 'L': turnLeft();     break;
        case 'R': turnRight();    break;
        case 'S': stop();         break;
        default:  stop();         break;
    }
}

void CarControl::moveForward() {
    PORTD = (PORTD & ~0x3C) | (1 << PD2) | (1 << PD4);
}

void CarControl::moveBackward() {
    PORTD = (PORTD & ~0x3C) | (1 << PD3) | (1 << PD5);
}

void CarControl::turnRight() {
    PORTD = (PORTD & ~0x3C) | (1 << PD2); 
}

void CarControl::turnLeft() {
    PORTD = (PORTD & ~0x3C) | (1 << PD4); 
}

void CarControl::stop() {
    PORTD &= ~0x3C;
}