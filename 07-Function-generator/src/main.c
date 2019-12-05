#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>

/*
   MCU freq = 16Mhz
   max counter = 256
   prescaler = 1
   16000000/256 = 62500 Hz
   interruption after 0.016 ms

   256*counter*0.016ms = 1s
   counter = 244.14 ~ 244
*/

/* Macros for setting and clearing pin PIN at position N. */
#define SET(PIN,N) (PIN |=  (1<<N))
#define CLR(PIN,N) (PIN &= ~(1<<N))

volatile unsigned long int counter;
volatile int j = 0;
int write_flag = 1;
char * msg;

char msg_sin[] = "Onda senoidal\n";
char msg_tri[] = "Onda triangular\n";
char msg_saw[] = "Onda dente-de-serra\n";
char msg_am[] = "Onda AM\n";
char msg_empty[] = "Nenhuma forma de onda selecionada\n";

unsigned char sin[200]={128, 132, 136, 140, 144, 148, 152, 155, 159, 163, 167, 171, 175, 178, 182, 186, 189, 193, 196, 199, 203, 206, 209, 212, 215, 218,221, 223, 226, 229, 231, 233, 235, 238, 240, 241, 243, 245, 246,248, 249, 250, 251, 252, 253, 254, 254, 255, 255, 255, 255, 255,255, 254, 254, 253, 253, 252, 251, 250, 248, 247, 246, 244, 242,241, 239, 237, 234, 232, 230, 227, 225, 222, 219, 217, 214, 211,208, 204, 201, 198, 194, 191, 187, 184, 180, 177, 173, 169, 165, 161, 157, 153, 150, 146, 142, 138, 134, 130, 125, 121, 117, 113, 109, 105, 102,  98,  94,  90,  86,  82,  78,  75,  71,  68,  64,61,  57,  54,  51,  47,  44,  41,  38,  36,  33,  30,  28,  25,23,  21,  18,  16,  14,  13,  11,   9,   8,   7,   5,   4,   3,2,   2,   1,   1,   0,   0,   0,   0,   0,   0,   1,   1,   2,3,   4,   5,   6,   7,   9,  10,  12,  14,  15,  17,  20,  22,24,  26,  29,  32,  34,  37,  40,  43,  46,  49,  52,  56,  59,62,  66,  69,  73,  77,  80,  84,  88,  92,  96, 100, 103, 107,111, 115, 119, 123, 127};

unsigned char am[200] = {128,141,155,168,181,192,203,211,218,223,226,226,223,218,211,201,189,176,161,145,128,110,93,76,60,46,34,23,16,10,8,9,13,19,29,41,55,71,89,108,127,147,166,185,202,217,230,241,249,253,255,253,249,241,230,217,202,185,166,147,128,108,89,71,55,41,29,19,13,9,8,10,16,23,34,46,60,76,93,110,127,145,161,176,189,201,211,218,223,226,226,223,218,211,203,192,181,168,155,141,128,114,102,91,81,72,65,60,57,55,56,58,62,67,73,81,90,99,108,118,127,137,145,153,160,166,171,175,177,178,178,177,174,171,166,161,155,148,142,135,128,121,114,108,102,97,93,89,87,86,85,86,87,89,93,97,102,108,114,121,127,135,142,148,155,161,166,171,174,177,178,178,177,175,171,166,160,153,145,137,128,118,108,99,90,81,73,67,62,58,56,55,57,60,65,72,81,91,102,114};

unsigned char saw[200] = {0, 1, 3, 4, 5, 6, 8, 9, 10, 11, 13, 14, 15, 17, 18, 19, 20, 22, 23, 24, 26, 27, 28, 29, 31, 32, 33, 34, 36, 37, 38, 40, 41, 42, 43, 45, 46, 47, 48, 50, 51, 52, 54, 55, 56, 57, 59, 60, 61, 62, 64, 65, 66, 68, 69, 70, 71, 73, 74, 75, 77, 78, 79, 80, 82, 83, 84, 85, 87, 88, 89, 91, 92, 93, 94, 96, 97, 98, 99, 101, 102, 103, 105, 106, 107, 108, 110, 111, 112, 113, 115, 116, 117, 119, 120, 121, 122, 124, 125, 126, 128, 129, 130, 131, 133, 134, 135, 136, 138, 139, 140, 142, 143, 144, 145, 147, 148, 149, 150, 152, 153, 154, 156, 157, 158, 159, 161, 162, 163, 164, 166, 167, 168, 170, 171, 172, 173, 175, 176, 177, 179, 180, 181, 182, 184, 185, 186, 187, 189, 190, 191, 193, 194, 195, 196, 198, 199, 200, 201, 203, 204, 205, 207, 208, 209, 210, 212, 213, 214, 215, 217, 218, 219, 221, 222, 223, 224, 226, 227, 228, 230, 231, 232, 233, 235, 236, 237, 238, 240, 241, 242, 244, 245, 246, 247, 249, 250, 251, 252, 254};

