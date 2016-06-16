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
	
} __attribute__((__packed__)) jugador;

typedef enum direccion_e { IZQ = 0xAAA, DER = 0x441, ARB = 0xA33, ABA = 0x883  } direccion;

void game_lanzar(unsigned int jugador);

void game_soy(unsigned int soy);

void game_donde(unsigned short* pos);

void game_mover_cursor(int jugador, direccion dir);

void game_matar_tarea();


#endif  /* !__GAME_H__ */
