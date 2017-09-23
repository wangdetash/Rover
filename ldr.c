//program to use two adc and transmit through uart
#include <LPC214X.H>
void transmit(char *);
void recieve(char *);
void main()
{
char a[10];
int ldr_out,lm35_out,temp;
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
 ldr_out=((AD0GDR>>6)&(0X03FF));
 while(!(AD1GDR)&(80000000));
 lm35_out=((AD1GDR>>6)&(0X03FF));
 temp=((lm35_out*100)/1024);
 sprintf(a,"temperature = %dDegree C \t %d\t",temp,lm35_out);
 transmit(a); 
 if(ldr_out<512)
 {
   IOSET0=0X80000000;
   transmit("lights on\t");
 }
 else 
 {
 IOCLR0=0X80000000;
 transmit("lights off\t");
 } 
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





