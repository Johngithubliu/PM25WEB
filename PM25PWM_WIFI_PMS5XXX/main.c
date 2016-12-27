
#include <STC15F2K60S2.H>
#include "mydefine.h"
#include <intrins.h>
#include "string.h"

const unsigned char *ATCWMODE3="AT+CWMODE=3\r\n";		
const unsigned char *ATCIPMUX0="AT+CIPMUX=0\r\n";		
const unsigned char *ATCIPMODE1="AT+CIPMODE=0\r\n";		
const unsigned char *ATCIPSTART="AT+CIPSTART=\"TCP\",\"23.106.128.180\",7270\r\n";	
const unsigned char *ATCWSTARTSMART="AT+CWSTARTSMART\r\n";
const unsigned char *ATCWSTOPSMART="AT+CWSTOPSMART\r\n";
const unsigned char *ATCIPSEND32="AT+CIPSEND=32\r\n";	
const unsigned char *ATCIPSEND14="AT+CIPSEND=14\r\n";	
const unsigned char *ATCIPSEND7="AT+CIPSEND=7\r\n";	
const unsigned char *ATCIPSEND5="AT+CIPSEND=5\r\n";	
const unsigned char *ATCIPCLOSE="AT+CIPCLOSE\r\n";
//const unsigned char *ATCIPLOGIN="1\r\ndy1\r\n123321";	
//const unsigned char *SENDTODY2="3\r\ndy2\r\n";	


idata unsigned char RxBuf[MAX_BUF];
idata unsigned char TempString[MAX_BUF];

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
void Uart9600P36();
bit init_bt();
void init_tcp();

void power_on();
void power_off();
void delay_01ms(unsigned char);
void delay_ms(unsigned char);
void delay_s(unsigned char);
void send(unsigned int );

unsigned char  RxStatus;
bit TxStatus;
bit	mBus_receive;
bit IR;
bit f_1000s;
bit power;

unsigned char time_led2;
unsigned char time_led1;
unsigned char time_delay_s;
unsigned char time_delay_10ms;
unsigned char time_delay_ms;
unsigned char time_delay_01ms;
unsigned char byte_time_out;
unsigned char receive_step;
unsigned int speed;

void main()
{
//	unsigned char i;
	init_sys(); 
  init_port();
	
	init_timer0();
	time_delay_ms=254;
	time_led1=253;
	time_led2=250;
	while(time_delay_ms){};
	
	power_off();
		power=1;
		speed=50;
		//-----power off--.....
	delay_power_off=TIME_OUT;
	time_led1=250;
	Uart115200P30();
	init_tcp();	
	
	Uart9600P36();
	mBus_receive=1;
	while(1)
	{
	
		Uart9600P36();
		mBus_receive=1;
		
		while(!RxStatus){};
		mBus_receive=0;
		EA=0;
		speed=((int)RxBuf[6]<<8)|RxBuf[7];
		EA=1;
		time_led2=200;
		Uart115200P30();
	
		memcpy(TempString,RxBuf,32);
			
			
		strcpy(RxBuf,ATCIPSTART);
		send(strlen(RxBuf));
		
		delay_ms(200);
		delay_ms(200);
		delay_ms(200);
		delay_ms(200);
		delay_ms(200);
		delay_ms(200);
		
		
		strcpy(RxBuf,"AT+CIPSEND=41\r\n");
		send(strlen(RxBuf));
		
		strcpy(RxBuf,"kiji6lu\r\n");
		send(strlen(RxBuf));
		
//		strcpy(RxBuf,ATCIPSEND32);
//		send(strlen(RxBuf));
		memcpy(RxBuf,TempString,32);
		send(32);	
		
		delay_ms(200);
		delay_ms(200);
		delay_ms(200);
		
		delay_ms(200);
		delay_ms(200);
		delay_ms(200);
		
					
		strcpy(RxBuf,ATCIPCLOSE);
		send(strlen(RxBuf));
		
		RxStatus=0;
		delay_s(3);
			
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
	P3M0=0x80;

	P1=0xff;
	
	P3=0xff;  
	P37=0;
	

}
void init_tcp()
{
	unsigned char i;
//	unsigned char TempBuf[15];
	//if(!BT_CONECT){mBus_receive=1;return 1;}//is ok.

	Uart115200P30();
	mBus_receive=0;
	delay_s(10);
	time_led2=200;

	strcpy(RxBuf,ATCWSTARTSMART);
	send(strlen(RxBuf));
	
	for(i=0;i<120;i++)
	{	
		time_led1=200;
		delay_s(1);
	}
	strcpy(RxBuf,ATCWSTOPSMART);
	send(strlen(RxBuf));
	strcpy(RxBuf,"AT+RST\r\n");
	send(strlen(RxBuf));
	delay_s(3);
		
	strcpy(RxBuf,ATCIPMUX0);
	send(strlen(RxBuf));
		
	strcpy(RxBuf,ATCIPMODE1);
	send(strlen(RxBuf));

	
	RxCounter=0;
		
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
	
	P55=1;

}
void Uart9600P36(void)
{
	SCON = 0x50;		//8???,?????
	AUXR |= 0x01;		//??1?????2???????
	AUXR |= 0x04;		//???2???Fosc,?1T
	SM2=0;
	T2L = 0xC0;		//??????
	T2H = 0xFD;		//??????
	AUXR |= 0x10;		//?????2
	
	S_P36;
	
	REN=1;
	ES=1;
	

	P55=1;

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
	P55=0;
	
}



void timer0 (void) interrupt 1  
{  
	static unsigned char 	count_01ms;
	static unsigned char 	count_ms;
	static unsigned char 	count_10ms;
	static unsigned int 	count_s;
	static unsigned int 	count_pwm;
	//01ms interrupt
	if(time_delay_01ms)time_delay_01ms--;
	
	if(++count_pwm>=MAX_SPEED)
	{
		count_pwm=0;
		if(power)P37=1;
		else P37=0;
	}
	if(count_pwm>=speed)
	{
		if(P32)P37=0;
	}
	
	
	if(++count_01ms>=10)
	{
		
		count_01ms=0;
		//1ms
		if(time_delay_ms)time_delay_ms--;
		
		if(time_led1){time_led1--;LED1_ON;}
		else LED1_OFF;
		if(time_led2){time_led2--;LED2_ON;}
		else LED2_OFF;
		
		if(byte_time_out)byte_time_out--;
		else {RxCounter=0;receive_step=0;}
		
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
				if(++count_s==1000)f_1000s=1;
		//		time_led1=10;
				//end 1s
				
			}
		}
	}
	
}


