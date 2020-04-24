#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <stdio.h>

const int bounce_floor = 8000;

void led_init()
{
    /* setup D13 led as output */
    DDRB |= (1 << DDB5);
}

void led_toggle()
{
    /* toggle on board LED */
    PORTB ^= (1 << PORTB5);
}

void button_init()
{
    DDRD &= ~(1 << DDD2); // setup as input   
    //  External Interrupt Control Register A
    EICRA |= ((1 << ISC00) | (1 << ISC01)); //rising edge
    // External Interrupt Mask Register
    EIMSK |= (1 << INT0); // enable
}

ISR (INT0_vect)
{
    led_toggle();
    // check timer
    if (TCNT0 > bounce_floor)
    {
        led_toggle();
        TCNT0 = 0;
    }
    
}

int main(void) 
{   
    led_init();
    button_init();
    cli();
    // setup clock
    TCCR0A = 0x00;
    TCCR0B  |= (1 << CS00);
    sei(); // enable global interrupt
    
    set_sleep_mode(SLEEP_MODE_STANDBY);
    for(;;)
    { 
        sleep_mode();
    }

    return 0;
}