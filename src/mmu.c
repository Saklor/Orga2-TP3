/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del manejador de memoria
*/

#include "mmu.h"

pdt_entry pdt[1024];
pte_entry pte[1024];

void mmu_inicializar() {
}

void mmu_inicialiar_dir_kernel() {
	unsigned int base = 0x00000000;
	int i;

	pdt[0] = (pdt_entry) {
	        (unsigned int)    	0x28000,       	/* base[0:19]  */
	        (unsigned char)    	0x00,         	/* reservados[0:2]   */
	        (unsigned char)     0x00,           /* g            */
	        (unsigned char)     0x00,           /* ps           */
	        (unsigned char)     0x00,           /* ignored      */
	        (unsigned char)     0x00,           /* a            */
	        (unsigned char)     0x00,           /* pcd          */
	        (unsigned char)     0x00,           /* pwt          */
	        (unsigned char)     0x00,           /* u_s          */
	        (unsigned char)     0x01,           /* r_w          */
	        (unsigned char)     0x01,           /* p            */
	    };

   	for (i = 1; i < 1024; i++) {
		pdt[i] = (pdt_entry) {
	        (unsigned int)    	0x00000000,    	/* base[0:19]  */
	        (unsigned char)    	0x00,         	/* reservados[0:2]   */
	        (unsigned char)     0x00,           /* g            */
	        (unsigned char)     0x00,           /* ps           */
	        (unsigned char)     0x00,           /* ignored      */
	        (unsigned char)     0x00,           /* a            */
	        (unsigned char)     0x00,           /* pcd          */
	        (unsigned char)     0x00,           /* pwt          */
	        (unsigned char)     0x00,           /* u_s          */
	        (unsigned char)     0x00,           /* r_w          */
	        (unsigned char)     0x00,           /* p            */
	    };
	}


	for (i = 0; i < 1024; i++) {
		pte[i] = (pte_entry) {
	        (unsigned int)    	base,         /* base[0:19]  */
	        (unsigned char)    	0x00,         /* reservados[0:2]   */
	        (unsigned char)     0x00,           /* g            */
	        (unsigned char)     0x00,           /* pat          */
	        (unsigned char)     0x00,           /* d            */
	        (unsigned char)     0x00,           /* a            */
	        (unsigned char)     0x00,           /* pcd          */
	        (unsigned char)     0x00,           /* pwt          */
	        (unsigned char)     0x00,           /* u_s          */
	        (unsigned char)     0x01,           /* r_w          */
	        (unsigned char)     0x01,           /* p            */
	    };
	    base = base + 0x00001000;
	}
}

