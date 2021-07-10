/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,��ɿƼ�
 * All rights reserved.
 * @file       		main
 * note				����
 ********************************************************************************************************************/

/****************����ͷ�ļ�********************/

#include "headfile.h"


/****************��������********************/
uint8 CONTROL_PERIOD = 5 ;
int stop_flag=0;
int g_n1MSEventCount = 0;
int g_nSpeedCount;
int g_nDirectionCount;



void main()
{		
	
	
//		if(1)			//������
//		{	
//			int star_flag=1;
			
		  DisableGlobalIRQ();			//�ر����ж�								  
		  all_init();
	
				TH1 = 0xf8;					//��ʱ��1�жϣ��ж�1ms
				TL1 = 0x30;
				TMOD = 0x00;	//�������ֲ� 00 16λ�Զ�
				TR1 = 1;
				ET1 = 1;	
			
//				T2L = 0xD8;						//��ʱ��2�жϣ��ж�5ms
//				T2H = 0xF0;
//				AUXR = 0x10; 
//				IE2 = 0x04; 
			
		  EnableGlobalIRQ();				//�����ж�
			
		 
			
			while(1)
		  {
	
				//OutPut_Vcan();

					oled_view();
				
			
			}
	}


void TM1_Rountine(void) interrupt 3 		//1ms���ж�	 5msһ����
{			
		
			g_n1MSEventCount++ ;
	
			if (g_n1MSEventCount >= CONTROL_PERIOD) {
				g_n1MSEventCount = 0;
				GetMotorPulse();   //��ȡ�����缫�������ֵ�����٣�					 
			}
			else if (g_n1MSEventCount == 1) 		//��ȡ���ֵ��Ϊֱ����������׼����
				{	 
					Rd_Ad_Value();
			
				} 
			else if(g_n1MSEventCount == 2) 		//ֱ�����㷨
				{			
					
				AngleCalculate();
				AngleControl();
				MotorOutput();
					
			} 
			else if (g_n1MSEventCount == 3)		//�ٶ����㷨
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
				{							   //�������㷨
				g_nDirectionCount ++;
		get_AD_Date();					//��ȡ��е���ֵ					
			
			if(g_nDirectionCount >= DIRECTION_CONTROL_COUNT) 
				{
				DirectionControl();		//10ms
			  	g_nDirectionCount = 0;
						}
					DirectionControlOutput();					
					}
									
}															



//void TM2_Isr() interrupt 12   			       //��ʱ5ms
//	{
//       
//		chuku();
//		round_judge();
//			zebra_judge();					
//		
//		
//		
//}
