#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <stdio.h>

#define XTAL_FREQUENCY (16000000UL) /* Unit is 1 Hz */
#define TICK_FREQUENCY (100U) /* Unit is 1 Hz */
#define XTAL_COUNTS_PER_TICK (XTAL_FREQUENCY / TICK_FREQUENCY)
#define MIN_TIME_BETWEEN_BUTTON_PUSHES (50U) /* Unit is 1 ms. */
#define MILLISECONDS_PER_SECOND (1000U)
#define MIN_TICKS_BETWEEN_BUTTON_PUSHES ( \
    (TICK_FREQUENCY * MIN_TIME_BETWEEN_BUTTON_PUSHES) \
    / MILLISECONDS_PER_SECOND \
)

volatile unsigned long last_button_push;

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
    if ((TCNT1 - last_button_push) > MIN_TICKS_BETWEEN_BUTTON_PUSHES)
    {
        led_toggle();
        last_button_push = TCNT1;
    }
}

void clock_init()
{
    cli();
    TCCR1A = 0x00;
    TCCR1B  |= (1 << CS10);
    TCNT1 = 0;
    sei();
}

int main(void)
{
    led_init();
    button_init();
    clock_init();
    set_sleep_mode(SLEEP_MODE_STANDBY);

    for(;;)
    { 
        sleep_mode();
    }

    return 0;
}