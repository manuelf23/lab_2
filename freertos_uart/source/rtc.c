#include "rtc.h"

// #define SEG 50/portTICK_PERIOD_MS

extern SemaphoreHandle_t mutex_time;
extern SemaphoreHandle_t mutex_time_char;
struct Tiempo tiempo;
char tiempo_char[] = "201911101947"; // se utiliza cuando el usuario cambia la fecha
int tiempo_int[] = {9999,99,99,99,99, 6}; // se utiliza para que otras tareas consulten la hora
char change_time = 1;// se utiliza cunado el usuario cambia la fecha (enable)


int codigo_siglo(int anio)
{

    int m = anio / 1000;
    int c = anio - (m * 1000);
    int d = c / 100;
    if (c%100 != 0)
    {
        d ++;
    }
    int siglo = (m * 10) + d;
    int bloque = siglo / 4;
    int nc = siglo - (4 * bloque);
    int codigo;
    if(nc == 0)
    {
        codigo = nc;
    }else if (nc == 1)
    {
        codigo = 6;
    }else if (nc == 2)
    {
        codigo = 4;
    }else
    {
        codigo = 2;
    }
    return codigo;
}
int codigo_mes(int mes)
{
    int codigo;
    if(mes == 1 || mes == 10)
    {
        codigo = 0;
    }
    if(mes == 5)
    {
        codigo = 1;
    }
    if(mes == 8)
    {
        codigo = 2;
    }
    if(mes == 2 || mes == 3 || mes == 11)
    {
        codigo = 3;
    }
    if(mes == 6)
    {
        codigo = 4;
    }
    if(mes == 9 || mes == 12)
    {
        codigo = 5;
    }
    if(mes == 4 || mes == 7)
    {
        codigo = 6;
    }
    return codigo;
}

int dia_semana(int dia, int mes, int anio)
{
    int codigo_s = codigo_siglo(anio);
    int codigo_m = codigo_mes(mes);
    int anio2ud;
    int m = anio / 1000;
    int c = anio - (m * 1000);
    int d = c / 100;
    anio2ud = c - (d * 100);
    int x = dia + codigo_m + anio2ud + (anio2ud/4) + codigo_s;
    int codigo_dia = x % 7;
    if(codigo_dia == 0)
    {
        codigo_dia = 7;
    }
    codigo_dia --;
    return codigo_dia;
}

void rtc(void *pvParameter){
    char anio[5] = {'1', '1', '1', '1', '\0'};
    char mes[3] = {'1', '1', '\0'};
    char dia[3] = {'1', '1', '\0'};
    char hora[3] ;
    char minuto[3] ;
    char segundo[3] ;
    
    TickType_t xLastWakeTime_a;
    xLastWakeTime_a = xTaskGetTickCount ();
    int dias_mes[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    while(1){
        vTaskDelayUntil( &xLastWakeTime_a, SEG );
        if( xSemaphoreTake( mutex_time_char, ( TickType_t ) portMAX_DELAY ) == pdTRUE )
        {
            if(change_time){
                for (int i = 0; i < 4; i++)
                {
                    anio[i] = tiempo_char[i];
                    
                }
                for (int i = 0; i < 2; i++)
                {   
                    mes[i] = tiempo_char[i + 4];
                    dia[i] = tiempo_char[i + 6];
                    hora[i] = tiempo_char[i + 8];
                    minuto[i] = tiempo_char[i + 10]; 
                }
                tiempo.f.anio = (int) strtol(anio, (char **)NULL, 10);
                tiempo.f.mes = (int) strtol(mes, (char **)NULL, 10);
                if(!tiempo.f.mes || tiempo.f.mes >12)
                {
                    tiempo.f.mes = 1;
                }
                tiempo.f.dia = (int) strtol(dia, (char **)NULL, 10);
                if(!tiempo.f.dia || tiempo.f.dia >31)
                {
                    tiempo.f.dia = 1;
                }
                tiempo.h.hora = (int) strtol(hora, (char **)NULL, 10);
                if(!tiempo.h.hora || tiempo.h.hora >24)
                {
                    tiempo.h.hora = 0;
                }
                tiempo.h.minuto = (int) strtol(minuto, (char **)NULL, 10);
                if(!tiempo.h.minuto || tiempo.h.minuto >59)
                {
                    tiempo.h.minuto = 0;
                }
                // tiempo.h.segundo = 0;
                tiempo.s = dia_semana(tiempo.f.dia, tiempo.f.mes, tiempo.f.anio);
                change_time = 0;

                // printf("%s/%s/-%s:%s:%s\n", mes, dia, hora, minuto, segundo);
                // printf("%d/%d/%d/-%d:%d:%d\n", tiempo.f.anio, tiempo.f.mes, tiempo.f.dia, tiempo.h.hora, tiempo.h.minuto, tiempo.h.segundo);
                
            }
            xSemaphoreGive( mutex_time_char );
        }
       
        if(tiempo.f.anio % 4 == 0){
            if(tiempo.f.anio % 100 != 0 || tiempo.f.anio % 400 == 0){
                dias_mes[1] = 29;
            }else{
                dias_mes[1] = 28;
            } 
        }else{
            dias_mes[1] = 28;
        }
        
        tiempo.h.segundo ++;
        if(tiempo.h.segundo > 59){
           tiempo.h.segundo = 0;
           tiempo.h.minuto ++; 
        }
        if(tiempo.h.minuto > 59){
            tiempo.h.minuto = 0;
            tiempo.h.hora ++; 
        }
        if(tiempo.h.hora > 23){
            tiempo.h.hora = 0;
            tiempo.f.dia ++; 
        }
        if (tiempo.f.dia > dias_mes[tiempo.f.mes - 1]){
            tiempo.f.dia = 1;
            tiempo.f.mes ++;
        }
        if(tiempo.f.mes > 12){
           tiempo.f.mes = 1;
           tiempo.f.anio ++; 
        }
        tiempo.s = dia_semana(tiempo.f.dia, tiempo.f.mes, tiempo.f.anio);
        // tiempo.s = 0;
        if( xSemaphoreTake( mutex_time, ( TickType_t ) portMAX_DELAY ) == pdTRUE )
        {
            // tiempo_int = {, tiempo.f.mes, tiempo.f.dia, tiempo.h.hora, tiempo.h.minuto};
            tiempo_int[0] = tiempo.f.anio;
            tiempo_int[1] = tiempo.f.mes;
            tiempo_int[2] = tiempo.f.dia;
            tiempo_int[3] = tiempo.h.hora;
            tiempo_int[4] = tiempo.h.minuto;
            tiempo_int[5] = tiempo.s;
            //printf("%d/%d/%d/-%d:%d\n", tiempo_int[0], tiempo_int[1], tiempo_int[2], tiempo_int[3], tiempo_int[4]);
            xSemaphoreGive( mutex_time );
        }
        // strcpy(tt, seg);
        // strcat(tt, min);
        // printf("segundos: %s \n", seg);
        // printf("tt: %s \n", tt);
       // printf("%d/%d/%d %d:%d:%d \n", tiempo.f.anio, tiempo.f.mes, tiempo.f.dia, tiempo.h.hora, tiempo.h.minuto, tiempo.h.segundo);
    }
}
