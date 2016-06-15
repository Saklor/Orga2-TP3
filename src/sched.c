/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#include "sched.h"

info_tarea tareas_sanas[15];
info_tarea tareas_a[5];
info_tarea tareas_b[5];

unsigned char turno = 0;
//Turno = 0: sanas
//Turno = 1: jugador a
//Turno = 2: jugador b

unsigned char indice[3] = {0,0,0};	//se llama con indice[turno]
unsigned char indice_max[3] = {15, 5, 5};

void sched_inicializar() {
	unsigned short tareas_sanas_pos_x[15] = {34,58,52,42,13,25,60,75,32,56,37,14,22,74,41};
	unsigned short tareas_sanas_pos_y[15] = {32,11,1,21,29,18,25,15,15,3,40,35,5,8,24};
	int i;
	unsigned int gdt_indice_tarea;

	for (i = 0; i < 15; i++) {
		gdt_indice_tarea = tss_inicializar_tarea(0, tareas_sanas_pos_x[i], tareas_sanas_pos_y[i]);
		tareas_sanas[i].indice_gdt = gdt_indice_tarea;
		tareas_sanas[i].esta_viva = 1;
		tareas_sanas[i].infectada = 0;
		tareas_sanas[i].pos_x = tareas_sanas_pos_x[i];
		tareas_sanas[i].pos_y = tareas_sanas_pos_y[i];
		tareas_sanas[i].target_x = tareas_sanas_pos_x[i];
		tareas_sanas[i].target_y = tareas_sanas_pos_y[i];
	}

	for (i = 0; i < 5; i++) {
		tareas_a[i].esta_viva = 0;
		tareas_a[i].infectada = 1;
		tareas_b[i].esta_viva = 0;
		tareas_b[i].infectada = 2;
	}
}

unsigned short sched_proximo_indice() {
	unsigned short proximo_indice;
	unsigned char indice_inicial;
	unsigned char turnos_revisados = 0;
	unsigned char encontre_alguna = 0;

	indice_inicial = indice[turno];

	turno = (turno + 1) % 3;
	indice[turno] = (indice[turno] + 1) % indice_max[turno];

	while(encontre_alguna == 0 && turnos_revisados < 3){
		while(encontre_alguna == 0 && indice[turno] != indice_inicial){
			if(turno == 0 && tareas_sanas[indice[turno]].esta_viva == 1){
				proximo_indice = tareas_sanas[indice[turno]].indice_gdt;
				encontre_alguna = 1;
			} else if(turno == 1 && tareas_a[indice[turno]].esta_viva == 1){
				proximo_indice = tareas_a[indice[turno]].indice_gdt;
				encontre_alguna = 1;
			} else if(turno == 2 && tareas_b[indice[turno]].esta_viva == 1){
				proximo_indice = tareas_b[indice[turno]].indice_gdt;
				encontre_alguna = 1;
			} else {
				indice[turno] = (indice[turno] + 1) % indice_max[turno];
			}
		}
		if (encontre_alguna == 0){
			turno = (turno + 1) % 3;
			indice_inicial = indice[turno];
			indice[turno] = (indice[turno] + 1) % indice_max[turno];
			turnos_revisados++;
		}
	}

	//Recorro la inicial
	if (encontre_alguna != 1){

	}

	if (encontre_alguna == 1){
		return (unsigned short) (proximo_indice * (unsigned short) 0x08);
	} else {
		return (unsigned short) 0x38;
	}
}

