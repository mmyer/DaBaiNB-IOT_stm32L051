/**
  ******************************************************************************
  * File Name          : DABAI_I2C.h
  * Description        : This file provides code for the configuration
  *                      of the DABAI_I2C instances.
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DABAI_I2C_H
#define __DABAI_I2C_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l0xx_hal.h"
<<<<<<< HEAD

=======
>>>>>>> dev



extern I2C_HandleTypeDef hi2c2;

extern void _Error_Handler(char *, int);

void MX_I2C2_Init(void);

/* USER CODE BEGIN Prototypes */
//******************************************************************************
// fn : HAL_I2C_Send
//
// brief : ����ָ������
//
// param : addr -> ���豸��ַ
//         pData -> ����ָ��
//         len  -> ���ݳ���
// 
// return : ״̬��Ϣ
uint8_t HAL_I2C_Send(uint8_t addr , uint8_t *pData, uint16_t len);
//******************************************************************************
// fn : HAL_I2C_Send
//
// brief : ����ָ������
//
// param : addr -> ���豸��ַ
//         pData -> ������ݵ�ַ
//         len  -> ���ݳ���
// 
// return : ״̬��Ϣ
uint8_t HAL_I2C_Read(uint8_t addr, uint8_t *pData, uint16_t len);
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ DABAI_I2C_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
