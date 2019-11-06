#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

/*
   MCU freq = 16Mhz
   max counter = 65536
   desired freq = 500 Hz
   OCR2A = 16000000/256/500 = 125 (< 256)
   
   count value for 0.5s = 0.5*500 = 250
   count value for 0.78 = 0.78*500 = 390
   count value for 5s	= 5*500 = 2500
*/

#define TOP13 250
#define TOP12 390
#define TOPMSG 2500

/* Macros for setting and clearing pin PIN at position N. */
#define SET(PIN,N) (PIN |=  (1<<N))
#define CLR(PIN,N) (PIN &= ~(1<<N))

char msg[] = " Atividade 8 - Interrupcoes temporizadas tratam concorrencia entre tarefas! \n";
volatile int counter12 = 0;
volatile int counter13 = 0;
volatile int counter_msg = 0;
int led_12_on = 0;
int led_13_on = 0;

void setup()
{
  SET(DDRB, 5);  // Pin 13 as output.
  CLR(PORTB, 5); // Led initially off.
  SET(DDRB, 4);  // Pin 12 as output.
  CLR(PORTB, 4); // Led initially off.

  /* Timer 2 configuration. */
  TCCR2A = 0;
  TCCR2B = 0;
  OCR2A = 125;
  
  SET(TCCR2A, WGM21); // Configure timer 2 as CTC
  SET(TCCR2B, CS21);
  SET(TCCR2B, CS22);  // No prescaler
  SET(TIMSK2, OCIE2A); // CompA interrupt enabled
  
  /* USART configuration. */
  
  CLR(UCSR0A, 1); // normal speed
  CLR(UCSR0A, 0); // multiprocess desactivated

  // Interruptions desactivated
  CLR(UCSR0B, 7);
  CLR(UCSR0B, 6);
  CLR(UCSR0B, 5);

  CLR(UCSR0B, 4); // Receiver
  SET(UCSR0B, 3); // Transmissor

  // USART asyncronous
  CLR(UCSR0C, 7);
  CLR(UCSR0C, 6);

  // Even parity bit
  SET(UCSR0C, 5);
  SET(UCSR0C, 4);

  CLR(UCSR0C, 3); // single stop bit

  // 8 bit / frame
  CLR(UCSR0B, 2);
  SET(UCSR0C, 2);
  SET(UCSR0C, 1);

  CLR(UCSR0C, 0);
  
  sei(); // Set interruptions on.
}

ISR(TIMER2_COMPA_vect)
{
  counter12++;
  counter13++;
  counter_msg++;
}

void send_byte(char c)
{
  while (!( UCSR0A & (1<<UDRE0)));
  UDR0 = c;
}

int main()
{
  int i;
  setup();
  while(1)
  {
    if (counter12 == TOP12)
    {
      counter12 = 0;
      if (led_12_on)
      {
        led_12_on--;
        CLR(PORTB, 4);
      }
      else
      {
        led_12_on++;
        SET(PORTB, 4);
      }
    }
    else if (counter13 == TOP13)
    {
      counter13 = 0;
      if (led_13_on)
      {
        led_13_on--;
        CLR(PORTB, 5);
      }
      else
      {
        led_13_on++;
        SET(PORTB, 5);
      }
    }
    else if (counter_msg == TOPMSG)
    {
      counter_msg = 0;
        for (i = 0; i < 78; i++)
          send_byte(msg[i]);
    }
  }
}
