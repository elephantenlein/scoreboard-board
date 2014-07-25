//--------------------------------------------------
#include <util/delay.h>

#include "globals.h"
#include "digits.h"
#include "hc-05.h"
#include "clock.h"
#include "debug.h"

//--------------------------------------------------
volatile uint16_t h, m, s;

//--------------------------------------------------
static void master_init(void);

//--------------------------------------------------
int main(void)
{
//uint8_t tmp1, tmp2;
h=12;
m=0;
s=0;

master_init();
digits_init();
hc05_init();
//clock_init_hw();
sei();

// initialize clock time
//clock_write_byte(0x07, 0x00); // enable write
//clock_write_byte(0x08, 0x00); // disable charger

/*
tmp1=clock_read_byte(0x00);
if(tmp1 & 0x80)
    clock_write_byte(0x00, 0x80);

_delay_ms(1);

tmp1=clock_read_byte(0x00);
tmp2=tmp1 >> 4;

s=(tmp1 & 0x0F) + 10*(tmp2 & 0x07);

tmp1=clock_read_byte(0x01);
tmp2=tmp1 >> 4;

m=(tmp1 & 0x0F) + 10*(tmp2 & 0x07);

tmp1=clock_read_byte(0x02);
tmp2=tmp1 >> 4;

if(tmp1 & 0x80) // 12h mode
    h=(tmp1 & 0x0F) + (tmp2 & 0x08 ? 10 : 0);
else // 24h mode
    h=(tmp1 & 0x0F) + 10*(tmp2 & 0x03);
    */

digits_set_digit(10, POS_BIG1);
digits_set_digit(10, POS_BIG2);
digits_set_digit(10, POS_BIG3);
digits_set_digit(10, POS_BIG4);
digits_set_digit(10, POS_SMALL1);
digits_set_digit(10, POS_SMALL2);
digits_set_digit(10, POS_DOTS);

while(1)
    {
    digits_task();

    if(hc05_rcv_complete() == 0)
	continue;

    if(hc05_buff[0] == 'H')
	{
	TIMSK &=~(1<<OCIE1A);
	digits_set_digit(1, POS_BIG1);
	digits_set_digit(1, POS_BIG2);
	}
    };
}

//--------------------------------------------------
static void master_init(void)/*{{{*/
{
DBG_INIT;

OCR3A   = 7365;

TCCR3A  = 0x00;
TCCR3C  = 0x00;
TCCR3B  = (1<<WGM32) | (1<<CS32) | (1<<CS30);
ETIMSK |= (1<<OCIE3A);
}
/*}}}*/
//--------------------------------------------------
ISR(TIMER3_COMPA_vect)/*{{{*/
{
DBG1_TGL;

s++;
if(s == 60)
    {
    s=0;
    m++;
    }
if(m == 60)
    {
    m=0;
    h++;
    }
if(h == 24)
    h=0;

digits_set_board(h, 10, 10, m);
digits_set_digit((s % 2)+1, POS_DOTS);
}
/*}}}*/
//--------------------------------------------------
