/******************************************************************************/
/* Blinky.c: LED Flasher and graphic demo                                     */
/******************************************************************************/
/* This file is part of the uVision/ARM development tools.                    */
/* Copyright (c) 2005-2009 Keil Software. All rights reserved.                */
/* This software may only be used under the terms of a valid, current,        */
/* end user licence from KEIL for a compatible version of KEIL software       */
/* development tools. Nothing else gives you the right to use this software.  */
/******************************************************************************/
                  
#include <LPC17xx.H>                    				//NXP LPC17xx definitions 
#include "LCD.h"
#include "string.h"
#include "sys.h"
#include "lcd.h"
#include "touch.h"
#include "exti.h"
#include "I2C.H"
#include "uart.h"


u8 buf1[32];
extern uint32_t SystemFrequency;
extern unsigned char data[10];
extern int len;
extern u16 pencolor;
u16 tccolor;
/*
*********************************************************************************************************
* Description: 	The function is setting font colors¡¢brush colors and the area of clear
* Arguments  : 	None
* Returns    : 	None
*********************************************************************************************************
*/
void Load_Drow_Dialog(void)
{
		
		LCD_Color(239,0,80,40,Blue);
		LCD_Color(239,40,80,40,Yellow);
		LCD_Color(239,80,80,40,Green);
		LCD_Color(239,120,80,40,Red);
		LCD_Color(239,160,80,40,Purple);
		LCD_Color(239,200,80,40,Cyan);
}
u16 colorchange(unsigned char color)
{
	switch(color)
	{
		case 0:return 0x0000;
		case 1:return 0xffff;
		case 2:return 0xf800;
		case 3:return 0x001f;
		case 4:return 0x07e0;
		case 5:return 0xffe0;
	}
}
/*
*********************************************************************************************************
* Description: 	Delay function
* Arguments  : 	The time of ms
* Returns    : 	None
*********************************************************************************************************
*/
void delay1ms(unsigned int delay1msdata)
{
	unsigned int i,j,k;
	for(i=0;i<delay1msdata;i++)
		for(j=0;j<1000;j++)
			for(k=0;k<100;k++)
		;
}

u8 KEY_Scan(void)
{
	if(!SW1)
	delay1ms(2);
	if(!SW1)
		return 1;
	else
		return 0;
}
/*
*********************************************************************************************************
* Description: 	Main function
* Arguments  : 	None
* Returns    : 	None
*********************************************************************************************************
*/
int main (void)                       
{
  u8 key;
	
	u8 x0,y0,x1,y1,x2,y2;
	u8 r,a,b;
	unsigned char flag;
	tccolor=Blue;
  SystemInit();
	UART0_Init();
  LCD_Init   ();
	LPC_GPIO0->FIODIR |= 0x00002C0;  										//P0.6 0.7 0.9 Output
	LPC_GPIO2->FIODIR |= 0x00000FF;  										//P2.13 input P2.0...P2.7 Output	 
	Touch_Init();
	LCD_Color(0,0,239,239,White);
	Load_Drow_Dialog();
	LCD_Color(279,0,40,40,White);
	while(1)
	{
		if(len==8)
		{
				len=0;
				flag=data[6];
				switch(flag)
				{
							case 0x01:
								x0=data[0];y0=data[1];x1=data[2];y1=data[3];
								pencolor=colorchange(data[7]);
								LCD_Color(0,0,239,239,pencolor);
								LCD_DrawLine(x0,y0,x1,y1);
								break;
							case 0x02:
								x0=data[0];y0=data[1];x1=data[2];y1=data[3];x2=data[4];y2=data[5];
								pencolor=colorchange(data[7]);
								LCD_Color(0,0,239,239,pencolor);
								LCD_DrawTriangle(x0,y0,x1,y1,x2,y2);
								break;
							case 0x03:
								x0=data[0];y0=data[1];x1=data[2];y1=data[3];
								pencolor=colorchange(data[7]);
								LCD_Color(0,0,239,239,pencolor);
								LCD_DrawRectangle(x0,y0,x1,y1);
								break;
							case 0x04:
								x0=data[0];y0=data[1];r=data[2];
								pencolor=colorchange(data[7]);
								LCD_Color(0,0,239,239,pencolor);
								LCD_DrawCircle(x0,y0,r);
								break;
							case 0x05:
								x0=data[0];y0=data[1];a=data[2];b=data[3];
								pencolor=colorchange(data[7]);
								LCD_Color(0,0,239,239,pencolor);
								LCD_DrawEllipse(x0,y0,a,b);
			}
		}
		key =  KEY_Scan();
		if(Pen_Point.Key_Sta==Key_Down)										//Touch screen is pressed 
		{
			Pen_Int_Set(0);													//Closed interrupt
			do
			{
				Convert_Pos();
				Pen_Point.Key_Sta=Key_Up;
//				Draw_Big_Point(Pen_Point.X0,Pen_Point.Y0);				//Drawing	
				if(Pen_Point.Y0>=240)
				{
					Load_Drow_Dialog();
					switch(Pen_Point.X0/40)
					{
						case 0:LCD_Color(279,0,40,40,White);tccolor=Blue;break;
						case 1:LCD_Color(279,40,40,40,White);tccolor=Yellow;break;
						case 2:LCD_Color(279,80,40,40,White);tccolor=Green;break;
						case 3:LCD_Color(279,120,40,40,White);tccolor=Red;break;
						case 4:LCD_Color(279,160,40,40,White);tccolor=Purple;break;
						case 5:LCD_Color(279,200,40,40,White);tccolor=Cyan;break;
					}
				}
		/*		else
				{
						LCD_Color(0,0,239,239,tccolor);
				}*/
						
			}while(PEN==0);                                                  //While the Touch screen is pressing
			Pen_Int_Set(1);													 //Opene the interrupt
		}
		else
			delay1ms(1);
	}
}


