void adds(){
    asm clc;
    asm add ax,bx;
    asm ret; 
    return 0;

}
void print()
{
   int count = 1;
   int values = 32;
   asm loop_print:;
   putc (values);
   setPar1 (values);
   setPar2 (count);
   call adds;
   getPar1 (values);
   if (values != 40 )
       asm jmp loop_print 
   return 0;
}
void main()
{
    char toPrint="hello world....\n";
    call print;
    return 0;
}
