#include "my_lcd.h"

//extern void start_lcd();

void my_lcd(void *pvParameter){
	start_lcd();

	vTaskDelay(2000 / portTICK_PERIOD_MS);
	while(1)
    {

		lcd_data_write("LCD", LCD_LINE1, UNSCROLL);
		__asm volatile ("nop");

    }

}
