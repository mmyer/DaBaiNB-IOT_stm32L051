/**
  ******************************************************************************
  * File Name          : DABAI_APP.c
  * Description        : This file provides code for the configuration
  *                      of the DABAI_APP instances.
  ******************************************************************************
<<<<<<< HEAD
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2018 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
=======
>>>>>>> dev
  */

/* Includes ------------------------------------------------------------------*/
#include "DaBai_APP.h"
<<<<<<< HEAD
#include "main.h"
#include "DaBai_GPIO.h"

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
=======
#include "DaBai_GPIO.h"
#include "DaBai_tim.h"
#include "DaBai_ADC.h"
#include "sht20_cfg.h"
#include "NB_Board.h"
#include "DaBai_rtc.h"


GPIO_PinState m_key[3] = {GPIO_PIN_RESET,GPIO_PIN_RESET,GPIO_PIN_RESET};
volatile uint8_t  m_key_flag[3]= {0};
volatile uint32_t m_key_counter[3] = {0};
volatile uint8_t  m_key_State[3] = {0};

GPIO_PinState m_chargeState = GPIO_PIN_RESET;
volatile uint8_t m_charge_flag = {0};
volatile uint8_t m_charge_counter = {0};
volatile Yes_No_Status g_USB_insert = NO;
volatile Yes_No_Status g_chargeing_flag = NO;

uint32_t m_fullBatTimeCnt = 0;//��¼���ʱ�ﵽ100%���ϵ�ʱ��

uint8_t  g_power_off_flag = 0;

uint8_t g_BatVoltage = 0;
uint16_t g_lightValue = 0;
float    g_Sht20Temp = 0;
float    g_Sht20RH = 0;
uint16_t g_BeepFreq = 0;
float    g_longitude = 0;//����
float    g_latitude = 0;//γ��


uint8_t g_BatVoltageLow = 0;//��������10%��־λ
uint8_t m_fullBatFlag = 0;
uint8_t m_fullBatHintCnt = 0;
uint32_t m_coapSendTimes = 0;

volatile NB_STATE_e  APP_STATE= NB_NONE;


void Fill_u16_To_u8(uint16_t x, char* h, char* l)
{
	*h = (char)(x >> 8);
	*l = (char)(x >> 0);
}

void Fill_int16_To_int8(int16_t x, char* h, char* l)
{
	*h = (char)(x >> 8);
	*l = (char)(x >> 0);
}

void Fill_u32_To_u8(uint32_t x, char* hh, char* hl, char* lh, char* ll)
{
	*hh = (char)(x >> 24);
	*hl = (char)(x >> 16);
	*lh = (char)(x >> 8);
	*ll = (char)(x >> 0);
}

/*************************************
fn : BeepToggle

brief : ��������ת����
param : none

return : none

*************************************/

void BeepToggle(uint16_t beepFrequency)
{
	static uint8_t m_lock = 0;
	
	if(m_lock == 0)
	{
		m_lock = 1;
		g_BeepFreq = beepFrequency;//1KHz
	}
	else if(m_lock == 1)
	{
		m_lock = 0;
		g_BeepFreq = 0;
	}
}
/*************************************
fn : KeyTask

brief : ������������
param : none

return : none

*************************************/

