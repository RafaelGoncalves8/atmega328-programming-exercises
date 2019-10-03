#include <util/delay.h>

#define F_CPU 16000000UL // 16 MHz clock speed

/* Macros for setting and clearing pin PIN at position N. */ 
#define SET(PIN,N) (PIN |=  (1<<N))
#define CLR(PIN,N) (PIN &= ~(1<<N))

char msg[] = " Out of the night that covers me,\n Black as the Pit from pole to pole,\n I thank whatever gods may be\n For my unconquerable soul.\n In the fell clutch of circumstance\n I have not winced nor cried aloud.\n Under the bludgeonings of chance\n My head is bloody, but unbowed.\n Beyond this place of wrath and tears\n Looms but the Horror of the shade,\n And yet the menace of the years\n Finds, and shall find, me unafraid.\n It matters not how strait the gate,\n How charged with punishments the scroll.\n I am the master of my fate:\n I am the captain of my soul.\n";

char success[] = " Mensagem transmitida com sucesso!\n";

unsigned char *p_UCSR0A;
unsigned char *p_UCSR0B;
unsigned char *p_UCSR0C;
unsigned char *p_UDR0;
unsigned char *p_UBRR0H;
unsigned char *p_UBRR0L;

void setup(void)
{
  /* Set adresses. */
  p_UBRR0H = (unsigned char *) 0xC5;
  p_UBRR0L = (unsigned char *) 0xC4;
  p_UCSR0A = (unsigned char *) 0xC0;
  p_UCSR0B = (unsigned char *) 0xC1;
  p_UCSR0C = (unsigned char *) 0xC2;
  p_UDR0 = (unsigned char *) 0xC6;

  // f = 38.4kHz, UBRR0 = 25
  *p_UBRR0H = 0b00000000;
  *p_UBRR0L = 0b00011001;

  CLR(*p_UCSR0A, 1); // normal speed
  CLR(*p_UCSR0A, 0); // multiprocess desactivated

  // Interruptions desactivated
  CLR(*p_UCSR0B, 7);
  CLR(*p_UCSR0B, 6);
  CLR(*p_UCSR0B, 5);

  CLR(*p_UCSR0B, 4); // Receiver
  SET(*p_UCSR0B, 3); // Transmissor

  // USART asyncronous
  CLR(*p_UCSR0C, 7);
  CLR(*p_UCSR0C, 6);

  // Even parity bit
  SET(*p_UCSR0C, 5);
  SET(*p_UCSR0C, 4);

  CLR(*p_UCSR0C, 3); // single stop bit

  // 8 bit / frame
  CLR(*p_UCSR0B, 2);
  SET(*p_UCSR0C, 2);
  SET(*p_UCSR0C, 1);

  CLR(*p_UCSR0C, 0);
}

void send_byte(char c)
{
  while((*p_UCSR0A & 0b00100000) == 0); // Can transmit

  *p_UDR0 = c;

  while((*p_UCSR0A & 0b01000000) == 0); // Empty buffer
}

int main(void)
{
  int i;
  setup();

  while(1)
  {
    for(i = 0; i < 551; i++)
      send_byte(msg[i]);
    send_byte('\n');

    for(i = 0; i < 35; i++)
      send_byte(success[i]);
    send_byte('\n');

    _delay_ms(5000);
  }
}
