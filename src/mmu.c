/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del manejador de memoria
*/

#include "mmu.h"

//pdt_entry pdt[1024];
//pte_entry pte[1024];

unsigned int proxima_pagina_libre;

void mmu_inicializar() {
	proxima_pagina_libre = 0x0100000;
}

unsigned int mmu_proxima_pagina_fisica_libre() {
	unsigned int pagina_libre = proxima_pagina_libre;
	proxima_pagina_libre += PAGE_SIZE;
	return pagina_libre;
}

void mmu_inicializar_dir_kernel() {
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

unsigned int mmu_inicializar_dir_tarea(unsigned int dir_codigo, short pos_x, short pos_y) {
	pdt_entry* pdt = (pdt_entry*) mmu_proxima_pagina_fisica_libre();
	int i;
	unsigned int cacho_codigo;

	for (i = 0; i < 1024; i++) {
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

	unsigned int* copiar_codigo = (unsigned int*) ( 0x40000 + (pos_x * 2) + (pos_y * 160) );

	for(i = 0; i < 1024; i++){
		cacho_codigo = *((unsigned int*) dir_codigo);
		*copiar_codigo = cacho_codigo;
		dir_codigo += 4;
		copiar_codigo += 4;
	}

	mmu_mapear_pagina(0x08000000, (unsigned int) pdt, (unsigned int) copiar_codigo);
	mmu_mapear_pagina(0x08001000, (unsigned int) pdt, (unsigned int) copiar_codigo);

	return (unsigned int) pdt;
	//Devuelve el puntero a la PDT
}

void mmu_mapear_pagina(unsigned int virtualaddr, unsigned int cr3, unsigned int fisicaaddr) {
	unsigned int pd_offset = virtualaddr >> 22;
    unsigned int pt_index = virtualaddr >> 12 & 0x03FF;

    int i;

    pdt_entry* pdte;
    

    //unsigned long * pd = (unsigned long *)0x00027000;

    pdte = (pdt_entry*) ( (unsigned int) ( cr3 & 0xFFFFFC00 ) + (unsigned int) (pd_offset * 4));		//RAVIOLI

    // Here you need to check whether the PD entry is present.
    // When it is not present, you need to create a new empty PT and
    // adjust the PDE accordingly.

    if ( (unsigned char) pdte->p != 0x01 ) {

    	pte_entry* pte = (pte_entry*) mmu_proxima_pagina_fisica_libre();

    	for (i = 0; i < 1024; i++) {
			pte[i] = (pte_entry) {
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
		        (unsigned int)    	0x00000000,	   	/* base[0:19]  */
		    };
		}
		pdte->p = (unsigned char) 0x01;
		pdte->r_w = (unsigned char) 0x01;
		pdte->dir_base = (((unsigned int) pte) >> 12);
    }


    //unsigned long * pt = ((unsigned long *)(pdte->base)) + (0x1000 * );
    pte_entry* ptee = (pte_entry*) ((unsigned int)(pdte->dir_base << 12) + (unsigned int)(pt_index * 4));		//RAVIOLI

    ptee->p = (unsigned char) 0x01;
    ptee->r_w = (unsigned char) 0x01;
    ptee->dir_base = (fisicaaddr >> 12);
    //ayy lmao

    tlbflush();

}

void mmu_unmapear_pagina(unsigned int virtualaddr, unsigned int cr3){
	unsigned long pd_offset = (unsigned long)virtualaddr >> 22;
    unsigned long pt_index = (unsigned long)virtualaddr >> 12 & 0x03FF;

    pdt_entry* pdte;

    //unsigned long * pd = (unsigned long *)0x00027000;

    pdte = (pdt_entry*) ( ( cr3 & 0xFFC00000 ) + (pd_offset * 4));		//RAVIOLI


    //unsigned long * pt = ((unsigned long *)(pdte->base)) + (0x1000 * );
    pte_entry* ptee = (pte_entry*) (pdte->dir_base << 12) + (pt_index * 4);		//RAVIOLI
    
    ptee->p = 0x00;
    ptee->r_w = 0x00;
    ptee->dir_base = 0x00000000;
    //ayy lmao

    tlbflush();
}