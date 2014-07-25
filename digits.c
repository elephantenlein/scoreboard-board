//--------------------------------------------------
// Модуль управления цифрами
//--------------------------------------------------
#include "globals.h"
#include "digits.h"

//--------------------------------------------------
//const uint8_t dimm_numbers[];

//--------------------------------------------------
volatile uint8_t dimm;

volatile uint8_t mask_a;
volatile uint8_t mask_b;
volatile uint8_t mask_c;
volatile uint8_t mask_d;
volatile uint8_t mask_e;
volatile uint8_t mask_f;
volatile uint8_t mask_g;

//--------------------------------------------------
void digits_init(void)/*{{{*/
{
dimm=0;

mask_a=0x00;
mask_b=0x00;
mask_c=0x00;
mask_d=0x00;
mask_e=0x00;
mask_f=0x00;
mask_g=0x00;

// turn on the lights!
PORTA  = 0xFF;
PORTB  = 0xFF;
PORTC  = 0xFF;
PORTD |= 0xC0;
PORTE  = 0xFF;
PORTF |= 0x7F;
PORTG |= 0x0F;

DDRA  = 0xFF;
DDRB  = 0xFF;
DDRC  = 0xFF;
DDRD |= 0xC0;
DDRE  = 0xFF;
DDRF |= 0x7F;
DDRG |= 0x0F;

// dimmer timer
OCR1A  = 376;

TCCR1A = 0x00;
TCCR1C = 0x00;
TCCR1B = (1<<WGM12) | (1<<CS10);
TIMSK |= (1<<OCIE1A);
}
/*}}}*/
//--------------------------------------------------
void digits_task(void)/*{{{*/
{
if(dimm >= 200)
    {
    PORTA  = 0xFF & mask_a;
    PORTB  = 0xFF & mask_b;
    PORTC  = 0xFF & mask_c;
    PORTD |= 0xC0 & mask_d;
    PORTE  = 0xFF & mask_e;
    PORTF |= 0x7F & mask_f;
    PORTG |= 0x0F & mask_g;

    dimm=0;
    return;
    }

if(dimm >= 52)
    {
    PORTC &= 0xBF;
    }

if(dimm >= 50)
    {
    PORTA &= 0x7F;
    PORTC &= 0x7F;
    PORTD &= 0x3F;
    PORTG &= 0xF8;

    PORTC &= 0xC9;
    }

if(dimm >= 40)
    {
    PORTE &= 0x80;
    }

if(dimm >= 10)
    {
    PORTC &= 0xF6;

    PORTB &= 0x03;
    PORTG &= 0xF7;
    }

if(dimm >= 3)
    {
    PORTA &= 0x80;
    PORTF &= 0x80;

    PORTB &= 0xFE;
    PORTE &= 0x7F;
    }
}
/*}}}*/
//--------------------------------------------------
void digits_set_digit(uint8_t dig, uint8_t pos)/*{{{*/
{
switch(pos)
    {
    case POS_BIG1:/*{{{*/
	mask_a &= 0x7F;
	mask_c &= 0x7F;
	mask_d &= 0x3F;
	mask_g &= 0xF8;
	switch(dig)
	    {
	    case 0: mask_a |= 0x00; mask_c |= 0x80; mask_d |= 0xC0; mask_g |= 0x07; break;
	    case 1: mask_a |= 0x00; mask_c |= 0x00; mask_d |= 0x80; mask_g |= 0x01; break;
	    case 2: mask_a |= 0x80; mask_c |= 0x80; mask_d |= 0xC0; mask_g |= 0x02; break;
	    case 3: mask_a |= 0x80; mask_c |= 0x00; mask_d |= 0xC0; mask_g |= 0x03; break;
	    case 4: mask_a |= 0x80; mask_c |= 0x00; mask_d |= 0x80; mask_g |= 0x05; break;
	    case 5: mask_a |= 0x80; mask_c |= 0x00; mask_d |= 0x40; mask_g |= 0x07; break;
	    case 6: mask_a |= 0x80; mask_c |= 0x80; mask_d |= 0x40; mask_g |= 0x07; break;
	    case 7: mask_a |= 0x00; mask_c |= 0x00; mask_d |= 0xC0; mask_g |= 0x01; break;
	    case 8: mask_a |= 0x80; mask_c |= 0x80; mask_d |= 0xC0; mask_g |= 0x07; break;
	    case 9: mask_a |= 0x80; mask_c |= 0x00; mask_d |= 0xC0; mask_g |= 0x07; break;
	    default: break;
	    };
	break; /*}}}*/
    case POS_BIG2:/*{{{*/
	mask_c &= 0x80;
	switch(dig)
	    {
	    case 0: mask_c |= 0x3F; break;
	    case 1: mask_c |= 0x06; break;
	    case 2: mask_c |= 0x5B; break;
	    case 3: mask_c |= 0x4F; break;
	    case 4: mask_c |= 0x66; break;
	    case 5: mask_c |= 0x6D; break;
	    case 6: mask_c |= 0x7D; break;
	    case 7: mask_c |= 0x07; break;
	    case 8: mask_c |= 0x7F; break;
	    case 9: mask_c |= 0x6F; break;
	    default: break;
	    };
	break;/*}}}*/
    case POS_BIG3:/*{{{*/
	mask_e &= 0x80;
	switch(dig)
	    {
	    case 0: mask_e |= 0x3F; break;
	    case 1: mask_e |= 0x06; break;
	    case 2: mask_e |= 0x5B; break;
	    case 3: mask_e |= 0x4F; break;
	    case 4: mask_e |= 0x66; break;
	    case 5: mask_e |= 0x6D; break;
	    case 6: mask_e |= 0x7D; break;
	    case 7: mask_e |= 0x07; break;
	    case 8: mask_e |= 0x7F; break;
	    case 9: mask_e |= 0x6F; break;
	    default: break;
	    };
	break;/*}}}*/
    case POS_BIG4:/*{{{*/
	mask_b &= 0x03;
	mask_g &= 0xF7;
	switch(dig)
	    {
	    case 0: mask_b |= 0xFC; break;
	    case 1: mask_b |= 0x18; break;
	    case 2: mask_b |= 0x6C; mask_g |= 0x08; break;
	    case 3: mask_b |= 0x3C; mask_g |= 0x08; break;
	    case 4: mask_b |= 0x98; mask_g |= 0x08; break;
	    case 5: mask_b |= 0xB4; mask_g |= 0x08; break;
	    case 6: mask_b |= 0xF4; mask_g |= 0x08; break;
	    case 7: mask_b |= 0x1C; break;
	    case 8: mask_b |= 0xFC; mask_g |= 0x08; break;
	    case 9: mask_b |= 0xBC; mask_g |= 0x08; break;
	    default: break;
	    };
	break;/*}}}*/
    case POS_SMALL1:/*{{{*/
	mask_a &= 0x80;
	switch(dig)
	    {
	    case 0: mask_a |= 0x3F; break;
	    case 1: mask_a |= 0x06; break;
	    case 2: mask_a |= 0x5B; break;
	    case 3: mask_a |= 0x4F; break;
	    case 4: mask_a |= 0x66; break;
	    case 5: mask_a |= 0x6D; break;
	    case 6: mask_a |= 0x7D; break;
	    case 7: mask_a |= 0x07; break;
	    case 8: mask_a |= 0x7F; break;
	    case 9: mask_a |= 0x6F; break;
	    default: break;
	    };
	break;/*}}}*/
    case POS_SMALL2:/*{{{*/
	switch(dig)
	    {
	    case 0: mask_f = 0x3F; break;
	    case 1: mask_f = 0x06; break;
	    case 2: mask_f = 0x5B; break;
	    case 3: mask_f = 0x4F; break;
	    case 4: mask_f = 0x66; break;
	    case 5: mask_f = 0x6D; break;
	    case 6: mask_f = 0x7D; break;
	    case 7: mask_f = 0x07; break;
	    case 8: mask_f = 0x7F; break;
	    case 9: mask_f = 0x6F; break;
	    default: break;
	    };
	break;/*}}}*/
    case POS_DOTS:/*{{{*/
	mask_b &= 0xFE;
	mask_e &= 0x7F;
	switch(dig)
	    {
	    case 0: break;
	    case 1: mask_e |= 0x80; break;
	    case 2: mask_b |= 0x01; break;
	    case 3: mask_b |= 0x01; mask_e |= 0x80; break;
	    default: break;
	    };
	break;/*}}}*/
    default:
	break;
    };
}
/*}}}*/
//--------------------------------------------------
void digits_set_board(uint16_t d1, uint8_t d2, uint8_t d3, uint16_t d4)/*{{{*/
{
uint8_t a1, a2;

a1=   d1/10;
a2=d1-a1*10;
digits_set_digit(a1, POS_BIG1);
digits_set_digit(a2, POS_BIG2);

digits_set_digit(d2, POS_SMALL1);
digits_set_digit(d3, POS_SMALL2);

a1=   d4/10;
a2=d4-a1*10;
digits_set_digit(a1, POS_BIG3);
digits_set_digit(a2, POS_BIG4);
}
/*}}}*/
//--------------------------------------------------
ISR(TIMER1_COMPA_vect)/*{{{*/
{
dimm++;
}
/*}}}*/
//--------------------------------------------------
