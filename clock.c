//--------------------------------------------------
// Модуль управления часами реального времени
//--------------------------------------------------
#include <util/delay.h>

#include "globals.h"
#include "clock.h"
#include "debug.h"

//--------------------------------------------------
#define CS_SET   PORTB |= (1<<PB1)
#define CS_RST   PORTB &=~(1<<PB1)

#define CLK_SET  PORTF |= (1<<PF7)
#define CLK_RST  PORTF &=~(1<<PF7)

#define DATA_SET PORTG |= (1<<PG4)
#define DATA_RST PORTG &=~(1<<PG4)

//--------------------------------------------------
void clock_init_hw()/*{{{*/
{
DDRB |= (1<<DDB1);
DDRF |= (1<<DDF7);
//DDRG |= (1<<DDG4);
DDRG &=~(1<<DDG4);

CS_RST;
CLK_RST;
DATA_RST;
}
/*}}}*/
//--------------------------------------------------
void clock_write_byte(const uint8_t addr, const uint8_t byte)/*{{{*/
{
uint8_t tmp;
tmp=addr << 1;
tmp &= 0xFE; // mark write operation
tmp |= 0x80; // must-have

// send address
CS_RST;
CLK_RST;
DATA_RST;
//DDRG |= (1<<PG4);

CS_SET;
for(uint8_t i=0; i<8; i++)
    {
    CLK_RST;
    if(tmp & 0x01)
	DATA_SET;
    else
	DATA_RST;

    _delay_us(10);
    tmp >>= 1;
    CLK_SET;
    _delay_us(10);
    }

// send data
tmp=byte;
for(uint8_t i=0; i<8; i++)
    {
    CLK_RST;
    if(tmp & 0x01)
	DATA_SET;
    else
	DATA_RST;

    _delay_us(10);
    tmp >>= 1;
    CLK_SET;
    _delay_us(10);
    }

CS_RST;
CLK_RST;
DATA_RST;
}
/*}}}*/
//--------------------------------------------------
uint8_t clock_read_byte(const uint8_t addr)/*{{{*/
{
uint8_t tmp;

tmp=addr << 1;
tmp |= 0x01; // mark read operation
tmp |= 0x80; // must-have

// send
CS_RST;
CLK_RST;
DATA_RST;
//DDRG |= (1<<PG4);

CS_SET;
for(uint8_t i=0; i<8; i++)
    {
    CLK_RST;
    if(tmp & 0x01)
	DATA_SET;
    else
	DATA_RST;

    _delay_us(10);
    tmp >>= 1;
    CLK_SET;
    _delay_us(10);
    }

DATA_RST;
DDRG &=~(1<<PG4);
for(uint8_t i=0; i<8; i++)
    {
    CLK_RST;
    tmp <<= 1;
    _delay_us(5);
    if(PING & (1<<PG4))
	tmp |= 1;
    else
	tmp &=~1;

    _delay_us(5);
    CLK_SET;
    _delay_us(10);
    }

CS_RST;
CLK_RST;
DATA_RST;
return tmp;
}
/*}}}*/
//--------------------------------------------------
