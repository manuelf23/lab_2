/*
 * teclado2.h
 *
 *  Created on: 18/11/2020
 *      Author: USUARIO
 */

#ifndef TECLADO2_H_
#define TECLADO2_H_

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "stdio.h"
#include "board.h"
#include "pin_mux.h"


extern QueueHandle_t  teclado_caracteres;

#endif /* INCLUDE_TECLADO_H_ */
