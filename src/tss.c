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
unsigned int primer_indice_vector_disponible=0;

void tss_inicializar() {
	//completar tss_inicial;
	int i;	
	agregar_tss(&tss_inicial);

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
	agregar_tss(&tss_idle);

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
 	if(tareaID == 0){
 		tareaDir = 0x00013000;
 		
 	}else if(tareaID == 1){
 		tareaDir = 0x00011000;

 	}else{
 		tareaDir = 0x00012000;
 	}

 	unsigned int gdt_indice = agregar_tss(&vectorTareas[primer_indice_vector_disponible]);
 	unsigned int cr3_tarea = mmu_inicializar_dir_tarea(tareaDir,pos_x,pos_y);

 	unsigned int esp0 = mmu_proxima_pagina_fisica_libre();
 	mmu_mapear_pagina(esp0, cr3_tarea, esp0, (unsigned char) 0x00);


 	vectorTareas[primer_indice_vector_disponible].eip = (unsigned int) 0x08000000;
	vectorTareas[primer_indice_vector_disponible].esp = (unsigned int) 0x08001000;
	vectorTareas[primer_indice_vector_disponible].ebp = (unsigned int) 0x08001000;
	vectorTareas[primer_indice_vector_disponible].ss = (unsigned short) GDT_IDX_DATA_LVL3 * 0x08;
	vectorTareas[primer_indice_vector_disponible].esp0 = (unsigned int) esp0;
	vectorTareas[primer_indice_vector_disponible].ss0 = (unsigned short) GDT_IDX_DATA_LVL0 * 0x08;
	vectorTareas[primer_indice_vector_disponible].cs = (unsigned short) GDT_IDX_COD_LVL3 * 0x08;
	vectorTareas[primer_indice_vector_disponible].cr3 = (unsigned int) cr3_tarea;
	vectorTareas[primer_indice_vector_disponible].eflags = (unsigned int) 0x00000202;

	vectorTareas[primer_indice_vector_disponible].es = (unsigned short) GDT_IDX_DATA_LVL3 * 0x08;
	vectorTareas[primer_indice_vector_disponible].gs = (unsigned short) GDT_IDX_DATA_LVL3 * 0x08;
	vectorTareas[primer_indice_vector_disponible].ds = (unsigned short) GDT_IDX_DATA_LVL3 * 0x08;

	vectorTareas[primer_indice_vector_disponible].fs = (unsigned short) GDT_IDX_DATA_VID * 0x08;

	vectorTareas[primer_indice_vector_disponible].iomap = (unsigned short) 0xFFFF;

 	primer_indice_vector_disponible++;
 	return gdt_indice;
}