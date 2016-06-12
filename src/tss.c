/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de estructuras para administrar tareas
*/

#include "tss.h"

tss tss_inicial;
tss tss_idle;

void tss_inicializar() {
	//completar tss_inicial;
	int i;
	agregar_tss(&tss_inicial);

	unsigned int* tss_inicial_en_memoria = (unsigned int*) &tss_inicial;

	for (i = 0; i < 26; i++){
		*tss_inicial_en_memoria = (unsigned int) 0x00000000;
		tss_inicial_en_memoria++;
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