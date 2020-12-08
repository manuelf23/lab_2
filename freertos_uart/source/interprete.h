
#if !defined(INTERPRETE_H)

#define INTERPRETE_H
#include <stdio.h>
#include <string.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "stdio.h"
#include "board.h"
#include "pin_mux.h"
#include "semphr.h"

#define ENT_1 3 // Tiene una resistencia de pullup interna
#define ENT_2 36
#define ENT_3 18
#define ENT_4 4
#define ENT_5 27
#define ENT_6 14
#define ENT_7 13

typedef struct Level Level;
struct Level
{
   int anterior;
   int actual;
};

typedef struct Evento Evento;
struct Evento
{
   char tipo;
   int  duracion;
   int  duracion_cont;
   int estado;
};

typedef struct Entrada Entrada;
struct Entrada
{
   int gpio;
   char alarma;
   Evento even;
   Level lvl;
};

#endif
