/**
  ******************************************************************************
  * @file    ADC/ADC1_DMA/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and peripherals
  *          interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"

#include "stdio.h"
#include "systic.h"
#include "OLED.h" 
#include "MPU6050.h"
#include "LED.h" 
#include "Buzzer.h"
#include "Switch.h"
#include "Key.h"
#include "Motor.h"
#include "USART.h"
#include "TIM.h"


extern uint32_t TimingDelay;		//延时ms定时器



/** @addtogroup STM32F10x_StdPeriph_Examples
  * @{
  */

/** @addtogroup ADC_ADC1_DMA
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSV_Handler exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
/******************************************************************************/
void SysTick_Handler(void)
{
  if (TimingDelay != 0x00)
  { 
    TimingDelay--;
  }
}
uint8_t TEMP[20];
float Volta;
void TIM3_IRQHandler(void)
{
  
	if(TIM_GetFlagStatus(TIM3,TIM_FLAG_Update) == SET)
	{
		//清除标志位
	TIM_ClearFlag(TIM3,TIM_FLAG_Update);
		//
	  Angle_Calcu();	 
	   
	  sprintf(TEMP,"Kp:%4.1f",kp);
//	  OLED_ShowNum(0,4,TEMP,5,16);//显示ASCII字符的码值 
	  OLED_ShowString(0,4,TEMP,16);
	   sprintf(TEMP,"Kd:%4.1f",kd);
//	  OLED_ShowNum(50,4,TEMP,3,16);//显示ASCII字符的码值
	  OLED_ShowString(70,4,TEMP,16); 
	  Volta=7.93;
	  sprintf(TEMP,"Volta: %2.2fV",Volta);
	  OLED_ShowString(0,6,TEMP,16);//显示ASCII字符的码值
	  Moto1=balance(angle,-GyroY);
	  Moto2=balance(angle,-GyroY);		
	  KeyScan();
//	  if(Key1_Count==1)
//		  {
//		  	Key1_Count=0;
//			kp=kp+1;
//		  }
//	  if(Key2_Count==1)
//		  {
//		  	Key2_Count=0;
//			kd=kd+1;
//		  }
//	  OLED_ShowNum(0,6,Key1_Count,6,16);//显示ASCII字符的码值
//    Moto1=balance(angle,-GyroY);
//	  Moto2=balance(angle,-GyroY);
	  Xianfu_Pwm();	
	  
//	  tt++;
//	  if(tt>=200)
//	  {	  
//        secons++;
//	    OLED_ShowNum(103,4,secons,3,16);//显示ASCII字符的码值 
//	    tt=0;
//	    
//	   }
	  if(Switch1==0)
	  {
	   	  LED1=0;
         Set_Pwm(Moto1,Moto2);
		 }
		 else
		 {
		  LED1=1;
		  Set_Pwm(0,0);
		  }
	  Stop_Move(); 
	}
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 

/**
  * @}
  */ 

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
