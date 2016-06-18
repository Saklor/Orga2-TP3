/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
*/

#include "game.h"

jugador jugadorA;
jugador jugadorB;
ca pixel_anterior_jugadorA;
ca pixel_anterior_jugadorB;



void inicializar_jugadores(){

	jugadorA.quien_soy = (unsigned int) 0;
	jugadorA.contador_tareas = (unsigned int) 20;
	jugadorA.cuantas_infectadas_vivas = (unsigned int) 0;
	jugadorA.pos_x = (unsigned short) 40;
	jugadorA.pos_y = (unsigned short) 20;
	jugadorA.tareas_lanzadas_vivas = 0;

	pixel_anterior_jugadorA.a = 0x7f;
	pixel_anterior_jugadorA.c = 0x00;
	pixel_anterior_jugadorB.a = 0x7f;
	pixel_anterior_jugadorB.c = 0x00;

	jugadorB.quien_soy = (unsigned int) 0;
	jugadorB.contador_tareas = (unsigned int) 20;
	jugadorB.cuantas_infectadas_vivas = (unsigned int) 0;
	jugadorB.pos_x = (unsigned short) 41;
	jugadorB.pos_y = (unsigned short) 20;
	jugadorB.tareas_lanzadas_vivas = 0;
}


void game_mover_cursor(int jugador, direccion dir) {
	//para que no se pisen los jugadores
	unsigned char misma_posicion = 0;
	if(jugadorA.pos_x == jugadorB.pos_x && jugadorA.pos_y == jugadorB.pos_y){
		misma_posicion = 1;
	}

	// unsigned short attr;
	// if(jugadorA.pos_x == jugadorB.pos_x && jugadorA.pos_y == jugadorB.pos_y){
	// 	if(jugador ==0){
	// 		attr = (unsigned short) 0x1f;
	// 	}else{
	// 		attr = (unsigned short) 0x4f;
	// 	}
	// }else{
	// 	attr = (unsigned short) 0x77;
	// }

	//Pantalla que empieza en la fila 1
	ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) (VIDEO_SCREEN);

	if(jugador == 0){
		//Pinto lo que habia antes
		if (misma_posicion == 0)
			print_char(pixel_anterior_jugadorA.c,(jugadorA.pos_x),jugadorA.pos_y, pixel_anterior_jugadorA.a);
		else
			print_char(7,(jugadorA.pos_x),jugadorA.pos_y, 0x1f);

		//Cambio la posicion solo si es valido el movimiento
		if(dir == IZQ){
			if(jugadorA.pos_x != 0)
				jugadorA.pos_x = jugadorA.pos_x -1;
		}else if(dir == DER){
			if(jugadorA.pos_x != 79)
				jugadorA.pos_x = jugadorA.pos_x +1;
		}else if(dir == ARB){
			if(jugadorA.pos_y !=1)
				jugadorA.pos_y = jugadorA.pos_y -1;
		}else if(dir == ABA){
			if(jugadorA.pos_y != 44)
				jugadorA.pos_y = jugadorA.pos_y +1;
		}

		//Guardo el valor de la nueva posicion y me pinto en ella
		if(jugadorA.pos_x == jugadorB.pos_x && jugadorA.pos_y == jugadorB.pos_y)
			pixel_anterior_jugadorA = pixel_anterior_jugadorB;
		else
			pixel_anterior_jugadorA = p[jugadorA.pos_y][jugadorA.pos_x];
		print_char(7,jugadorA.pos_x,jugadorA.pos_y,(unsigned short) 0x4f);
	}else{
		//Pinto lo que habia antes
		if (misma_posicion == 0)
			print_char(pixel_anterior_jugadorB.c,(jugadorB.pos_x),jugadorB.pos_y, pixel_anterior_jugadorB.a);
		else
			print_char(7,(jugadorB.pos_x),jugadorB.pos_y, 0x4f);

		//Cambio la posicion solo si es valido el movimiento
		if(dir == IZQ){
			if(jugadorB.pos_x != 0)
				jugadorB.pos_x = jugadorB.pos_x -1;
		}else if(dir == DER){
			if(jugadorB.pos_x != 79)
				jugadorB.pos_x = jugadorB.pos_x +1;
		}else if(dir == ARB){
			if(jugadorB.pos_y !=1)
				jugadorB.pos_y = jugadorB.pos_y -1;
		}else if(dir == ABA){
			if(jugadorB.pos_y != 44)
				jugadorB.pos_y = jugadorB.pos_y +1;
		}

		//Guardo el valor de la nueva posicion y me pinto en ella
		if(jugadorA.pos_x == jugadorB.pos_x && jugadorA.pos_y == jugadorB.pos_y)
			pixel_anterior_jugadorB = pixel_anterior_jugadorA;
		else
			pixel_anterior_jugadorB = p[jugadorB.pos_y][jugadorB.pos_x];
		print_char(7,jugadorB.pos_x,jugadorB.pos_y,(unsigned short) 0x1f);

	}
}

