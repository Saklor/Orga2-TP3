/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
*/

#include "game.h"


void game_mover_cursor(int jugador, direccion dir) {
}

void game_lanzar(unsigned int jugador) {

}

void game_soy(unsigned int yoSoy) {
	//yoSoy = 0: no infectado
	//yoSoy = 1: infectado A
	//yoSoy = 2: infectado B

	unsigned char indice_de_tarea = (unsigned char) (rtr() / 0x08);
	sched_infectar(indice_de_tarea, yoSoy);

	//Actualizar estructuras de control del juego

	//Actualizar en pantalla

}

void game_donde(unsigned short* pos) {
	unsigned char indice_de_tarea = (unsigned char) (rtr() / 0x08);
	sched_carga_pos_x_y(indice_de_tarea, pos);
}

void game_mapear(int x, int y) {
	unsigned char indice_de_tarea = (unsigned char) (rtr() / 0x08);
	sched_mapear(indice_de_tarea, (unsigned short) x, (unsigned short) y);

	//Actualizar en pantalla

}

void game_matar_tarea() {
	unsigned char indice_de_tarea = (unsigned char) (rtr() / 0x08);
	sched_matar_tarea(indice_de_tarea);

	//Actualizar estructuras de control del juego

	//Actualizar en pantalla
}

