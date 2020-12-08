/*
 * lcd.h
 *
 *  Created on: 6/12/2020
 *      Author: USUARIO
 */

#ifndef DISPLAY_H_
#define DISPLAY_H_

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "stdio.h"
#include "board.h"
#include "pin_mux.h"
#include "lcd_lib.h"
#include "semphr.h"
#include "rtc.h"
#include "teclado2.h"
#define HWM 48//HIGH WATER MARK for XON XOFF
#define LWM 16//LOW WATER MARK for XON XOFF
#define EX_UART_NUM UART_NUM_2
#define TAM_SECUENCIA 25


#endif /* DISPLAY_H_ */
