#include "carcontrol.h"

ISR(USART_RXC_vect) {
    char receivedChar = UDR;
    CarControl::processCommand(receivedChar);
}

// قراءة الموتور اليمين
ISR(INT0_vect) {
    CarControl::pulseCountRight++;
}

// قراءة الموتور الشمال
ISR(INT1_vect) {
    CarControl::pulseCountLeft++;
}

int main() {
    CarControl::init(9600);
    while (1) {
        // البرنامج شغال بالكامل بالـ Interrupts
    }
    return 0;
}