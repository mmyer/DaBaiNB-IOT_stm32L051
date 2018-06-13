/**
******************************************************************************
* �ļ�����: main.c 
* ��    ��: DaBai_IOT Cifi
* ��    ��: V1.0
* ��д����: 2018-6-04
* ��    ��: BC95��BC28 ATָ����������
******************************************************************************
* ˵����
* ���������״��stm32������DaBai_L051_V1.0ʹ�á�
* 
����Ա���ַ��https://dabairobot.taobao.com

�����ǶԱ����������������˵����

//////////////////////////////////////////////////////
1������˵����
�ڽ����غ�ϵͳ�����Զ���������Ҫ�̰�key1(S1)�����ŻῪ����
�ڲ�������ʱ������ֱ����USB�߹��磬����USB�ߺ�ϵͳ���Զ��ϵ翪����

//////////////////////////////////////////////////////
2�����ػ��߼���
		�������̰�key1(S1)������ϵͳ�Ὺ��������������0.3s,5��LED��ȫ��0.3s,���ϵͳ����ָʾ��LED1��1Hz��Ƶ����˸
		�ػ�������key1(S1)����1s���ϣ�����������1s���ң�����������������ɿ�key1(S1)������ϵͳ���Զ��ػ�

//////////////////////////////////////////////////////
3����ع�������
		��磺����ʱ�����ָʾ��LED5��1s��˸һ�Σ��������LED5�������ҷ���������5����ʾ
		�ŵ磺�ŵ��ʱ�� 
						 ��ص��� >= 10% �� LED5Ϩ��
			 7% <= ��ص��� < 10%  �� LED5��5HzƵ����˸�����ҷ�������5HzƵ�ʱ���
						 ��ص��� < 7%   �� ��������1s���Զ��ػ�

//////////////////////////////////////////////////////
4����������
	��������g_lightValue > 700ʱ������Խ��g_lightValue��ֵԽ�󣩣���������
	�¶ȣ����¶�ֵg_Sht20Temp > 31���϶�ʱ��g_Sht20Temp����ֵ���Ǳ�ʾʵ�ʵ��¶ȣ�����������
	ʪ�ȣ���ʪ��ֵg_Sht20RH > 70ʱ��ʪ��Խ��g_Sht20RH����ֵ��Խ�󣩣���������

//////////////////////////////////////////////////////
5�������͵ƽӿڣ�
	key4(S4)������MCU�ĸ�λ������
	key1(S1)��key2(S2)��key3(S3)��������ΪMCU�İ�������ӿ���ʹ��
	��key2(S2)��key3(S3)����ΪMCU�İ����ӿ�ʹ��ʱ���ɵ�����ͨ��IO����ʹ��
	
	LED5��Ϊ��ص����ͳ����ص�ָʾ�ƣ��������������Ϊ����״̬��ָʾ
	LED1��LED2��LED3��LED4�û����������Զ���״ָ̬ʾ�ơ�
	��LED1��LED2��LED3��LED4����ΪMCU��LED�ƽӿ���ʹ��ʱ�����Ե�����ͨ��IO����ʹ��

//////////////////////////////////////////////////////
6��BC95��BC28ָ�
		����BC95��BC28��ATָ����ȫ��ͬ�����Ա������ļ���һЩ����������ʽ���ǰ���֮ǰ��BC95�������ģ�
		��BC28ģ����ȫ���ã������߲��ص����������
		
7���������ӣ�
	�������ṩ���ǣ�ʹ��ATָ���BC28ͨ��TCPЭ�����Ӵ�׵Ĳ��Է��������������ݣ�
	�������յ��󷵻ط��ͳ�ȥ�����ݡ�
	���紫�����ݲ������裺
	1��BC28���İ����NB�������������İ����STM32�������
	2������STM32���MicroUSB�ߣ����Դ򿪴������֣�ѡ�����USB�Ĵ��ںźͲ����ʣ�115200bps��,�򿪴˴��ڡ�
	3��ϵͳ�����󣬶̰�һ��key1(S1)������
		������Զ���ʼ��BC28ģ�飬���ҽ����������̵Ĳ�����
		(1)��
	���������Ƴ�����coapЭ����������ӣ���������IOTƽ̨��
	


//////////////////////////////////////////////////////

* ��Ȩ�������������С�
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "DaBai_i2c.h"
#include "DaBai_usart.h"
#include "DaBai_ADC.h"
#include "DaBai_tim.h"
#include "DaBai_APP.h"
#include "DaBai_GPIO.h"
#include "NB_Board_Cfg.h"
#include "timer_user_poll.h"

#define TASKTIME_10MS  		10
#define TASKTIME_100MS  	100
#define TASKTIME_500MS  	500
#define TASKTIME_1000MS   1000

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

static GPIO_InitTypeDef  GPIO_InitStruct;
/* Private functions ---------------------------------------------------------*/


	

uint8_t uartData[5] = {1,2,3,4,5};
uint8_t  readFlag = 0;
uint32_t 	Delay1msCnt = 0;
uint16_t g_TaskTime10ms = 0;
uint16_t g_TaskTime100ms = 0;
uint16_t g_TaskTime500ms = 0;
uint16_t g_TaskTime1000ms = 0;


