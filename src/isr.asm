; ** por compatibilidad se omiten tildes **
; ==============================================================================
; TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==============================================================================
; definicion de rutinas de atencion de interrupciones

%include "imprimir.mac"


BITS 32

sched_tarea_offset:     dd 0x00
sched_tarea_selector:   dw 0x00

;; PIC
extern fin_intr_pic1

;; Sched
extern sched_proximo_indice
; extern sched_idle

;;Manejo del teclado
extern manejo_teclado

;;Extra para interrupciones
extern print_hex
extern imprimo_interrupcion_pantalla
extern restauro_pantalla

;; Game
extern game_soy
extern game_donde
extern game_mapear
extern game_matar_tarea

;;
;; Definición de MACROS
;; -------------------------------------------------------------------------- ;;

%macro ISR 1
global _isr%1

_isr%1:
sub esp, 4
pushad
    mov eax, %1

    cmp eax, 0xA
    je .error_code
    cmp eax, 0xB
    je .error_code
    cmp eax, 0xC
    je .error_code
    cmp eax, 0xD
    je .error_code
    cmp eax, 0xE
    je .error_code
    jmp .no_error_code

    .error_code:
    ; Borro el error code si es que lo hay
    popad
    add esp, 4
    pushad

    .no_error_code:

    ; imprimir_texto_mp se_rompio_todo_msg, se_rompio_todo_len, 0x40, 1, 0
    ; imprimir_texto_mp excepcion_msg, excepcion_len, 0x40, 2, 0
    ; ;Llamo a print en hexa para el codigo de error, que lo tengo en la pila
    ; push 0x40               ;Attributes
    ; push 2                  ;Y
    ; push 0 + excepcion_len  ;X
    ; push 2                  ;Size
    ; push eax                ;text = error code
    ; call print_hex

    ; imprimir_texto_mp posicion_msg, posicion_len, 0x40, 3, 0
    ; ;Llamo a print en hexa para la posicion de la cual vengo, es decir el EIP, que lo tengo en la pila
    ; push 0x40               ;Attributes
    ; push 3                  ;Y
    ; push 0 + posicion_len   ;X
    ; push 8                  ;Size
    ; push ebx                ;text = EIP
    ; call print_hex

    ; push ebx
    ; push edi
    call fin_intr_pic1

    call game_matar_tarea   ;EAX = ID_TAREA (1 para jugador A, 2 para jugador B)
    mov [esp + 32], eax


    cmp BYTE [modo_debug], 0
    je .no_debug

    xor ecx, ecx
    inc ecx
    mov [debug_en_screen], cl

    popad
    ; push eflags
    ; push ss
    push gs
    push fs
    push es
    push ds
    ; push cs
    ; push eip
    ; push esp
    push ebp
    push edi
    push esi
    push edx
    push ecx
    push ebx
    push eax
    call imprimo_interrupcion_pantalla
    add esp, 44

    .no_debug:
    call sched_idle
; popad
iret

%endmacro

;;
;; Datos
;; -------------------------------------------------------------------------- ;;
; Scheduler
isrnumero:           dd 0x00000000
isrClock:            db '|/-\'
offset:              dd 0
selector:            dw 0

;Interrupcions
se_rompio_todo_msg  db      'Se rompio todo.'    ;Completar para que diga el numero de excepcion
se_rompio_todo_len  equ     $ - se_rompio_todo_msg
excepcion_msg       db      'Excepcion: 0x'
excepcion_len       equ     $ - excepcion_msg
posicion_msg        db      'Posicion: 0x'
posicion_len        equ     $ - posicion_msg

modo_debug          db 0
debug_en_screen     db 0

;;
;; Rutina de atención de las EXCEPCIONES
;; -------------------------------------------------------------------------- ;;
ISR 0
ISR 1
ISR 2
ISR 3
ISR 4
ISR 5
ISR 6
ISR 7
ISR 8
ISR 9
ISR 10
ISR 11
ISR 12
ISR 13
ISR 14
ISR 15
ISR 16
ISR 17
ISR 18
ISR 19
ISR 20

