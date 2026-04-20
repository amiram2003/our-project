#include "carcontrol.h"

volatile long CarControl::pulseCountRight = 0;
volatile long CarControl::pulseCountLeft = 0;

void CarControl::init(uint32_t baud) {
    // UART Setup
    uint16_t ubrr = F_CPU/16/baud - 1;
    UBRRH = (ubrr >> 8);
    UBRRL = ubrr;
    UCSRB = (1 << RXEN) | (1 << TXEN) | (1 << RXCIE);
    UCSRC = (1 << URSEL) | (1 << UCSZ1) | (1 << UCSZ0);

    // Motor Pins on Port B (PB1, PB2, PB3, PB4)
    DDRB |= (1 << PB1) | (1 << PB2) | (1 << PB3) | (1 << PB4);
    
    // PWM Pins on Port D (PD4, PD5)
    DDRD |= (1 << PD4) | (1 << PD5);
    
    // Encoder Pins as Inputs (PD2, PD3)
    DDRD &= ~((1 << PD2) | (1 << PD3));
    PORTD |= (1 << PD2) | (1 << PD3); // Pull-up

    // PWM Timer 1 Setup
    TCCR1A = (1 << COM1A1) | (1 << COM1B1) | (1 << WGM10);
    TCCR1B = (1 << WGM12) | (1 << CS11);

    // External Interrupts (INT0 & INT1)
    GICR |= (1 << INT0) | (1 << INT1);
    MCUCR |= (1 << ISC01) | (1 << ISC00) | (1 << ISC11) | (1 << ISC10); // Rising Edge

    sei();
}

void CarControl::setSpeed(uint8_t speed) {
    OCR1A = speed; // ENA
    OCR1B = speed; // ENB
}

void CarControl::moveForward() {
    // PB1 & PB3 High
    PORTB = (PORTB & ~0x1E) | (1 << PB1) | (1 << PB3);
}

void CarControl::moveBackward() {
    // PB2 & PB4 High
    PORTB = (PORTB & ~0x1E) | (1 << PB2) | (1 << PB4);
}

void CarControl::turnRight() {
    PORTB = (PORTB & ~0x1E) | (1 << PB1); // Only right side
}

void CarControl::turnLeft() {
    PORTB = (PORTB & ~0x1E) | (1 << PB3); // Only left side
}

void CarControl::stop() {
    PORTB &= ~0x1E;
    setSpeed(0);
}

void CarControl::processCommand(char cmd) {
    if (cmd >= '0' && cmd <= '9') {
        setSpeed((cmd - '0') * 28);
        return;
    }
    switch (cmd) {
        case 'F': moveForward();  break;
        case 'B': moveBackward(); break;
        case 'L': turnLeft();      break;
        case 'R': turnRight();     break;
        case 'S': stop();          break;
        default:  stop();          break;
    }
}