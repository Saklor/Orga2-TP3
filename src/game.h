/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
*/

#ifndef __GAME_H__
#define __GAME_H__

#include "defines.h"
#include "screen.h"
#include "mmu.h"
#include "sched.h"

typedef struct jugador_str {
	unsigned int quien_soy; // 0= jugador A , 1= jugador B 
	unsigned int contador_tareas; //Empieza en 20
	unsigned int cuantas_infectadas_vivas;//Empiezan los dos en 0
	unsigned int tareas_lanzadas_vivas;
	unsigned short pos_x;//Empieza en la mitad de la pantalla 40 si es A, 41 si es B
	unsigned short pos_y;//los dos empiezan en 20
	
} __attribute__((__packed__)) jugador;

typedef enum direccion_e { IZQ = 0xAAA, DER = 0x441, ARB = 0xA33, ABA = 0x883  } direccion;

void game_lanzar(unsigned int jugadorID);

void game_soy(unsigned int soy);

void game_donde(unsigned short* pos);

void game_mover_cursor(int jugador, direccion dir);

void game_matar_tarea();

void inicializar_jugadores();


#endif  /* !__GAME_H__ */
