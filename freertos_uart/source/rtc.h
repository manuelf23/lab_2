
#if !defined(RTC_H)

#define RTC_H
#include <stdio.h>
#include <string.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"


#define factor 60 // minuto como un segundo
// #define factor 1
#define SEG (1000/factor)/portTICK_PERIOD_MS
#define MIN SEG*60


#define SI	1
#define NO	0

typedef struct Fecha Fecha;
struct Fecha
   {
   int anio;
   int  mes,
         dia;
   };
   
typedef struct Hora Hora;
struct Hora
   {
   int  hora,
         minuto,
         segundo;
   };
   
typedef struct Tiempo Tiempo;
struct Tiempo
   {
   Fecha f;
   Hora h;
   int s;  /* Dia de la semana: 0 es lunes, 6 es domingo */
   };

   extern char tiempo_char[12];
   extern char change_time; 
   extern struct Tiempo tiempo;

#endif
