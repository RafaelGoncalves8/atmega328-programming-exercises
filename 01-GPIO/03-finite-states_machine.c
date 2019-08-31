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
  unsigned char state = 0;

  p_ddrc = (unsigned char *) DDRC;
  p_portc = (unsigned char *) PORTC;

  p_ddrd = (unsigned char *) DDRD;
  p_pind = (unsigned char *) PIND;

  *p_ddrc |= 0b00000001; /* Set port as output. */
  *p_ddrd &= 0b01111111; /* Set port as input. */

  *p_portc &= 0b1111110; /* LED initially off. */

  while(1) {
    switch(state)
    {
      case 0:
        if ((*p_pind & 0b10000000) == 0)
        {
          state = 1;
          *p_portc |= 0b0000001;
        }
        break;
      case 1:
        if ((*p_pind & 0b10000000) != 0)
        {
          state = 2;
          *p_portc |= 0b0000001;
        }
        break;
      case 2:
        if ((*p_pind & 0b10000000) == 0)
        {
          state = 3;
          *p_portc &= 0b11111110;
        }
        break;
      case 3:
        if ((*p_pind & 0b10000000) != 0)
        {
          state = 0;
          *p_portc &= 0b11111110;
        }
        break;
    } 

  }

  return 0;
}
