#include <util/delay.h>

#define F_CPU 16000000UL // 16 MHz clock speed

/* Macros for setting and clearing pin PIN at position N. */ 
#define SET(PIN,N) (PIN |=  (1<<N))
#define CLR(PIN,N) (PIN &= ~(1<<N))

/* Pin addresses. */
unsigned char *p_ddrc = (unsigned char *) 0x27;
unsigned char *p_pinc = (unsigned char *) 0x26;
unsigned char *p_ddrd = (unsigned char *) 0x2A;
unsigned char *p_portd = (unsigned char *) 0x2B;
/* Adress of each segment of the 7-segments decoder. */
unsigned char decoder[7] = {
  0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80
};
/* Masks representing segments abcdefg from 7-segments decoder. */
unsigned char mask_nums[10] = {
  0b01111110, // 0
  0b00110000, // 1
  0b01101101, // 2
  0b01111001, // 3
  0b00110011, // 4
  0b01011011, // 5
  0b01011111, // 6
  0b01110000, // 7
  0b01111111, // 8
  0b01111011  // 9
};

/*
 * Function that returns the value for setting  portd after
 * combining each value from mask for segment i from the decoder.
 */
unsigned char
unite_segments(unsigned char mask)
{
  unsigned char i;
  unsigned char segments = 0;
  for (i = 0; i < 7; i++)
  {
    if (mask & (1<<(6-i)))
      segments |= decoder[i];
  }

  return segments;
}

int
main()
{
  unsigned char mask_ddrd = 0b11111110;
  unsigned char state = 0;

  /* Setup. */
  CLR(*p_ddrc, 0); /* PC0 as input. */
  SET(*p_pinc, 0); /* Set internal pullup resistor at PC0. */
  *p_ddrd |= mask_ddrd; /* PD1-8 as output. */
  *p_portd = unite_segments(mask_nums[0]);

  while(1)
  {
    if ((*p_pinc & 0b00000001) == 1) // Is not pressed
    {
      _delay_ms(30);
      if ((*p_pinc & 0b00000001) == 0) // Is pressed
      {
        state = (state + 1)%10;
        *p_portd = unite_segments(mask_nums[state]);
        _delay_ms(200);
      }
    }
  }

  return 0;
}
