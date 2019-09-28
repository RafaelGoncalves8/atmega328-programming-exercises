#include <avr/interrupt.h>

/*Ponteiros para os registradores de interesse definidos como variáveis globais para que possam ser usados em qualquer
escopo do programa*/
unsigned char *p_ddrc;

unsigned char *p_portc;
unsigned char *p_eicra;
unsigned char *p_eimsk;

/* rotina de serviço de interrupção que é executada toda vez que o botão é pressionado (borda de descida no pino INT0)
*/
ISR(INT0_vect)
{
  if ((*p_portc & 1) == 1)
  {
    *p_portc &= 0xFE; /* se o LED está aceso, apaga o LED */
  }
  else
  {
    *p_portc |= 1; /* se o LED está apagado, acende o LED */
  }
}

void setup(void)
{
  p_ddrc = (unsigned char *) 0x27;
  p_portc = (unsigned char *) 0x28;
  p_eicra = (unsigned char *) 0x69;
  p_eimsk = (unsigned char *) 0x3D;
  *p_ddrc |= 1; /* PC0 como saida */
  *p_eicra |= 2; /* INT0 configurada para ser ativada pela borda de descida no pino */
  *p_eimsk |= 1; /* habilita INT0 */
  *p_portc &= 0xFE; /* LED inicialmente apagado (led = 0) */
  sei(); /* seta o flag I no SREG, habilitando as interrupções */
}

int main(void)
{
  setup();
  /* laço infinito */
  while (1)
  {
  }
}
