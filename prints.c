void start(){
    asm [BITS 16];
    asm org 0x100;
    asm jmp main;
    return 0
}
void printc(){
   asm mov dl,al;
   asm mov ah,2;
   asm int 0x21;
   return 0;
}
void print()
{
   int count = 0;
   asm cs
   asm mov bx,toPrint_main;
   asm mov ax,bx
   asm cs
   asm mov [count_print],ax
   asm loop_print:;
   asm cs
   asm mov bx,[count_print]
   asm cs;
   asm mov al,[bx];
   call printc;
   asm cs
   asm mov bx,[count_print]
   asm cs;
   asm mov al,[bx];
   asm inc bx;
   asm mov ax,bx
   asm cs
   asm mov [count_print],bx
   
   asm cmp al,0;
   asm jnz loop_print 
   return 0;
}
void main()
{
    char toPrint="hello world....\n";
    call print;
    return 0;
}
