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

	pixel_anterior_jugadorA.a = 0x77;
	pixel_anterior_jugadorA.c = 0x00;
	pixel_anterior_jugadorB.a = 0x77;
	pixel_anterior_jugadorB.c = 0x00;

	jugadorB.quien_soy = (unsigned int) 0;
	jugadorB.contador_tareas = (unsigned int) 20;
	jugadorB.cuantas_infectadas_vivas = (unsigned int) 0;
	jugadorB.pos_x = (unsigned short) 41;
	jugadorB.pos_y = (unsigned short) 20;
}


void game_mover_cursor(int jugador, direccion dir) {
	//para que no se pisen los jugadores
	unsigned char misma_posicion = 0;
	if(jugadorA.pos_x == jugadorB.pos_x && jugadorA.pos_y == jugadorB.pos_y){
		misma_posicion = 1;
	}

	unsigned short attr;
	if(jugadorA.pos_x == jugadorB.pos_x && jugadorA.pos_y == jugadorB.pos_y){
		if(jugador ==0){
			attr = (unsigned short) 0x1f;
		}else{
			attr = (unsigned short) 0x4f;
		}
	}else{
		attr = (unsigned short) 0x77;
	}

	//Pantalla que empieza en la fila 1
	ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) (VIDEO_SCREEN);

	if(jugador == 0){
		if(dir == IZQ){
			if(jugadorA.pos_x != 0){
				//hay que chequear  ademas de mi cursor las cosas que estan mapeadas aca para dejarlo del color que estaba antes (se puede hacer syscall o algo?)
				if (misma_posicion == 0)
					print_hex(pixel_anterior_jugadorA.c,1,(jugadorA.pos_x),jugadorA.pos_y, pixel_anterior_jugadorA.a);
				else
					print_hex(11,1,(jugadorA.pos_x),jugadorA.pos_y, 0x1f);
				jugadorA.pos_x = jugadorA.pos_x -1;
				pixel_anterior_jugadorA = p[jugadorA.pos_y][jugadorA.pos_x];
				print_hex(10,1,jugadorA.pos_x,jugadorA.pos_y,(unsigned short) 0x4f);
			}
		}else if(dir == DER){
			if(jugadorA.pos_x != 79){
				if (misma_posicion == 0)
					print_hex(pixel_anterior_jugadorA.c,1,(jugadorA.pos_x),jugadorA.pos_y, pixel_anterior_jugadorA.a);
				else
					print_hex(11,1,(jugadorA.pos_x),jugadorA.pos_y, 0x1f);
				jugadorA.pos_x = jugadorA.pos_x +1;
				pixel_anterior_jugadorA = p[jugadorA.pos_y][jugadorA.pos_x];
				print_hex(10,1,jugadorA.pos_x,jugadorA.pos_y,(unsigned short) 0x4f);
			}
		}else if(dir == ARB){
			if(jugadorA.pos_y !=1){
				if (misma_posicion == 0)
					print_hex(pixel_anterior_jugadorA.c,1,(jugadorA.pos_x),jugadorA.pos_y, pixel_anterior_jugadorA.a);
				else
					print_hex(11,1,(jugadorA.pos_x),jugadorA.pos_y, 0x1f);
				jugadorA.pos_y = jugadorA.pos_y -1;
				pixel_anterior_jugadorA = p[jugadorA.pos_y][jugadorA.pos_x];
				print_hex(10,1,jugadorA.pos_x,jugadorA.pos_y,(unsigned short) 0x4f);
			}
		}else if(dir == ABA){
			if(jugadorA.pos_y != 44){
				if (misma_posicion == 0)
					print_hex(pixel_anterior_jugadorA.c,1,(jugadorA.pos_x),jugadorA.pos_y, pixel_anterior_jugadorA.a);
				else
					print_hex(11,1,(jugadorA.pos_x),jugadorA.pos_y, 0x1f);
				jugadorA.pos_y = jugadorA.pos_y +1;
				pixel_anterior_jugadorA = p[jugadorA.pos_y][jugadorA.pos_x];
				print_hex(10,1,jugadorA.pos_x,jugadorA.pos_y,(unsigned short) 0x4f);
			}
		}
	}else{
		if(dir == IZQ){
			if(jugadorB.pos_x != 0){
				//hay que chequear  ademas de mi cursor las cosas que estan mapeadas aca para dejarlo del color que estaba antes (se puede hacer syscall o algo?)
				print_hex(11,1,(jugadorB.pos_x - 1),jugadorB.pos_y,(unsigned short) 0x1f);
				print_hex(10,1,(jugadorB.pos_x),jugadorB.pos_y,(unsigned short) attr);
				jugadorB.pos_x = jugadorB.pos_x -1;

			}
		}else if(dir == DER){
			if(jugadorB.pos_x != 79){
				print_hex(11,1,(jugadorB.pos_x + 1),jugadorB.pos_y,(unsigned short) 0x1f);
				print_hex(10,1,(jugadorB.pos_x),jugadorB.pos_y,(unsigned short) attr);
				jugadorB.pos_x = jugadorB.pos_x +1;
			}
		}else if(dir == ARB){
			if(jugadorB.pos_y !=1){
				print_hex(11,1,jugadorB.pos_x,(jugadorB.pos_y - 1),(unsigned short) 0x1f);
				print_hex(10,1,jugadorB.pos_x,jugadorB.pos_y,(unsigned short) attr);
				jugadorB.pos_y = jugadorB.pos_y -1;
			}
		}else if(dir == ABA){
			if(jugadorB.pos_y != 44){
				print_hex(11,1,jugadorB.pos_x,(jugadorB.pos_y +1),(unsigned short) 0x1f);
				print_hex(10,1,jugadorB.pos_x,jugadorB.pos_y,(unsigned short) attr);
				jugadorB.pos_y = jugadorB.pos_y +1;
			}
		}

	}
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

