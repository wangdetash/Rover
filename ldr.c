//program to use two adc and transmit through uart and incorporated temperature sensing using lm35 and ldr
#include <LPC214X.H>
void transmit(char *);
void forward();
void backward();
void stop();
void left();
void right();

char control_data;

void main()
{
char a[20];
int ldr_out,lm35_out,temp;
PINSEL0=0X00000005;	 	  
IODIR0=0X000000001;
PINSEL2=0X00000000;
IODIR1=0X0000003F;
U0LCR=0X83;
U0DLL=97;
U0LCR=0X03;
while(1)
{
	while(!(U0LSR&1<<0));
		control_data=U0RBR;
		U0THR=control_data;
		while(!(U0LSR&1<<5));
	 
        if(control_data=='w')
	      {
	      	forward();
	      	transmit("FORWARD\n");
	      }
		else if(control_data=='s')
		  {
		 	stop();
		 	transmit("STOP\n");
		  }
		else if(control_data=='x')
	      {
	       	backward();
	       	transmit("BACKWARD\n");
	      }
		else if(control_data=='a')
	      {
	       	left();
	       	transmit("LEFT\n");
	      }
		else if(control_data=='d')
	      {
	       	right();
	       	transmit("RIGHT\n");
	      }	
 		AD0CR=0X01200008;
 		AD1CR=0X01200008;
 		while(!(AD0GDR)&(80000000));
 		ldr_out=((AD0GDR>>6)&(0X03FF));
 		while(!(AD1GDR)&(80000000));
 		lm35_out=((AD1GDR>>6)&(0X03FF));
 		temp=((lm35_out*100)/1024);
 		sprintf(a,"temperature=%d\tlm35_out=%d\tldr_out=%d\n",temp,lm35_out,ldr_out);
 		transmit(a); 
 		if(ldr_out<512)
 		{
   			IOSET0=0X80000000;
   			transmit("lights on\n");
		}
 		else 
 		{
 			IOCLR0=0X80000000;
 			transmit("lights off\n");
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



void forward()
{
	IOSET1=0X00010000;
	IOSET1=0X00020000;
	IOSET1=0X00040000;
    IOSET1=0X00100000;
}

void backward()
{
	IOSET1=0X00010000;
	IOSET1=0X00020000;
	IOSET1=0X00080000;
    IOSET1=0X00200000;
}

void stop()
{	
	IOCLR1=0X00FF0000;
}

void left()
{
	IOSET1=0X00010000;
	IOSET1=0X00020000;
	IOSET1=0X00080000;
	IOSET1=0X00100000;
}

void right()
{
	IOSET1=0X00010000;
	IOSET1=0X00020000;
	IOSET1=0X00200000;
	IOSET1=0X00040000;
}


