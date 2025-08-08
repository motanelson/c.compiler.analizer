void start(){
    asm [BITS 16];
    asm org 0x100;
    asm jmp main;
    return 0
}
void adds(){
    asm add ax,bx;
    asm ret;
    return 0;
}
void peek(){
     asm mov ax,0;
     asm cs;
     asm mov al,[bx];
     asm ret;
     return 0;
}
void incs(){
    asm inc ax;
    asm ret;
    return 0;
}
void getadds(){
     asm cs;
     asm mov ax,[count_print];
     asm mov bx,ax;
     call peek;
     asm ret;
     return 0;
}
void setadds(){
     asm cs;
     asm mov ax,[count_print];
     call incs;
     asm cs;
     asm mov [count_print],ax;
     call getadds;
     asm ret;
     return 0;
}
void getp(){
    asm cs;
    asm mov ax,toPrint_main;
    asm cs;
    asm mov [count_print],ax;
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
   call getp;
   asm loop_print:;
   call getadds;
   call printc;
   call setadds;      
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
