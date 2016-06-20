/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de estructuras para administrar tareas
*/

#include "tss.h"

tss tss_inicial;
tss tss_idle;
tss vectorTareas[25];
unsigned int primer_indice_vector_disponible = 0;
unsigned int indices_disponibles_jugador_a[5] = {15, 16, 17, 18, 19};
unsigned int indices_disponibles_jugador_b[5] = {20, 21, 22, 23, 24};

void tss_inicializar() {
	//completar tss_inicial;
	int i;	
	agregar_tss((unsigned int) &tss_inicial, 4);

	unsigned int* vector_tareas_en_memoria = (unsigned int*) vectorTareas; //RAVIOL

	unsigned int* tss_inicial_en_memoria = (unsigned int*) &tss_inicial;

	for (i = 0; i < 26; i++){
		*tss_inicial_en_memoria = (unsigned int) 0x00000000;
		tss_inicial_en_memoria++;
	}

	for (i = 0; i <650 ; i++){//INICIALIZAMOS TODAS LAS TSS EN 0 DEL VECTOR TAREAS SON 25 TAREAS Y 26 DOUBLE WORDS POR TSS
		*vector_tareas_en_memoria= (unsigned int) 0x00000000;
		vector_tareas_en_memoria++;
	}
}

void tss_inicializar_idle(){
	int i;
	agregar_tss((unsigned int) &tss_idle, 4);

	//Inicializo en 0
	unsigned int* tss_idle_en_memoria = (unsigned int*) &tss_idle;

	for (i = 0; i < 26; i++){
		*tss_idle_en_memoria = (unsigned int) 0x00000000;
		tss_idle_en_memoria++;
	}

	tss_idle.eip = (unsigned int) 0x10000;
	tss_idle.esp = (unsigned int) 0x27000;
	tss_idle.ss = (unsigned short) GDT_IDX_DATA_LVL0 * 0x08;
	tss_idle.esp0 = (unsigned int) 0x27000;
	tss_idle.ss0 = (unsigned short) GDT_IDX_DATA_LVL0 * 0x08;
	tss_idle.cs = (unsigned short) GDT_IDX_COD_LVL0 * 0x08;
	tss_idle.cr3 = (unsigned int) 0x27000;
	tss_idle.eflags = (unsigned int) 0x00000202;

	tss_idle.es = (unsigned short) GDT_IDX_DATA_LVL0 * 0x08;
	tss_idle.gs = (unsigned short) GDT_IDX_DATA_LVL0 * 0x08;
	tss_idle.ds = (unsigned short) GDT_IDX_DATA_LVL0 * 0x08;

	tss_idle.fs = (unsigned short) GDT_IDX_DATA_VID * 0x08;

	tss_idle.iomap = (unsigned short) 0xFFFF;
}

 unsigned int tss_inicializar_tarea(char tareaID,short pos_x, short pos_y){
 	//tarea 0 = Tarea h //0x00013000
 	//tarea 1 = tarea A //0x00011000
 	//tarea 2 = tarea b	//0x00012000
 	unsigned int tareaDir;
 	unsigned int indice;

 	if(tareaID == 0){
 		tareaDir = 0x00013000;
 	}else if(tareaID == 1){
 		tareaDir = 0x00011000;
 	}else{
 		tareaDir = 0x00012000;
 	}

 	indice = dame_proximo_indice(tareaID);
 	unsigned int gdt_indice = agregar_tss((unsigned int) &vectorTareas[indice], tareaID);
 	unsigned int cr3_tarea = mmu_inicializar_dir_tarea(tareaDir,pos_x,pos_y);

 	unsigned int esp0 = mmu_proxima_pagina_fisica_libre();
 	//mmu_mapear_pagina(esp0, cr3_tarea, esp0, (unsigned char) 0x00);

 	vectorTareas[indice].eip = (unsigned int) 0x08000000;
	vectorTareas[indice].ebp = (unsigned int) 0x08001000;

	vectorTareas[indice].esp = (unsigned int) 0x08001000;
	vectorTareas[indice].ss = (unsigned short) ((GDT_IDX_DATA_LVL3 * 0x08) | 0x0003);

	vectorTareas[indice].esp0 = (unsigned int) esp0;
	vectorTareas[indice].ss0 = (unsigned short) (GDT_IDX_DATA_LVL0 * 0x08);

	vectorTareas[indice].cs = (unsigned short) ((GDT_IDX_COD_LVL3 * 0x08) | 0x0003);
	vectorTareas[indice].cr3 = (unsigned int) cr3_tarea;
	vectorTareas[indice].eflags = (unsigned int) 0x202;

	vectorTareas[indice].es = (unsigned short) ((GDT_IDX_DATA_LVL3 * 0x08) | 0x0003);
	vectorTareas[indice].gs = (unsigned short) ((GDT_IDX_DATA_LVL3 * 0x08) | 0x0003);
	vectorTareas[indice].ds = (unsigned short) ((GDT_IDX_DATA_LVL3 * 0x08) | 0x0003);

	vectorTareas[indice].fs = (unsigned short) ((GDT_IDX_DATA_LVL3 * 0x08) | 0x0003); //Ravioli, porque no va esto? //(GDT_IDX_DATA_VID * 0x08);

	vectorTareas[indice].iomap = (unsigned short) 0xFFFF;

 	return gdt_indice;
}

