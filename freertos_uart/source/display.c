#include "display.h"


#define MAIN_MENU 0
#define CONFIG_MENU 1
#define TIME_MENU 2
#define MONITORING_MENU 3
#define OUTS_MENU 4
#define FORCE_ON_MENU 5
#define FORCE_OFF_MENU 6
#define OK_MENU 7

#define cursor_config_menu 14
#define cursor_monitoring_menu 11
#define cursor_force_menu 12
#define cursor_outs_menu 13
#define TIMEOUT 3000/portTICK_PERIOD_MS
// #define TIMEOUT SEG * 3

extern SemaphoreHandle_t mutex_time;
extern SemaphoreHandle_t mutex_time_char;
extern QueueHandle_t  teclado_caracteres;
extern QueueHandle_t  q_interprete;
extern SemaphoreHandle_t mutex_monitoreo;
extern int monit_global;
extern int tiempo_int[6];

extern char change_time;

int tiempo_int_copy[6];
char mem_caracter[13] = "999999999999";
char tiempo_copy_char[13];
char fecha_char[11];
char hora_char[16];
int cursor_time_menu = 1;
int flag_cursor = 0;
const TickType_t xDelay = 1000 / portTICK_PERIOD_MS;
char monitoreo_texto[16] = "3.Monitoring:   ";
char fecha_texto[16]     = ":               ";
char entradas_texto[16]  = "2.Disable :     ";
char salidas_texto[16]   = "2.Force OFF :   ";
char force_on_texto[16]  = "number 0-6 :    ";


char caracter_press[2];
char estado_menu = MAIN_MENU;
static char caracter[20];
char comodin[20];


const char * int2char(int num)
{

    memset(caracter, 0, sizeof caracter);
    memset(comodin, 0, sizeof comodin);

    if (num < 10){
            sprintf(comodin, "%d", num);
            caracter[0] = '0';
            caracter[1] = comodin[0];
    }else
    {
        sprintf(caracter, "%d", num);
    }

    return caracter;
}

void limpiarPantalla(void){
	lcd_data_write("                ", LCD_LINE1, UNSCROLL);
	lcd_data_write("                ", LCD_LINE2, UNSCROLL);
}

const char * nombre_dia(int dia)
{
    if(dia == 0)
    {
        return "Lunes    ";
    }else if (dia == 1)
    {
        return "Martes   ";
    }else if (dia == 2)
    {
        return "Miercoles";
    }else if (dia == 3)
    {
        return "Jueves   ";
    }else if (dia == 4)
    {
        return "Viernes  ";
    }else if (dia == 5)
    {
        return "Sabado   ";
    }else
    {
        return "Domingo  ";
    }
}