unsigned char tri[200] = {0,   3,   5,   8,  10,  13,  15,  18,  21,  23,  26,  28,  31,
33,  36,  39,  41,  44,  46,  49,  52,  54,  57,  59,  62,  64,
67,  70,  72,  75,  77,  80,  82,  85,  88,  90,  93,  95,  98,
100, 103, 106, 108, 111, 113, 116, 118, 121, 124, 126, 129, 131,
134, 137, 139, 142, 144, 147, 149, 152, 155, 157, 160, 162, 165,
167, 170, 173, 175, 178, 180, 183, 185, 188, 191, 193, 196, 198,
201, 203, 206, 209, 211, 214, 216, 219, 222, 224, 227, 229, 232,
234, 237, 240, 242, 245, 247, 250, 252, 255, 255, 252, 250, 247,
245, 242, 240, 237, 234, 232, 229, 227, 224, 222, 219, 216, 214,
211, 209, 206, 203, 201, 198, 196, 193, 191, 188, 185, 183, 180,
178, 175, 173, 170, 167, 165, 162, 160, 157, 155, 152, 149, 147,
144, 142, 139, 137, 134, 131, 129, 126, 124, 121, 118, 116, 113,
111, 108, 106, 103, 100,  98,  95,  93,  90,  88,  85,  82,  80,
77,  75,  72,  70,  67,  64,  62,  59,  57,  54,  52,  49,  46,
44,  41,  39,  36,  33,  31,  28,  26,  23,  21,  18,  15,  13,
10,   8,   5,   3,   0};

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

void setup()
{
  SET(DDRB, 5);  // Pin 13 as output.
  SET(DDRB, 3);  // Pin 11 as output.

  /* USART configuration. */
  /* f = 9.6kHz, UBRR0 = 103. */
  UBRR0H = 0b00000000;
  UBRR0L = 0b01100111;

  CLR(UCSR0A, 1); // normal speed
  CLR(UCSR0A, 0); // multiprocess desactivated

  // Interruptions
  SET(UCSR0B, 7); // Reception complete
  CLR(UCSR0B, 6); // Transmission complete
  SET(UCSR0B, 5); // Empty data register

  SET(UCSR0B, 4); // Receiver
  SET(UCSR0B, 3); // Transmissor

  // USART asyncronous
  CLR(UCSR0C, 7);
  CLR(UCSR0C, 6);

  // No parity bit
  CLR(UCSR0C, 5);
  CLR(UCSR0C, 4);

  SET(UCSR0C, 3); // Double stop bit

  // 8 bit / frame
  CLR(UCSR0B, 2);
  SET(UCSR0C, 2);
  SET(UCSR0C, 1);

  CLR(UCSR0C, 0);

  /* Timer 2 configuration. */
  TCCR2A = 0;
  TCCR2B = 0;
  OCR2A = 0;

  SET(TCCR2B, CS20);  // No prescaler
  SET(TCCR2B, CS21);  // No prescaler
  SET(TCCR2B, CS22);  // No prescaler
  SET(TIMSK2, TOIE2); // Overflow interrupt enabled

  SET(TCCR2A, COM2A1);
  SET(TCCR2A, WGM20);
  SET(TCCR2A, WGM21); // Configure timer 2 as fast PWM

  sei(); // Set interruptions on.
}

void send_byte(char c)
{
  UDR0 = c;
}

ISR(TIMER2_OVF_vect) 
{
  counter++;
}

ISR (USART_UDRE_vect)
{
  if (write_flag)
  {
    if (msg[j] == '\0')
    {
      j = 0;
      write_flag = 0;
    }
    else
    {
      send_byte(msg[j]);
      j++;
    }
  }
}

ISR (USART_RX_vect)
{
  buffer_write(UDR0);
}

int main()
{
  setup();
  char c;
  unsigned long int max;
  int led_on = 0;
  int blink_flag = 0;
  unsigned char *func;

  CLR(PORTB, 5);

  while(1)
  {
    if (buffer_len != 0)
    {
      c = buffer_read();

      switch(c)
      {
        case 's':
          func = sin;
          msg = msg_sin;
          write_flag = 1;
          max = 62500; // 1s
          blink_flag = 1;
          break;
        case 't':
          func = tri;
          msg = msg_tri;
          write_flag = 1;
          max = 31250; // 0.5s
          blink_flag = 1;
          break;
        case 'd':
          func = saw;
          msg = msg_saw;
          write_flag = 1;
          max = 15625; // 0.25s
          blink_flag = 1;
          break;
        case 'a':
          func = am;
          msg = msg_am;
          write_flag = 1;
          max = 7813; // 0.125s
          blink_flag = 1;
          break;

        default:
          msg = msg_empty;
          write_flag = 1;
          CLR(PORTB, 5);
          led_on = 0;
          blink_flag = 0;
          break;
      }
    }

    if (blink_flag)
    {
      if (counter >= max)
      {
        if (led_on)
        {
          CLR(PORTB, 5);
          led_on = 0;
        }
        else
        {
          SET(PORTB, 5);
          led_on = 1;
        }
        counter = 0;
      }
      OCR2A = func[(int) (counter*200)/max];
    }
  }
}
