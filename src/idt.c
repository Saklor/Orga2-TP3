/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de las rutinas de atencion de interrupciones
*/

#include "defines.h"
#include "idt.h"
#include "isr.h"
#include "screen.h"

#include "tss.h"

idt_entry idt[255] = { };


idt_descriptor IDT_DESC = {
    sizeof(idt) - 1,
    (unsigned int) &idt
};

/*
    La siguiente es una macro de EJEMPLO para ayudar a armar entradas de
    interrupciones. Para usar, descomentar y completar CORRECTAMENTE los
    atributos y el registro de segmento. Invocarla desde idt_inicializar() de
    la siguiene manera:

    void idt_inicializar() {
        IDT_ENTRY(0);
        ...
        IDT_ENTRY(19);

        ...
    }
*/


#define IDT_ENTRY(numero)                                                                                        \
    idt[numero].offset_0_15 = (unsigned short) ((unsigned int)(&_isr ## numero) & (unsigned int) 0xFFFF);        \
    idt[numero].segsel = (unsigned short) 0x08;                                                                  \
    idt[numero].attr = (unsigned short) 0x8E00;                                                                  \
    idt[numero].offset_16_31 = (unsigned short) ((unsigned int)(&_isr ## numero) >> 16 & (unsigned int) 0xFFFF);

void idt_inicializar() {
    // Excepciones
    IDT_ENTRY(0);
    IDT_ENTRY(1);
    IDT_ENTRY(2);
    IDT_ENTRY(3);
    IDT_ENTRY(4);
    IDT_ENTRY(5);
    IDT_ENTRY(6);
    IDT_ENTRY(7);
    IDT_ENTRY(8);
    IDT_ENTRY(9);
    IDT_ENTRY(10);
    IDT_ENTRY(11);
    IDT_ENTRY(12);
    IDT_ENTRY(13);
    IDT_ENTRY(14);
    IDT_ENTRY(15);
    IDT_ENTRY(16);
    IDT_ENTRY(17);
    IDT_ENTRY(18);
    IDT_ENTRY(19);
    IDT_ENTRY(20);

    IDT_ENTRY(32);
    idt[32].attr = (unsigned short) 0x8E00;
    IDT_ENTRY(33);
    idt[33].attr = (unsigned short) 0x8E00;

    IDT_ENTRY(102);
    idt[102].attr = (unsigned short) 0xEE00;
}

void manejo_teclado(char scanCode, char debug_en_screen){
    char valor = 1;
    int player;
    direccion dir;

    if (debug_en_screen == 0 || scanCode == 0x15) {
        if(scanCode == 0x2a){ 
            valor = '1';
            game_lanzar(1);

        } else if(scanCode == 0x36){
            valor = '2';
            game_lanzar(2);

        } else if(scanCode == 0x1f){
            valor = 's'; 
            dir = ABA;
            player = 0;
            game_mover_cursor(player,dir);

        } else if(scanCode == 0x1e){
            valor = 'a';
            dir = IZQ;
            player = 0;
            game_mover_cursor(player,dir);

        } else if(scanCode == 0x20){
            valor = 'd';
            dir = DER;
            player = 0;
            game_mover_cursor(player,dir);

        } else if(scanCode == 0x11){
            valor = 'w';
            dir = ARB;
            player = 0;
            game_mover_cursor(player,dir);

        } else if(scanCode ==0x17 ){
            valor = 'i';
            dir = ARB;
            player = 1;
            game_mover_cursor(player,dir);

        } else if(scanCode == 0x24){
            valor = 'j';
            dir = IZQ;
            player = 1;
            game_mover_cursor(player,dir);

        } else if(scanCode == 0x25){
            valor ='k' ;
            dir = ABA;
            player = 1;
            game_mover_cursor(player,dir);

        } else if(scanCode == 0x26 ){
            valor = 'l';
            dir = DER;
            player = 1;
            game_mover_cursor(player,dir);          
        } else if(scanCode == 0x15){
            //modo debug
            valor = 'y';
            lanzar_modo_debug();
        }
    }
    
    print_char(valor, 79, 0, (unsigned short)0x0E);
}