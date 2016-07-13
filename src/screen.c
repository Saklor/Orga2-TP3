/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#include "screen.h"

char valores[4] = "|/-\\";
unsigned char reloj_actual_sanas[15];
unsigned char reloj_actual_jugadores[2][5];
unsigned short pantalla_guardada[4000] = { };


void print(const char * text, unsigned int x, unsigned int y, unsigned short attr) {
    ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO_SCREEN;
    int i;
    for (i = 0; text[i] != 0; i++) {
        p[y][x].c = (unsigned char) text[i];
        p[y][x].a = (unsigned char) attr;
        x++;
        if (x == VIDEO_COLS) {
            x = 0;
            y++;
        }
    }
}

void print_char(char  text, unsigned int x, unsigned int y, unsigned short attr) {
    ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO_SCREEN;
    p[y][x].c = (unsigned char) text;
    p[y][x].a = (unsigned char) attr; 
}

void print_hex(unsigned int numero, int size, unsigned int x, unsigned int y, unsigned short attr) {
    ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO_SCREEN;
    int i;
    char hexa[8];
    char letras[16] = "0123456789ABCDEF";
    hexa[0] = letras[ ( numero & 0x0000000F ) >> 0  ];
    hexa[1] = letras[ ( numero & 0x000000F0 ) >> 4  ];
    hexa[2] = letras[ ( numero & 0x00000F00 ) >> 8  ];
    hexa[3] = letras[ ( numero & 0x0000F000 ) >> 12 ];
    hexa[4] = letras[ ( numero & 0x000F0000 ) >> 16 ];
    hexa[5] = letras[ ( numero & 0x00F00000 ) >> 20 ];
    hexa[6] = letras[ ( numero & 0x0F000000 ) >> 24 ];
    hexa[7] = letras[ ( numero & 0xF0000000 ) >> 28 ];
    for(i = 0; i < size; i++) {
        p[y][x + size - i - 1].c = hexa[i];
        p[y][x + size - i - 1].a = attr;
    }
}

void print_int(unsigned int n, unsigned int x, unsigned int y, unsigned short attr) {
    ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO_SCREEN;
    if( n > 9 ) {
      int a = n / 10;
      n -= 10 * a;
      print_int(a,x-1,y,attr);
    }
    p[y][x].c = '0'+n;
    p[y][x].a = attr;
}

void inicializar_pantalla(){
    unsigned int i;
    unsigned int j;
    char* inicializacion_reloj_tareas_jugador = "x x x x x <A";
    unsigned short tareas_sanas_pos_x[15] = {34,58,52,42,13,25,60,75,32,56,37,14,22,74,41};
    unsigned short tareas_sanas_pos_y[15] = {32,11,1,21,29,18,25,15,15,3,40,35,5,8,24};

    for ( i = 0; i < 4000; i++){
        pantalla_guardada[i] = (unsigned short) 0;
    }

    for ( j = 0; j < VIDEO_COLS; j++){
        print_char(0,j,0,0);
    }

    for ( i = 1; i <= 44; i++){
        for ( j = 0; j < VIDEO_COLS; j++){
            print_char(0,j,i,(unsigned short)0x7f);
        }
    }

    for (i = 45; i < 50; i++){
        for (j = 0; j < 80; j++){
            print_char(0,j,i,0);
        }
    }


    //Inicializo relojes de sanas y jugadores
    for (i = 0; i < 15; i++){
        reloj_actual_sanas[i] = 0;
        screen_avanzar_reloj_tarea(0, i);
    }

    for (i = 0; i < 5; i++){
        reloj_actual_jugadores[0][i] = 0;
        reloj_actual_jugadores[1][i] = 0;
    }
    print((char*) inicializacion_reloj_tareas_jugador, 4, 46, 0x07);
    inicializacion_reloj_tareas_jugador = "B> x x x x x";
    print((char*) inicializacion_reloj_tareas_jugador, 21, 46, 0x07);


    //pinto jugador A y B
    print_char(7,40,20,(unsigned short) 0x4f);
    print_char(7,41,20,(unsigned short) 0x1f);
    
    //imprimo las h
    for (i  = 0; i < 15; i++){
        print_char(0, (unsigned int) tareas_sanas_pos_x[i], (unsigned int) tareas_sanas_pos_y[i], (unsigned short) 0x2f);
    }
    // print_char(0,34,32,(unsigned short) 0x2f);
    // print_char(0,58,11,(unsigned short) 0x2f);
    // print_char(0,52,1,(unsigned short) 0x2f);
    // print_char(0,42,21,(unsigned short) 0x2f);
    // print_char(0,13,19,(unsigned short) 0x2f);
    // print_char(0,25,18,(unsigned short) 0x2f);
    // print_char(0,60,25,(unsigned short) 0x2f);
    // print_char(0,75,15,(unsigned short) 0x2f);
    // print_char(0,32,15,(unsigned short) 0x2f);
    // print_char(0,56,3,(unsigned short) 0x2f);
    // print_char(0,37,40,(unsigned short) 0x2f);
    // print_char(0,14,35,(unsigned short) 0x2f);
    // print_char(0,22,5,(unsigned short) 0x2f);
    // print_char(0,74,8,(unsigned short) 0x2f);
    // print_char(0,41,24,(unsigned short) 0x2f);

   for (i = 45; i < 50; i++){
        for (j = 50; j < 57; j++){
            print_char(0,j,i,(unsigned short) 0x4f);
        }
    }

    for (i = 45; i < 50; i++){
        for (j = 57; j < 64; j++){
            print_char(0,j,i,(unsigned short) 0x1f);
        }
    }

    char vidas[6]={'V','i','d','a','s',0};
    char puntaje[8] = {'P','u','n','t','a','j','e',0};
    
    print(puntaje,54,45,(unsigned short) 0x0f);
    screen_pintar_puntaje(1, 0);
    screen_pintar_puntaje(2, 0);

    print(vidas,44,46,(unsigned short) 0x0f);
    print(vidas,65,46,(unsigned short) 0x0f);

    screen_pintar_vidas(1, 20);
    screen_pintar_vidas(2, 20);
}

