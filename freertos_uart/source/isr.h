#ifndef _ISR_H
#define _ISR_H

#include <stdint.h>

/**************************************************
Global Variables
***************************************************/
extern volatile uint8_t flag_50msec;
extern volatile uint8_t flag_100msec;
extern volatile uint8_t flag_500msec;
extern volatile uint8_t flag_1sec;

#endif // end of _ISR_H

