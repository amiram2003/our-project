#include "carcontrol.h"

// ISR: Interrupt Service Routine for UART Receive Complete
ISR(USART_RXC_vect) {
    char receivedChar = UDR; // Read the received byte from Buffer
    CarControl::processCommand(receivedChar); // Execute action immediately
}

int main() {
    // Initialize system with 9600 baud for Bluetooth
    CarControl::init(9600);

    while (1) {
    }
    return 0;
}