void game_lanzar(unsigned int jugadorID) {
	jugador* jugador_que_lanza;
	unsigned short attr;
	if (jugadorID == 1)
		jugador_que_lanza = &jugadorA;
	else
		jugador_que_lanza = &jugadorB;

	if (jugador_que_lanza->contador_tareas > 0){
		sched_lanzar(jugadorID, jugador_que_lanza->pos_x, jugador_que_lanza->pos_y);

		jugador_que_lanza->tareas_lanzadas_vivas++;
		jugador_que_lanza->contador_tareas--;

		//Actualizar estructuras de control del juego
		jugadorA.cuantas_infectadas_vivas = sched_infectadas(1);
		jugadorB.cuantas_infectadas_vivas = sched_infectadas(2);

		//Actualizar pantalla
		if (jugadorID == 1)
			attr = 0x4f;
		else if (jugadorID == 2)
			attr = 0x1f;

		if (jugadorA.pos_x == jugador_que_lanza->pos_x && jugadorA.pos_y == jugador_que_lanza->pos_y)
			pixel_anterior_jugadorA.a = attr;
		if (jugadorB.pos_x == jugador_que_lanza->pos_x && jugadorB.pos_y == jugador_que_lanza->pos_y)
			pixel_anterior_jugadorB.a = attr;

		//
	} else {
		print("No te quedan vidas, sorry", 0, 0, (unsigned short) 0x0E);
	}
}

void game_soy(unsigned int yoSoy) {
	//yoSoy = 0: no infectado
	//yoSoy = 1: infectado A
	//yoSoy = 2: infectado B
	unsigned short pos[2] = {0, 0};
	unsigned short attr;

	unsigned char indice_de_tarea = (unsigned char) (rtr() / 0x08);
	sched_infectar(indice_de_tarea, yoSoy);

	//Actualizar estructuras de control del juego
	jugadorA.cuantas_infectadas_vivas = sched_infectadas(1);
	jugadorB.cuantas_infectadas_vivas = sched_infectadas(2);

	//Actualizar en pantalla
	sched_carga_pos_x_y(indice_de_tarea, pos);

	if (yoSoy == 1)
		attr = 0x4f;
	else if (yoSoy == 2)
		attr = 0x1f;
	
	if (jugadorA.pos_x == pos[0] && jugadorA.pos_y == pos[1])
		pixel_anterior_jugadorA.a = attr;
	if (jugadorB.pos_x == pos[0] && jugadorB.pos_y == pos[1])
		pixel_anterior_jugadorB.a = attr;
	
	if ((jugadorA.pos_x != pos[0] || jugadorA.pos_y != pos[1]) && (jugadorB.pos_x != pos[0] || jugadorB.pos_y != pos[1]))
		screen_pintar_sector(pos[0], pos[1], attr);
}

void game_donde(unsigned short* pos) {
	unsigned char indice_de_tarea = (unsigned char) (rtr() / 0x08);
	sched_carga_pos_x_y(indice_de_tarea, pos);

	//La tarea tiene acceso al mapa de la fila 1 a la 44 inclusive, pero tiene que verlas como 0-43, por lo que le restamos uno
	pos[1] = pos[1] - 1;
}

