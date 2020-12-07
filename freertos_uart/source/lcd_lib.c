/*
 * lcd.c
 *
 *  Created on: 18/11/2020
 *      Author: USUARIO
 */
#include <lcd_lib.h>
#include <stdint.h>
//#include "BSPInit.h"
#include "isr.h"

/**************************************************
Data Structures and Typedefs
***************************************************/
// Time structure
typedef struct
{
	uint8_t hour; // 0-12
	uint8_t min;  // 0-60
	uint8_t sec;  // 0-60
}time_str;
/**************************************************
Local Function Prototypes
***************************************************/
static void lcd_Cmd(uint8_t cmd);
static void write_nibble(uint8_t nibble);
static void delay(uint16_t cnt);

/**************************************************
File Global (Static) Variables
***************************************************/
static uint8_t scroll_data_stored;
static char lcd_scroll_msg[50]; // msg to be scrolled

/**************************************************
* @function: start_lcd
* @brief:    Init and Start the LCD with standard
*            LCD commands
* @param:    none
* @return:	 none
***************************************************/
void start_lcd(void)
{
	lcd_Cmd(0x02);  // Move the cursor to original position
    lcd_Cmd(0x28);  // Enable 4-bit, 2 line, 5x7 dots mode for chars
	lcd_Cmd(0x0C);  // Display ON, Cursor OFF
	lcd_Cmd(0x01);  // Clear Display
	delay(1000);
}


void lcd_data_write(char *data, lcd_line line, lcd_scrolling scroll_type)
{
	uint8_t char_written; // actual number of characters
	                      // written on LCD
	char *temp = " ";     // Space to be filled for
	                      // unused blocks on Line
	char *pscroll_msg;    // Pointing to scrolling msg
	uint8_t i;
	char tmp;
	static uint8_t scroll_chars;

	// Enter here if data to be displayed without scrolling OR
	// Data to be scrolled on LCD needs to be stored for first time
	 //printf("estoy en lcd_data_write: %c\n", data);

	if (line == LCD_LINE1)
	{
		lcd_Cmd(0x80);   // Move the cursor to beginning
										 // of first line
	}
	else
	{
		lcd_Cmd(0xC0);   // Move the cursor to beginning
										 // of second line
	}

	if ((scroll_type == UNSCROLL) || (!scroll_data_stored))
	{
		// Write data string to LCD (in returns gets how many
		// characters are written)
		char_written = lcd_string_write(&data);
		if (scroll_type == SCROLL)
		{
			scroll_chars = char_written;

			// Fill remaining scroll message
			while (*data && scroll_chars < 49)
			{
				lcd_scroll_msg[scroll_chars] = *data++;
				scroll_chars++;
			}

			// End the scroll message
			lcd_scroll_msg[scroll_chars] = '\0';

			// Indicate data to be scrolled is stored
			scroll_data_stored = 1;
		}
		else
		{
			// Fill remaining blocks in the line with space
			// (unscroll message)
			while (char_written < 16)
			{
				lcd_string_write(&temp);
				char_written++;
			}
		}
	}
	else // If message is to be scrolled
	{
		// Get the first character of message (to be stored
		// at the end of message to form a circular loop)
		tmp = lcd_scroll_msg[0];

		// Shift the message by 1 character to left
		for (i=0; i<scroll_chars; i++)
		{
			lcd_scroll_msg[i] = lcd_scroll_msg[i+1];
		}

		// Store first character at the end (loopback)
		lcd_scroll_msg[scroll_chars-1] = tmp;

		// Display shifted message (The message is shifted
		// to left by 1 character every 300msec to get a
		// feeling that it is actually scrolling from right
		// to left)
		pscroll_msg = lcd_scroll_msg;
		(void)lcd_string_write(&pscroll_msg);
	}
}


uint8_t lcd_string_write(char **str)
{
	uint8_t cnt = 0; // Counting string length

	// Write until entire string (msg) is finished
	// (max 16 char per line)
	//printf("estoy en lcd_string_write: %c\n");
	while (**str && cnt < 16)
	{
		if (!scroll_data_stored)
		{
			// Store the message be scrolled (character by character)
			lcd_scroll_msg[cnt] = **str;
		}

		GPIO_WritePinOutput(GPIOC,BOARD_INITPINS_RS_PIN, 1); // Select data register    (RS=HIGH)
		GPIO_WritePinOutput(GPIOC,BOARD_INITPINS_RW_PIN, 0); // Select write operation  (RW=LOW)

		write_nibble(**str & 0xF0); // Write Upper nibble

    // Generate a High-to-low pulse on EN pin to latch the data
	GPIO_WritePinOutput(GPIOC,BOARD_INITPINS_E_PIN, 1);   // EN=HIGH
    delay(10000);           // Wait for ~1msec
    GPIO_WritePinOutput(GPIOC,BOARD_INITPINS_E_PIN, 0); // EN=LOW
    delay(10000);           // Wait for ~1msec

		write_nibble((**str << 4) & 0xF0); // Write Lower nibble

    // Generate a High-to-low pulse on EN pin to latch the data
	GPIO_WritePinOutput(GPIOC,BOARD_INITPINS_E_PIN, 1);   // EN=HIGH
    delay(10000);           // Wait for ~1msec
    GPIO_WritePinOutput(GPIOC,BOARD_INITPINS_E_PIN, 0);  // EN=LOW
    //delay(10000);           // Wait for ~1msec

		(*str)++; // Go to next character
		cnt++;     // Increment string length check count
	}

	return cnt;
}

