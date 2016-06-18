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

unsigned char turno = 2;
//Turno = 0: sanas
//Turno = 1: jugador a
//Turno = 2: jugador b

unsigned char indice[3] = {14,4,4};	//se llama con indice[turno]
unsigned char indice_max[3] = {15, 5, 5};

const unsigned short idle_offset = 0x38;

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

	turno = (turno + 1) % 3;
	indice[turno] = (indice[turno] + 1) % indice_max[turno];
	indice_inicial = indice[turno];

	while(encontre_alguna == 0 && turnos_revisados < 3){
		do {
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
		} while (encontre_alguna == 0 && indice[turno] != indice_inicial);
		if (encontre_alguna == 0){
			turno = (turno + 1) % 3;
			indice_inicial = indice[turno];
			indice[turno] = (indice[turno] + 1) % indice_max[turno];
			turnos_revisados++;
		}
	}

	if (encontre_alguna == 1){
		proximo_indice = (unsigned short) (proximo_indice * (unsigned short) 0x08);
	} else {
		proximo_indice = (unsigned short) 0x38;
	}

	if (rtr() != proximo_indice){
		screen_avanzar_reloj_tarea(turno, indice[turno]);
		return proximo_indice;
	} else {
		return 0;
	}
}

void sched_infectar(unsigned char indice_tarea, unsigned int inf){
	info_tarea* tarea = dame_info_a_partir_de_indice(indice_tarea);
	tarea->infectada = inf;
}

void sched_carga_pos_x_y(unsigned char indice_tarea, unsigned short* pos){
	info_tarea* tarea = dame_info_a_partir_de_indice(indice_tarea);
	breakpoint();
	pos[0] = tarea->pos_x;
	pos[1] = tarea->pos_y;
}

void sched_mapear(unsigned char indice_tarea, unsigned short tar_x, unsigned short tar_y){
	info_tarea* tarea = dame_info_a_partir_de_indice(indice_tarea);
	tarea->target_x = tar_x;
	tarea->target_y = tar_y;
}

info_tarea* dame_info_a_partir_de_indice(unsigned char indice_tarea){
	info_tarea* resultado;
	int i;

	//Indices de 8 a 22 inclusive son para tareas sanas
    //Indices de 23 a 27 inclusive son para tareas del jugador A
    //Indices de 28 a 32 inclusive son para tareas del jugador B
	if (indice_tarea >= 8 && indice_tarea <= 22){
		//Tarea sana
		for (i = 0; i < 15; i++) {
			if(tareas_sanas[i].indice_gdt == indice_tarea)
				resultado = &tareas_sanas[i];
		}
	} else if (indice_tarea >= 23 && indice_tarea <= 27){
		//Tarea A
		for (i = 0; i < 5; i++) {
			if(tareas_a[i].indice_gdt == indice_tarea)
				resultado = &tareas_a[i];
		}
	} else if (indice_tarea >= 23 && indice_tarea <= 27){
		//Tarea B
		for (i = 0; i < 5; i++) {
			if(tareas_b[i].indice_gdt == indice_tarea)
				resultado = &tareas_b[i];
		}
	}

	return resultado;
}

unsigned char sched_matar_tarea(unsigned char indice_tarea){
	//Mato a la tarea en mi informacion
	info_tarea* tarea = dame_info_a_partir_de_indice(indice_tarea);
	tarea->esta_viva = 0;

	//Cambio el Task Register a la tarea inicial
	ltr(0x30);

	//Libero la entrada de la tss
	tss_liberar_tarea(indice_tarea);

	if (indice_tarea >= 8 && indice_tarea <= 22){
		//Tarea sana
		return 0;
	} else if (indice_tarea >= 23 && indice_tarea <= 27){
		//Tarea A
		return 1;
	} else if (indice_tarea >= 23 && indice_tarea <= 27){
		//Tarea B
		return 2;
	}

	return 0;
}

unsigned int sched_infectadas(unsigned char infectionID){
	int i;
	unsigned int infectadas;

	for (i = 0; i < 15; i++) {
		if (tareas_sanas[i].infectada == infectionID && tareas_sanas[i].esta_viva == 1)
			infectadas++;
	}

	for (i = 0; i < 5; i++) {
		if (tareas_a[i].infectada == infectionID && tareas_a[i].esta_viva == 1)
			infectadas++;	
	}

	for (i = 0; i < 5; i++) {
		if (tareas_b[i].infectada == infectionID && tareas_b[i].esta_viva == 1)
			infectadas++;
	}

	return infectadas;
}

unsigned char sched_es_infectada(unsigned char indice_tarea){
	info_tarea* tarea = dame_info_a_partir_de_indice(indice_tarea);
	return tarea->infectada;
}

void sched_lanzar(unsigned char tareaID, int x, int y){
	int i;
	info_tarea* tarea;
	unsigned int gdt_indice_tarea;

	
	gdt_indice_tarea = tss_inicializar_tarea(tareaID, x, y);

	if (tareaID == 1){
		for (i = 0; i < 5; i++){
			if (tareas_a[i].esta_viva == 0){
				tarea = &tareas_a[i];
				i = 5;
			}
		}
	} else {
		for (i = 0; i < 5; i++){
			if (tareas_b[i].esta_viva == 0){
				tarea = &tareas_b[i];
				i = 5;
			}
		}
	}
	tarea->indice_gdt = gdt_indice_tarea;
	tarea->esta_viva = 1;
	tarea->infectada = tareaID;
	tarea->pos_x = x;
	tarea->pos_y = y;
	tarea->target_x = x;
	tarea->target_y = y;
}

void sched_carga_target_x_y(unsigned char indice_tarea, unsigned short* pos){
	info_tarea* tarea = dame_info_a_partir_de_indice(indice_tarea);
	pos[0] = tarea->target_x;
	pos[1] = tarea->target_y;
}