/**
  ******************************************************************************
  * File Name          : DABAI_ADC.h
  * Description        : This file provides code for the configuration
  *                      of the DABAI_ADC instances.
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DABAI_ADC_H
#define __DABAI_ADC_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l0xx_hal.h"


#define  ADC_DATA_BUFFER_SIZE  20

/*
��ص�ѹΪ4.2Vʱ ADC����ֵΪ 3799
��ص�ѹΪ3.3Vʱ ADC����ֵΪ 2988
����ѹ����3.3Vʱ��ADC��ֵʼ��Ϊ2988����
*/
#define FULL_VOLTAGE  		4.2
#define EMPTY_VOLTAGE		3.0
#define FULL_4V2_ADC_VALUE  3720 //ʵ��3720Ϊ4.1V����ֹ�еĵ�ص�ѹ�䲻�ߣ�APP��ʾ����һֱ�䲻��
 //��ȥ30��Ϊ���� (�ɼ����ĵ�ص�ѹ - FULL_3V3_ADC_VALUE) >0
 //����ѹ����3.3Vʱ��ADC��ֵʼ��Ϊ2988����
#define FULL_3V3_ADC_VALUE  (2988-30)

extern void _Error_Handler(char *, int);
extern ADC_HandleTypeDef    AdcHandle;

void MX_ADC_Init(void);
uint16_t getLightValue(void);
uint8_t GetBatVoltage(void);
#ifdef __cplusplus
}
#endif
#endif /*__ DABAI_ADC_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