static void lcd_Cmd(uint8_t cmd)
{

		GPIO_WritePinOutput(GPIOC,BOARD_INITPINS_RS_PIN, 0); // Select command register (RS=LOW)
		GPIO_WritePinOutput(GPIOC,BOARD_INITPINS_RW_PIN, 0); // Select write operation  (RW=LOW)

		write_nibble(cmd & 0xF0); // Write Upper nibble

    // Generate a High-to-low pulse on EN pin to latch the data
	GPIO_WritePinOutput(GPIOC,BOARD_INITPINS_E_PIN, 1);   // EN=HIGH
    delay(10000);           // Wait for ~1msec
    GPIO_WritePinOutput(GPIOC,BOARD_INITPINS_E_PIN, 0);  // EN=LOW
    delay(10000);           // Wait for ~1msec

	write_nibble((cmd << 4) & 0xF0); // Write Lower nibble

    // Generate a High-to-low pulse on EN pin to latch the data
	GPIO_WritePinOutput(GPIOC,BOARD_INITPINS_E_PIN, 1);   // EN=HIGH
    delay(10000);           // Wait for ~1msec
    GPIO_WritePinOutput(GPIOC,BOARD_INITPINS_E_PIN, 0);  // EN=LOW
    delay(10000);           // Wait for ~1msec
}



static void write_nibble(uint8_t nibble)
{
	//printf("estoy en write_niblle: %c\n");
		//uint32_t gpio_temp;

	//	Get GPIO-C Data Register;
		//gpio_temp = GPIOC->PDOR;

	  // Update the LCD data line DB7
		if (nibble & 0x80){

            //printf("Llegue aqui: %c\n");

			GPIO_WritePinOutput(GPIOC,BOARD_INITPINS_DB7_PIN, 1);
		}
		else{
			GPIO_WritePinOutput(GPIOC,BOARD_INITPINS_DB7_PIN, 0);
		}

		// Update the LCD data line DB6
		if (nibble & 0x40){
			GPIO_WritePinOutput(GPIOC,BOARD_INITPINS_DB6_PIN, 1);		}
		else{
			GPIO_WritePinOutput(GPIOC,BOARD_INITPINS_DB6_PIN, 0);
		}

		// Update the LCD data line DB5
		if (nibble & 0x20){
			GPIO_WritePinOutput(GPIOC,BOARD_INITPINS_DB5_PIN, 1);		}
		else{
			GPIO_WritePinOutput(GPIOC,BOARD_INITPINS_DB5_PIN, 0);
		}

		// Update the LCD data line DB4
		if (nibble & 0x10){
			GPIO_WritePinOutput(GPIOC,BOARD_INITPINS_DB4_PIN, 1);		}
		else{
			GPIO_WritePinOutput(GPIOC,BOARD_INITPINS_DB4_PIN, 0);
		}
/*
		if (nibble & 0x08){
			GPIO_WritePinOutput(GPIOC,BOARD_INITPINS_DB3_PIN, 1);		}
		else{
			GPIO_WritePinOutput(GPIOC,BOARD_INITPINS_DB3_PIN, 0);
		}
		if (nibble & 0x04){
			GPIO_WritePinOutput(GPIOC,BOARD_INITPINS_DB2_PIN, 1);		}
		else{
			GPIO_WritePinOutput(GPIOC,BOARD_INITPINS_DB2_PIN, 0);
		}
		if (nibble & 0x02){
			GPIO_WritePinOutput(GPIOA,BOARD_INITPINS_DB1_PIN, 1);		}
		else{
			GPIO_WritePinOutput(GPIOA,BOARD_INITPINS_DB1_PIN, 0);
		}
		if (nibble & 0x01){
			GPIO_WritePinOutput(GPIOA,BOARD_INITPINS_DB0_PIN, 1);		}
		else{
			GPIO_WritePinOutput(GPIOA,BOARD_INITPINS_DB0_PIN, 0);
		}*/

		// Write updated data to GPIO-C Port
		//GPIOC->PDOR = gpio_temp;
}



static void delay(uint16_t cnt)
{
	  int i,j;

		// 1 instruction takes 1/48000000 = 20nsec to execute
	  // at 48Mhz system clock
		for (j=0; j<5; j++)
		{
			for(i=0;i<cnt;i++);
		}
}