void power_off()
{
	Uart9600P36();
	
	
//	time_led1=199;
//	delay_ms(200);
	
	PCON|=0x02;

	_nop_();
	_nop_();	
}

void receive(void) interrupt 4
{
	static unsigned char UartVerify;
	unsigned char dat;
	
	if(RI)
	{
		RI=0;
		byte_time_out=BYTE_TIME_OUT;
		delay_power_off=TIME_OUT;
		dat=SBUF;
		if(RxCounter>=MAX_BUF)RxCounter=0;
	
		if(mBus_receive)
		{
			switch(receive_step)
			{
				case 0:
					if(dat==0x42)
					{
						RxBuf[RxCounter++]=dat;
						UartVerify=dat;
						receive_step=1;
					//	time_led1=10;
					}
					else RxCounter=0;
					
					break;
				
				
				case 1:
					if(dat==0x4d)
					{
						RxBuf[RxCounter++]=dat;
						UartVerify+=dat;
						receive_step=2;
					//	time_led1=20;
					}
					else 
					{
						RxCounter=0;
						receive_step=0;
					}
					
					break;
							
				case 2:
					if(dat==0x00)
					{
						RxBuf[RxCounter++]=dat;
						UartVerify+=dat;
						receive_step=3;
					//	time_led1=20;
					}
					else 
					{
						RxCounter=0;
						receive_step=0;
					}
							
					break;
				
				
				case 3:
					if(dat==0x1c)
					{
						RxBuf[RxCounter++]=dat;
						UartVerify+=dat;
						receive_step=4;
					//	time_led1=20;
					}
					else 
					{
						RxCounter=0;
						receive_step=0;
					}
		
				break;
				
				
				case 4:
					RxBuf[RxCounter++]=dat;
					if(RxCounter==32)
					{
						RxStatus=RxCounter;
			
						time_led1=50;
					}
						
					
					break;
					
				default:
					RxCounter=0;
					receive_step=0;
						
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
		
			
		}
			
			
	}
	
}
void send(unsigned int len)
{
	TxCounter=0;
	TxLength=len;
	TxStatus=1;
	TI=1;
	while(TxStatus){};
	delay_ms(200);
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
void delay_s(unsigned char dt)
{
	time_delay_s=dt;
	while(time_delay_s);
}
