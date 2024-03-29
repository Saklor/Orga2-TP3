/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#ifndef __SCREEN_H__
#define __SCREEN_H__

/* Definicion de la pantalla */
#define VIDEO_FILS 50
#define VIDEO_COLS 80

#include "colors.h"
#include "defines.h"
#include "i386.h"
/* Estructura de para acceder a memoria de video */
typedef struct ca_s {
    unsigned char c;
    unsigned char a;
} ca;

void print(const char * text, unsigned int x, unsigned int y, unsigned short attr);

void print_hex(unsigned int numero, int size, unsigned int x, unsigned int y, unsigned short attr);

void print_int(unsigned int n, unsigned int x, unsigned int y, unsigned short attr);
void inicializar_pantalla();
// void pintar_tarea_en_mapa(int jugador, unsigned short pos_x, unsigned short pos_y);

void screen_avanzar_reloj_tarea(unsigned char tareaID, unsigned char tarea_indice);
void screen_resetear_reloj_tarea(unsigned char tareaID, unsigned char tarea_indice);

void print_char(char  text, unsigned int x, unsigned int y, unsigned short attr); 
void print_char_sin_attr(char text, unsigned int x, unsigned int y);
void screen_pintar_sector(unsigned int x, unsigned int y, unsigned short attr);

void screen_pintar_puntaje(int jugador, int puntaje);
void screen_pintar_vidas(int jugador, int vidas);

void imprimo_interrupcion_pantalla(
    int eax, int ebx, int ecx, int edx, int esi, int edi, int ebp,
    int ds,  int es,  int fs,  int gs, unsigned char idTarea,
    int eip, int cs, int eflags, int esp, int ss);
void restauro_pantalla();

#endif  /* !__SCREEN_H__ */
