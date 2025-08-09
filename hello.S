; Código Assembly 16-bit (NASM)

section .text
    [BITS 16]
    org 0x100
    call main
   ret
puts:
    mov bx,ax
puts1_puts:
     cs
    mov dl,[bx]
    mov ah,2
    int 0x21
    inc bx
    cs
     mov al,[bx]
     cmp al,0
     jnz puts1_puts
     ret
adds:
    ; ignorado: 
    clc
    add ax,bx
    ret
    mov ax, 0
    ret
    ; ignorado: 

print:
    ; ignorado: 
    loop_print:
    cs
    mov al,[values_print]
    mov ah,0
    mov dl,al
    mov ah,2
    int 0x21
    cs
    mov ax,[values_print]
    cs
    mov bx,[count_print]
    call adds
    cs
    mov [values_print],ax
    mov ax, [values_print]
    cmp ax, 40
    je if_end_0
    ; ... (bloco do if)
    jmp loop_print
    if_end_0:
    mov ax, 0
    ret

main:
    ; ignorado: 
    call print
    mov ax, 0
    ret

section .data

count_print: dw 1
values_print: dw 32
toPrint_main: db 'hello world....\n', 0
