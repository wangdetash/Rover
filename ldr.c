//program to use two adc and transmit through uart and incorporated temperature sensing using lm35,ldr,inbuilt rtc and gps.


#include <LPC214X.H>
#include <stdio.h>

char control_data;

void transmit(char *);
void recievegps(char *);
void drive(char);
void forward();
void backward();
void stop();
void left();
void right();



void recieve()__irq
{	
control_data=U0RBR;
drive(control_data);
VICVectAddr=0;
}


void main()
{
int i,j;
char a[60],b[30],c[30],d[]=" $GPGGA,154653,4428.2011,N,00440.5161,W,0,00,,-00044.7,M,051.6,M,,*6C";
int ldr_out,light,lm35_out,temp,date,month,year,hour,min,sec;

PINSEL0=0X00050005;	 	  
IODIR0=0X000000101;
PINSEL2=0X00000000;
IODIR1=0X0000003F;

U0LCR=0X83;
U0DLL=97;
U0LCR=0X03;
U0IER=0X01;

U1LCR=0X83;
U1DLL=97;
U1LCR=0X03;

VICVectCntl0=(1<<5)|6;
VICIntEnable=1<<6;
VICVectAddr0=(unsigned)recieve;

CCR =0X11;
DOM=23;
MONTH=10;
YEAR=2107;
HOUR=22;
MIN=10;
SEC=40;

	while(1)
	{
		AD0CR=0X01200008;
 		while(!(AD0GDR)&(80000000));
 		ldr_out=((AD0GDR>>6)&(0X03FF));
 		light=((ldr_out*100)/1024);
 		if(ldr_out<512)
 		{
   			IOSET0=0X80000000;
		}
 		else 
 		{
 			IOCLR0=0X80000000;
 		}

 		AD1CR=0X01200008; 
		while(!(AD1GDR)&(80000000));
 		lm35_out=((AD1GDR>>6)&(0X03FF));
 		temp=((lm35_out*100)/1024);


	      	 
 			hour=HOUR;
        	min=MIN; 
        	sec=SEC; 
        	date=DOM;   
        	month=MONTH;  
        	year=YEAR;
 			sprintf(a,"lm35_out=%d  temperature=%d  ldr_out=%d light_intensity=%d  time:%d:%d:%d date:%d-%d-%d  ",lm35_out,temp,ldr_out,light,hour,min,sec,date,month,year);
 			transmit(a);
 			//recievegps(d);
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
			sprintf(a,"longitude:%s  latitude:%s\n",b,c);
  			transmit(a);
  			control_data='q';
  			
 		
 		
 		
	}
}

void drive(char control_data1)
{
	
	switch(control_data1)
    	
	      {
	      	case 'w':
	      	forward();
	      	transmit("moving forward\n");
	      	break;

			case 's':
		  	stop();
		 	transmit("stop\n");
		 	break;
		  
		 	case 'x':
	       	backward();
	       	transmit("moving backward\n");
	      	break;

			case 'a':
	       	left();
	       	transmit("turning left\n");
	      	break;
			
			case 'd':
	       	right();
	       	transmit("turning right\n");
	      	break;
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

void recievegps(char *q)
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