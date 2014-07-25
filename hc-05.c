//--------------------------------------------------
// Интерфейс к модулю HC-05
//--------------------------------------------------
#include "globals.h"
#include "hc-05.h"
#include "debug.h"

#include <util/delay.h>

//--------------------------------------------------
#define HC_SET_RESET PORTD &= ~(1<<PD4)
#define HC_CLR_RESET PORTD |=  (1<<PD4)

#define HC_COMM_MODE PORTD &= ~(1<<PD0)
#define HC_AT_MODE   PORTD |=  (1<<PD0)

//--------------------------------------------------
volatile char hc05_buff[HC_BUFF_SIZE];
volatile uint8_t buff_fill, hc05_i;

//--------------------------------------------------
void hc05_init(void)/*{{{*/
{
uint8_t i=0;
for(i=0; i<HC_BUFF_SIZE; i++)
    hc05_buff[i]=0;
buff_fill=0;
hc05_i=0;

// pins
DDRD  |= (1<<DDD0) | (1<<DDD3) | (1<<DDD4);
PORTD |= (1<<PD2)  | (1<<PD3)  | (1<<PD4);

// uart 9600  8N1
UBRR1L = 50; // tuned from datasheet value (51)
UBRR1H = 0;

UCSR1C = (1<<UCSZ10) | (1<<UCSZ11);
UCSR1B = (1<<TXEN1)  | (1<<RXEN1)  | (1<<RXCIE1);

// reset interrupt
UCSR1A |= (1<<RXC1);

_delay_ms(10);
HC_SET_RESET;
_delay_ms(10);
HC_CLR_RESET;
_delay_ms(1000);

DBG1_SET;
HC_AT_MODE;
hc05_send("AT\r\n");
hc05_recv();
if(hc05_buff[0] != 'O' ||
   hc05_buff[1] != 'K')
    HALT;

/*
hc05_send("AT+VERSION?\r\n");
hc05_recv();
hc05_recv();
*/

hc05_send("AT+NAME=\"Scoreboard-board\"\r\n");
hc05_recv();

hc05_send("AT+ROLE=0\r\n");
hc05_recv();

hc05_send("AT+CMODE=1\r\n");
hc05_recv();

HC_COMM_MODE;
}
/*}}}*/
//--------------------------------------------------
void hc05_send(char *msg)/*{{{*/
{
uint8_t i=0;

buff_fill=0;
do
    {
    while(~UCSR1A & (1<<UDRE1)) {};
    UDR1=msg[i];

    i++;
    if(msg[i] == '\0')
	break;
    }
while(1);

UCSR1A |= (1<<TXC1);
while(~UCSR1A & (1<<TXC1)) {};

hc05_i=0;
}
/*}}}*/
//--------------------------------------------------
void hc05_recv(void)/*{{{*/
{
uint8_t rcv;

UCSR1A |= (1<<RXC1);
while(1)
    {
    while(~UCSR1A & (1<<RXC1)) {};
    rcv=UDR1;
    hc05_buff[hc05_i]=rcv;

    hc05_i++;
    if(hc05_i > HC_BUFF_SIZE)
	hc05_i=0;
    if(rcv == '\n')
	{
	buff_fill=hc05_i;
	break;
	}
    }
}
/*}}}*/
//--------------------------------------------------
uint8_t hc05_rcv_complete(void)/*{{{*/
{
if(buff_fill != 0)
    return 1;

return 0;
}
/*}}}*/
//--------------------------------------------------
ISR(USART1_RX_vect)/*{{{*/
{
uint8_t rcv=UDR1;
hc05_buff[hc05_i]=rcv;

hc05_i++;
if(rcv == '\n' ||
   rcv == '\r')
    {
    buff_fill=hc05_i;
    hc05_i=0;
    }
}
/*}}}*/
//--------------------------------------------------
