#include "main.h"

SemaphoreHandle_t mutex_time= NULL;
SemaphoreHandle_t mutex_time_char= NULL;

QueueHandle_t  q_interprete;
QueueHandle_t  cola_caracteres;

int main(void) {



  	/* Init board hardware. */
                                                                                                                                                                                                                                                          BOARD_InitBootPins();

       BOARD_InitBootPins();
       BOARD_InitBootClocks();
       BOARD_InitBootPeripherals();

	mutex_time=xSemaphoreCreateMutex();
	mutex_time_char=xSemaphoreCreateMutex();

#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
    /* Init FSL debug console. */
    BOARD_InitDebugConsole();
#endif

    //void teclado2(void *pvParameters );
    //xTaskCreate(teclado2, "Teclado", configMINIMAL_STACK_SIZE + 10, NULL, 1, NULL);

    //void rtc(void *pvParameters );
    //xTaskCreate(rtc, "rtc", configMINIMAL_STACK_SIZE + 10, NULL, 2, NULL);

  // void comunicacion(void *pvParameters );
  // xTaskCreate(comunicacion, "comunicacion", configMINIMAL_STACK_SIZE + 10, NULL, 3, NULL);

  void my_lcd(void *pvParameters );
  xTaskCreate(my_lcd, "my_lcd", 200, NULL, 4, NULL);

    vTaskStartScheduler();
       for (;;)
           ;

}







