; Código Assembly 16-bit (NASM)

section .text
    [BITS 16]
    org 0x100
    call main
   ret

getp:
    ; ignorado: 
    cs
    mov ax,toPrint_main
    cs
    mov [count_print],ax
    mov ax, 0
    ret

print:
    ; ignorado: 
    call getp
    loop_print:
    cs
    mov bx,[count_print]
    cs
    mov al,[bx]
    mov ah,0
    cs
    mov [values_print],ax
    cs
    mov al,[values_print]
    mov ah,0
    mov dl,al
    mov ah,2
    int 0x21
    mov ax,[count_print]
    inc ax
    mov [count_print],ax
    cs
    mov bx,[count_print]
    cs
    mov al,[bx]
    mov ah,0
    cs
    mov [values_print],ax
    mov ax, [values_print]
    cmp ax, 0
    je if_end_0
    ; ... (bloco do if)
    jnz loop_print
    if_end_0:
    mov ax, 0
    ret

main:
    ; ignorado: 
    call print
    mov ax, 0
    ret

section .data

count_print: dw 0
values_print: dw 0
toPrint_main: db 'hello world....\n', 0
