//--------------------------------------------------
#include "globals.h"
#include "hc-05.h"

#include <avr/sleep.h>

//--------------------------------------------------
static void master_init(void);
static void go_to_sleep(void);

//--------------------------------------------------
int main(void)
{
master_init();
hc05_init();
sei();

while(1)
    {
    go_to_sleep();
    };
}

//--------------------------------------------------
static void master_init(void)/*{{{*/
{
MCUCR |= (1<<SM0);
}
/*}}}*/
//--------------------------------------------------
static void go_to_sleep(void)/*{{{*/
{
sleep_enable();
sleep_cpu();
sleep_disable();
}
/*}}}*/
//--------------------------------------------------
