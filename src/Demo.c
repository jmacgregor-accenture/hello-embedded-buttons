#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <time.h>
#include <stdio.h>
#include <stdbool.h>

void led_init()
{
    /* setup D13 led as output */
    DDRB |= (1 << DDB5);
}

void led_toggle()
{
    /* toggle on board LED */
    PINB |= (1 << PINB5);

    // can also use xor
    //PORTB ^= (1 << PORTB5);
}

void button_init()
{
    DDRD &= ~(1 << DDD2); // setup as input   
    //  External Interrupt Control Register A
    EICRA |= ((1 << ISC00) | (1 << ISC01)); //rising edge
    // External Interrupt Mask Register
    EIMSK |= (1 << INT0); // enable
}

volatile time_t last_interrupt;
volatile time_t current_time;

// volatile int last_interrupt;
// volatile int current_time;

void update_current_time()
{
    current_time = time(NULL);
    // current_time = micros();
}

void update_interrupt_time()
{
    last_interrupt = time(NULL);
    // last_interrupt = micros();
}

long interrupt_minimum_in_ms = 100;

ISR (INT0_vect)
{
    // get time of interrupt
    update_current_time();
    // compare to "lastInterrupt"
    long difference = current_time * 1000 - last_interrupt * 1000;
    //int difference = current_time - last_interrupt;

    if (difference > interrupt_minimum_in_ms)
    {
        update_interrupt_time(); // should we get time(NULL) again?
        led_toggle();
    }
    else 
    {
        // ignore that bouncey clown
    }

    //led_toggle();
}

int main(void) 
{   
    led_init();
    button_init();
    // set time of "lastInterrupt" as baseline
    update_interrupt_time();
    sei(); // enable global interrupt
    
    set_sleep_mode(SLEEP_MODE_STANDBY);
    for(;;)
    { 
        sei();
        sleep_mode();
    }

    return 0;
}