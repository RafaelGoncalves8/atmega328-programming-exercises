#define F_CPU 16000000UL // 16 MHz clock speed
#include <avr/interrupt.h>
#include <util/delay.h>


/* Macros for setting and clearing pin PIN at position N. */ 
#define SET(PIN,N) (PIN |=  (1<<N))
#define CLR(PIN,N) (PIN &= ~(1<<N))

// Button B in PC0
unsigned char *p_ddrc;
unsigned char *p_pinc;

// Rgb led output in PB0, PB1, PB2
unsigned char *p_ddrb;
unsigned char *p_portb;

// Button A in PD2
unsigned char *p_ddrd;
unsigned char *p_pind;

// Interrupt control registers
unsigned char *p_eicra;
unsigned char *p_eimsk;
unsigned char *p_pcicr;
unsigned char *p_pcmsk1;

// Aux global variables
volatile int delay = 1000;
unsigned char is_clockwise = 1;
unsigned char state = 0;

unsigned char colors[7] = {
  0b00000000, // Off
  0b00000001, // Red
  0b00000010, // Green
  0b00000100, // Blue
  0b00000011, // Yellow
  0b00000110, // Cyan
  0b00000101  // Magenta
};

// If delay > 125ms delay is set to half of its value
ISR(INT0_vect)
{
  if (delay > 125)
  {
    delay /= 2;
  }
  else
  {
    delay = 1000;
  }
}

// Change colors between cw and ccw orientation
ISR(PCINT1_vect)
{
  if (is_clockwise)
    is_clockwise = 0;
  else
    is_clockwise = 1;
}

void setup(void)
{
  /* Set adresses. */
  p_ddrb = (unsigned char *) 0x24;
  p_portb = (unsigned char *) 0x25;
  p_ddrc = (unsigned char *) 0x27;
  p_pinc = (unsigned char *) 0x26;
  p_ddrd = (unsigned char *) 0x2A;
  p_pind = (unsigned char *) 0x29;
  p_eicra = (unsigned char *) 0x69;
  p_eimsk = (unsigned char *) 0x3D;
  p_pcicr = (unsigned char *) 0x68;
  p_pcmsk1 = (unsigned char *) 0x6C;

  CLR(*p_ddrc, 0); // PC0 as input
  CLR(*p_ddrd, 2); // PD2 as input
  SET(*p_ddrb, 0); // PB0 as output
  SET(*p_ddrb, 1); // PB1 as output
  SET(*p_ddrb, 2); // PB2 as output

  /* Led initially turn off. */
  *p_portb = colors[0];

  // SETS PCINT8 interruption at PC0
  *p_pcicr |= 0b00000010;
  SET(*p_pcmsk1, 0);

  *p_eicra |= 0b00000011; // INT0 configured to be triggered at rising edge
  SET(*p_eimsk,0); // Turn on INT0 interruption

  sei(); // Turn on interruptions
}

int main(void)
{
  setup();

  while(1)
  {
    _delay_ms(delay);

    if (is_clockwise)
      state = (state+8)%7;
    else
      state = (state+6)%7;

    *p_portb = (*p_portb & 0b11111000) | colors[state];
  }
}
