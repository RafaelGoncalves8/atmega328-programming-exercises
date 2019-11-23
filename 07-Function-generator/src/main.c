#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>

/*
   MCU freq = 16Mhz
   max counter = 256
   prescaler = 0
   16000000/256 = 62500 Hz
   interruption after 0.016 ms
   
   256*counter*0.016ms = 1s
   counter = 244.14 ~ 244
*/


/* Macros for setting and clearing pin PIN at position N. */
#define SET(PIN,N) (PIN |=  (1<<N))
#define CLR(PIN,N) (PIN &= ~(1<<N))


volatile int counter;


void setup()
{
  SET(DDRB, 3);  // Pin 11 as output.

  /* Timer 2 configuration. */
  TCCR2A = 0;
  TCCR2B = 0;
  OCR2A = 0;
  
  SET(TCCR2B, CS20);  // No prescaler
  SET(TIMSK2, TOIE2); // Overflow interrupt enabled

  SET(TCCR2A, COM2A1);
  SET(TCCR2A, WGM20);
  SET(TCCR2A, WGM21); // Configure timer 2 as fast PWM
    
  sei(); // Set interruptions on.
}


ISR(TIMER2_OVF_vect) 
{
  counter++;
}


int main()
{
  int is_up = 1;
  
  setup();
  
  while(1)
  {
    if (counter >= 244)
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
