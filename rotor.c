// EN12    IOSET1(1<<16)=001
// EN34    IOSET1(1<<17)=002
// LEFT_1  IOSET1(1<<18)=004
// LEFT_2  IOSET1(1<<19)=008
// RIGHT_1 IOSET1(1<<20)=016
// RIGHT_2 IOSET1(1<<21)=032

#include <LPC214X.H>

void forward();
void backward();
void stop();
void left();
void right();

char control_data;

void main()
{
        PINSEL0=0X00000005;
        IODIR0=0X00000001;
		U0LCR=0X83;
        U0DLL=97;
        U0LCR=0X03;
		PINSEL2=0X00000000;
		IODIR1=0X0000003F;
	
	

	while(1)
	{
		while(!(U0LSR&1<<0));
		control_data=U0RBR;
		U0THR=control_data;
		while(!(U0LSR&1<<5));
	 
        if(control_data=='w')
	      forward();
	else if(control_data=='s')
		 stop();
	else if(control_data=='x')
	       backward();
	else if(control_data=='a')
	       left();
	else if(control_data=='d')
	       right();	
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
