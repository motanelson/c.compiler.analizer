void hello()
{
   int a = 5;
   float b = 0.9;
   long c = 1;
   char ccc="hello world....\n";
   char *cccc="hello world....\n";
   if (a==b)
      a = b;
   return a;
}
void nops(){
    int a=0;
    asm nop;
    return a;
}
void main()
{
    int a = 5;
    int b = 0;
    int c = 1;
    
    if (a==5)
        call hello;
    while (a<b)
        call hello;
    for (a=b;a<c;a++)
        call hello
    return 0;
}
