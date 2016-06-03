/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del manejador de memoria
*/

#include "mmu.h"

pdt_entry pdt[1024];
pte_entry pte[1024];

unsigned int proxima_pagina_libre;

void mmu_inicializar() {
	proxima_pagina_libre = 0x11C0000;
}

unsigned int mmu_proxima_pagina_fisica_libre() {
	unsigned int pagina_libre = proxima_pagina_libre;
	proxima_pagina_libre += PAGE_SIZE;
	return pagina_libre;
}

void mmu_inicialiar_dir_kernel() {
	pdt_entry* pdt = (pdt_entry*) 0x00027000;
	pte_entry* pte = (pte_entry*) 0x00028000;
	int i;

	pdt[0] = (pdt_entry) {
	        (unsigned char)     0x01,           /* p            */
	        (unsigned char)     0x01,           /* r_w          */
	        (unsigned char)     0x00,           /* u_s          */
	        (unsigned char)     0x00,           /* pwt          */
	        (unsigned char)     0x00,           /* pcd          */
	        (unsigned char)     0x00,           /* a            */
	        (unsigned char)     0x00,           /* ignored      */
	        (unsigned char)     0x00,           /* ps           */
	        (unsigned char)     0x00,           /* g            */
	        (unsigned char)    	0x00,         	/* reservados[0:2]   */
	        (unsigned int)    	0x00000028,   	/* base[0:19]  */
	    };

   	for (i = 1; i < 1024; i++) {
		pdt[i] = (pdt_entry) {
			(unsigned char)     0x00,           /* p            */
	        (unsigned char)     0x00,           /* r_w          */
	        (unsigned char)     0x00,           /* u_s          */
	        (unsigned char)     0x00,           /* pwt          */
	        (unsigned char)     0x00,           /* pcd          */
	        (unsigned char)     0x00,           /* a            */
	        (unsigned char)     0x00,           /* ignored      */
	        (unsigned char)     0x00,           /* ps           */
	        (unsigned char)     0x00,           /* g            */
	        (unsigned char)    	0x00,         	/* reservados[0:2]   */
	        (unsigned int)    	0x00000000,   	/* base[0:19]  */
	    };
	}


	for (i = 0; i < 1024; i++) {
		pte[i] = (pte_entry) {
			(unsigned char)     0x01,           /* p            */
	        (unsigned char)     0x01,           /* r_w          */
	        (unsigned char)     0x00,           /* u_s          */
	        (unsigned char)     0x00,           /* pwt          */
	        (unsigned char)     0x00,           /* pcd          */
	        (unsigned char)     0x00,           /* a            */
	        (unsigned char)     0x00,           /* ignored      */
	        (unsigned char)     0x00,           /* ps           */
	        (unsigned char)     0x00,           /* g            */
	        (unsigned char)    	0x00,         	/* reservados[0:2]   */
	        (unsigned int)    	i,			   	/* base[0:19]  */
	    };
	}
}

void mmu_mapear_pagina(unsigned int virtualaddr, unsigned int cr3, unsigned int fisicaaddr) {
	unsigned long pd_offset = (unsigned long)virtualaddr >> 22;
    unsigned long pt_index = (unsigned long)virtualaddr >> 12 & 0x03FF;
    
}