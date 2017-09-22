
#include <LPC214X.H>
void transmit(char *);
void recieve(char *);
void main()
{
char a[10];
int x,y;
PINSEL0=0X00000005;
IODIR0=0X000000001;
U0LCR=0X83;
U0DLL=97;
U0LCR=0X03;
while(1)
{
 AD0CR=0X01200008;
 AD1CR=0X01200008;
 while(!(AD0GDR)&(80000000));
 x=((AD0GDR>>6)&(0X03FF));
 while(!(AD1GDR)&(80000000));
 y=((AD1GDR>>6)&(0X03FF));
 sprintf(a,"%d\t%d\n",x,y);  
 transmit(a);
}
}

void transmit(char *p)
{
int i;
for(i=0;p[i]!='\0';i++)
{
U0THR=p[i];
while(!(U0LSR&0X20));
}
}																														  





