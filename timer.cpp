#include <avr/io.h>
#include "timer.hPP"

// Macro definitions
#define SET_BIT(REG, BIT) (REG |= (1 << BIT))

void Timer::wave_PWM(void)
{
    /* set OC0 as output pin */
    SET_BIT(DDRB, 3);
    /* select fast PWM mode */
    SET_BIT(TCCR0, WGM00);
    SET_BIT(TCCR0, WGM01);

    /* load a value in OCR0 */
    OCR0 = 64;

    /* select timer clock */
    SET_BIT(TCCR0, CS00);
    SET_BIT(TCCR0, CS02);

    /* Set OC0 on compare match, clear OC0 at BOTTOM (inverting mode) */
    SET_BIT(TCCR0, COM00);
    SET_BIT(TCCR0, COM01);
}