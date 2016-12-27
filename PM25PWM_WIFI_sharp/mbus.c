
#include <STC15F2K60S2.H>


#ifndef UART_C
#define UART_C
#define UART_MEM

unsigned char wRxCounter;
/***********************************************************************************************************************
* Function Name: uart_send
* Description  : UART
* Arguments    : None                    
* Return Value : None
***********************************************************************************************************************/
#define Manu_IDH 0x88
#define Manu_IDL 0x66
#define MAX_BUF	90
extern unsigned char RxBuf[MAX_BUF];

//extern unsigned char TxBuf[64];
extern bit RxStatus;
extern bit TxStatus;
void receive(void) interrupt 3
{
	static unsigned char RxCounter=0;
	static unsigned char UartVerify;
	unsigned char Uarttemp;
	if((RxCounter==0)&&(SBUF!=0x68))
	{
		UartVerify=0;
		return;
	}
	RxBuf[RxCounter]=SBUF;
	UartVerify+=SBUF;
	RxCounter++;
	if((RxCounter>=MAX_BUF)||(RxBuf[10]<3)||((RxBuf[10]>0x4b)))
	{
		RxCounter=0;
		UartVerify=0;
		return;
	}
	
	if((RxCounter==RxBuf[10]+13)&&(16==SBUF))
	{
    Uarttemp=UartVerify-RxBuf[RxCounter-2]-RxBuf[wRxCounter-1];
		if(RxBuf[RxCounter-2]==Uarttemp)
		{
			RxStatus=1;
	    UartVerify=0;
		}

	}

}

#endif 
