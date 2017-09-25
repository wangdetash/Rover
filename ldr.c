//program to use two adc and transmit through uart and incorporated temperature sensing using lm35,ldr,inbuilt rtc and gps
#include <LPC214X.H>
#include <stdio.h>

void transmit(char *);
void recieve(char *);
void forward();
void backward();
void stop();
void left();
void right();

char control_data;

void main()
{
int i,j;
char a[60],b[30],c[30],d[]=" $GPGGA,154653,4428.2011,N,00440.5161,W,0,00,,-00044.7,M,051.6,M,,*6C";
int ldr_out,lm35_out,temp,date,month,year,hour,min,sec;
PINSEL0=0X00050005;	 	  
IODIR0=0X000000101;
PINSEL2=0X00000000;
IODIR1=0X0000003F;
U0LCR=0X83;
U0DLL=97;
U0LCR=0X03;
U1LCR=0X83;
U1DLL=97;
U1LCR=0X03;

CCR =0X11;
DOM=23;
MONTH=10;
YEAR=2107;
HOUR=22;
MIN=10;
SEC=40;

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
	else if(control_data=='q')
	    {  
 			AD0CR=0X01200008;
 			AD1CR=0X01200008;
 			while(!(AD0GDR)&(80000000));
 			ldr_out=((AD0GDR>>6)&(0X03FF));
 			while(!(AD1GDR)&(80000000));
 			lm35_out=((AD1GDR>>6)&(0X03FF));
 			temp=((lm35_out*100)/1024);
 			hour = HOUR;
        	min  = MIN; 
        	sec  = SEC; 
        	date  = DOM;   
        	month = MONTH;  
        	year = YEAR;
 			sprintf(a,"temperature=%d\tlm35_out=%d\tldr_out=%d\ttime:%d:%d:%d\t%d-%d-%d\t",temp,lm35_out,ldr_out,hour,min,sec,date,month,year);
 			transmit(a);
 			//recieve(d);
  			for(i=8,j=0;i<=25;i++,j++)
			{
				b[j]=d[i];
			}
			b[j]='\0';
			for(i=27,j=0;i<=38;i++,j++)
			{
				c[j]=d[i];
			}
			c[j]='\0';
			sprintf(a,"longitude:%s\tlatitude:%s\n",b,c);
  			transmit(a);
 		} 
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

void recieve(char *q)
{
	int i;
	for(i=0;;i++)
	{
		while(!(U1LSR&(0X01)));
		q[i]=U1RBR;
		if(q[i]=='\r')
		{
			q[i]='\0';
			break;
		}
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


