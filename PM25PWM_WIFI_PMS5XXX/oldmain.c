
#include <STC15F2K60S2.H>
#include "mydefine.h"
#include <intrins.h>
#include "string.h"

		
unsigned char RxBuf[64];

//unsigned char TxBuf[];


unsigned char delay_power_off;
unsigned char TxCounter;
unsigned char TxLength;
unsigned char RxCounter;


void init_sys(); 
void init_port();

void init_timer0();

void Uart115200P30();
void Uart2400P36();
bit init_bt();

void power_on();
void power_off();
void delay_01ms(unsigned char);
void delay_ms(unsigned char);

unsigned char  RxStatus;
bit TxStatus;
bit	mBus_receive;
bit IR;

unsigned char time_led2;
unsigned char time_led1;
unsigned char time_delay_s;
unsigned char time_delay_10ms;
unsigned char time_delay_ms;
unsigned char time_delay_01ms;
unsigned char byte_time_out;
unsigned char receive_step;
void main()
{
	init_sys(); 
  init_port();
	
	init_timer0();
	time_delay_ms=254;
	time_led1=253;
	time_led2=250;
	while(time_delay_ms){};
	
	power_off();
		//-----power off--.....
	delay_power_off=TIME_OUT;
		
	while(1)
	{
		if(init_bt())
		{
		//	time_led1=200;
			if(RxStatus)
			{//receive from bt
				time_led1=500;
							
				//send to IR
				Uart2400P36();
				ES=0;
				//send 0xfe to IR
				ACC=0xfe;
				TB8=P;
				SBUF=0xfe;
				while(!TI){};
				TI=0;
				
				ACC=0xfe;
				TB8=P;	
				SBUF=0xfe;
				while(!TI){};
				TI=0;
					
					
				ACC=0xfe;
				TB8=P;
				SBUF=0xfe;
				while(!TI){};
				TI=0;
				SBUF=0xfe;
				while(!TI){};
				TI=0;

				ES=1;
					
				TxCounter=RxStatus;
				RxStatus=0;
				TI=1;
				while(TxCounter){};
        
				time_delay_10ms=80;
				IR_REN=1;
					P12=0;
				while(time_delay_10ms)
				{
					if(RxStatus)
					{//receive from IR
						
						//time_led1=100;
						
						//send to bt
						Uart115200P30();
						TxCounter=0;
						TxLength=RxStatus;
						RxStatus=0;
						TxStatus=1;
						BRTS=0;
						delay_01ms(2);
						TI=1;
						while(TxStatus){};
							
					}
				}
				P12=1;
				IR_REN=0;
				Uart115200P30();
				
	
			}
		}
		
		
		if(!delay_power_off)
		{
			power_off();
		// power down ......
		
		//power on!!
			delay_power_off=TIME_OUT;
		}

	}
}
void init_sys()
{
	CLK_DIV=0;
	RxStatus=0;
	TxStatus=0;
	delay_power_off=0;
	time_delay_s=0;
	time_delay_10ms=0;
	time_delay_ms=0;
	time_led1=0;
	time_led2=0;
	receive_step=0;
	
}
void init_port()
{
	
	P1M1=0x04;
	P1M0=0x00;

	P3M1=0x00;
	P3M0=0x00;

	P1=0xff;
	
	P3=0xff;  

}
bit init_bt()
{
	unsigned char TempBuf[15];
		
	Uart115200P30();
	
	if(!BT_CONECT)return 1;//is ok.
	BT_EN=0;
	
	delay_01ms(10);

	mBus_receive=0;
	//get MAC
	strcpy(RxBuf,"TTM:MAC-?");
	TxCounter=0;
	TxLength=9;
	TxStatus=1;
	BRTS=0;
	delay_01ms(2);
	TI=1;
	while(TxStatus){};
	delay_ms(20);
	
	//set name to DYBT+MAC		
	if(!memcmp(RxBuf,"TTM:MAC",7))
	{
		if(RxBuf[7]=='-')
		{
			time_led2=100;
			while(time_led2){};
			time_delay_10ms=20;
			while(time_delay_10ms){};
			return 0;
		}
		memcpy(TempBuf,RxBuf+7,12);
		strcpy(RxBuf,"TTM:REN-DYBT");
		memcpy(RxBuf+12,TempBuf,12);
		TxCounter=0;
		TxLength=24;
		TxStatus=1;
		BRTS=0;
		delay_01ms(10);
		TI=1;
		while(TxStatus){};
		delay_ms(40);
	}
				
	//set broadcast peri to 200mS
	strcpy(RxBuf,"TTM:ADP-X(2)");
	TxCounter=0;
	TxLength=12;
	TxStatus=1;
	BRTS=0;
	delay_01ms(2);
	TI=1;
	while(TxStatus){};
	delay_ms(40);
		
		//set PID to DY
	strcpy(RxBuf,"TTM:PID-DY");
	TxCounter=0;
	TxLength=10;
	TxStatus=1;
	BRTS=0;
	delay_01ms(2);
	TI=1;
	while(TxCounter){};
	delay_ms(50);
		//power to +4DBm
	strcpy(RxBuf,"TTM:TPL-(+4)");
	TxCounter=0;
	TxLength=13;
	TxStatus=1;
	BRTS=0;
	delay_01ms(2);
	TI=1;
	while(TxStatus){};
	delay_ms(40);
		
	while(BT_CONECT)
	{
		time_led2=10;
		while(time_led2){};
		time_delay_10ms=100;
		while(time_delay_10ms){};
			
		if(!delay_power_off)return 0;
			
	}
	
	delay_ms(30);
	/*
	strcpy(RxBuf,"TTM:CIT-20mS");
	TxCounter=0;
	TxLength=12;
	TxStatus=1;
	BRTS=0;
	delay_01ms(2);
	TI=1;
	while(TxStatus){};*/


	mBus_receive=1;
	RxCounter=0;
	
	return 1;
}



