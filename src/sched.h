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

void sched_infectar(unsigned char indice_tarea, unsigned int inf);
void sched_carga_pos_x_y(unsigned char indice_tarea, unsigned short* pos);
void sched_mapear(unsigned char indice_tarea, unsigned short tar_x, unsigned short tar_y);

unsigned char sched_matar_tarea(unsigned char indice_tarea);
void sched_lanzar(unsigned char tareaID, int x, int y);

//Consultas
unsigned int sched_infectadas(unsigned char infectionID);
unsigned char sched_es_infectada(unsigned char indice_tarea);
void sched_carga_target_x_y(unsigned char indice_tarea, unsigned short* pos);

info_tarea* dame_info_a_partir_de_indice(unsigned char indice_tarea);

#endif	/* !__SCHED_H__ */
