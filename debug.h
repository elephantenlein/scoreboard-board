//--------------------------------------------------
// Отладка
//--------------------------------------------------
#ifndef DEBUG_H
#define DEBUG_H

//--------------------------------------------------
#define DBG_INIT DDRD  |= (1<<DDD5)

//--------------------------------------------------
#define DBG1_SET PORTD |= (1<<PD5)
#define DBG1_TGL PORTD ^= (1<<PD5)
#define DBG1_CLR PORTD &=~(1<<PD5)

#endif
//--------------------------------------------------

