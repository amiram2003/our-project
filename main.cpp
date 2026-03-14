#include <avr/io.h>
#include "timer.hpp"

int main(void)
{   Timer t;
   t.wave_PWM();
    while(1)
    {
        // main loop
    }
    return 0;
}