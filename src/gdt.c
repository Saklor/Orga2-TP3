/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de la tabla de descriptores globales
*/

#include "gdt.h"

gdt_entry gdt[GDT_COUNT] = {
    /* Descriptor nulo*/
    /* Offset = 0x00 */
    [GDT_IDX_NULL_DESC] = (gdt_entry) {
        (unsigned short)    0x0000,         /* limit[0:15]  */
        (unsigned short)    0x0000,         /* base[0:15]   */
        (unsigned char)     0x00,           /* base[23:16]  */
        (unsigned char)     0x00,           /* type         */
        (unsigned char)     0x00,           /* s            */
        (unsigned char)     0x00,           /* dpl          */
        (unsigned char)     0x00,           /* p            */
        (unsigned char)     0x00,           /* limit[16:19] */
        (unsigned char)     0x00,           /* avl          */
        (unsigned char)     0x00,           /* l            */
        (unsigned char)     0x00,           /* db           */
        (unsigned char)     0x00,           /* g            */
        (unsigned char)     0x00,           /* base[31:24]  */
    },


    /* Descriptor codigo privilegio 00*/
    /* Offset = 0x08 */
    [GDT_IDX_COD_LVL0] = (gdt_entry) {
        (unsigned short)    0x6E00,         /* limit[0:15]  */
        (unsigned short)    0x0000,         /* base[0:15]   */
        (unsigned char)     0x00,           /* base[23:16]  */
        (unsigned char)     0x0A,           /* type =  Code Execute/Read */
        (unsigned char)     0x01,           /* s            */
        (unsigned char)     0x00,           /* dpl          */
        (unsigned char)     0x01,           /* p            */
        (unsigned char)     0x03,           /* limit[16:19] */
        (unsigned char)     0x01,           /* avl          */
        (unsigned char)     0x00,           /* l            */
        (unsigned char)     0x01,           /* db           */
        (unsigned char)     0x01,           /* g            */
        (unsigned char)     0x00,           /* base[31:24]  */
    },

    /* Descriptor codigo privilegio 11*/
    /* Offset = 0x10 */
    [GDT_IDX_COD_LVL3] = (gdt_entry) {
        (unsigned short)    0x6E00,         /* limit[0:15]  */
        (unsigned short)    0x0000,         /* base[0:15]   */
        (unsigned char)     0x00,           /* base[23:16]  */
        (unsigned char)     0x0A,           /* type =  Code Execute/Read */
        (unsigned char)     0x01,           /* s            */
        (unsigned char)     0x03,           /* dpl          */
        (unsigned char)     0x01,           /* p            */
        (unsigned char)     0x03,           /* limit[16:19] */
        (unsigned char)     0x01,           /* avl          */
        (unsigned char)     0x00,           /* l            */
        (unsigned char)     0x01,           /* db           */
        (unsigned char)     0x01,           /* g            */
        (unsigned char)     0x00,           /* base[31:24]  */
    },

    /* Descriptor Data privilegio 00*/
    /* Offset = 0x18 */
    [GDT_IDX_DATA_LVL0] = (gdt_entry) {
        (unsigned short)    0x6E00,         /* limit[0:15]  */
        (unsigned short)    0x0000,         /* base[0:15]   */
        (unsigned char)     0x00,           /* base[23:16]  */
        (unsigned char)     0x02,           /* type = Data Read/Write */
        (unsigned char)     0x01,           /* s            */
        (unsigned char)     0x00,           /* dpl          */
        (unsigned char)     0x01,           /* p            */
        (unsigned char)     0x03,           /* limit[16:19] */
        (unsigned char)     0x01,           /* avl          */
        (unsigned char)     0x00,           /* l            */
        (unsigned char)     0x01,           /* db           */
        (unsigned char)     0x01,           /* g            */
        (unsigned char)     0x00,           /* base[31:24]  */
    },

    /* Descriptor Data privilegio 11*/
    /* Offset = 0x20 */
    [GDT_IDX_DATA_LVL3] = (gdt_entry) {
        (unsigned short)    0x6E00,         /* limit[0:15]  */
        (unsigned short)    0x0000,         /* base[0:15]   */
        (unsigned char)     0x00,           /* base[23:16]  */
        (unsigned char)     0x02,           /* type = Data Read/Write */
        (unsigned char)     0x01,           /* s            */
        (unsigned char)     0x03,           /* dpl          */
        (unsigned char)     0x01,           /* p            */
        (unsigned char)     0x03,           /* limit[16:19] */
        (unsigned char)     0x01,           /* avl          */
        (unsigned char)     0x00,           /* l            */
        (unsigned char)     0x01,           /* db           */
        (unsigned char)     0x01,           /* g            */
        (unsigned char)     0x00,           /* base[31:24]  */
    },

    /* Descriptor Data Video*/
    /* Offset = 0x28 */
    [GDT_IDX_DATA_VID] = (gdt_entry) {
        (unsigned short)    0x6E00,         /* limit[0:15]  */
        (unsigned short)    0x8000,         /* base[0:15]   */
        (unsigned char)     0x0B,           /* base[23:16]  */
        (unsigned char)     0x02,           /* type = Data Read/Write */
        (unsigned char)     0x01,           /* s            */
        (unsigned char)     0x00,           /* dpl          */
        (unsigned char)     0x01,           /* p            */
        (unsigned char)     0x03,           /* limit[16:19] */
        (unsigned char)     0x01,           /* avl          */
        (unsigned char)     0x00,           /* l            */
        (unsigned char)     0x01,           /* db           */
        (unsigned char)     0x01,           /* g            */
        (unsigned char)     0x00,           /* base[31:24]  */
    },


};

gdt_descriptor GDT_DESC = {
    sizeof(gdt) - 1,
    (unsigned int) &gdt
};

unsigned int agregar_tss(unsigned int donde){
    unsigned int tss_index;
    tss_index = dame_entrada_libre();
    unsigned char donde_alta = (unsigned char) (donde >> 24);
    unsigned char donde_intermedio = (unsigned char) ((donde >> 16) & 0x000000FF);
    unsigned short donde_baja = (unsigned short) (donde & 0x0000FFFF);

    gdt[tss_index] = (gdt_entry) {
        (unsigned short)    0x0068,         /* limit[0:15]  */
        (unsigned short)    donde_baja,         /* base[0:15]   */
        (unsigned char)     donde_intermedio,           /* base[23:16]  */
        (unsigned char)     0x09,           /* type = Task Segment papu */
        (unsigned char)     0x00,           /* s            */
        (unsigned char)     0x00,           /* dpl          */
        (unsigned char)     0x01,           /* p            */
        (unsigned char)     0x00,           /* limit[16:19] */
        (unsigned char)     0x01,           /* avl          */
        (unsigned char)     0x00,           /* l            */
        (unsigned char)     0x00,           /* db           */
        (unsigned char)     0x01,           /* g            */
        (unsigned char)     donde_alta,           /* base[31:24]  */
    };

    return tss_index;
}

unsigned int dame_entrada_libre(){
    unsigned int i;
    unsigned int indice_libre;
    for(i = 1; i < GDT_COUNT; i++){
        if( (unsigned short) gdt[i].limit_0_15 == (unsigned short) 0x0000 && (unsigned char) gdt[i].limit_16_19 == (unsigned char) 0x00){
            indice_libre = i;
            i = GDT_COUNT;
        }
    }
    return indice_libre;
}