unsigned int dame_proximo_indice(char tareaID){
	unsigned int resultado;
	int i;

	//Asumo que hay un indice libre para la tareaID que se me da

	if (tareaID == 0){
		//Simplemente retorno el siguiente indice disponible para tareas sanas
		resultado = primer_indice_vector_disponible;
		primer_indice_vector_disponible++;
	} else if (tareaID == 1) {
		//Encuentro el primer indice no 0 de los indices disponibles para el jugador A en el vectorTareas
		for (i=0; i < 5; i++){
			if (indices_disponibles_jugador_a[i] != 0){
				resultado = indices_disponibles_jugador_a[i];
				indices_disponibles_jugador_a[i] = 0;
				i = 5;
			}
		}
	} else {
		//Encuentro el primer indice no 0 de los indices disponibles para el jugador B en el vectorTareas
		for (i=0; i < 5; i++){
			if (indices_disponibles_jugador_b[i] != 0){
				resultado = indices_disponibles_jugador_b[i];
				indices_disponibles_jugador_b[i] = 0;
				i = 5;
			}
		}
	}

	return resultado;
}

void tss_liberar_tarea(unsigned int indice_tarea){
	//Como las primeras 8 entradas de la GDT son selectores de segmento, TSS inicial y TSS idle, el indice en el vector de tareas es (indice_tarea - 8)
	unsigned int indice_vector_tareas = indice_tarea - 8;
	int i;
	//Los primeras 15 indices son de tareas sanas y no debo hacer nada para ellos
	if (indice_vector_tareas < 15){
		gdt_borrar_entrada(indice_tarea);
		//Nothing else
	} else {
		//Libero TSS
		gdt_borrar_entrada(indice_tarea);

		//Marco el indice como liberado en indices_disponibles_jugador_x
		// 15 <= indice < 20  ->  Jugador A
		// 20 <= indice < 25  ->  Jugador B
		if (indice_vector_tareas < 20){
			//Encuentro el primer indice 0 de los indices disponibles para el jugador A en el vectorTareas y cargo el nuevo indice disponible
			for (i=0; i < 5; i++){
				if (indices_disponibles_jugador_a[i] == 0){
					indices_disponibles_jugador_a[i] = indice_vector_tareas;
					i = 5;
				}
			}	
		} else {
			//Encuentro el primer indice 0 de los indices disponibles para el jugador B en el vectorTareas y cargo el nuevo indice disponible
			for (i=0; i < 5; i++){
				if (indices_disponibles_jugador_b[i] == 0){
					indices_disponibles_jugador_b[i] = indice_vector_tareas;
					i = 5;
				}
			}
		}
	}
}

void tss_remapear_tarea(unsigned char indice_tarea, unsigned short pos_x, unsigned short pos_y) {
	unsigned int indice_vector_tareas = indice_tarea - 8;
	unsigned int cr3_tarea;
	unsigned int dir_a_mapear = (unsigned int) ( 0x400000 + (pos_x * 0x1000) + (pos_y * 0x80000) );

	cr3_tarea = vectorTareas[indice_vector_tareas].cr3;

	mmu_mapear_pagina((unsigned int) 0x08001000, cr3_tarea, dir_a_mapear, (unsigned char) 0x01);
}