void screen_pintar_puntaje(int jugador, int puntaje){
    unsigned short attr;
    unsigned int x;
    if (jugador == 1){
        attr = 0x4f;
        x = 53;
    } else {
        attr = 0x1f;
        x = 60;
    }

    print_int(puntaje,x,47,attr);
}

void screen_pintar_vidas(int jugador, int vidas){
    unsigned short attr;
    unsigned int x;
    if (jugador == 1){
        attr = 0x4f;
        x = 46;
    } else {
        attr = 0x1f;
        x = 67;
    }

    print_int(vidas,x,48,attr);
}

// void pintar_tarea_en_mapa(int jugador, unsigned short pos_x, unsigned short pos_y){

// }

void screen_avanzar_reloj_tarea(unsigned char tareaID, unsigned char tarea_indice){
    ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO_SCREEN;
    unsigned char texto_a_mostrar;
    unsigned char y;
    unsigned char x_inicial;

    if (tareaID == 0){
        y = 48;
        x_inicial = 4;
        texto_a_mostrar = valores[reloj_actual_sanas[tarea_indice]];
        reloj_actual_sanas[tarea_indice] = (reloj_actual_sanas[tarea_indice] + 1) % 4;
    } else if (tareaID == 1){
        y = 46;
        x_inicial = 4;
        texto_a_mostrar = valores[reloj_actual_jugadores[0][tarea_indice]];
        reloj_actual_jugadores[0][tarea_indice] = (reloj_actual_jugadores[0][tarea_indice] + 1) % 4;
    } else {
        y = 46;
        x_inicial = 24;
        texto_a_mostrar = valores[reloj_actual_jugadores[1][tarea_indice]];
        reloj_actual_jugadores[1][tarea_indice] = (reloj_actual_jugadores[1][tarea_indice] + 1) % 4;
    }
        
    
    p[y][x_inicial + 2 * tarea_indice].c = texto_a_mostrar;
    p[y][x_inicial + 2 * tarea_indice].a = 0x07;
}

void screen_resetear_reloj_tarea(unsigned char tareaID, unsigned char tarea_indice){
    ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO_SCREEN;
    unsigned char y;
    unsigned char x_inicial;

    if (tareaID == 0){
        y = 48;
        x_inicial = 4;
    } else if (tareaID == 1){
        y = 46;
        x_inicial = 4;
    } else {
        y = 46;
        x_inicial = 24;
    }

    p[y][x_inicial + 2 * tarea_indice].c = 'x';
    p[y][x_inicial + 2 * tarea_indice].a = 0x07;
}

void screen_pintar_sector(unsigned int x, unsigned int y, unsigned short attr) {
    ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO_SCREEN;
    p[y][x].a = (unsigned char) attr; 
}

