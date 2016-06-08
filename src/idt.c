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
    idt[numero].attr = (unsigned short) 0x8700;                                                                  \
    idt[numero].offset_16_31 = (unsigned short) ((unsigned int)(&_isr ## numero) >> 16 & (unsigned int) 0xFFFF);

void idt_inicializar() {
    // Excepciones
    IDT_ENTRY(0);
    IDT_ENTRY(1);

    IDT_ENTRY(13);

    IDT_ENTRY(32);
    idt[32].attr = (unsigned short) 0x8E00;
    IDT_ENTRY(33);
    idt[33].attr = (unsigned short) 0x8E00;

    IDT_ENTRY(66);
}

char manejo_teclado(char scanCode){
    char valor;

    if(scanCode == 0x2a){
        valor = '1';    
    }
    if(scanCode == 0x36){
        valor = '2';    
    }
    if(scanCode == 0x1f){
        valor = 's';    
    }
    //1e (A), 1f (S), 20 
    if(scanCode == 0x1e){
        valor = 'a';    
    }
    if(scanCode == 0x20){
        valor = 'd';
    }
    if(scanCode == 0x11){
        valor = 'w';
    }
    //24 (J), 25 (K), 26 (L)
    if(scanCode ==0x17 ){
        valor = 'i';    
    }
    if(scanCode == 0x24){
        valor = 'j';    
    }
    if(scanCode == 0x25){
        valor ='k' ;    
    }
    if(scanCode == 0x26 ){
        valor = 'l';    
    }

    short ayylmao = (short) valor;

    print((char *)&ayylmao,79 ,0,(unsigned short)0x0E);
    return valor;
}