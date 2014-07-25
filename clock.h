//--------------------------------------------------
// Модуль управления часами реального времени
//--------------------------------------------------
#ifndef CLOCK_H
#define CLOCK_H

//--------------------------------------------------
extern void clock_init_hw(void);
extern void clock_write_byte(const uint8_t, const uint8_t);
extern uint8_t clock_read_byte(const uint8_t);

#endif
//--------------------------------------------------
