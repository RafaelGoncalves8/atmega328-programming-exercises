#define DDRC  0x27
#define PORTC 0x28

int
main()
{
  unsigned char *p_ddrc;
  unsigned char *p_portc;

  p_ddrc = (unsigned char *) DDRC;
  *p_ddrc &= 0b11111110; /* Set port as output. */

  p_portc = (unsigned char *) PORTC;
  *p_portc |= 0b00000001; /* Turn led on. */
  /* *p_portc &= 0b11111110; #<{(| Turn led off. |)}># */

  while(1) {};

  return 0;
}
