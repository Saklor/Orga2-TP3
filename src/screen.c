/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#include "screen.h"

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

    for ( j = 0; j < VIDEO_COLS; j++){
        print_hex(0,1,j,0,0);
    }

    for ( i = 1; i <= 44; i++){
        for ( j = 0; j < VIDEO_COLS; j++){
            print_hex(0,1,j,i,(unsigned short)0x77);
        }
    }

    for (i = 45; i < 50; i++){
        for (j = 0; j < 80; j++){
            print_hex(0,1,j,i,0);
        }
    }

    //pinto jugador A y B

    print_hex(10,1,40,20,(unsigned short) 0x4f);
    print_hex(11,1,41,20,(unsigned short) 0x1f);
    //imprimo las h
    
    print_hex(0,1,34,32,(unsigned short) 0x22);
    print_hex(0,1,58,11,(unsigned short) 0x22);
    print_hex(0,1,52,1,(unsigned short) 0x22);
    print_hex(0,1,42,21,(unsigned short) 0x22);
    print_hex(0,1,13,19,(unsigned short) 0x22);
    print_hex(0,1,25,18,(unsigned short) 0x22);
    print_hex(0,1,60,25,(unsigned short) 0x22);
    print_hex(0,1,75,15,(unsigned short) 0x22);
    print_hex(0,1,32,15,(unsigned short) 0x22);
    print_hex(0,1,56,3,(unsigned short) 0x22);
    print_hex(0,1,37,40,(unsigned short) 0x22);
    print_hex(0,1,14,35,(unsigned short) 0x22);
    print_hex(0,1,22,5,(unsigned short) 0x22);
    print_hex(0,1,74,8,(unsigned short) 0x22);
    print_hex(0,1,41,24,(unsigned short) 0x22);


    for (i = 45; i < 50; i++){
        for (j = 50; j < 57; j++){
            print_hex(0,1,j,i,(unsigned short) 0x44);
        }
    }

    for (i = 45; i < 50; i++){
        for (j = 57; j < 64; j++){
            print_hex(0,1,j,i,(unsigned short) 0x11);
        }
    }    

}

void pintar_tarea_en_mapa(int jugador, unsigned short pos_x, unsigned short pos_y){

}


