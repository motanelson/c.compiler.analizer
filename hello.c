void print()
{
   int count = 0;
   int values = 0;
   getPointer (count,toPrint_main);
   asm loop_print:;
   peek (values,count);
   putc (values);
   inc (count);   
   peek (values,count);
   if (values != 0)
       asm jnz loop_print 
   return 0;
}
void main()
{
    char toPrint="hello world....\n";
    call print;
    return 0;
}