void display(void *pvParameter)
{
    BaseType_t xHigherPriorityTaskWoken;
    xHigherPriorityTaskWoken = pdTRUE;


    char dato_caracter;
    char secuencia[TAM_SECUENCIA];
    while(1)
    {
    	//printf("(DISPLAY)\r\n");
    	vTaskDelay(20 / portTICK_PERIOD_MS);
        switch(estado_menu)
        {
            case MAIN_MENU:
                if( xSemaphoreTake( mutex_time, ( TickType_t ) portMAX_DELAY ) == pdTRUE )
                {
                    for( int i = 0; i < 6; i ++ )
                    {
                        tiempo_int_copy[i] = tiempo_int[i];
                    }
                    xSemaphoreGive( mutex_time );
                }
                strcpy(fecha_char, int2char(tiempo_int_copy[0]));
                strcat(fecha_char, "/");
                strcat(fecha_char, int2char(tiempo_int_copy[1]));
                strcat(fecha_char, "/");
                strcat(fecha_char, int2char(tiempo_int_copy[2]));

                strcpy(hora_char, int2char(tiempo_int_copy[3]));
                strcat(hora_char, ":");
                strcat(hora_char, int2char(tiempo_int_copy[4]));
                strcat(hora_char, " ");

                strcat(hora_char, nombre_dia(tiempo_int_copy[5]));


                lcd_data_write(fecha_char, LCD_LINE1, UNSCROLL);

                lcd_data_write(hora_char, LCD_LINE2, UNSCROLL);
                if(xQueueReceive(teclado_caracteres,&dato_caracter,(TickType_t )(0)))
                {
                    if(dato_caracter == '#')
                    {
                        estado_menu = CONFIG_MENU;
                    }
                }
                break;
            case CONFIG_MENU :
                lcd_data_write("1.Time 2.Outputs", LCD_LINE1, UNSCROLL);
                lcd_data_write("3.Monitoring:   ", LCD_LINE2, UNSCROLL);

                if(xQueueReceive(teclado_caracteres,&dato_caracter,(TickType_t )(TIMEOUT)))
                {
                	printf("La tecla oprimida fue: %c\n", dato_caracter);
                    if(dato_caracter == '1' || dato_caracter == '2' || dato_caracter == '3')
                    {

                        flag_cursor = 1;
                        monitoreo_texto[14] = dato_caracter;
                        caracter_press[0] = dato_caracter;
                        caracter_press[1] = '\0';
                        lcd_data_write(monitoreo_texto, LCD_LINE2, UNSCROLL);

                    }else if(dato_caracter == '#' && flag_cursor == 1)
                    {
                    	limpiarPantalla();
                        switch(caracter_press[0])
                        {
                            case '1':
                                estado_menu = TIME_MENU;
                                flag_cursor = 0;
                                break;
                            case '2':
                                estado_menu = OUTS_MENU;
                                flag_cursor = 0;
                                break;
                            default:
                                estado_menu = MONITORING_MENU;
                                flag_cursor = 0;
                                break;
                        }
                    }else if(dato_caracter == '*' && flag_cursor == 1)
                    {

						flag_cursor = 1;
						monitoreo_texto[14] = ' ';
						lcd_data_write(monitoreo_texto, LCD_LINE2, UNSCROLL);
                        flag_cursor = 0;
                    }
                }else
                {
                    estado_menu = MAIN_MENU;
                    flag_cursor = 0;
                    limpiarPantalla();
                }
                break;
            case TIME_MENU :
            	lcd_data_write(" AAAAMMDDHHMM   ", LCD_LINE1, UNSCROLL);
            	lcd_data_write(":               ", LCD_LINE2, UNSCROLL);

                if(xQueueReceive(teclado_caracteres,&dato_caracter,(TickType_t )(TIMEOUT))){
                    if(dato_caracter != '*' && dato_caracter != '#' && cursor_time_menu < 13){

                        fecha_texto[cursor_time_menu] = dato_caracter;
                        caracter_press[0] = dato_caracter;
                        caracter_press[1] = '\0';
                        lcd_data_write(fecha_texto, LCD_LINE2, UNSCROLL);
                        mem_caracter[cursor_time_menu - 1] = caracter_press[0];
                        cursor_time_menu ++;
                    }else if(cursor_time_menu > 12 && ( dato_caracter == '#')){
                        if( xSemaphoreTake( mutex_time_char, ( TickType_t ) portMAX_DELAY ) == pdTRUE )
                        {
                            change_time = 1;
                            for (int i = 0; i < 13; i++)
                            {
                                tiempo_char[i] = mem_caracter[i];
                            }
                            xSemaphoreGive( mutex_time_char );
                            limpiarPantalla();
                        }
                        estado_menu = OK_MENU;
                        cursor_time_menu = 1;
                    }else if( dato_caracter == '*'){
                        cursor_time_menu --;
                        fecha_texto[cursor_time_menu] = ' ';
                        lcd_data_write(fecha_texto, LCD_LINE2, UNSCROLL);

                    }
                }else{
                    estado_menu = MAIN_MENU;
                    cursor_time_menu = 1;
                    limpiarPantalla();
                }
                break;
            case MONITORING_MENU :

                lcd_data_write("1.Enable        ", LCD_LINE1, UNSCROLL);
                lcd_data_write("2.Disable :     ", LCD_LINE2, UNSCROLL);
                if(xQueueReceive(teclado_caracteres,&dato_caracter,(TickType_t )(TIMEOUT))){
                    if(dato_caracter == '1' || dato_caracter == '2'){
                        flag_cursor = 1;
                        entradas_texto[cursor_monitoring_menu] = dato_caracter;
                        lcd_data_write(entradas_texto, LCD_LINE2, UNSCROLL);
                        caracter_press[0] = dato_caracter;
                        caracter_press[1] = '\0';

                    }else if(dato_caracter == '#' && flag_cursor == 1){
                        if( xSemaphoreTake( mutex_monitoreo, ( TickType_t ) portMAX_DELAY ) == pdTRUE )
                        {
                            if (caracter_press[0] == '1')
                            {
                                monit_global = 1;
                            }else
                            {
                                monit_global = 0;
                            }

                            xSemaphoreGive( mutex_monitoreo );
                        }
                        limpiarPantalla();
                        estado_menu = OK_MENU;
                        flag_cursor = 0;

                    }else if(dato_caracter == '*' && flag_cursor == 1){
                    	entradas_texto[cursor_monitoring_menu] = ' ';
                    	lcd_data_write(entradas_texto, LCD_LINE2, UNSCROLL);

                        flag_cursor = 0;
                    }
                }else{
                    estado_menu = MAIN_MENU;
                    flag_cursor = 0;
                    limpiarPantalla();
                }
                break;
            case OUTS_MENU :
                lcd_data_write("1.Force ON      ", LCD_LINE1, UNSCROLL);
                lcd_data_write("2.Force OFF :   ", LCD_LINE2, UNSCROLL);

                if(xQueueReceive(teclado_caracteres,&dato_caracter,(TickType_t )(TIMEOUT))){
                    if(dato_caracter == '1' || dato_caracter == '2'){
                        flag_cursor = 1;
                        salidas_texto[cursor_outs_menu] = dato_caracter;
                        lcd_data_write(salidas_texto, LCD_LINE2, UNSCROLL);
                        caracter_press[0] = dato_caracter;
                        caracter_press[1] = '\0';


                    }else if(dato_caracter == '#' && flag_cursor == 1){
                        limpiarPantalla();
                        switch(caracter_press[0]) {
                            case '1':
                                estado_menu = FORCE_ON_MENU;
                                flag_cursor = 0;
                                break;
                            default:
                                estado_menu = FORCE_OFF_MENU;
                                flag_cursor = 0;
                                break;
                        }

                    }else if(dato_caracter == '*' && flag_cursor == 1){
                        entradas_texto[cursor_outs_menu] = ' ';
                        lcd_data_write(entradas_texto, LCD_LINE2, UNSCROLL);
                        flag_cursor = 0;
                    }
                }else{
                    estado_menu = MAIN_MENU;
                    flag_cursor = 0;
                    limpiarPantalla();
                }
                break;
            case FORCE_ON_MENU :
            	lcd_data_write("ON output       ", LCD_LINE1, UNSCROLL);
            	lcd_data_write("number 0-6 :    ", LCD_LINE2, UNSCROLL);

                if(xQueueReceive(teclado_caracteres,&dato_caracter,(TickType_t )(TIMEOUT))){
                    if(dato_caracter != '*' && dato_caracter != '#' && dato_caracter != '7' && dato_caracter != '8' && dato_caracter != '9'){
                        flag_cursor = 1;
                        force_on_texto[cursor_force_menu] = dato_caracter;
                        lcd_data_write(force_on_texto, LCD_LINE2, UNSCROLL);
                        caracter_press[0] = dato_caracter;
                        caracter_press[1] = '\0';

                    }else if(dato_caracter == '#' && flag_cursor == 1){
                        limpiarPantalla();
                        flag_cursor = 0;
                        //enviar a la cola para forzar on
                        secuencia[0] = 'S';
                        secuencia[1] = 'O';
                        secuencia[2] = caracter_press[0];
                        secuencia[3] = '\0';
                        xQueueSend(q_interprete,(void *)&secuencia, &xHigherPriorityTaskWoken);
                        estado_menu = OK_MENU;

                    }else if(dato_caracter == '*' && flag_cursor == 1){
                    	force_on_texto[cursor_force_menu] = ' ';
                    	lcd_data_write(force_on_texto, LCD_LINE2, UNSCROLL);
                        flag_cursor = 0;
                    }
                }else{
                    estado_menu = MAIN_MENU;
                    flag_cursor = 0;
                    limpiarPantalla();
                }
                break;
            case FORCE_OFF_MENU:
            	lcd_data_write("OFF output      ", LCD_LINE1, UNSCROLL);
            	lcd_data_write("number 0-6 :    ", LCD_LINE2, UNSCROLL);

                if(xQueueReceive(teclado_caracteres,&dato_caracter,(TickType_t )(TIMEOUT))){
                    if(dato_caracter != '*' && dato_caracter != '#' && dato_caracter != '7' && dato_caracter != '8' && dato_caracter != '9'){
                        flag_cursor = 1;
                        caracter_press[0] = dato_caracter;
                        caracter_press[1] = '\0';
                        force_on_texto[cursor_force_menu] = ' ';
                        lcd_data_write(force_on_texto, LCD_LINE2, UNSCROLL);

                    }else if(dato_caracter == '#' && flag_cursor == 1){
                        limpiarPantalla();
                        flag_cursor = 0;
                        //enviar a la cola para forzar off
                        secuencia[0] = 'S';
                        secuencia[1] = 'F';
                        secuencia[2] = caracter_press[0];
                        secuencia[3] = '\0';
                        xQueueSend(q_interprete,(void *)&secuencia, &xHigherPriorityTaskWoken);
                        estado_menu = OK_MENU;

                    }else if(dato_caracter == '*' && flag_cursor == 1){
                    	force_on_texto[cursor_force_menu] = ' ';
                    	lcd_data_write(force_on_texto, LCD_LINE2, UNSCROLL);
                        flag_cursor = 0;
                    }
                }else{
                    estado_menu = MAIN_MENU;
                    flag_cursor = 0;
                    limpiarPantalla();
                }
                break;
            default : //OK_MENU
            	limpiarPantalla();
            	lcd_data_write("ACTION OK       ", LCD_LINE1, UNSCROLL);
                vTaskDelay( xDelay );
                estado_menu = MAIN_MENU;
                limpiarPantalla();
                break;
        }

    }
}
