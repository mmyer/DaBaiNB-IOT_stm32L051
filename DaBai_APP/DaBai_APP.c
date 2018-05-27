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


GPIO_PinState key[3] = {GPIO_PIN_SET,GPIO_PIN_SET,GPIO_PIN_SET};
uint8_t  key_flag[3]= {0};
uint16_t key_counter[3] = {0};
uint8_t  key_State[3] = {0};
void KeyProcess(void)
{
	uint8_t i = 0;
	key_State[KEY1] = 0,key_State[KEY2] = 0,key_State[KEY3] = 0;
	
	/*��ȡ����״̬*/
	key[KEY1] = HAL_GPIO_ReadPin(GPIOA, KEY1_Pin);
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




/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
