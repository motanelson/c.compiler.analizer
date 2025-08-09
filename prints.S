; Código Assembly 16-bit (NASM)

section .text
    [BITS 16]
    org 0x100
    call main
   ret

printc:
    ; ignorado: 
    mov dl,al
    mov ah,2
    int 0x21
    mov ax, 0
    ret

print:
    ; ignorado: 
    cs
    mov bx,toPrint_main
    mov ax,bx
    cs
    mov [count_print],ax
    loop_print:
    cs
    mov bx,[count_print]
    cs
    mov al,[bx]
    call printc
    cs
    mov bx,[count_print]
    cs
    mov al,[bx]
    inc bx
    mov ax,bx
    cs
    mov [count_print],bx
    ; ignorado: 
    cmp al,0
    jnz loop_print
    mov ax, 0
    ret

main:
    ; ignorado: 
    call print
    mov ax, 0
    ret

section .data

count_print: dw 0
toPrint_main: db 'hello world....\n', 0