void KeyTask(void)
{
	uint8_t i = 0;
	m_key_State[KEY1] = 0,m_key_State[KEY2] = 0,m_key_State[KEY3] = 0;
	
	/*��ȡ����״̬*/
	if(HAL_GPIO_ReadPin(GPIOA, KEY1_PIN) == GPIO_PIN_SET)
		m_key[KEY1] = GPIO_PIN_RESET;
	else
		m_key[KEY1] = GPIO_PIN_SET;
	
	m_key[KEY2] = HAL_GPIO_ReadPin(GPIOB, KEY2_PIN);
	m_key[KEY3] = HAL_GPIO_ReadPin(GPIOB, KEY3_PIN);
		
	for(i = 0; i < 3; i++)//�ֱ��3��������״̬���д���
	{
		m_key_flag[i] <<= 1;
		if( m_key[i] == GPIO_PIN_RESET )//����������
		{
			m_key_flag[i] |= 0x01;
		}
		
		if( m_key_flag[i] == 0xff )//�����������������ȶ����£���ʱm_key_flag����0xff,ֱ�������ɿ�
		{
			m_key_counter[i]++;//��¼���������±�־����ֵԽ�󱻰���ʱ��Խ��
			if(m_key_counter[i] == 0) m_key_counter[i] = 1;//����ʱ����������ڱ�����
>>>>>>> dev
			switch(i)
			{
				case KEY1:
				{ 
<<<<<<< HEAD
					key_State[KEY1] = 1;
				}break;
				case KEY2:
				{
					key_State[KEY2] = 1;
=======
					m_key_State[KEY1] = 1;
					if(m_key_counter[KEY1] > 100)//����key1����1s����
					{
						g_power_off_flag = 1;
						PowerOffGpioConfig();
						if(m_key_counter[KEY1] < 130)
							g_BeepFreq = BEEP_FREQ_2000Hz;
					}
				}break;
				case KEY2:
				{
					m_key_State[KEY2] = 1;
>>>>>>> dev
				}break;

				case KEY3:
				{
<<<<<<< HEAD
					key_State[KEY3] = 1;
=======
					m_key_State[KEY3] = 1;
>>>>>>> dev
				}break;

				default:
				{}break;
			}
		}
<<<<<<< HEAD
		else if(key_flag[i] == 0x00)//��������
		{	
			if( key_counter[i] >= 1 )//�൱�����ּ��
=======
		else if(m_key_flag[i] == 0x00)//��������
		{	
			if( m_key_counter[i] >= 1 )//�൱�����ּ��
>>>>>>> dev
			{
				switch(i)
				{
					case KEY1:
					{ 
<<<<<<< HEAD

					}break;
					case KEY2:
					{

=======
						printf("key1 pressed\r\n");
						APP_STATE = NB_INIT;
						if(g_power_off_flag == 1)
							POWER_OFF;	
					}break;
					case KEY2:
					{					
						printf("key2 pressed\r\n");
						APP_STATE = NB_CoAP_SEVER;
						//APP_STATE = NB_SIGN;
>>>>>>> dev
					}break;

					case KEY3:
					{
<<<<<<< HEAD

=======
						printf("key3 pressed\r\n");
						//APP_STATE = NB_TCP_CR;//NB_TCP_ST;
>>>>>>> dev
					}break;
					
					default:
					{}break;
				}	
				
<<<<<<< HEAD
				key_counter[i] = 0;
=======
				m_key_counter[i] = 0;
>>>>>>> dev
			}
		}
	}
	
<<<<<<< HEAD
	if(key_State[KEY2] == 1 || key_State[KEY3] == 1)
	{
		//HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET);
	}
	else
	{
		//HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET);
=======
	if(m_key_State[KEY2] == 1 || m_key_State[KEY3] == 1)
	{

	}
	else
	{

	}

}

/*************************************
fn : ChargeTask

brief : ��س������
1������ʱ��LED5����ָʾ�ƻ�1s��˸һ�Σ��������LED5����
param : none

return : none

*************************************/

void ChargeTask(void)
{
	uint8_t i;
	
	m_chargeState	= HAL_GPIO_ReadPin(GPIOA, CHG_DETECT_PIN);//USB�ӿڲ�����

	if( m_chargeState == GPIO_PIN_SET )////USB����
	{
		m_charge_counter++;
		if(m_charge_counter == 0) m_charge_counter = 1;//��ֹ���ʱ����������ڼӵ����ֵ������
		
		g_USB_insert = YES;//USB�ӿڲ���
  	g_chargeing_flag = YES;//�ڸ���س��
	}
	else
	{
		if( m_charge_counter >= 1 )//USB�γ����
		{ 
			m_charge_counter = 0;
			g_USB_insert = NO;
			g_chargeing_flag = NO;
			m_fullBatFlag = 0;
			m_fullBatHintCnt = 0;
		}
	}

	if(g_USB_insert == YES)//USB����
	{
		if(g_chargeing_flag == YES)//����ڳ��
		{		
			if(m_fullBatTimeCnt < 1200000)//20����
			{
				HAL_GPIO_TogglePin(GPIOB,CHG_LED5_PIN);//�ڳ��ʱ1s��˸һ��
			}	
			else//��ص����ﵽ100%���20���ӣ����һ�ᣬ�õ����ﵽ���
			{
				g_chargeing_flag = NO;
				m_fullBatFlag = 1;
			}

		}
		if(m_fullBatFlag == 1)//��س�����
		{
			CHG_LED5_ON;//�����磬һֱ��

			if(m_fullBatHintCnt <  10)//���������������5����ʾ
			{	
				m_fullBatHintCnt++;
				BeepToggle(BEEP_FREQ_1000Hz);
			}
		}
>>>>>>> dev
	}

}

<<<<<<< HEAD



/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
=======
/*************************************
fn : BatManageTask

brief : ��ع�������
1������ʱ��LED5����ָʾ�ƻ�1s��˸һ�Σ��������LED5����
2���ŵ��ʱ�� ��
		     ��ص��� >= 10% �� LED5Ϩ��
	 7% <= ��ص��� < 10%  �� LED5��5HzƵ����˸�����ҷ�����5HzƵ�ʱ���
		     ��ص��� < 7%   �� ��������1s���Զ��ػ�
param : none

return : none

*************************************/

void BatManageTask(void)
{
	uint16_t start_tick = 0;
	g_BatVoltageLow = 0;
	
	ChargeTask();
	
	g_BatVoltage = GetBatVoltage();
	
	if(g_USB_insert == NO && g_BatVoltage >= 10)//��ص�������10%
	{
		CHG_LED5_OFF;
	}	
	else if(g_USB_insert == NO && g_BatVoltage >= 7)//7%<=��ص���<10%
	{
		g_BatVoltageLow = 1;//�����ͱ�־λ
	}
	else if(g_USB_insert == NO && g_BatVoltage < 7)//��ص���<7%
	{
		g_power_off_flag = 1;
		start_tick = HAL_GetTick();
		while((HAL_GetTick()- start_tick) <2000)
		{
			g_BeepFreq = BEEP_FREQ_2000Hz;
			SetBeepFreq(g_BeepFreq);
		}
			POWER_OFF;
	}
}

void DaBaiSensorTask(void)
{
	
	g_lightValue = getLightValue();
	if(g_lightValue > LIGHT_THRESHOLD || g_Sht20Temp > TEMP_THRESHOLD || g_Sht20RH > RH_THRESHOLD)//��������ֵ�����趨����ֵ
	{
		//g_BeepFreq = BEEP_FREQ_1500Hz;
		//BeepToggle(BEEP_FREQ_2500Hz);		
	}
	g_Sht20Temp = SHT20_Convert(SHT20_ReadTemp(),1);
	g_Sht20RH   = SHT20_Convert(SHT20_ReadRH(),0);
}


/*************************************
fn : DaBai_10msTask

brief : 10ms����
param : none

return : none

*************************************/
void DaBai_10msTask(void)
{
	KeyTask();
}
/*************************************
fn : DaBai_100msTask

brief : 100ms����
param : none

return : none

*************************************/
void DaBai_100msTask(void)
{
	//HAL_GPIO_TogglePin(GPIOB,LED1_PIN);
	if(g_BatVoltageLow == 1)
	{
		BeepToggle(BEEP_FREQ_1000Hz);
		HAL_GPIO_TogglePin(GPIOB,CHG_LED5_PIN);
	}
}

/*************************************
fn : DaBai_500msTask

brief : 500ms����
param : none

return : none

*************************************/

void DaBai_500msTask(void)
{
	HAL_GPIO_TogglePin(GPIOB,LED1_PIN);
	DaBaiSensorTask();
	BatManageTask();//��ع�����������رմ˺����������ز��׹����޷��鿴���״̬
}

/*************************************
fn : DaBai_1000msTask

brief : 1000ms����
param : none

return : none

*************************************/

void DaBai_1000msTask(void)
{

}

/*************************************
fn : DaBai_1MinTask

brief : 60s����
param : none

return : none

*************************************/
void DaBai_1MinTask(void)
{
	APP_STATE = NB_CoAP_ST;
}

/*************************************
fn : DaBai_10MinTask

brief : 60s����
param : none

return : none

*************************************/
void DaBai_10MinTask(void)
{
	APP_STATE = NB_CoAP_ST;
}






//******************************************************************************
// fn : NB_MsgreportCb
//
// brief : NBģ����Ϣ�ϱ��ص�
//
// param : 
//
// return : none
int  NB_MsgreportCb(msg_types_t types,int len,char* msg)
{
  switch(types)
  {
  case MSG_INIT:
    {
      printf("\r\nINIT = %s\r\n",msg);
      if(*msg == 'S')
      {
        LED2_ON;
        APP_STATE = NB_SIGN;
      }
    }
    break;
  case MSG_IMSI:
    {
      printf("\r\nIMSI = %s\r\n",msg);
    }
    break;
  case MSG_REG:
    {
			printf("\r\nNET = %s\r\n",(*msg) == 1 ?"ON":"0FF");
    }
    break;
  case MSG_SIGN:
    {
      printf("\r\n%sdbm\r\n",msg);
			APP_STATE = NB_TCP_CR;
    }
    break;
  case MSG_MODULE_INFO:
    {
      printf("\r\nMinfo = %s\r\n",msg);
    }
    break;
  case MSG_MID:
    {
      printf("\r\nMID = %s\r\n",msg);
    }
    break;
  case MSG_MMODEL:
    {
      printf("\r\nModel = %s\r\n",msg);
    }
    break;
  case MSG_MREV:
    {
      printf("\r\nREV = %s\r\n",msg);
    }
    break;
  case MSG_BAND:
    {
      printf("\r\nFreq = %s\r\n",msg);
    }
    break;
  case MSG_IMEI:
    {
      printf("\r\nIMEI = %s\r\n",msg);
    }
    break;
  case MSG_UDP_CREATE:
    {
      printf("\r\nUDP_CR = %s\r\n",msg);
    }
    break;
  case MSG_UDP_CLOSE:
    {
      printf("\r\nUDP_CL = %s\r\n",msg);
    }
    break;
  case MSG_UDP_SEND:
    {
      printf("\r\nUDP_SEND = %s\r\n",msg);
    }
    break;
  case MSG_UDP_RECE:
    {
      printf("\r\nUDP_RECE = %s\r\n",msg);
    }
    break;
	case MSG_TCP_CREATE:
    {
      printf("\r\nTCP_CR = %s\r\n",msg);
			APP_STATE = NB_TCP_CNT;
    }
    break;
  case MSG_TCP_CLOSE:
    {
      printf("\r\nTCP_CL = %s\r\n",msg);
    }
    break;
		
	case MSG_TCP_CONNECT:
    {
      printf("\r\nTCP_CONNECT = %s\r\n",msg);
			APP_STATE = NB_TCP_ST;
    }
    break;	
		
  case MSG_TCP_SEND:
    {
      printf("\r\nTCP_SEND = %s\r\n",msg);
    }
    break;
  case MSG_TCP_RECE:
    {
      printf("\r\nTCP_RECE = %s\r\n",msg);
			APP_STATE = NB_TCP_CL;
    }
    break;
  case MSG_COAP:
    {
      printf("\r\nCOAP = %s\r\n",msg);
			APP_STATE = NB_CoAP_ST;
    }
    break;
  case MSG_COAP_SEND:
    {
			m_coapSendTimes++;
      printf("\r\nCOAP_SENT = %s ,times = %d\r\n",msg,m_coapSendTimes);
    }
    break;
    
  case MSG_COAP_RECE:
    {
      printf("\r\nCOAP_RECE = %s\r\n",msg);
    }
    break;
  default :
    {
      break;
    }
  }
  return 0;
}


/************************ (C) COPYRIGHT DaBai_IOT *****END OF FILE****/
>>>>>>> dev
