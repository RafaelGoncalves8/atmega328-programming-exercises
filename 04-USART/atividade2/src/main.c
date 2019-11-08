#define F_CPU 16000000UL // 16 MHz clock speed
#include <util/delay.h>
#include <avr/interrupt.h>

#define MAX 10000

/* Macros for setting and clearing pin PIN at position N. */ 
#define SET(PIN,N) (PIN |=  (1<<N))
#define CLR(PIN,N) (PIN &= ~(1<<N))

/* Macro for setting colors. */
#define COLOR(PIN,N) (PIN = (colors[N] & 0b00000111))

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
  0b00000111  // White
};

/* USART registers */
unsigned char *p_UCSR0A;
unsigned char *p_UCSR0B;
unsigned char *p_UCSR0C;
unsigned char *p_UDR0;
unsigned char *p_UBRR0H;
unsigned char *p_UBRR0L;

char *empty = "Vazio!\n";
char *msg = "Comando incorreto\n";
char *red = "Comando: Acender LED - cor vermelha\n";
char *green = "Comando: Acender LED - cor verde\n";
char *blue = "Comando: Acender LED - cor azul\n";
char *yellow = "Comando: Acender LED - cor amarelo\n";
char *cyan = "Comando: Acender LED - cor ciano\n";
char *magenta = "Comando: Acender LED - cor magenta\n";
char *white = "Comando: Acender LED - cor branco\n";

volatile unsigned char receivedByte;

volatile int j = 0;

/* Circular buffer. */
#define SIZE_OF_BUFFER 10
volatile int buffer_len = 0;
volatile char buffer[SIZE_OF_BUFFER];
volatile int write_idx = 0;
volatile int read_idx = 0;
void buffer_write(char c);
char buffer_read();

void buffer_write(char c)
{
	if (buffer_len != SIZE_OF_BUFFER)
	{
		buffer[write_idx] = c;
		write_idx = (write_idx + 11)%SIZE_OF_BUFFER;
		buffer_len++;
	}
}

char buffer_read()
{
	char c = '\0';
	
	if (buffer_len)
	{
		c = buffer[read_idx];
		buffer_len--;
		read_idx = (read_idx + 11)%SIZE_OF_BUFFER;
	}
	
	return c;	
}

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

void send_byte(char c)
{
  *p_UDR0 = c;
}

ISR (USART_RX_vect)
{
  buffer_write(*p_UDR0);
}

ISR (USART_UDRE_vect)
{
  if (msg[j] == '\0')
  {
	j = 0;
	CLR(*p_UCSR0B, 5);
  }
  else
  {
    send_byte(msg[j]);
    j++;
  }
}

void turn_led(unsigned char c)
{

  switch (c)
  {
    case 'R':
    case 'r':
      COLOR(*p_portb, 1);
      msg = red;
      break;
    case 'G':
    case 'g':
      COLOR(*p_portb, 2);
      msg = green;
      break;
    case 'B':
    case 'b':
      COLOR(*p_portb, 3);
      msg = blue;
      break;
    case 'Y':
    case 'y':
      COLOR(*p_portb, 4);
      msg = yellow;
      break;
    case 'C':
    case 'c':
      COLOR(*p_portb, 5);
      msg = cyan;
      break;
    case 'M':
    case 'm':
      COLOR(*p_portb, 6);
      msg = magenta;
      break;
    case 'W':
    case 'w':
      COLOR(*p_portb, 7);
      msg = white;
      break;
  }

  _delay_ms(200);
  COLOR(*p_portb, colors[0]);
}

int main(void)
{
  setup();

  while(1)
  {

    if (buffer_len != 0)
    {
	  SET(*p_UCSR0B, 5);
      turn_led(buffer_read());
    }
	else
	{
	  msg = empty;
	  SET(*p_UCSR0B, 5);
	}
  }
}
