; ** por compatibilidad se omiten tildes **
; ==============================================================================
; TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==============================================================================

%include "imprimir.mac"

global start
extern GDT_DESC
extern IDT_DESC
extern idt_inicializar
extern mmu_inicialiar_dir_kernel

;; Saltear seccion de datos
jmp start

;;
;; Seccion de datos.
;; -------------------------------------------------------------------------- ;;
iniciando_mr_msg db     'Iniciando kernel (Modo Real)...'
iniciando_mr_len equ    $ - iniciando_mr_msg

iniciando_mp_msg db     'Iniciando kernel (Modo Protegido)...'
iniciando_mp_len equ    $ - iniciando_mp_msg

;;
;; Seccion de código.
;; -------------------------------------------------------------------------- ;;

;; Punto de entrada del kernel.
BITS 16
start:
    ; Deshabilitar interrupciones
    cli

    ; Cambiar modo de video a 80 X 50
    mov ax, 0003h
    int 10h ; set mode 03h
    xor bx, bx
    mov ax, 1112h
    int 10h ; load 8x8 font

    ; Imprimir mensaje de bienvenida
    imprimir_texto_mr iniciando_mr_msg, iniciando_mr_len, 0x07, 0, 0
    

    ; Habilitar A20
    call habilitar_A20      ;Ravioli

    ; Cargar la GDT
    lgdt [GDT_DESC]

    ; Setear el bit PE del registro CR0
    mov eax, cr0
    or al, 1
    mov cr0, eax

    ; Saltar a modo protegido
    jmp 0x08:modoprotegido

BITS 32
    modoprotegido:
    ; Establecer selectores de segmentos
    xor eax, eax
    
    mov ax, 0x18
    mov es, ax      ;RAVIOLI
    mov gs, ax      ;RAVIOLI
    mov ds, ax      ;RAVIOLI
    mov ss, ax      ;RAVIOLI

    mov ax, 0x28
    mov fs, ax      ;RAVIOLI

    ; Establecer la base de la pila
    mov esp, 0x27000

    ; Imprimir mensaje de bienvenida
    imprimir_texto_mp iniciando_mp_msg, iniciando_mp_len, 0x07, 0, 0

    ; Inicializar pantalla
    ;mov eax, 50     ;eax = filas
    ;mov ebx, 80     ;ebx = columnas
    ;imul ebx        ;eax = filas * columnas

    call pintar_pantalla

    ; Inicializar el manejador de memoria

    ; Inicializar el directorio de paginas
    call mmu_inicialiar_dir_kernel

    ; Cargar directorio de paginas
    

    ; Habilitar paginacion
    ;mov eax, page_directory
    ;mov cr3, eax

    ;mov eax, cr0
    ;or eax, 0x80000000
    ;mov cr0, eax

    ; Inicializar tss

    ; Inicializar tss de la tarea Idle

    ; Inicializar el scheduler

    ; Inicializar la IDT
    call idt_inicializar
    
    ; Cargar IDT
    lidt [IDT_DESC]

    ; Configurar controlador de interrupciones

    ; Cargar tarea inicial

    ; Habilitar interrupciones
    sti

    ; Saltar a la primera tarea: Idle

    ; Ciclar infinitamente (por si algo sale mal...)
    mov eax, 0xFFFF
    mov ebx, 0xFFFF
    mov ecx, 0xFFFF
    mov edx, 0xFFFF
    jmp $
    jmp $

pintar_pantalla:
    ;al = color
    ;edi = fila_inicio
    ;esi = col_inicio
    ;edx = cant_fila
    ;ecx = cant_col

    xor eax, eax
    mov ax, 0x7000
    mov ecx, 4000
    mov ebx, 0xb8000
    .limpiar_pantalla:
        cmp ecx, 400
        jg .limpiar_pantala_continuar
            mov ax, 0x0F00
        .limpiar_pantala_continuar:
        mov [ebx], ax
        add ebx, 2
    loop .limpiar_pantalla
ret
;; -------------------------------------------------------------------------- ;;

%include "a20.asm"
