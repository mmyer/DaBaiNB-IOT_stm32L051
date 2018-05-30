/**
  ******************************************************************************
  * File Name          : DABAI_APP.c
  * Description        : This file provides code for the configuration
  *                      of the DABAI_APP instances.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "DaBai_APP.h"
#include "DaBai_GPIO.h"
#include "DaBai_tim.h"
#include "DaBai_ADC.h"
#include "sht20_cfg.h"

GPIO_PinState key[3] = {GPIO_PIN_RESET,GPIO_PIN_RESET,GPIO_PIN_RESET};
uint8_t  key_flag[3]= {0};
uint32_t key_counter[3] = {0};
uint8_t  key_State[3] = {0};
uint8_t  power_off_flag = 0;
uint32_t g_lightValue = 0;
float    g_Sht20Temp = 0;
float    g_Sht20RH = 0;


void KeyProcess(void)
{
	uint8_t i = 0;
	key_State[KEY1] = 0,key_State[KEY2] = 0,key_State[KEY3] = 0;
	
	/*��ȡ����״̬*/
	if(HAL_GPIO_ReadPin(GPIOA, KEY1_Pin) == GPIO_PIN_SET)
		key[KEY1] = GPIO_PIN_RESET;
	else
		key[KEY1] = GPIO_PIN_SET;
	
	key[KEY2] = HAL_GPIO_ReadPin(GPIOB, KEY2_Pin);
	key[KEY3] = HAL_GPIO_ReadPin(GPIOB, KEY3_Pin);
		
	for(i = 0; i < 3; i++)//�ֱ��3��������״̬���д���
	{
		key_flag[i] <<= 1;
		if( key[i] == GPIO_PIN_RESET )//����������
		{
			key_flag[i] |= 0x01;
		}
		
		if( key_flag[i] == 0xff )//�����������������ȶ����£���ʱkey_flag����0xff,ֱ�������ɿ�
		{
			key_counter[i]++;//��¼���������±�־����ֵԽ�󱻰���ʱ��Խ��
			if(key_counter[i] == 0) key_counter[i] = 1;//����ʱ����������ڱ�����
			switch(i)
			{
				case KEY1:
				{ 
					key_State[KEY1] = 1;
					if(key_counter[KEY1] > 20)
					{
						power_off_flag = 1;
						PowerOffGpioConfig();
						if(key_counter[KEY1] < 25)
							BEPP_ON;
						else
							BEPP_OFF;
					}
				}break;
				case KEY2:
				{
					key_State[KEY2] = 1;
				}break;

				case KEY3:
				{
					key_State[KEY3] = 1;
				}break;

				default:
				{}break;
			}
		}
		else if(key_flag[i] == 0x00)//��������
		{	
			if( key_counter[i] >= 1 )//�൱�����ּ��
			{
				switch(i)
				{
					case KEY1:
					{ 
						if(power_off_flag == 1)
							POWER_OFF;	
					}break;
					case KEY2:
					{					
							
					}break;

					case KEY3:
					{

					}break;
					
					default:
					{}break;
				}	
				
				key_counter[i] = 0;
			}
		}
	}
	
	if(key_State[KEY2] == 1 || key_State[KEY3] == 1)
	{
		//HAL_TIM_PWM_Start(&TimHandle, TIM_CHANNEL_2);
		//HAL_GPIO_WritePin(GPIOB, LED5, GPIO_PIN_RESET);
	}
	else
	{
		//HAL_TIM_PWM_Stop(&TimHandle, TIM_CHANNEL_2);
		//HAL_GPIO_WritePin(GPIOB, LED5, GPIO_PIN_SET);
	}

}


void DaBaiSensorTask(void)
{
	g_lightValue = getLightValue();
	if(g_lightValue > 700 || g_Sht20Temp > 29)
	{
		HAL_TIM_PWM_Start(&TimHandle, TIM_CHANNEL_2);
		//HAL_GPIO_WritePin(GPIOB,GPIO_PIN_7, GPIO_PIN_RESET);
	}
	else
	{
		HAL_TIM_PWM_Stop(&TimHandle, TIM_CHANNEL_2);
		//HAL_GPIO_WritePin(GPIOB,GPIO_PIN_7, GPIO_PIN_SET);
	}
	g_Sht20Temp = SHT20_Convert(SHT20_ReadTemp(),1);
	g_Sht20RH   = SHT20_Convert(SHT20_ReadRH(),0);
}

/************************ (C) COPYRIGHT DaBai_IOT *****END OF FILE****/
