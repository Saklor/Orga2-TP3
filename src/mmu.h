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
    unsigned int 	dir_base:20;
    unsigned char  	disponibles:3;
    
    unsigned char   g:1;
    unsigned char   ps:1;
    unsigned char   ignored:1;
    unsigned char   a:1;
    unsigned char   pcd:1;
    unsigned char   pwt:1;
    unsigned char   u_s:1;
    unsigned char   r_w:1;
    unsigned char   p:1;
    
} __attribute__((__packed__)) pdt_entry;

typedef struct str_pte_entry {
    unsigned int 	dir_base:20;
    unsigned char  	disponibles:3;
    
    unsigned char   g:1;
    unsigned char   pat:1;
    unsigned char   d:1;
    unsigned char   a:1;
    unsigned char   pcd:1;
    unsigned char   pwt:1;
    unsigned char   u_s:1;
    unsigned char   r_w:1;
    unsigned char   p:1;
    
} __attribute__((__packed__)) pte_entry;

void mmu_inicializar();
void mmu_inicialiar_dir_kernel();

#endif	/* !__MMU_H__ */




