#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>


/*
   MCU freq = 16Mhz
   max counter = 256
   prescaler = 0
   16000000/256 = 62500 Hz
   flip led after 0.016 ms
   
   change pwm after 10ms
   freq = 625 Hz 
*/


/* Macros for setting and clearing pin PIN at position N. */
#define SET(PIN,N) (PIN |=  (1<<N))
#define CLR(PIN,N) (PIN &= ~(1<<N))


volatile int counter;
volatile int is_up = 1;


void setup()
{
  SET(DDRB, 3);  // Pin 11 as output.
  SET(PORTB, 3); // Led initially off.

  /* Timer 2 configuration. */
  TCCR2A = 0;
  TCCR2B = 0;
  OCR2A = 0;
  OCR2B = 0;
  
  SET(TCCR2B, CS20);  // No prescaler
  SET(TIMSK2, TOIE2); // Overflow interrupt enabled
  SET(TCCR2A, WGM21); // Configure timer 2 as fast PWM
  SET(TCCR2A, WGM20);
    
  sei(); // Set interruptions on.
}


ISR(TIMER2_OVF_vect) 
{
  counter++;
}


int main()
{
  int i;
  setup();
  while(1)
  {
    if (counter >= 625)
    {
      if (is_up)
      {
        OCR2A++;
        if (OCR2A == 255)
          is_up--;
      }   
      else
      {
        OCR2A--;
        if (OCR2A == 0)
          is_up++;
      }
      counter = 0;
    }
  }
}
