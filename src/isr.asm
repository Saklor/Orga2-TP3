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

;;Manejo del teclado
extern manejo_teclado

;;Extra para interrupciones
extern print_hex

;;
;; Definición de MACROS
;; -------------------------------------------------------------------------- ;;

%macro ISR 1
global _isr%1

_isr%1:
    mov eax, %1

    pop edi
    pop ebx ;ebx = EIP

    imprimir_texto_mp se_rompio_todo_msg, se_rompio_todo_len, 0x40, 1, 0
    imprimir_texto_mp excepcion_msg, excepcion_len, 0x40, 2, 0
    ;Llamo a print en hexa para el codigo de error, que lo tengo en la pila
    push 0x40               ;Attributes
    push 2                  ;Y
    push 0 + excepcion_len  ;X
    push 2                  ;Size
    push eax                ;text = error code
    call print_hex

    imprimir_texto_mp posicion_msg, posicion_len, 0x40, 3, 0
    ;Llamo a print en hexa para la posicion de la cual vengo, es decir el EIP, que lo tengo en la pila
    push 0x40               ;Attributes
    push 3                  ;Y
    push 0 + posicion_len   ;X
    push 8                  ;Size
    push ebx                ;text = EIP
    call print_hex

    push ebx
    push edi

    jmp $
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

;;
;; Rutina de atención de las EXCEPCIONES
;; -------------------------------------------------------------------------- ;;
ISR 0   ;Divison Error
ISR 1   ;Divison Error
;ISR 2   ;Divison Error
;ISR 3   ;Divison Error
;ISR 4   ;Divison Error
;ISR 5   ;Divison Error
;ISR 6   ;Divison Error
;ISR 7   ;Divison Error
;ISR 8   ;Divison Error
;ISR 9   ;Divison Error
ISR 10
;ISR 11   ;Divison Error
ISR 12   ;Divison Error
ISR 13   ;Divison Error
ISR 14

;;
;; Rutina de atención del RELOJ
;; -------------------------------------------------------------------------- ;;
global _isr32
_isr32:
pushad
    
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
extern manejo_keyboard

_isr33:
pushad
    xor edi,edi
    xor eax,eax
    in al, 0x60
    
    push eax
    call manejo_teclado
    pop eax

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

global _isr66
_isr66:
pushad
    mov eax, 0x42
    call fin_intr_pic1
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
        
        
