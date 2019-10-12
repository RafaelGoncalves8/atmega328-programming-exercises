#include <util/delay.h>

#define F_CPU 16000000UL // 16 MHz clock speed

#define MAX 10000

/* Macros for setting and clearing pin PIN at position N. */ 
#define SET(PIN,N) (PIN |=  (1<<N))
#define CLR(PIN,N) (PIN &= ~(1<<N))

/* Macro for setting colors. */
#define COLOR(PIN,N) (PIN = (PIN & 0b11111000) & (colors[N] & 0b00000111))

/* Rgb led output in PB0, PB1, PB2 */
unsigned char *p_ddrb;
unsigned char *p_portb;

unsigned char colors[8] = {
  0b00000000, // Off
  0b00000001, // Red
  0b00000010, // Green
  0b00000100, // Blue
  0b00000011, // Yellow
  0b00000110, // Cyan
  0b00000101, // Magenta
  0b00000111
};

/* USART registers */
unsigned char *p_UCSR0A;
unsigned char *p_UCSR0B;
unsigned char *p_UCSR0C;
unsigned char *p_UDR0;
unsigned char *p_UBRR0H;
unsigned char *p_UBRR0L;


volatile unsigned char receivedByte;
volatile unsigned char new = 0; // There isnt new info at receivedByte

void setup(void)
{
  p_ddrb = (unsigned char *) 0x24;
  p_portb = (unsigned char *) 0x25;

  SET(*p_ddrb, 0); // PB0 as output
  SET(*p_ddrb, 1); // PB1 as output
  SET(*p_ddrb, 2); // PB2 as output

  p_UBRR0H = (unsigned char *) 0xC5;
  p_UBRR0L = (unsigned char *) 0xC4;
  p_UCSR0A = (unsigned char *) 0xC0;
  p_UCSR0B = (unsigned char *) 0xC1;
  p_UCSR0C = (unsigned char *) 0xC2;
  p_UDR0 = (unsigned char *) 0xC6;

  // f = 9.6kHz, UBRR0 = 103
  *p_UBRR0H = 0b00000000;
  *p_UBRR0L = 0b01100111;

  CLR(*p_UCSR0A, 1); // normal speed
  CLR(*p_UCSR0A, 0); // multiprocess desactivated

  // Interruptions
  SET(*p_UCSR0B, 7); // Reception complete
  CLR(*p_UCSR0B, 6); // Transmission complete
  CLR(*p_UCSR0B, 5); // Empty data register

  SET(*p_UCSR0B, 4); // Receiver
  SET(*p_UCSR0B, 3); // Transmissor

  // USART asyncronous
  CLR(*p_UCSR0C, 7);
  CLR(*p_UCSR0C, 6);

  // No parity bit
  CLR(*p_UCSR0C, 5);
  CLR(*p_UCSR0C, 4);

  SET(*p_UCSR0C, 3); // Double stop bit

  // 8 bit / frame
  CLR(*p_UCSR0B, 2);
  SET(*p_UCSR0C, 2);
  SET(*p_UCSR0C, 1);

  CLR(*p_UCSR0C, 0);

  COLOR(*p_portb, 0); // Led initially off

  sei(); // Turn on interruptions
}


ISR (USART_RXC_vect)
{
  receivedByte = UDR0;
  new = 1;
}

ISR (USART_TX_vect)
{
}

ISR (USART_UDRE_vect)
{
}

void send_byte(char c)
{
  while((*p_UCSR0A & 0b00100000) == 0); // Can transmit

  SET(*p_UCSR0B, 5); // Set empty data register
  *p_UDR0 = c;

  while((*p_UCSR0A & 0b01000000) == 0); // Empty buffer
}

void send_msg(char *msg)
{
  int i = 0;
  while(msg[i] != '\0')
  {
    send_byte(msg[i]);
    i++
  }
}

void turn_led(unsigned char c)
{
  char msg[MAX] = "Comando incorreto\n"

  switch (led_color)
  {
    case 'R':
    case 'r':
      COLOR(*p_portb, colors[1]);
      msg = "Comando: Acender LED - cor vermelha\n"
      break;
    case 'G':
    case 'g':
      COLOR(*p_portb, colors[2]);
      break;
    case 'B':
    case 'b':
      COLOR(*p_portb, colors[3]);
      break;
    case 'Y':
    case 'y':
      COLOR(*p_portb, colors[4]);
      break;
    case 'C':
    case 'c':
      COLOR(*p_portb, colors[5]);
      break;
    case 'M':
    case 'm':
      COLOR(*p_portb, colors[6]);
      break;
    case 'W':
    case 'w':
      COLOR(*p_portb, colors[7]);
      break;
  }

  send_msg(msg);
  _delay_ms(200);
  COLOR(*p_portb, colors[0]);
}

int main(void)
{
  setup();

  while(1)
  {
    if (new)
    {
      turn_led(receivedByte);
    }
  }
}