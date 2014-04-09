//--------------------------------------------------
// Модуль управления цифрами
//--------------------------------------------------
#ifndef DIGITS_H
#define DIGITS_H

//--------------------------------------------------
enum _positions {
    POS_BIG1,
    POS_BIG2,
    POS_SMALL1,
    POS_SMALL2,
    POS_BIG3,
    POS_BIG4,

    POS_DOTS
};

//--------------------------------------------------
extern void digits_init(void);
extern void digits_set_digit(uint8_t, uint8_t);
extern void digits_set_board(uint16_t, uint8_t, uint8_t, uint16_t);

#endif
//--------------------------------------------------
