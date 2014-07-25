//--------------------------------------------------
// Интерфейс к модулю HC-05
//--------------------------------------------------
#ifndef HC_05_H
#define HC_05_H

//--------------------------------------------------
#include <stdbool.h>

//--------------------------------------------------
#define HC_BUFF_SIZE 100

//--------------------------------------------------
enum hc05_state {
    HC05_OFFLINE,
    HC05_NOT_PAIRED,
    HC05_PAIRED
};

//--------------------------------------------------
extern volatile char hc05_buff[HC_BUFF_SIZE];

//--------------------------------------------------
extern enum hc05_state hc05_init(void);
extern void hc05_send(char *);
extern bool hc05_recv(void);
extern bool hc05_recv_complete(void);

#endif
//--------------------------------------------------
