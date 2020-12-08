#include "interprete.h"
extern QueueHandle_t  q_interprete;
extern QueueHandle_t  q_config_entradas;
extern QueueHandle_t  q_config_salidas;
extern SemaphoreHandle_t mutex_monitoreo;

int monit_global = 0;
void interprete_task(void *pvParameter)
{
    BaseType_t xHigherPriorityTaskWoken;
	xHigherPriorityTaskWoken = pdTRUE;
    char secuencia[25];
    while (true)
    {
        if(xQueueReceive(q_interprete,&secuencia,(TickType_t )(portMAX_DELAY)))
        {
            switch (secuencia[0])
            {
                case 'E':
                    xQueueSend(q_config_entradas,(void *)&secuencia, &xHigherPriorityTaskWoken);
                    break;
                case 'S':
                    xQueueSend(q_config_salidas,(void *)&secuencia, &xHigherPriorityTaskWoken);
                    break;
                case 'M':
                    if( xSemaphoreTake( mutex_monitoreo, ( TickType_t ) portMAX_DELAY ) == pdTRUE )
                    {
                        switch(secuencia[1])
                        {
                            case 'A':
                                monit_global = 1;
                                break;
                            case 'I':
                                monit_global = 0;
                                break;
                            default:
                                break;
                        }
                        xSemaphoreGive( mutex_monitoreo );
                    }
                break;
                default:
                break;
            }
        }
    }
}
