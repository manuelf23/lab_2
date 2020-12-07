/*
 * lcd.h
 *
 *  Created on: 4/12/2020
 *      Author: USUARIO
 */

#ifndef LCD_LIB_H_
#define LCD_LIB_H_


/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "stdio.h"
#include "board.h"
#include "pin_mux.h"
#include <stdint.h>

/**************************************************
Data Structures and Typedefs
***************************************************/

typedef enum
{
	LCD_LINE1,
	LCD_LINE2
}lcd_line;

typedef enum{
	SCROLL,
	UNSCROLL
}lcd_scrolling;

/**************************************************
Global Function Prototypes
***************************************************/
//extern void start_lcd(void);
extern void display_lcd(char *msg, lcd_line line);
extern void lcd_data_write(char *data, lcd_line line, lcd_scrolling scroll_type);
extern void lcd_byte_write(uint8_t input, uint8_t len);
extern uint8_t lcd_string_write(char **pmsg);
extern void time_update_lcd(void);

#endif  // end of _DISPLAY_H