void print_char_sin_attr(char text, unsigned int x, unsigned int y) {
    ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO_SCREEN;
    p[y][x].c = (unsigned char) text;
}

void imprimo_interrupcion_pantalla(
    int eax, int ebx, int ecx, int edx, int esi, int edi, int ebp,
    int ds,  int es,  int fs,  int gs, unsigned char idTarea,
    int eip, int cs, int eflags, int esp, int ss) {
    ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO_SCREEN;
    ca (*p_guardada)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) &pantalla_guardada;
    unsigned int i, j;

    unsigned char color_tarea;
    char* texto = "TAREA SANA";

    if (idTarea == 1){
        color_tarea = 0x4f;
        texto = "TAREA A";
    }
    else if (idTarea == 2){
        color_tarea = 0x1f;
        texto = "TAREA B";
    }
    else {
        color_tarea = 0x2f;
    }

    // Guardo pantalla
    for (i = 0; i < 50; i++) {
        for (j = 0; j < 80; j++){
            p_guardada[i][j] = p[i][j];
        }
    }

    // Hago espacio gris en el medio
    for (i = 25; i < 55; i++) {
        for (j = 7; j < 43; j++){
            if (i == 25 || i == 54 || j == 7 || j == 42)
                print_char(0,i,j,(unsigned short) 0x0f);
            else
                print_char(0,i,j,(unsigned short) 0x7f);
        }
    }

    i = 8;
    for (j = 26; j < 54; j++){
        print_char(0,j,i,(unsigned short) color_tarea);
    }
    print(texto, 26, 8, color_tarea);
    // print_int((unsigned int)idTarea, 0, 0, 0x0f);

    print("eax", 27, 10, 0x70);
    print("ebx", 27, 12, 0x70);
    print("ecx", 27, 14, 0x70);
    print("edx", 27, 16, 0x70);
    print("esi", 27, 18, 0x70);
    print("edi", 27, 20, 0x70);
    print("ebp", 27, 22, 0x70);
    print("esp", 27, 24, 0x70);
    print("eip", 27, 26, 0x70);
    print("cs", 28, 28, 0x70);
    print("ds", 28, 30, 0x70);
    print("es", 28, 32, 0x70);
    print("fs", 28, 34, 0x70);
    print("gs", 28, 36, 0x70);
    print("ss", 28, 38, 0x70);
    print("eflags", 28, 40, 0x70);
    print("cr0", 41, 10, 0x70);
    print("cr2", 41, 12, 0x70);
    print("cr3", 41, 14, 0x70);
    print("cr4", 41, 16, 0x70);
    print("stack", 41, 27, 0x70);

    print_hex(eax, 8, 31, 10, 0x7f);
    print_hex(ebx, 8, 31, 12, 0x7f);
    print_hex(ecx, 8, 31, 14, 0x7f);
    print_hex(edx, 8, 31, 16, 0x7f);
    print_hex(esi, 8, 31, 18, 0x7f);
    print_hex(edi, 8, 31, 20, 0x7f);
    print_hex(ebp, 8, 31, 22, 0x7f);
    print_hex(esp, 8, 31, 24, 0x7f);
    print_hex(eip, 8, 31, 26, 0x7f);
    print_hex(cs,  4, 31, 28, 0x7f);
    print_hex(ds,  4, 31, 30, 0x7f);
    print_hex(es,  4, 31, 32, 0x7f);
    print_hex(fs,  4, 31, 34, 0x7f);
    print_hex(gs,  4, 31, 36, 0x7f);
    print_hex(ss,  4, 31, 38, 0x7f);
    print_hex(eflags, 8, 35, 40, 0x7f);
    print_hex(rcr0(), 8, 45, 10, 0x7f);
    print_hex(rcr2(), 8, 45, 12, 0x7f);
    print_hex(rcr3(), 8, 45, 14, 0x7f);
    print_hex(rcr4(), 8, 45, 16, 0x7f);

    unsigned int* stack;
    i = 29;
    while(esp < ebp){
        stack = (unsigned int*) esp;
        print_hex((*stack), 8, 41, i, 0x7f);
        esp += 32;
        i += 2;
    }
}

void restauro_pantalla(){
    ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO_SCREEN;
    ca (*p_guardada)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) pantalla_guardada;
    unsigned int i, j;
    for (i = 0; i < 50; i++) {
        for (j = 0; j < 80; j++){
            p[i][j] = p_guardada[i][j];
        }
    }
}