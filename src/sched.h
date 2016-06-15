/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#ifndef __SCHED_H__
#define __SCHED_H__

#include "screen.h"
#include "tss.h"
#include "gdt.h"

typedef struct str_info_tarea {
	unsigned int indice_gdt;
	unsigned char esta_viva;
	unsigned char infectada;	
	//Infectada = 0: no
	//Infectada = 1: si, a
	//Infectada = 2: si, b
	unsigned short pos_x;
	unsigned short pos_y;
	unsigned short target_x;
	unsigned short target_y;
} __attribute__((__packed__)) info_tarea;

void sched_inicializar();
unsigned short sched_proximo_indice();

#endif	/* !__SCHED_H__ */