;;
;; Rutina de atención del RELOJ
;; -------------------------------------------------------------------------- ;;
global _isr32
_isr32:
pushad
    
    ; En modo debug el scheduler no salta a otra tarea si hay una impresion en pantalla de una excepcion
    cmp BYTE [debug_en_screen], 0
    jne .nojump

    call proximo_reloj

    call sched_proximo_indice
    
    cmp ax, 0
    je  .nojump
        mov [selector], ax
        call fin_intr_pic1
        jmp far [offset]
        jmp .end
    .nojump:
    
    call fin_intr_pic1
    .end:
popad
iret
;;
;; Rutina de atención del TECLADO
;; -------------------------------------------------------------------------- ;;
global _isr33

_isr33:
pushad
    xor eax,eax
    xor ebx, ebx

    in al, 0x60
    mov bl, [debug_en_screen]

    push ebx
    push eax
    call manejo_teclado
    pop eax
    pop ebx

    call fin_intr_pic1
popad
iret
;;
;; Rutinas de atención de las SYSCALLS
;; -------------------------------------------------------------------------- ;;

%define DONDE  0x124
%define SOY    0xA6A
%define MAPEAR 0xFF3

%define VIRUS_ROJO 0x841
%define VIRUS_AZUL 0x325

global _isr102
_isr102:
pushad
    ;EAX = codigo
    ;EBX = primer parametro
    ;ECX = segundo parametro

    ;DONDE
    cmp eax, DONDE
    jne .no_es_donde
        push ebx
        call game_donde
        pop ebx
        jmp .fin
    .no_es_donde:

    ;SOY
    cmp eax, SOY
    jne .no_es_soy
        ;Seteo eax en 0
        xor eax, eax
        
        ;Si ebx es VIRUS_ROJO entonces pongo en eax 1 (Infectado A)
        cmp ebx, VIRUS_ROJO
        jne .no_soy_infectado_A
            mov eax, 1
        .no_soy_infectado_A:

        ;Si ebx es VIRUS_AZUL entonces pongo en eax 2 (Infectado B)
        cmp ebx, VIRUS_AZUL
        jne .no_soy_infectado_B
            mov eax, 2
        .no_soy_infectado_B:
        
        push eax
        call game_soy
        pop eax
        jmp .fin
    .no_es_soy:

    ;MAPEAR
    cmp eax, MAPEAR
    jne .no_es_mapear
        push ecx
        push ebx
        call game_mapear
        pop ebx
        pop ecx
        jmp .fin
    .no_es_mapear:

    .fin:
    call fin_intr_pic1
    call sched_idle
popad
iret


;; Funciones Auxiliares
;; -------------------------------------------------------------------------- ;;
proximo_reloj:
        pushad
        inc DWORD [isrnumero]
        mov ebx, [isrnumero]
        cmp ebx, 0x4
        jl .ok
                mov DWORD [isrnumero], 0x0
                mov ebx, 0
        .ok:
                add ebx, isrClock
                imprimir_texto_mp ebx, 1, 0x0f, 49, 79
                popad
        ret

sched_idle:
pushad
    jmp 0x38:0x04201337
popad
ret

global lanzar_modo_debug
lanzar_modo_debug:

push eax

; Si no estoy en modo debug, entonces activo modo debug
cmp BYTE [modo_debug], 0
je .activar_md
; Si estoy en modo debug, pero no tengo nada en screen, lo desactivo
cmp BYTE [debug_en_screen], 0
je .desactivar_md
; Si llego aca es porque hay algo impreso en pantalla, asi que lo limpio
jmp .limpio_pantalla

.activar_md:
    xor eax, eax
    inc eax
    mov [modo_debug], al
jmp .fin

.desactivar_md:
    xor eax, eax
    mov [modo_debug], al
jmp .fin

.limpio_pantalla:
    call restauro_pantalla
    xor eax, eax
    mov [debug_en_screen], al
.fin
pop eax
ret