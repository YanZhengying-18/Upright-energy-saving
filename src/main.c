/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,逐飞科技
 * All rights reserved.
 * @file       		main
 * note				破浪
 ********************************************************************************************************************/

/****************声明头文件********************/

#include "headfile.h"


/****************参数声明********************/
uint8 CONTROL_PERIOD = 5 ;
int stop_flag=0;
int g_n1MSEventCount = 0;
int g_nSpeedCount;
int g_nDirectionCount;



void main()
{		
	
	
//		if(1)			//充电完成
//		{	
//			int star_flag=1;
			
		  DisableGlobalIRQ();			//关闭总中断								  
		  all_init();
	
				TH1 = 0xf8;					//定时器1中断，中断1ms
				TL1 = 0x30;
				TMOD = 0x00;	//看数据手册 00 16位自动
				TR1 = 1;
				ET1 = 1;	
			
//				T2L = 0xD8;						//定时器2中断，中断5ms
//				T2H = 0xF0;
//				AUXR = 0x10; 
//				IE2 = 0x04; 
			
		  EnableGlobalIRQ();				//开总中断
			
		 
			
			while(1)
		  {
	
				//OutPut_Vcan();

					oled_view();
				
			
			}
	}


void TM1_Rountine(void) interrupt 3 		//1ms的中断	 5ms一周期
{			
		
			g_n1MSEventCount++ ;
	
			if (g_n1MSEventCount >= CONTROL_PERIOD) {
				g_n1MSEventCount = 0;
				GetMotorPulse();   //读取两个电极脉冲计数值（测速）					 
			}
			else if (g_n1MSEventCount == 1) 		//读取电感值（为直立、方向做准备）
				{	 
					Rd_Ad_Value();
			
				} 
			else if(g_n1MSEventCount == 2) 		//直立类算法
				{			
					
				AngleCalculate();
				AngleControl();
				MotorOutput();
					
			} 
			else if (g_n1MSEventCount == 3)		//速度类算法
				{							
			g_nSpeedCount ++;		
			if(g_nSpeedCount >= SPEED_CONTROL_COUNT) 
				{
			SpeedControl();		//100ms
			g_nSpeedCount = 0;
				}
				SpeedControlOutput();		
			} 
			else if (g_n1MSEventCount == 4) 
				{							   //方向类算法
				g_nDirectionCount ++;
		get_AD_Date();					//获取电感的数值					
			
			if(g_nDirectionCount >= DIRECTION_CONTROL_COUNT) 
				{
				DirectionControl();		//10ms
			  	g_nDirectionCount = 0;
						}
					DirectionControlOutput();					
					}
									
}															



//void TM2_Isr() interrupt 12   			       //定时5ms
//	{
//       
//		chuku();
//		round_judge();
//			zebra_judge();					
//		
//		
//		
//}
