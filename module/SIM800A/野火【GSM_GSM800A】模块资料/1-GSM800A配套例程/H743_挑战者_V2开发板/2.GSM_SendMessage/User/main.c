/**
  ******************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2019-xx-xx
  * @brief   GSM模块发送短信测试程序
  ******************************************************************
  * @attention
  *
  * 实验平台:野火 STM32H743 开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************
  */  
#include "stm32h7xx.h"
#include "main.h"
#include "./led/bsp_led.h"
#include "./usart/bsp_usart.h"
#include "./gsm_gprs/bsp_gsm_usart.h"
#include "./systick/bsp_SysTick.h"
#include "./gsm_gprs/bsp_gsm_gprs.h"
#include "./key/bsp_key.h"
#include <string.h>

/* 请修改接收短信的电话号码 */
const char num[]="13926769552";
char namenum[20*4]={0},str[512]={0},gbkstr[256]={0},namegbk[256]={0};


/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{  
 	uint8_t newmessadd=0,IsRead=0;
	uint8_t testCard = 0;
	/* 系统时钟初始化成400MHz */
	SystemClock_Config();
 
  SCB_EnableICache();    // 使能指令 Cache
  SCB_EnableDCache();    // 使能数据 Cache
    
	SysTick_Init();
	/* LED 端口初始化 */
	LED_GPIO_Config();
	
  Key_GPIO_Config();
    /* 配置串口1为：115200 8-N-1 */
	UARTx_Config();
  
  printf("\r\n野火BH-GSM模块发送短信例程\r\n");
  printf("\r\n测试前请确认GSM模块已接入电话卡\r\n");
  printf("\r\n测试前请在程序中修改接收短信的目标手机号码\r\n");

		//检测模块响应是否正常
	while(gsm_init()!= GSM_TRUE)
	{
		printf("\r\n模块响应测试不正常！！\r\n");
		printf("\r\n若模块响应测试一直不正常，请检查模块的连接或是否已开启电源开关\r\n");
		GSM_DELAY(500);

	}
    
    
    printf("\r\n通过了模块响应测试\r\n");

	GSM_DELAY(1000); 
	//先执行次设置文本模式
	if(gsm_cmd("AT+CMGF=1\r","OK", 100) != GSM_TRUE)
	{
		printf("\r\n设置文本模式错误\r\n");
	}
	GSM_DELAY(1000); 	 

	printf("\r\n正在等待GSM模块初始化...\r\n");
	while(IsInsertCard() != GSM_TRUE)
	{
		
		if(++testCard >20)
		{
			printf("\r\n检测不到电话卡，请断电并重新接入电话卡\r\n");
		}
		GSM_DELAY(1000); 		
	}		
	
	printf("\r\n初始化完成，5秒后开始发送短信测试... \r\n");

    /* 延时5秒再发送命令到模块 */
    GSM_DELAY(5000);    
    
    //发送英文短信
		
		if(gsm_sms((char *)num,"hellow world") == GSM_TRUE)
			printf("\r\n英文短信已发送至：%s，为方便测试，请在程序中修改接收短信的手机号码",num);
		else
		{
			printf("\r\n短信发送失败，请确认目标号码有效");
		}
			GSM_DELAY(2000);    

		//中英文短信，实际测试时请把电话号码修改成要接收短信的手机号
		if(gsm_sms((char *)num,"野火WF-GSM模块短信测试") == GSM_TRUE)
			printf("\r\n中英文短信已发送至：%s，为方便测试，请在程序中修改接收短信的手机号码",num);
		else
		{
			printf("\r\n短信发送失败，请确认目标号码有效");
			while(1);
		}		

		printf("\r\n接收短信测试：通过串口1打印新短信信息\r\n");
		
  while(1)
	{
		GSM_DELAY(1000);
		newmessadd=IsReceiveMS();	
		if(newmessadd)
		{			
			IsRead=readmessage(newmessadd,namenum,str);				
			 
//			printf("newmessadd=%d,IsRead:%d\n",newmessadd,IsRead);
			if(IsRead)
			{				
				printf("新短信\n\r");

				hexuni2gbk(namenum,namegbk);	
				hexuni2gbk(str,gbkstr);						
				printf("新短信:\n发件人:%s\n内容:%s\n\r",namegbk,gbkstr);
			}
		}
		
	}
	
}



/**
  * @brief  System Clock 配置
  *         system Clock 配置如下: 
	*            System Clock source  = PLL (HSE)
	*            SYSCLK(Hz)           = 400000000 (CPU Clock)
	*            HCLK(Hz)             = 200000000 (AXI and AHBs Clock)
	*            AHB Prescaler        = 2
	*            D1 APB3 Prescaler    = 2 (APB3 Clock  100MHz)
	*            D2 APB1 Prescaler    = 2 (APB1 Clock  100MHz)
	*            D2 APB2 Prescaler    = 2 (APB2 Clock  100MHz)
	*            D3 APB4 Prescaler    = 2 (APB4 Clock  100MHz)
	*            HSE Frequency(Hz)    = 25000000
	*            PLL_M                = 5
	*            PLL_N                = 160
	*            PLL_P                = 2
	*            PLL_Q                = 4
	*            PLL_R                = 2
	*            VDD(V)               = 3.3
	*            Flash Latency(WS)    = 4
  * @param  None
  * @retval None
  */
static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;
  HAL_StatusTypeDef ret = HAL_OK;
  
  /*使能供电配置更新 */
  MODIFY_REG(PWR->CR3, PWR_CR3_SCUEN, 0);

  /* 当器件的时钟频率低于最大系统频率时，电压调节可以优化功耗，
		 关于系统频率的电压调节值的更新可以参考产品数据手册。  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}
 
  /* 启用HSE振荡器并使用HSE作为源激活PLL */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_OFF;
  RCC_OscInitStruct.CSIState = RCC_CSI_OFF;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;

  RCC_OscInitStruct.PLL.PLLM = 5;
  RCC_OscInitStruct.PLL.PLLN = 160;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
 
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_2;
  ret = HAL_RCC_OscConfig(&RCC_OscInitStruct);
//  if(ret != HAL_OK)
//  {

//    while(1) { ; }
//  }
  
	/* 选择PLL作为系统时钟源并配置总线时钟分频器 */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK  | \
																 RCC_CLOCKTYPE_HCLK    | \
																 RCC_CLOCKTYPE_D1PCLK1 | \
																 RCC_CLOCKTYPE_PCLK1   | \
                                 RCC_CLOCKTYPE_PCLK2   | \
																 RCC_CLOCKTYPE_D3PCLK1);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;  
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2; 
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2; 
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2; 
  ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4);
  if(ret != HAL_OK)
  {
    while(1) { ; }
  }
}
/****************************END OF FILE***************************/