void game_mapear(int x, int y) {
	unsigned char es_infectada = 0;
	unsigned char letra = 0;
	unsigned short target_anterior[2] = {0, 0};
	unsigned char indice_de_tarea = (unsigned char) (rtr() / 0x08);

	sched_carga_target_x_y(indice_de_tarea, target_anterior);
	
	//La tarea ve al mapa de la fila 0 a la 43 inclusive, pero en la pantalla se encuentra entre las filas 1-44, por lo que le sumamos uno al Y
	sched_mapear(indice_de_tarea, (unsigned short) x, (unsigned short) y + 1);

	es_infectada = sched_es_infectada(indice_de_tarea);

	//Actualizar en pantalla
	if(es_infectada == 1) {
		letra = 'A';
	} else if(es_infectada == 2) {
		letra = 'B';
	} else {
		letra = 'H';
		print("Hay una tarea sana mapeando, dafuq?", 0, 0, (unsigned short) 0x0E);
	}

	if (jugadorA.pos_x == x && jugadorA.pos_y == (y + 1))
		pixel_anterior_jugadorA.c = letra;
	if (jugadorB.pos_x == x && jugadorB.pos_y == (y + 1))
		pixel_anterior_jugadorB.c = letra;
	if ((jugadorA.pos_x != x || jugadorA.pos_y != (y + 1)) && (jugadorB.pos_x != x || jugadorB.pos_y != (y + 1)))
		print_char_sin_attr(letra,x,y);

	if (jugadorA.pos_x == target_anterior[0] && jugadorA.pos_y == target_anterior[1])
		pixel_anterior_jugadorA.c = 0;
	if (jugadorB.pos_x == target_anterior[0] && jugadorB.pos_y == target_anterior[1])
		pixel_anterior_jugadorB.c = 0;
	if ((jugadorA.pos_x != target_anterior[0] || jugadorA.pos_y != target_anterior[1]) && (jugadorB.pos_x != target_anterior[0] || jugadorB.pos_y != target_anterior[1]))
		print_char_sin_attr(0,target_anterior[0],target_anterior[1]);	
}

void game_matar_tarea() {
	unsigned short pos[2] = {0, 0};
	unsigned short target[2] = {0, 0};
	unsigned char tareaID_matada = 0;
	unsigned char indice_de_tarea = (unsigned char) (rtr() / 0x08);

	sched_carga_pos_x_y(indice_de_tarea, pos);
	sched_carga_target_x_y(indice_de_tarea, target);

	tareaID_matada = sched_matar_tarea(indice_de_tarea);

	//Actualizar estructuras de control del juego
	if (tareaID_matada == 1)
		jugadorA.tareas_lanzadas_vivas--;
	else if (tareaID_matada == 2)
		jugadorB.tareas_lanzadas_vivas--;

	jugadorA.cuantas_infectadas_vivas = sched_infectadas(1);
	jugadorB.cuantas_infectadas_vivas = sched_infectadas(2);

	//Actualizar en pantalla
	if (jugadorA.pos_x == pos[0] && jugadorA.pos_y == pos[1])
		pixel_anterior_jugadorA.a = 0x7f;
	if (jugadorB.pos_x == pos[0] && jugadorB.pos_y == pos[1])
		pixel_anterior_jugadorB.a = 0x7f;
	
	if ((jugadorA.pos_x != pos[0] || jugadorA.pos_y != pos[1]) && (jugadorB.pos_x != pos[0] || jugadorB.pos_y != pos[1]))
		screen_pintar_sector(pos[0], pos[1], 0x7f);

	if (jugadorA.pos_x == target[0] && jugadorA.pos_y == target[1])
		pixel_anterior_jugadorA.c = 0;
	if (jugadorB.pos_x == target[0] && jugadorB.pos_y == target[1])
		pixel_anterior_jugadorB.c = 0;
	if ((jugadorA.pos_x != target[0] || jugadorA.pos_y != target[1]) && (jugadorB.pos_x != target[0] || jugadorB.pos_y != target[1]))
		print_char_sin_attr(0,target[0],target[1]);	
}