void init_timer0()
{

	TMOD&=0xf0;
	AUXR&=0x7f;

	TL0=T01MS;
	TH0=T01MS>>8;


	TR0=1;
	ET0=1;
	EA=1;


}


void Uart115200P30(void)
{
	SCON = 0x50;		//8???,?????
	AUXR |= 0x01;		//??1?????2???????
	AUXR |= 0x04;		//???2???Fosc,?1T
	SM2=0;
	T2L = 0xD0;		//??????
	T2H = 0xFF;		//??????
	AUXR |= 0x10;		//?????2
	
	S_P30;
	
	REN=1;
	ES=1;
	IR=0;
	mBus_receive=1;

}
void Uart2400P36(void)
{
	SCON = 0xD0;		//9???,?????
	AUXR |= 0x01;		//??1?????2???????
	AUXR |= 0x04;		//???2???Fosc,?1T
	SM2=0;
	
	T2L = 0x00;		//??????
	T2H = 0xF7;		//??????
	S_P36;

	AUXR |= 0x10;		//?????2
	REN=1;
	ES=1;
	IR=1;
	mBus_receive=1;
	
}



void timer0 (void) interrupt 1  
{  
	static unsigned char 	count_01ms;
	static unsigned char 	count_ms;
	static unsigned char 	count_10ms;
	//01ms interrupt
	if(time_delay_01ms)time_delay_01ms--;
	
	if(++count_01ms>=10)
	{
		
		count_01ms=0;
		//1ms
		if(time_delay_ms)time_delay_ms--;
		
		if(time_led1){time_led1--;LED1_ON;}
		else LED1_OFF;
		if(time_led2){time_led2--;LED2_ON;}
		else LED2_OFF;
		
	//	if(byte_time_out)byte_time_out--;
	//	else RxCounter=0;
		
		//end 1ms
		if(++count_ms>=10)
		{
			count_ms=0;
			//10ms
						
			if(time_delay_10ms)time_delay_10ms--;
			//end 10 ms
		
			if(++count_10ms>=100)
			{
				count_10ms=0;
				//1s
				if(delay_power_off)delay_power_off--;
				if(time_delay_s)time_delay_s--;

				//end 1s
				
			}
		}
	}
	
}


void power_off()
{

	Uart2400P36();
	BT_EN=1;
	BCTS=1;
	BRTS=1;
	
//	time_led1=199;
//	delay_ms(200);
	
	PCON|=0x02;

	_nop_();
	_nop_();	
}

void receive(void) interrupt 4
{

	static unsigned char UartVerify;
	unsigned char Uarttemp;
	unsigned char dat;
	
	//static unsigned char send=0;

	if(RI)
	{
		RI=0;
		byte_time_out=BYTE_TIME_OUT;
		delay_power_off=TIME_OUT;
		dat=SBUF;
		if(RxCounter>=MAX_BUF)RxCounter=0;
		
		if(IR)
		{
			ACC=dat;
			if(RB8!=P) {receive_step=0;RxCounter=0;return;}
		}		
		if(mBus_receive)
		{
			switch receive_step
			{
				case 0:
					if(dat==0x68)
					{
						RxBuf[RxCounter++]=dat;
						UartVerify=dat;
						receive_step=1;
					}
					
					break;
				
				
				case 1:
					if(dat==0x10)
					{
						RxBuf[RxCounter++]=dat;
						UartVerify+=dat;
						receive_step=2;
					}
					else 
					{
						RxCounter=0;
						receive_step=0;
					}
					
					break;
				
				
				case 2:
						RxBuf[RxCounter++]=dat;
						UartVerify+=dat;
						if(RxCounter==)
							receive_step=3;
					break;
				
				
				case 3:
					
					break;
				
				
				case 0:
					
					break;
			}
		}
		else
		{
			RxBuf[RxCounter++]=dat;
	
		}
	}
	else if(TI)
	{
		TI=0;
		if(TxCounter<TxLength)
		{
			
			dat=RxBuf[TxCounter];
			ACC=dat;
			TB8=P;
			SBUF=dat;
			TxCounter++;
		}
		else
		{
			TxStatus=0;
			BRTS=1;
			
		}
			
			
	}
}

void delay_01ms(unsigned char dt)
{
	time_delay_01ms=dt;
	while(time_delay_01ms);
}
void delay_ms(unsigned char dt)
{
	time_delay_ms=dt;
	while(time_delay_ms);
}
