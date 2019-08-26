#define DDRC  0x27
#define PORTC 0x28
#define DDRD  0x2A
#define PIND  0x29

int
main()
{
  unsigned char *p_ddrc;
  unsigned char *p_portc;
  unsigned char *p_ddrd;
  unsigned char *p_pind;

  p_ddrc = (unsigned char *) DDRC;
  *p_ddrc &= 0b11111110; /* Set port as output. */

  p_ddrd = (unsigned char *) DDRD;
  *p_ddrd |= 0b00000001; /* Set port as input. */

  p_portc = (unsigned char *) PORTC;
  p_pind = (unsigned char *) PIND;

  while(1) {
    if ((*p_pind & 0b10000000) == 0b10000000) /* Button is not pressed. */
      *p_portc &= 0b11111110;
    else
      *p_portc |= 0b00000001;
  };

  return 0;
}
