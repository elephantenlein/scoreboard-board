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
const uint8_t max_at_try=5;

//--------------------------------------------------
volatile char hc05_buff[HC_BUFF_SIZE];
volatile uint8_t buff_fill, hc05_i;

//--------------------------------------------------
enum hc05_state hc05_init(void)/*{{{*/
{
uint8_t i=0;
for(i=0; i<HC_BUFF_SIZE; i++)
    hc05_buff[i]=0;
buff_fill=0;
hc05_i=0;

// pins
DDRD  |= (1<<DDD0) | (1<<DDD3) | (1<<DDD4);
PORTD |= (1<<PD2)  | (1<<PD3)  | (1<<PD4);

// start out at 38400 8N1
UBRR1L = 12;
UBRR1H = 0;

UCSR1C = (1<<UCSZ10) | (1<<UCSZ11);
UCSR1B = (1<<TXEN1)  | (1<<RXEN1);

_delay_ms(10);
HC_SET_RESET;
_delay_ms(10);
HC_CLR_RESET;
_delay_ms(1000);

HC_AT_MODE;
for(i=0; i<max_at_try; i++)/*{{{*/
    {
    _delay_ms(100);

    hc05_send("AT\r\n");
    if(!hc05_recv())
	{
	if(i % 2 == 0)
	    { // try 9600
	    UBRR1L = 50; // tuned by-hand
	    UBRR1H = 0;
	    }
	else
	    { // try 38400
	    UBRR1L = 12;
	    UBRR1H = 0;
	    }
	continue;
	}

    if(hc05_buff[0] == 'O' &&
       hc05_buff[1] == 'K')
	break;
    }

if(i >= max_at_try)
    return HC05_OFFLINE;
/*}}}*/
DBG1_SET;
/*
hc05_send("AT+VERSION?\r\n");
if(!hc05_recv())
    return HC05_OFFLINE;
if(!hc05_recv())
    return HC05_OFFLINE;
    */

hc05_send("AT+UART=9600,0,0\r\n");
if(!hc05_recv())
    return HC05_OFFLINE;

hc05_send("AT+NAME=\"Scoreboard-board\"\r\n");
if(!hc05_recv())
    return HC05_OFFLINE;

hc05_send("AT+ROLE=0\r\n");
if(!hc05_recv())
    return HC05_OFFLINE;

hc05_send("AT+CMODE=1\r\n");
if(!hc05_recv())
    return HC05_OFFLINE;

UCSR1A |= (1<<RXC1);
UCSR1B |= (1<<RXCIE1);
HC_COMM_MODE;
return HC05_PAIRED;
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
bool hc05_recv(void)/*{{{*/
{
const uint16_t max_to=1000;

uint8_t rcv;
uint16_t to;

UCSR1A |= (1<<RXC1);
while(1)
    {
    to=0;
    while(~UCSR1A & (1<<RXC1))
	{
	_delay_us(100);
	to++;
	if(to >= max_to)
	    break;
	};

    if(to >= max_to)
	break;

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

if(to >= max_to)
    return false;
else
    return true;
}
/*}}}*/
//--------------------------------------------------
bool hc05_recv_complete(void)/*{{{*/
{
if(buff_fill == 0)
    return false;

buff_fill=0;
return true;
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
