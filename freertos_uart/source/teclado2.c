/*
 * teclado2.c
 *
 *  Created on: 18/11/2020
 *      Author: USUARIO
 */


#include "teclado2.h"

extern QueueHandle_t  teclado_caracteres;
char obtener_tecla(void);
char tecla;
char teclas[4][3] = {
    {'1', '2', '3'},
    {'4', '5', '6'},
    {'7', '8', '9'},
    {'*', '0', '#'}
};

int Filas[4] = {BOARD_INITPINS_Fila1_PIN, BOARD_INITPINS_Fila2_PIN, BOARD_INITPINS_Fila3_PIN, BOARD_INITPINS_Fila4_PIN};
int Columnas[3] = {BOARD_INITPINS_Col1_PIN, BOARD_INITPINS_Col2_PIN,BOARD_INITPINS_Col3_PIN};
int teclas_mem[4][3] = {
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0}
};
int val, val2, val3, val4;
char obtener_tecla(void){

    for (int i = 0; i < 4; i++)
    {
        vTaskDelay(20 / portTICK_PERIOD_MS);
        GPIO_WritePinOutput(GPIOD,Filas[i], 1);
        for (int j = 0; j < 3; j++)
        {
            val = GPIO_ReadPinInput(GPIOB,Columnas[j]);
            if(!teclas_mem[i][j]&& val){

            	GPIO_WritePinOutput(GPIOD,Filas[i], 0);
            	teclas_mem[i][j] = val;
                return(teclas[i][j]);
            }
            teclas_mem[i][j] = val;
        };
        GPIO_WritePinOutput(GPIOD,Filas[i], 0);
    };
    return('n');
}


void teclado2(void *pvParameter){
    BaseType_t xHigherPriorityTaskWoken;
	xHigherPriorityTaskWoken = pdTRUE;
 //   int val;
	while(1)
    {
    //	 val = GPIO_ReadPinInput(GPIOB,BOARD_INITPINS_Col1_PIN);
    	//printf("eNTRO %c\n");
        tecla = obtener_tecla();
       // printf("oooooooo La tecla oprimida fue: %c\n", tecla);
        //printf("(TECLADO)\n");

        if (tecla != 'n'){

            xQueueSend(teclado_caracteres,(void *)&tecla, &xHigherPriorityTaskWoken);
            //printf("La tecla oprimida fue: %c\n", tecla);
        }

    }

}


