#include "main.h"
#define PRIORIDAD (tskIDLE_PRIORITY)

SemaphoreHandle_t mutex_time= NULL;
SemaphoreHandle_t mutex_time_char= NULL;
SemaphoreHandle_t mutex_monitoreo = NULL;

QueueHandle_t  q_config_entradas = NULL;
QueueHandle_t  q_config_salidas = NULL;

QueueHandle_t  q_interprete;
QueueHandle_t  cola_caracteres;

QueueHandle_t  teclado_caracteres = NULL;

QueueHandle_t  q_interprete= NULL;

int main(void) {



  	/* Init board hardware. */
                                                                                                                                                                                                                                                          BOARD_InitBootPins();

       BOARD_InitBootPins();
       BOARD_InitBootClocks();
       BOARD_InitBootPeripherals();

       q_interprete = xQueueCreate(50,sizeof(char[25]));
       q_config_entradas = xQueueCreate(50,sizeof(char[25]));
       q_config_salidas = xQueueCreate(50,sizeof(char[25]));
	mutex_time=xSemaphoreCreateMutex();
	mutex_time_char=xSemaphoreCreateMutex();
	mutex_monitoreo = xSemaphoreCreateMutex();
	teclado_caracteres = xQueueCreate(27,sizeof(char));

#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
    /* Init FSL debug console. */
    BOARD_InitDebugConsole();
#endif

    void display(void *pvParameters );
      if (xTaskCreate(display, "my_lcd", configMINIMAL_STACK_SIZE + 50, NULL, PRIORIDAD + 2, NULL) != pdPASS)
      	 {
      		 PRINTF("my_lcd Task creation failed!.\r\n");
      	 }

    void teclado2(void *pvParameters );
    if (xTaskCreate(teclado2, "Teclado", configMINIMAL_STACK_SIZE + 10, NULL, PRIORIDAD + 1, NULL) != pdPASS)
	 {
		 PRINTF("Teclado Task creation failed!.\r\n");
	 }

    void interprete_task(void *pvParameters );
        if (xTaskCreate(interprete_task, "interprete", configMINIMAL_STACK_SIZE + 10, NULL, PRIORIDAD + 3, NULL) != pdPASS)
    	 {
    		 PRINTF("interprete Task creation failed!.\r\n");
    	 }

    void rtc(void *pvParameters );
    xTaskCreate(rtc, "rtc", configMINIMAL_STACK_SIZE + 10, NULL, PRIORIDAD + 4, NULL);

  // void comunicacion(void *pvParameters );
  // xTaskCreate(comunicacion, "comunicacion", configMINIMAL_STACK_SIZE + 10, NULL, 3, NULL);



    vTaskStartScheduler();
       for (;;)
           ;

}







