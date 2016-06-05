/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

    Definiciones globales del sistema.
*/

#ifndef __DEFINES_H__
#define __DEFINES_H__

/* Bool */
/* -------------------------------------------------------------------------- */
#define TRUE                    0x00000001
#define FALSE                   0x00000000
#define ERROR                   1


/* Misc */
/* -------------------------------------------------------------------------- */
#define CANT_H                 15
#define CANT                    5
#define SIZE_W                  80
#define SIZE_H                  44


/* Indices en la gdt */
/* -------------------------------------------------------------------------- */
#define GDT_COUNT 6

#define GDT_IDX_NULL_DESC           0
#define GDT_IDX_COD_LVL0            1
#define GDT_IDX_COD_LVL3            2
#define GDT_IDX_DATA_LVL0           3
#define GDT_IDX_DATA_LVL3           4
#define GDT_IDX_DATA_VID            5

/* Offsets en la gdt */
/* -------------------------------------------------------------------------- */
#define GDT_OFF_NULL_DESC           (GDT_IDX_NULL_DESC      << 3)
#define GDT_OFF_COD_LVL0            (GDT_IDX_COD_LVL0       << 3)
#define GDT_OFF_COD_LVL3            (GDT_IDX_COD_LVL3       << 3)
#define GDT_OFF_DATA_LVL0           (GDT_IDX_DATA_LVL0      << 3)
#define GDT_OFF_DATA_LVL3           (GDT_IDX_DATA_LVL3      << 3)
#define GDT_OFF_DATA_VID            (GDT_IDX_DATA_VID       << 3)

/* Direcciones de memoria */
/* -------------------------------------------------------------------------- */
#define VIDEO_SCREEN            0x000B8000 /* direccion fisica del buffer de video */

/* Otros */
/* -------------------------------------------------------------------------- */
#define PAGE_SIZE				0x1000

#endif  /* !__DEFINES_H__ */
