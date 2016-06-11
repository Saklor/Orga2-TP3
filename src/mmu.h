/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del manejador de memoria
*/

#ifndef __MMU_H__
#define __MMU_H__

#include "defines.h"
#include "i386.h"
#include "tss.h"
#include "game.h"

typedef struct str_pdt_entry {
    unsigned char   p:1;
    unsigned char   r_w:1;
    unsigned char   u_s:1;
    unsigned char   pwt:1;
    unsigned char   pcd:1;
    unsigned char   a:1;
    unsigned char   ignored:1;
    unsigned char   ps:1;
    unsigned char   g:1;
    
    unsigned char   disponibles:3;
    unsigned int    dir_base:20;
    
} __attribute__((__packed__)) pdt_entry;

typedef struct str_pte_entry {
    unsigned char   p:1;
    unsigned char   r_w:1;
    unsigned char   u_s:1;
    unsigned char   pwt:1;
    unsigned char   pcd:1;
    unsigned char   a:1;
    unsigned char   d:1;
    unsigned char   pat:1;
    unsigned char   g:1;
    
    unsigned char   disponibles:3;
    unsigned int 	dir_base:20;
    
} __attribute__((__packed__)) pte_entry;

void mmu_inicializar();
unsigned int mmu_proxima_pagina_fisica_libre();
void mmu_inicializar_dir_kernel();
unsigned int mmu_inicializar_dir_tarea(unsigned int dir_codigo, short pos_x, short pos_y);

void mmu_mapear_pagina(unsigned int virtualaddr, unsigned int cr3, unsigned int fisicaaddr);
void mmu_unmapear_pagina(unsigned int virtualaddr, unsigned int cr3);

#endif	/* !__MMU_H__ */




