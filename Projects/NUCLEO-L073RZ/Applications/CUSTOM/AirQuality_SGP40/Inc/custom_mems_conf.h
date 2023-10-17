/**
  ******************************************************************************
  * @file    custom_mems_conf.h
  * @author  MEMS Software Solutions Team
  * @brief   This file contains definitions of the MEMS components bus interfaces for custom boards
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef CUSTOM_MEMS_CONF_H
#define CUSTOM_MEMS_CONF_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l0xx_hal.h"
#include "stm32l0xx_nucleo_bus.h"
#include "stm32l0xx_nucleo_errno.h"

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

#define USE_CUSTOM_ENV_SENSOR_LPS22DF_0           1U

#define USE_CUSTOM_ENV_SENSOR_SHT40AD1B_0         1U

#define USE_CUSTOM_ENV_SENSOR_SGP40_0             1U

#define CUSTOM_LPS22DF_0_I2C_Init BSP_I2C1_Init
#define CUSTOM_LPS22DF_0_I2C_DeInit BSP_I2C1_DeInit
#define CUSTOM_LPS22DF_0_I2C_ReadReg BSP_I2C1_ReadReg
#define CUSTOM_LPS22DF_0_I2C_WriteReg BSP_I2C1_WriteReg

#define CUSTOM_SHT40AD1B_0_I2C_Init BSP_I2C1_Init
#define CUSTOM_SHT40AD1B_0_I2C_DeInit BSP_I2C1_DeInit
#define CUSTOM_SHT40AD1B_0_I2C_Read BSP_I2C1_Recv
#define CUSTOM_SHT40AD1B_0_I2C_Write BSP_I2C1_Send

#define CUSTOM_SGP40_0_I2C_Init BSP_I2C1_Init
#define CUSTOM_SGP40_0_I2C_DeInit BSP_I2C1_DeInit
#define CUSTOM_SGP40_0_I2C_Read BSP_I2C1_Recv
#define CUSTOM_SGP40_0_I2C_Write BSP_I2C1_Send

#ifdef __cplusplus
}
#endif

#endif /* CUSTOM_MEMS_CONF_H*/
