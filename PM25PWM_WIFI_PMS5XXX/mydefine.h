#ifndef __MYDEFINE_H__
#define __MYDEFINE_H__



//define LED
#define LED1_ON		{P1M0|=0x02;P11=0;}
#define LED1_OFF	{P11=1;P1M0&=~0x02;}
#define LED1_FLASH	P1M0^=0x02

#define LED2_ON		{P1M0|=0x01;P10=0;}
#define LED2_OFF	{P1M0&=~0x01;P10=1;}
#define LED2_FLASH	P1M0^=0x01


//deifne input-------------
#define BT_CONECT	P10
#define BT_EN			P14
#define BCTS			P32
#define BRTS			P33

#define IR_REN		P13
//define const

//#define TRUE	1
//#define FALSE	0

#define Manu_IDH 0x88
#define Manu_IDL 0x66
#define MAX_BUF	60

#define TIME_OUT	30
//systme power off time out 30S
#define BYTE_TIME_OUT	100
//byte time out 30mS


#define SYSCLK			22118400L
#define T01MS			(65536-SYSCLK/12/10000)

#define S_P30	P_SW1&=~0x40
#define S_P36	P_SW1|=0x40

#define MAX_SPEED	200

//end define const

#endif