/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
	static  uint32_t start_tick = 0;
	static uint32_t cur_tick = 0;
 
  HAL_Init();
  SystemClock_Config();
	MX_TIM_Init();
	MX_GPIO_Init();
	MX_I2C2_Init();
	MX_ADC_Init();
	MX_USART1_UART_Init();
	MX_LPUART1_UART_Init();
	
	NBModule_open(&nb_config);
  APP_STATE = NB_NONE;
	
	start_tick = HAL_GetTick();
	while((HAL_GetTick()- start_tick) <300)
	{
		// power on beep remind
	}
	LED1_OFF;
	LED2_OFF;
	LED3_OFF;
	LED4_OFF;
	CHG_LED5_OFF;
	printf("\r\nDaBai Init OK \r\n");
	
  /* Infinite loop */
  while (1)
  {	
		HAL_UART_Poll();
		NBModule_Main(&nb_config);
    MX_TimerPoll();
		
		if(g_TaskTime10ms > TASKTIME_10MS)
		{
			g_TaskTime10ms = 0;
			DaBai_10msTask();
		}
		if(g_TaskTime100ms > TASKTIME_100MS)
		{
			g_TaskTime100ms = 0;
			DaBai_100msTask();
		}
		
		if(g_TaskTime500ms > TASKTIME_500MS)
		{
			g_TaskTime500ms = 0;
			g_BeepFreq = 0;
			DaBai_500msTask();
		}
		if(g_TaskTime1000ms > TASKTIME_1000MS)
		{
			g_TaskTime1000ms = 0;
			DaBai_1000msTask();
		}
		switch(APP_STATE)
    {
    case NB_NONE:
      {
        
      }
      break;
    case NB_INIT:
      {
        printf("\r\n<----BC95\28 Init---->\r\n");
        NBModule_Init(&nb_config);
        APP_STATE = NB_END;
      }
      break;
    case NB_SIGN:
      {
         printf("\r\n<----BC95\28 get signal---->\r\n");
         NBModule_Sign(&nb_config);
         APP_STATE = NB_END;
      }
      break;
    case NB_MODULE:
      {
        printf("\r\n<----Module info ---->\r\n");
        NBModule_Info(&nb_config);
        APP_STATE = NB_END;
      }
      break;
    case NB_TCP_CR:
      {
        printf("\r\n<----Create tcp ---->\r\n");
        NBModule_CreateTCP(&nb_config);
        APP_STATE = NB_END;
      }
      break;
    case NB_TCP_CL:
      {
        printf("\r\n<----Close udp ---->\r\n");
        NBModule_CloseTCP(&nb_config);
        APP_STATE = NB_END;
      }
      break;
    case NB_TCP_CNT:
      {
        printf("\r\n<---- Connect tcp ---->\r\n");
        NBModule_ConnectTcp(&nb_config);
        
        APP_STATE = NB_END;
      }
      break;
		case NB_TCP_ST:
      {
        printf("\r\n<---- Send tcp ---->\r\n");
        char* userPacket = "DaBaiIOT";
        NBModule_SendTcpData(&nb_config,sizeof("DaBaiIOT"),userPacket);
        
        APP_STATE = NB_END;
      }
      break;
    case NB_TCP_RE:
      {
        //do nothing
        APP_STATE = NB_END; 
      }
      break;
    default:
      {
        
      }
      break;
    }
		
		SetBeepFreq(g_BeepFreq);
		
	/**uart debug start**/	
	//HAL_UART_Log(uartData,5);	
	//HAL_UART_Transmit_DMA(&hlpuart1,uartData,5); 
	//HAL_LPUART1_Write(uartData,5);
	/**uart debug end**/
  }
	
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = HSE
  *            SYSCLK(Hz)                     = 8000000
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit;
	
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE|RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;//open 8M����
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;//open 32.768KHz���� 
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }
  
  /* Select HSE as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSE;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;  
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0)!= HAL_OK)
  {
    /* Initialization Error */
    while(1); 
  }
  /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE();
  
  /* The voltage scaling allows optimizing the power consumption when the device is 
     clocked below the maximum system frequency, to update the voltage scaling value 
     regarding system frequency refer to product datasheet.  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);
  
  /* Disable Power Control clock */
  __HAL_RCC_PWR_CLK_DISABLE();
	
	PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1
                              |RCC_PERIPHCLK_LPUART1|RCC_PERIPHCLK_USART2|RCC_PERIPHCLK_RTC|RCC_PERIPHCLK_I2C2;//
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
  PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
  PeriphClkInit.Lpuart1ClockSelection = RCC_LPUART1CLKSOURCE_PCLK1;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  /**Configure the Systick interrupt time 1ms
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
}


/**
  * @brief  This function is executed in case of error occurrence.
  * @param  file: The file name as string.
  * @param  line: The line in file as a number.
  * @retval None
  */
void _Error_Handler(char *file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}


/************************ (C) COPYRIGHT DaBai_IOT *****END OF FILE****/
