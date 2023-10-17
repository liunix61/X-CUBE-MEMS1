/**
  ******************************************************************************
  * @file    motion_pe_manager.h
  * @author  MEMS Software Solutions Team
  * @brief   This file contains definitions for the motion_pe_manager.c file
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
#ifndef MOTION_PE_MANAGER_H
#define MOTION_PE_MANAGER_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "string.h"
#include "motion_pe.h"
#include "main.h"
#include "iks4a1_mems_control.h"
#include "iks4a1_mems_control_ex.h"

/* Extern variables ----------------------------------------------------------*/
/* Exported Macros -----------------------------------------------------------*/
/* Exported Types ------------------------------------------------------------*/
/* Imported Variables --------------------------------------------------------*/
/* Exported Functions Prototypes ---------------------------------------------*/
void MotionPE_manager_init(void);
void MotionPE_manager_run(MPE_input_t *data_in, MPE_output_t *data_out);
void MotionPE_manager_get_version(char *version, int *length);

#ifdef __cplusplus
}
#endif

#endif /* MOTION_PE_MANAGER_H */
