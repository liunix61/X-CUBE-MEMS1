/**
  ******************************************************************************
  * @file           : sensor_commands.c
  * @brief          : Handle commands for sensor
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Software License Agreement
  * SLA0077, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0077
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "com.h"
#include "demo_serial.h"
#include "iks01a3_env_sensors.h"
#include "iks01a3_env_sensors_ex.h"
#include "iks01a3_motion_sensors.h"
#include "iks01a3_motion_sensors_ex.h"
#include "iks01a3_hybrid_sensors.h"
#include "iks01a3_hybrid_sensors_ex.h"
#include "nucleo_f401re_errno.h"
#include "sensor_commands.h"
#include "string.h"

/** @addtogroup X_NUCLEO_IKS01A3_Examples X_NUCLEO_IKS01A3 Examples
 * @{
 */

/** @addtogroup DATALOG_EXTENDED DATALOG EXTENDED
 * @{
 */

/* Private typedef -----------------------------------------------------------*/

/* Private defines -----------------------------------------------------------*/
#define SENSOR_NAME_MAX_LENGTH  19

/* Private macro -------------------------------------------------------------*/

/* Public variables ----------------------------------------------------------*/
/* Currently selected sensor instances (defaults) */
uint32_t AccInstance = IKS01A3_LSM6DSO_0;
uint32_t GyrInstance = IKS01A3_LSM6DSO_0;
uint32_t MagInstance = IKS01A3_LIS2MDL_0;
uint32_t HumInstance = IKS01A3_HTS221_0;
uint32_t TmpInstance = IKS01A3_HTS221_0;
uint32_t PrsInstance = IKS01A3_LPS22HH_0;

uint32_t IsHybridAccSensor = NOT_HYBRID_SENSOR;
uint32_t IsHybridTmpSensor = NOT_HYBRID_SENSOR;

/* Private variables ---------------------------------------------------------*/
/* Supported sensor names. Please verify that second index of array is HIGHER than longest string in array!!! */
static uint8_t AccNameList[][SENSOR_NAME_MAX_LENGTH] = {"LSM6DSO", "LIS2DW12", "ASM330LHH (DIL24)", "IIS2DLPC (DIL24)",
                                                        "ISM303DAC (DIL24)", "ISM330DLC (DIL24)", "LIS2DH12 (DIL24)",
                                                        "LSM6DSOX (DIL24)", "AIS2DW12 (DIL24)", "LSM6DSR (DIL24)",
                                                        "AIS328DQ (DIL24)", "AIS3624DQ (DIL24)", "H3LIS331DL (DIL24)",
                                                        "LSM6DSRX (DIL24)", "ISM330DHCX (DIL24)", "LSM6DSO32 (DIL24)",
                                                        "IIS2ICLX (DIL24)", "AIS2IH (DIL24)", "LSM6DSO32X (DIL24)",
                                                        "LIS2DTW12 (DIL24)"};
static uint8_t GyrNameList[][SENSOR_NAME_MAX_LENGTH] = {"LSM6DSO", "ASM330LHH (DIL24)", "ISM330DLC (DIL24)",
                                                        "LSM6DSOX (DIL24)", "LSM6DSR (DIL24)", "A3G4250D (DIL24)",
                                                        "LSM6DSRX (DIL24)", "ISM330DHCX (DIL24)", "LSM6DSO32 (DIL24)",
                                                        "LSM6DSO32X (DIL24)"};
static uint8_t MagNameList[][SENSOR_NAME_MAX_LENGTH] = {"LIS2MDL", "IIS2MDC (DIL24)", "ISM303DAC (DIL24)", "LIS3MDL (DIL24)",
                                                        "LIS2MDL SH (DIL24)"};
static uint8_t HumNameList[][SENSOR_NAME_MAX_LENGTH] = {"HTS221"};
static uint8_t TmpNameList[][SENSOR_NAME_MAX_LENGTH] = {"HTS221", "STTS751", "LPS22HH", "LPS33HW (DIL24)", "STTS22H (DIL24)",
                                                        "LPS33K (DIL24)", "LPS22CH (DIL24)", "LPS27HHTW (DIL24)", "LPS22DF (DIL24)",
                                                        "LIS2DTW12 (DIL24)"};
static uint8_t PrsNameList[][SENSOR_NAME_MAX_LENGTH] = {"LPS22HH", "LPS33HW (DIL24)", "LPS33K (DIL24)", "LPS22CH (DIL24)",
                                                        "LPS27HHTW (DIL24)", "LPS22DF (DIL24)"};

/* Supported sensor instances (have to correspond with supported sensor names above) */
static uint32_t AccInstanceList[] = {
  IKS01A3_LSM6DSO_0,
  IKS01A3_LIS2DW12_0,
  IKS01A3_ASM330LHH_0,
  IKS01A3_IIS2DLPC_0,
  IKS01A3_ISM303DAC_ACC_0,
  IKS01A3_ISM330DLC_0,
  IKS01A3_LIS2DH12_0,
  IKS01A3_LSM6DSOX_0,
  IKS01A3_AIS2DW12_0,
  IKS01A3_LSM6DSR_0,
  IKS01A3_AIS328DQ_0,
  IKS01A3_AIS3624DQ_0,
  IKS01A3_H3LIS331DL_0,
  IKS01A3_LSM6DSRX_0,
  IKS01A3_ISM330DHCX_0,
  IKS01A3_LSM6DSO32_0,
  IKS01A3_IIS2ICLX_0,
  IKS01A3_AIS2IH_0,
  IKS01A3_LSM6DSO32X_0,
  IKS01A3_LIS2DTW12_0,
};
static uint32_t GyrInstanceList[] = {
  IKS01A3_LSM6DSO_0,
  IKS01A3_ASM330LHH_0,
  IKS01A3_ISM330DLC_0,
  IKS01A3_LSM6DSOX_0,
  IKS01A3_LSM6DSR_0,
  IKS01A3_A3G4250D_0,
  IKS01A3_LSM6DSRX_0,
  IKS01A3_ISM330DHCX_0,
  IKS01A3_LSM6DSO32_0,
  IKS01A3_LSM6DSO32X_0,
};
static uint32_t MagInstanceList[] = {
  IKS01A3_LIS2MDL_0,
  IKS01A3_IIS2MDC_0,
  IKS01A3_ISM303DAC_MAG_0,
  IKS01A3_LIS3MDL_0,
  IKS01A3_LSM6DSOX_SENSORHUB_LIS2MDL_0,
};
static uint32_t HumInstanceList[] = {
  IKS01A3_HTS221_0,
};
static uint32_t TmpInstanceList[] = {
  IKS01A3_HTS221_0,
  IKS01A3_STTS751_0,
  IKS01A3_LPS22HH_0,
  IKS01A3_LPS33HW_0,
  IKS01A3_STTS22H_0,
  IKS01A3_LPS33K_0,
  IKS01A3_LPS22CH_0,
  IKS01A3_LPS27HHTW_0,
  IKS01A3_LPS22DF_0,
  IKS01A3_LIS2DTW12_0,
};
static uint32_t PrsInstanceList[] = {
  IKS01A3_LPS22HH_0,
  IKS01A3_LPS33HW_0,
  IKS01A3_LPS33K_0,
  IKS01A3_LPS22CH_0,
  IKS01A3_LPS27HHTW_0,
  IKS01A3_LPS22DF_0,
};
static uint32_t HybridAccInstanceList[] = {
  NOT_HYBRID_SENSOR, /* LSM6DSO */
  NOT_HYBRID_SENSOR, /* LIS2DW12 */
  NOT_HYBRID_SENSOR, /* ASM330LHH */
  NOT_HYBRID_SENSOR, /* IIS2DLPC */
  NOT_HYBRID_SENSOR, /* ISM303DAC_ACC */
  NOT_HYBRID_SENSOR, /* ISM330DLC */
  NOT_HYBRID_SENSOR, /* LIS2DH12 */
  NOT_HYBRID_SENSOR, /* LSM6DSOX */
  NOT_HYBRID_SENSOR, /* AIS2DW12 */
  NOT_HYBRID_SENSOR, /* LSM6DSR */
  NOT_HYBRID_SENSOR, /* AIS328DQ */
  NOT_HYBRID_SENSOR, /* AIS3624DQ */
  NOT_HYBRID_SENSOR, /* H3LIS331DL */
  NOT_HYBRID_SENSOR, /* LSM6DSRX */
  NOT_HYBRID_SENSOR, /* ISM330DHCX */
  NOT_HYBRID_SENSOR, /* LSM6DSO32 */
  NOT_HYBRID_SENSOR, /* IIS2ICLX */
  NOT_HYBRID_SENSOR, /* AIS2IH */
  NOT_HYBRID_SENSOR, /* LSM6DSO32X */
  HYBRID_SENSOR,     /* LIS2DTW12 */
};
static uint32_t HybridTmpInstanceList[] = {
  NOT_HYBRID_SENSOR, /* HTS221 */
  NOT_HYBRID_SENSOR, /* STTS751 */
  NOT_HYBRID_SENSOR, /* LPS22HH */
  NOT_HYBRID_SENSOR, /* LPS33HW */
  NOT_HYBRID_SENSOR, /* STTS22H */
  NOT_HYBRID_SENSOR, /* LPS33K */
  NOT_HYBRID_SENSOR, /* LPS22CH */
  NOT_HYBRID_SENSOR, /* LPS27HHTW */
  NOT_HYBRID_SENSOR, /* LPS22DF */
  HYBRID_SENSOR,     /* LIS2DTW12 */
};

/* Sensor fullscale lists (have to correspond with supported sensor names above)
 * Please verify that second index of array is equal to or higher than count of longest sub-array items */
static uint32_t AccFsList[][5] = { /* g */
  {4, 2, 4, 8, 16},                /* LSM6DSO */
  {4, 2, 4, 8, 16},                /* LIS2DW12 */
  {4, 2, 4, 8, 16},                /* ASM330LHH */
  {4, 2, 4, 8, 16},                /* IIS2DLPC */
  {4, 2, 4, 8, 16},                /* ISM303DAC */
  {4, 2, 4, 8, 16},                /* ISM330DLC */
  {4, 2, 4, 8, 16},                /* LIS2DH12 */
  {4, 2, 4, 8, 16},                /* LSM6DSOX */
  {2, 2, 4},                       /* AIS2DW12 */
  {4, 2, 4, 8, 16},                /* LSM6DSR */
  {3, 2, 4, 8},                    /* AIS328DQ */
  {3, 6, 12, 24},                  /* AIS3624DQ */
  {3, 100, 200, 400},              /* H3LIS331DL */
  {4, 2, 4, 8, 16},                /* LSM6DSRX */
  {4, 2, 4, 8, 16},                /* ISM330DHCX */
  {4, 4, 8, 16, 32},               /* LSM6DSO32 */
  {4, 500, 1000, 2000, 3000},      /* IIS2ICLX [mg] */
  {4, 2, 4, 8, 16},                /* AIS2IH */
  {4, 4, 8, 16, 32},               /* LSM6DSO32X */
  {4, 2, 4, 8, 16},                /* LIS2DTW12 */
};
static uint32_t GyrFsList[][7] = {      /* dps */
  {5, 125, 250, 500, 1000, 2000},       /* LSM6DSO */
  {6, 125, 250, 500, 1000, 2000, 4000}, /* ASM330LHH */
  {5, 125, 250, 500, 1000, 2000},       /* ISM330DLC */
  {5, 125, 250, 500, 1000, 2000},       /* LSM6DSOX */
  {6, 125, 250, 500, 1000, 2000, 4000}, /* LSM6DSR */
  {1, 245},                             /* A3G4250D */
  {6, 125, 250, 500, 1000, 2000, 4000}, /* LSM6DSRX */
  {6, 125, 250, 500, 1000, 2000, 4000}, /* ISM330DHCX */
  {5, 125, 250, 500, 1000, 2000},       /* LSM6DSO32 */
  {5, 125, 250, 500, 1000, 2000},       /* LSM6DSO32X */
};
static uint32_t MagFsList[][5] = { /* Ga */
  {1, 50},                         /* LIS2MDL */
  {1, 50},                         /* IIS2MDC */
  {1, 50},                         /* ISM303DAC */
  {4, 4, 8, 12, 16},               /* LIS3MDL */
  {1, 50},                         /* LSM6DSOX SENSORHUB LIS2MDL */
};
static uint32_t HumFsList[][1] = { /* % */
  {0},                             /* HTS221 */
};
static uint32_t TmpFsList[][1] = { /* C */
  {0},                             /* HTS221 */
  {0},                             /* STTS751 */
  {0},                             /* LPS22HH */
  {0},                             /* LPS33HW */
  {0},                             /* STTS22H */
  {0},                             /* LPS33K */
  {0},                             /* LPS22CH */
  {0},                             /* LPS27HHTW */
  {0},                             /* LPS22DF */
  {0},                             /* LIS2DTW12 */
};
static uint32_t PrsFsList[][1] = { /* Pa */
  {0},                             /* LPS22HH */
  {0},                             /* LPS33HW */
  {0},                             /* LPS33K */
  {0},                             /* LPS22CH */
  {0},                             /* LPS27HHTW */
  {0},                             /* LPS22DF */
};

/* Sensor output data rate lists (have to correspond with supported sensor names above)
 * Please verify that second index of array is equal to or higher than count of longest sub-array items */
static float AccOdrList[][12] = {                                /* Hz */
  {10, 12.5, 26, 52, 104, 208, 417, 833, 1667, 3333, 6667},      /* LSM6DSO */
  {8, 12.5, 25, 50, 100, 200, 400, 800, 1600},                   /* LIS2DW12 */
  {10, 12.5, 26, 52, 104, 208, 417, 833, 1667, 3333, 6667},      /* ASM330LHH */
  {8, 12.5, 25, 50, 100, 200, 400, 800, 1600},                   /* IIS2DLPC */
  {11, 1, 12.5, 25, 50, 100, 200, 400, 800, 1600, 3200, 6400},   /* ISM303DAC */
  {10, 12.5, 26, 52, 104, 208, 416, 833, 1660, 3330, 6660},      /* ISM330DLC */
  {10, 1, 10, 25, 50, 100, 200, 400, 1344, 1620, 5376},          /* LIS2DH12 */
  {10, 12.5, 26, 52, 104, 208, 417, 833, 1667, 3333, 6667},      /* LSM6DSOX */
  {5, 1.6, 12.5, 25, 50, 100},                                   /* AIS2DW12 */
  {10, 12.5, 26, 52, 104, 208, 417, 833, 1667, 3333, 6667},      /* LSM6DSR */
  {9, 0.5, 1, 2, 5, 10, 50, 100, 400, 1000},                     /* AIS328DQ */
  {9, 0.5, 1, 2, 5, 10, 50, 100, 400, 1000},                     /* AIS3624DQ */
  {9, 0.5, 1, 2, 5, 10, 50, 100, 400, 1000},                     /* H3LIS331DL */
  {10, 12.5, 26, 52, 104, 208, 417, 833, 1667, 3333, 6667},      /* LSM6DSRX */
  {10, 12.5, 26, 52, 104, 208, 417, 833, 1667, 3333, 6667},      /* ISM330DHCX */
  {10, 12.5, 26, 52, 104, 208, 417, 833, 1667, 3333, 6667},      /* LSM6DSO32 */
  {7, 12.5, 26, 52, 104, 208, 416, 833},                         /* IIS2ICLX */
  {9, 1.6, 12.5, 25, 50, 100, 200, 400, 800, 1600},              /* AIS2IH */
  {11, 1.6, 12.5, 26, 52, 104, 208, 417, 833, 1667, 3333, 6667}, /* LSM6DSO32X */
  {8, 12.5, 25, 50, 100, 200, 400, 800, 1600},                   /* LIS2DTW12 */
};
static float GyrOdrList[][11] = {                           /* Hz */
  {10, 12.5, 26, 52, 104, 208, 417, 833, 1667, 3333, 6667}, /* LSM6DSO */
  {10, 12.5, 26, 52, 104, 208, 417, 833, 1667, 3333, 6667}, /* ASM330LHH */
  {10, 12.5, 26, 52, 104, 208, 416, 833, 1660, 3330, 6660}, /* ISM330DLC */
  {10, 12.5, 26, 52, 104, 208, 417, 833, 1667, 3333, 6667}, /* LSM6DSOX */
  {10, 12.5, 26, 52, 104, 208, 417, 833, 1667, 3333, 6667}, /* LSM6DSR */
  {4, 105, 208, 420, 840},                                  /* A3G4250D */
  {10, 12.5, 26, 52, 104, 208, 417, 833, 1667, 3333, 6667}, /* LSM6DSRX */
  {10, 12.5, 26, 52, 104, 208, 417, 833, 1667, 3333, 6667}, /* ISM330DHCX */
  {10, 12.5, 26, 52, 104, 208, 417, 833, 1667, 3333, 6667}, /* LSM6DSO32 */
  {10, 12.5, 26, 52, 104, 208, 417, 833, 1667, 3333, 6667}, /* LSM6DSO32X */
};
static float MagOdrList[][9] = {            /* Hz */
  {4, 10, 20, 50, 100},                     /* LIS2MDL */
  {4, 10, 20, 50, 100},                     /* IIS2MDC */
  {4, 10, 20, 50, 100},                     /* ISM303DAC */
  {8, 0.625, 1.25, 2.5, 5, 10, 20, 40, 80}, /* LIS3MDL */
  {4, 10, 20, 50, 100},                     /* LSM6DSOX SENSORHUB LIS2MDL */
};
static float HumOdrList[][4] = { /* Hz */
  {3, 1, 7, 12.5}, /* HTS221 */
};
static float TmpOdrList[][11] = {                     /* Hz */
  {3, 1, 7, 12.5},                                    /* HTS221 */
  {10, 0.0625, 0.125, 0.25, 0.5, 1, 2, 4, 8, 16, 32}, /* STTS751 */
  {7, 1, 10, 25, 50, 75, 100, 200},                   /* LPS22HH */
  {5, 1, 10, 25, 50, 75},                             /* LPS33HW */
  {5, 1, 25, 50, 100, 200},                           /* STTS22H */
  {5, 1, 10, 25, 50, 75},                             /* LPS33K */
  {7, 1, 10, 25, 50, 75, 100, 200},                   /* LPS22CH */
  {7, 1, 10, 25, 50, 75, 100, 200},                   /* LPS27HHTW */
  {8, 1, 4, 10, 25, 50, 75, 100, 200},                /* LPS22DF */
  {4, 1.6, 12.5, 25, 50},                             /* LIS2DTW12 */
};
static float PrsOdrList[][9] = {       /* Hz */
  {7, 1, 10, 25, 50, 75, 100, 200},    /* LPS22HH */
  {5, 1, 10, 25, 50, 75},              /* LPS33HW */
  {5, 1, 10, 25, 50, 75},              /* LPS33K */
  {7, 1, 10, 25, 50, 75, 100, 200},    /* LPS22CH */
  {7, 1, 10, 25, 50, 75, 100, 200},    /* LPS27HHTW */
  {8, 1, 4, 10, 25, 50, 75, 100, 200}, /* LPS22DF */
};

/* Supported sensor names of same kind in one string (comma separated) */
static uint8_t AccNamesString[sizeof(AccNameList) + sizeof(AccNameList) / SENSOR_NAME_MAX_LENGTH];
static uint8_t GyrNamesString[sizeof(GyrNameList) + sizeof(GyrNameList) / SENSOR_NAME_MAX_LENGTH];
static uint8_t MagNamesString[sizeof(MagNameList) + sizeof(MagNameList) / SENSOR_NAME_MAX_LENGTH];
static uint8_t TmpNamesString[sizeof(TmpNameList) + sizeof(TmpNameList) / SENSOR_NAME_MAX_LENGTH];
static uint8_t HumNamesString[sizeof(HumNameList) + sizeof(HumNameList) / SENSOR_NAME_MAX_LENGTH];
static uint8_t PrsNamesString[sizeof(PrsNameList) + sizeof(PrsNameList) / SENSOR_NAME_MAX_LENGTH];

/* Currently selected sensor indexes */
static uint8_t AccIndex;
static uint8_t GyrIndex;
static uint8_t MagIndex;
static uint8_t HumIndex;
static uint8_t TmpIndex;
static uint8_t PrsIndex;

/* Private function prototypes -----------------------------------------------*/
static int SC_Get_Sensor_Name(TMsg *Msg);
static int SC_Read_Register(TMsg *Msg);
static int SC_Write_Register(TMsg *Msg);
static int SC_Get_Full_Scale_List(TMsg *Msg);
static int SC_Set_Full_Scale(TMsg *Msg);
static int SC_Get_Full_Scale(TMsg *Msg);
static int SC_Get_ODR_List(TMsg *Msg);
static int SC_Set_ODR(TMsg *Msg);
static int SC_Get_ODR(TMsg *Msg);
static int SC_Get_Sensor_List(TMsg *Msg);
static int SC_Set_Sensor_Index(TMsg *Msg);

static void Send_Sensor_Name(TMsg *Msg, uint8_t *SensorName);
static void Send_Sensor_FS_List(TMsg *Msg, uint32_t *FsList);
static void Send_Sensor_ODR_List(TMsg *Msg, float *OdrList);

static int Is_DIL24_Sensor(uint8_t *SensorName);
static void DIL24_INT1_Init(void);

static void FloatToArray(uint8_t *Dest, float data);
static void ArrayToFloat(uint8_t *Source, float *data);

/* Public functions ----------------------------------------------------------*/
/**
 * @brief  Handle Sensors command
 * @param  Msg the pointer to the message to be handled
 * @retval 1 if the message is correctly handled, 0 otherwise
 */
int Handle_Sensor_command(TMsg *Msg)
{
  int ret;

  /* Commands */
  switch (Msg->Data[3])
  {
    case SC_GET_SENSOR_NAME:
      ret = SC_Get_Sensor_Name(Msg);
      break;

    case SC_READ_REGISTER:
      ret = SC_Read_Register(Msg);
      break;

    case SC_WRITE_REGISTER:
      ret = SC_Write_Register(Msg);
      break;

    case SC_GET_FULL_SCALE_LIST:
      ret = SC_Get_Full_Scale_List(Msg);
      break;

    case SC_SET_FULL_SCALE:
      ret = SC_Set_Full_Scale(Msg);
      break;

    case SC_GET_FULL_SCALE:
      ret = SC_Get_Full_Scale(Msg);
      break;

    case SC_GET_ODR_LIST:
      ret = SC_Get_ODR_List(Msg);
      break;

    case SC_SET_ODR:
      ret = SC_Set_ODR(Msg);
      break;

    case SC_GET_ODR:
      ret = SC_Get_ODR(Msg);
      break;

    case SC_GET_SENSOR_LIST:
      ret = SC_Get_Sensor_List(Msg);
      break;

    case SC_SET_SENSOR_INDEX:
      ret = SC_Set_Sensor_Index(Msg);
      break;

    default:
      ret = 0;
      break;
  }

  return ret;
}

/* Private functions ---------------------------------------------------------*/
/**
 * @brief  Get sensor name
 * @param  Msg the pointer to the message to be handled
 * @retval 1 if the message is correctly handled, 0 otherwise
 */
static int SC_Get_Sensor_Name(TMsg *Msg)
{
  int ret = 1;

  /* Sensor Type */
  switch (Msg->Data[4])
  {
    case SC_ACCELEROMETER:
      Send_Sensor_Name(Msg, AccNameList[AccIndex]);
      break;

    case SC_GYROSCOPE:
      Send_Sensor_Name(Msg, GyrNameList[GyrIndex]);
      break;

    case SC_MAGNETOMETER:
      Send_Sensor_Name(Msg, MagNameList[MagIndex]);
      break;

    case SC_TEMPERATURE:
      Send_Sensor_Name(Msg, TmpNameList[TmpIndex]);
      break;

    case SC_HUMIDITY:
      Send_Sensor_Name(Msg, HumNameList[HumIndex]);
      break;

    case SC_PRESSURE:
      Send_Sensor_Name(Msg, PrsNameList[PrsIndex]);
      break;

    default:
      ret = 0;
      break;
  }

  return ret;
}

/**
 * @brief  Read sensor register
 * @param  Msg the pointer to the message to be handled
 * @retval 1 if the message is correctly handled, 0 otherwise
 */
static int SC_Read_Register(TMsg *Msg)
{
  uint8_t reg_value = 0;
  int ret = 1;

  /* Sensor Type */
  switch (Msg->Data[4])
  {
    case SC_ACCELEROMETER:
      if (IsHybridAccSensor == HYBRID_SENSOR && AccInstance == IKS01A3_LIS2DTW12_0)
      {
        if (IKS01A3_HYBRID_SENSOR_Read_Register(AccInstance, Msg->Data[5], &reg_value) != BSP_ERROR_NONE)
        {
          return 0;
        }
      }
      else
      {
        if (IKS01A3_MOTION_SENSOR_Read_Register(AccInstance, Msg->Data[5], &reg_value) != BSP_ERROR_NONE)
        {
          return 0;
        }
      }
      break;

    case SC_GYROSCOPE:
      if (IKS01A3_MOTION_SENSOR_Read_Register(GyrInstance, Msg->Data[5], &reg_value) != BSP_ERROR_NONE)
      {
        return 0;
      }
      break;

    case SC_MAGNETOMETER:
      if (IKS01A3_MOTION_SENSOR_Read_Register(MagInstance, Msg->Data[5], &reg_value) != BSP_ERROR_NONE)
      {
        return 0;
      }
      break;

    case SC_TEMPERATURE:
      if (IsHybridTmpSensor == HYBRID_SENSOR && TmpInstance == IKS01A3_LIS2DTW12_0)
      {
        if (IKS01A3_HYBRID_SENSOR_Read_Register(TmpInstance, Msg->Data[5], &reg_value) != BSP_ERROR_NONE)
        {
          return 0;
        }
      }
      else
      {
        if (IKS01A3_ENV_SENSOR_Read_Register(TmpInstance, Msg->Data[5], &reg_value) != BSP_ERROR_NONE)
        {
          return 0;
        }
      }
      break;

    case SC_HUMIDITY:
      if (IKS01A3_ENV_SENSOR_Read_Register(HumInstance, Msg->Data[5], &reg_value) != BSP_ERROR_NONE)
      {
        return 0;
      }
      break;

    case SC_PRESSURE:
      if (IKS01A3_ENV_SENSOR_Read_Register(PrsInstance, Msg->Data[5], &reg_value) != BSP_ERROR_NONE)
      {
        return 0;
      }
      break;

    default:
      ret = 0;
      break;
  }

  if (ret == 0)
  {
    return ret;
  }

  BUILD_REPLY_HEADER(Msg);
  Msg->Data[6] = reg_value;
  Msg->Len = 7;
  UART_SendMsg(Msg);
  return ret;
}

/**
 * @brief  Write to sensor register
 * @param  Msg the pointer to the message to be handled
 * @retval 1 if the message is correctly handled, 0 otherwise
 */
static int SC_Write_Register(TMsg *Msg)
{
  int ret = 1;

  /* Sensor Type */
  switch (Msg->Data[4])
  {
    case SC_ACCELEROMETER:
      if (IsHybridAccSensor == HYBRID_SENSOR && AccInstance == IKS01A3_LIS2DTW12_0)
      {
        if (IKS01A3_HYBRID_SENSOR_Write_Register(AccInstance, Msg->Data[5], Msg->Data[6]) != BSP_ERROR_NONE)
        {
          return 0;
        }
      }
      else
      {
        if (IKS01A3_MOTION_SENSOR_Write_Register(AccInstance, Msg->Data[5], Msg->Data[6]) != BSP_ERROR_NONE)
        {
          return 0;
        }
      }
      break;

    case SC_GYROSCOPE:
      if (IKS01A3_MOTION_SENSOR_Write_Register(GyrInstance, Msg->Data[5], Msg->Data[6]) != BSP_ERROR_NONE)
      {
        return 0;
      }
      break;

    case SC_MAGNETOMETER:
      if (IKS01A3_MOTION_SENSOR_Write_Register(MagInstance, Msg->Data[5], Msg->Data[6]) != BSP_ERROR_NONE)
      {
        return 0;
      }
      break;

    case SC_TEMPERATURE:
      if (IsHybridTmpSensor == HYBRID_SENSOR && TmpInstance == IKS01A3_LIS2DTW12_0)
      {
        if (IKS01A3_HYBRID_SENSOR_Write_Register(TmpInstance, Msg->Data[5], Msg->Data[6]) != BSP_ERROR_NONE)
        {
          return 0;
        }
      }
      else
      {
        if (IKS01A3_ENV_SENSOR_Write_Register(TmpInstance, Msg->Data[5], Msg->Data[6]) != BSP_ERROR_NONE)
        {
          return 0;
        }
      }
      break;

    case SC_HUMIDITY:
      if (IKS01A3_ENV_SENSOR_Write_Register(HumInstance, Msg->Data[5], Msg->Data[6]) != BSP_ERROR_NONE)
      {
        return 0;
      }
      break;

    case SC_PRESSURE:
      if (IKS01A3_ENV_SENSOR_Write_Register(PrsInstance, Msg->Data[5], Msg->Data[6]) != BSP_ERROR_NONE)
      {
        return 0;
      }
      break;

    default:
      ret = 0;
      break;
  }

  if (ret == 0)
  {
    return ret;
  }

  BUILD_REPLY_HEADER(Msg);
  Msg->Len = 7;
  UART_SendMsg(Msg);
  return ret;
}

/**
 * @brief  Get sensor full scale list
 * @param  Msg the pointer to the message to be handled
 * @retval 1 if the message is correctly handled, 0 otherwise
 */
static int SC_Get_Full_Scale_List(TMsg *Msg)
{
  int ret = 1;

  /* Sensor Type */
  switch (Msg->Data[4])
  {
    case SC_ACCELEROMETER:
      Send_Sensor_FS_List(Msg, AccFsList[AccIndex]);
      break;

    case SC_GYROSCOPE:
      Send_Sensor_FS_List(Msg, GyrFsList[GyrIndex]);
      break;

    case SC_MAGNETOMETER:
      Send_Sensor_FS_List(Msg, MagFsList[MagIndex]);
      break;

    case SC_TEMPERATURE:
      Send_Sensor_FS_List(Msg, TmpFsList[TmpIndex]);
      break;

    case SC_HUMIDITY:
      Send_Sensor_FS_List(Msg, HumFsList[HumIndex]);
      break;

    case SC_PRESSURE:
      Send_Sensor_FS_List(Msg, PrsFsList[PrsIndex]);
      break;

    default:
      ret = 0;
      break;
  }

  return ret;
}

/**
 * @brief  Set sensor full scale
 * @param  Msg the pointer to the message to be handled
 * @retval 1 if the message is correctly handled, 0 otherwise
 */
static int SC_Set_Full_Scale(TMsg *Msg)
{
  uint32_t full_scale = Deserialize(&Msg->Data[5], 4);
  int ret = 1;

  /* Sensor Type */
  switch (Msg->Data[4])
  {
    case SC_ACCELEROMETER:
      if (IsHybridAccSensor == HYBRID_SENSOR && AccInstance == IKS01A3_LIS2DTW12_0)
      {
        if (IKS01A3_HYBRID_MOTION_SENSOR_SetFullScale(AccInstance, HYBRID_ACCELERO, (int32_t)full_scale) != BSP_ERROR_NONE)
        {
          return 0;
        }
      }
      else
      {
        if (IKS01A3_MOTION_SENSOR_SetFullScale(AccInstance, MOTION_ACCELERO, (int32_t)full_scale) != BSP_ERROR_NONE)
        {
          return 0;
        }
      }
      break;

    case SC_GYROSCOPE:
      if (IKS01A3_MOTION_SENSOR_SetFullScale(GyrInstance, MOTION_GYRO, (int32_t)full_scale) != BSP_ERROR_NONE)
      {
        return 0;
      }
      break;

    case SC_MAGNETOMETER:
      if (IKS01A3_MOTION_SENSOR_SetFullScale(MagInstance, MOTION_MAGNETO, (int32_t)full_scale) != BSP_ERROR_NONE)
      {
        return 0;
      }
      break;

    case SC_TEMPERATURE:
    case SC_HUMIDITY:
    case SC_PRESSURE:
      break;

    default:
      ret = 0;
      break;
  }

  if (ret == 0)
  {
    return ret;
  }

  BUILD_REPLY_HEADER(Msg);
  Msg->Len = 9;
  UART_SendMsg(Msg);
  return ret;
}

/**
 * @brief  Get sensor full scale
 * @param  Msg the pointer to the message to be handled
 * @retval 1 if the message is correctly handled, 0 otherwise
 */
static int SC_Get_Full_Scale(TMsg *Msg)
{
  int32_t full_scale = 0;
  int ret = 1;

  /* Sensor Type */
  switch (Msg->Data[4])
  {
    case SC_ACCELEROMETER:
      if (IsHybridAccSensor == HYBRID_SENSOR && AccInstance == IKS01A3_LIS2DTW12_0)
      {
        if (IKS01A3_HYBRID_MOTION_SENSOR_GetFullScale(AccInstance, HYBRID_ACCELERO, &full_scale) != BSP_ERROR_NONE)
        {
          return 0;
        }
      }
      else
      {
        if (IKS01A3_MOTION_SENSOR_GetFullScale(AccInstance, MOTION_ACCELERO, &full_scale) != BSP_ERROR_NONE)
        {
          return 0;
        }
      }
      break;

    case SC_GYROSCOPE:
      if (IKS01A3_MOTION_SENSOR_GetFullScale(GyrInstance, MOTION_GYRO, &full_scale) != BSP_ERROR_NONE)
      {
        return 0;
      }
      break;

    case SC_MAGNETOMETER:
      if (IKS01A3_MOTION_SENSOR_GetFullScale(MagInstance, MOTION_MAGNETO, &full_scale) != BSP_ERROR_NONE)
      {
        return 0;
      }
      break;

    case SC_TEMPERATURE:
    case SC_HUMIDITY:
    case SC_PRESSURE:
      break;

    default:
      ret = 0;
      break;
  }

  if (ret == 0)
  {
    return ret;
  }

  BUILD_REPLY_HEADER(Msg);
  Serialize(&Msg->Data[5], full_scale, 4);
  Msg->Len = 9;
  UART_SendMsg(Msg);
  return ret;
}

/**
 * @brief  Get sensor output data rate list
 * @param  Msg the pointer to the message to be handled
 * @retval 1 if the message is correctly handled, 0 otherwise
 */
static int SC_Get_ODR_List(TMsg *Msg)
{
  int ret = 1;

  /* Sensor Type */
  switch (Msg->Data[4])
  {
    case SC_ACCELEROMETER:
      Send_Sensor_ODR_List(Msg, AccOdrList[AccIndex]);
      break;

    case SC_GYROSCOPE:
      Send_Sensor_ODR_List(Msg, GyrOdrList[GyrIndex]);
      break;

    case SC_MAGNETOMETER:
      Send_Sensor_ODR_List(Msg, MagOdrList[MagIndex]);
      break;

    case SC_TEMPERATURE:
      Send_Sensor_ODR_List(Msg, TmpOdrList[TmpIndex]);
      break;

    case SC_HUMIDITY:
      Send_Sensor_ODR_List(Msg, HumOdrList[HumIndex]);
      break;

    case SC_PRESSURE:
      Send_Sensor_ODR_List(Msg, PrsOdrList[PrsIndex]);
      break;

    default:
      ret = 0;
      break;
  }

  return ret;
}

/**
 * @brief  Set sensor output data rate
 * @param  Msg the pointer to the message to be handled
 * @retval 1 if the message is correctly handled, 0 otherwise
 */
static int SC_Set_ODR(TMsg *Msg)
{
  float odr = 0.0f;
  int ret = 1;

  ArrayToFloat(&Msg->Data[5], &odr);

  /* Sensor Type */
  switch (Msg->Data[4])
  {
    case SC_ACCELEROMETER:
      if (IsHybridAccSensor == HYBRID_SENSOR && AccInstance == IKS01A3_LIS2DTW12_0)
      {
        if (IKS01A3_HYBRID_MOTION_SENSOR_SetOutputDataRate(AccInstance, HYBRID_ACCELERO, odr) != BSP_ERROR_NONE)
        {
          return 0;
        }
      }
      else
      {
        if (IKS01A3_MOTION_SENSOR_SetOutputDataRate(AccInstance, MOTION_ACCELERO, odr) != BSP_ERROR_NONE)
        {
          return 0;
        }
      }
      break;

    case SC_GYROSCOPE:
      if (IKS01A3_MOTION_SENSOR_SetOutputDataRate(GyrInstance, MOTION_GYRO, odr) != BSP_ERROR_NONE)
      {
        return 0;
      }
      break;

    case SC_MAGNETOMETER:
      if (IKS01A3_MOTION_SENSOR_SetOutputDataRate(MagInstance, MOTION_MAGNETO, odr) != BSP_ERROR_NONE)
      {
        return 0;
      }
      break;

    case SC_TEMPERATURE:
      if (IsHybridTmpSensor == HYBRID_SENSOR && TmpInstance == IKS01A3_LIS2DTW12_0)
      {
        if (IKS01A3_HYBRID_ENV_SENSOR_SetOutputDataRate(TmpInstance, HYBRID_TEMPERATURE, odr) != BSP_ERROR_NONE)
        {
          return 0;
        }
      }
      else
      {
        if (IKS01A3_ENV_SENSOR_SetOutputDataRate(TmpInstance, ENV_TEMPERATURE, odr) != BSP_ERROR_NONE)
        {
          return 0;
        }
      }
      break;

    case SC_HUMIDITY:
      if (IKS01A3_ENV_SENSOR_SetOutputDataRate(HumInstance, ENV_HUMIDITY, odr) != BSP_ERROR_NONE)
      {
        return 0;
      }
      break;

    case SC_PRESSURE:
      if (IKS01A3_ENV_SENSOR_SetOutputDataRate(PrsInstance, ENV_PRESSURE, odr) != BSP_ERROR_NONE)
      {
        return 0;
      }
      break;

    default:
      ret = 0;
      break;
  }

  if (ret == 0)
  {
    return ret;
  }

  BUILD_REPLY_HEADER(Msg);
  Msg->Len = 9;
  UART_SendMsg(Msg);
  return ret;
}

/**
 * @brief  Get sensor output data rate
 * @param  Msg the pointer to the message to be handled
 * @retval 1 if the message is correctly handled, 0 otherwise
 */
static int SC_Get_ODR(TMsg *Msg)
{
  float odr = 0.0f;
  int ret = 1;

  /* Sensor Type */
  switch (Msg->Data[4])
  {
    case SC_ACCELEROMETER:
      if (IsHybridAccSensor == HYBRID_SENSOR && AccInstance == IKS01A3_LIS2DTW12_0)
      {
        if (IKS01A3_HYBRID_MOTION_SENSOR_GetOutputDataRate(AccInstance, HYBRID_ACCELERO, &odr) != BSP_ERROR_NONE)
        {
          return 0;
        }
      }
      else
      {
        if (IKS01A3_MOTION_SENSOR_GetOutputDataRate(AccInstance, MOTION_ACCELERO, &odr) != BSP_ERROR_NONE)
        {
          return 0;
        }
      }
      break;

    case SC_GYROSCOPE:
      if (IKS01A3_MOTION_SENSOR_GetOutputDataRate(GyrInstance, MOTION_GYRO, &odr) != BSP_ERROR_NONE)
      {
        return 0;
      }
      break;

    case SC_MAGNETOMETER:
      if (IKS01A3_MOTION_SENSOR_GetOutputDataRate(MagInstance, MOTION_MAGNETO, &odr) != BSP_ERROR_NONE)
      {
        return 0;
      }
      break;

    case SC_TEMPERATURE:
      if (IsHybridTmpSensor == HYBRID_SENSOR && TmpInstance == IKS01A3_LIS2DTW12_0)
      {
        if (IKS01A3_HYBRID_ENV_SENSOR_GetOutputDataRate(TmpInstance, HYBRID_TEMPERATURE, &odr) != BSP_ERROR_NONE)
        {
          return 0;
        }
      }
      else
      {
        if (IKS01A3_ENV_SENSOR_GetOutputDataRate(TmpInstance, ENV_TEMPERATURE, &odr) != BSP_ERROR_NONE)
        {
          return 0;
        }
      }
      break;

    case SC_HUMIDITY:
      if (IKS01A3_ENV_SENSOR_GetOutputDataRate(HumInstance, ENV_HUMIDITY, &odr) != BSP_ERROR_NONE)
      {
        return 0;
      }
      break;

    case SC_PRESSURE:
      if (IKS01A3_ENV_SENSOR_GetOutputDataRate(PrsInstance, ENV_PRESSURE, &odr) != BSP_ERROR_NONE)
      {
        return 0;
      }
      break;

    default:
      ret = 0;
      break;
  }

  if (ret == 0)
  {
    return ret;
  }

  BUILD_REPLY_HEADER(Msg);
  FloatToArray(&Msg->Data[5], odr);
  Msg->Len = 9;
  UART_SendMsg(Msg);
  return ret;
}

/**
 * @brief  Get sensor list
 * @param  Msg the pointer to the message to be handled
 * @retval 1 if the message is correctly handled, 0 otherwise
 */
static int SC_Get_Sensor_List(TMsg *Msg)
{
  int ret = 1;
  int i;

  /* Sensor Type */
  switch (Msg->Data[4])
  {
    case SC_ACCELEROMETER:
      /* Concatenate all sensor names of same kind to one string (comma separated) */
      for (i = 0; i < sizeof(AccNamesString); i++)
      {
        AccNamesString[i] = '\0';
      }
      (void)strcat((char *)AccNamesString, (char *)AccNameList[0]);
      for (i = 1; i < sizeof(AccNameList) / SENSOR_NAME_MAX_LENGTH; i++)
      {
        (void)strcat((char *)AccNamesString, ",");
        (void)strcat((char *)AccNamesString, (char *)AccNameList[i]);
      }
      Send_Sensor_Name(Msg, AccNamesString);
      break;

    case SC_GYROSCOPE:
      /* Concatenate all sensor names of same kind to one string (comma separated) */
      for (i = 0; i < sizeof(GyrNamesString); i++)
      {
        GyrNamesString[i] = '\0';
      }
      (void)strcat((char *)GyrNamesString, (char *)GyrNameList[0]);
      for (i = 1; i < sizeof(GyrNameList) / SENSOR_NAME_MAX_LENGTH; i++)
      {
        (void)strcat((char *)GyrNamesString, ",");
        (void)strcat((char *)GyrNamesString, (char *)GyrNameList[i]);
      }
      Send_Sensor_Name(Msg, GyrNamesString);
      break;

    case SC_MAGNETOMETER:
      /* Concatenate all sensor names of same kind to one string (comma separated) */
      for (i = 0; i < sizeof(MagNamesString); i++)
      {
        MagNamesString[i] = '\0';
      }
      (void)strcat((char *)MagNamesString, (char *)MagNameList[0]);
      for (i = 1; i < sizeof(MagNameList) / SENSOR_NAME_MAX_LENGTH; i++)
      {
        (void)strcat((char *)MagNamesString, ",");
        (void)strcat((char *)MagNamesString, (char *)MagNameList[i]);
      }
      Send_Sensor_Name(Msg, MagNamesString);
      break;

    case SC_TEMPERATURE:
      /* Concatenate all sensor names of same kind to one string (comma separated) */
      for (i = 0; i < sizeof(TmpNamesString); i++)
      {
        TmpNamesString[i] = '\0';
      }
      (void)strcat((char *)TmpNamesString, (char *)TmpNameList[0]);
      for (i = 1; i < sizeof(TmpNameList) / SENSOR_NAME_MAX_LENGTH; i++)
      {
        (void)strcat((char *)TmpNamesString, ",");
        (void)strcat((char *)TmpNamesString, (char *)TmpNameList[i]);
      }
      Send_Sensor_Name(Msg, TmpNamesString);
      break;

    case SC_HUMIDITY:
      /* Concatenate all sensor names of same kind to one string (comma separated) */
      for (i = 0; i < sizeof(HumNamesString); i++)
      {
        HumNamesString[i] = '\0';
      }
      (void)strcat((char *)HumNamesString, (char *)HumNameList[0]);
      for (i = 1; i < sizeof(HumNameList) / SENSOR_NAME_MAX_LENGTH; i++)
      {
        (void)strcat((char *)HumNamesString, ",");
        (void)strcat((char *)HumNamesString, (char *)HumNameList[i]);
      }
      Send_Sensor_Name(Msg, HumNamesString);
      break;

    case SC_PRESSURE:
      /* Concatenate all sensor names of same kind to one string (comma separated) */
      for (i = 0; i < sizeof(PrsNamesString); i++)
      {
        PrsNamesString[i] = '\0';
      }
      (void)strcat((char *)PrsNamesString, (char *)PrsNameList[0]);
      for (i = 1; i < sizeof(PrsNameList) / SENSOR_NAME_MAX_LENGTH; i++)
      {
        (void)strcat((char *)PrsNamesString, ",");
        (void)strcat((char *)PrsNamesString, (char *)PrsNameList[i]);
      }
      Send_Sensor_Name(Msg, PrsNamesString);
      break;

    default:
      ret = 0;
      break;
  }

  return ret;
}

/**
 * @brief  Set sensor index
 * @param  Msg the pointer to the message to be handled
 * @retval 1 if the message is correctly handled, 0 otherwise
 */
static int SC_Set_Sensor_Index(TMsg *Msg)
{
  int ret = 1;

  /* Sensor Type */
  switch (Msg->Data[4])
  {
    case SC_ACCELEROMETER:
      AccIndex = Msg->Data[5];
      if ((AccInstance != AccInstanceList[AccIndex]) || (IsHybridAccSensor != HybridAccInstanceList[AccIndex]))
      {
        if (IsHybridAccSensor == HYBRID_SENSOR && AccInstance == IKS01A3_LIS2DTW12_0)
        {
          if (IKS01A3_HYBRID_MOTION_SENSOR_Disable(AccInstance, HYBRID_ACCELERO) != BSP_ERROR_NONE)
          {
            ret = 0;
          }
        }
        else
        {
          if (IKS01A3_MOTION_SENSOR_Disable(AccInstance, MOTION_ACCELERO) != BSP_ERROR_NONE)
          {
            ret = 0;
          }
        }
        IsHybridAccSensor = HybridAccInstanceList[AccIndex];
        if (IsHybridAccSensor == HYBRID_SENSOR && AccInstanceList[AccIndex] == IKS01A3_LIS2DTW12_0)
        {
          if (IKS01A3_HYBRID_SENSOR_Init(AccInstanceList[AccIndex], HYBRID_ACCELERO) != BSP_ERROR_NONE)
          {
            ret = 0;
          }
        }
        else
        {
          if (IKS01A3_MOTION_SENSOR_Init(AccInstanceList[AccIndex], MOTION_ACCELERO) != BSP_ERROR_NONE)
          {
            ret = 0;
          }
        }
        if (Is_DIL24_Sensor(&AccNameList[AccIndex][0]) == 1)
        {
          DIL24_INT1_Init();
        }
        AccInstance = AccInstanceList[AccIndex];
      }
      break;

    case SC_GYROSCOPE:
      GyrIndex = Msg->Data[5];
      if (GyrInstance != GyrInstanceList[GyrIndex])
      {
        if (IKS01A3_MOTION_SENSOR_Disable(GyrInstance, MOTION_GYRO) != BSP_ERROR_NONE)
        {
          ret = 0;
        }
        if (IKS01A3_MOTION_SENSOR_Init(GyrInstanceList[GyrIndex], MOTION_GYRO) != BSP_ERROR_NONE)
        {
          ret = 0;
        }
        if (Is_DIL24_Sensor(&GyrNameList[GyrIndex][0]) == 1)
        {
          DIL24_INT1_Init();
        }
        GyrInstance = GyrInstanceList[GyrIndex];
      }
      break;

    case SC_MAGNETOMETER:
      MagIndex = Msg->Data[5];
      if (MagInstance != MagInstanceList[MagIndex])
      {
        if (IKS01A3_MOTION_SENSOR_Disable(MagInstance, MOTION_MAGNETO) != BSP_ERROR_NONE)
        {
          ret = 0;
        }
        if (IKS01A3_MOTION_SENSOR_Init(MagInstanceList[MagIndex], MOTION_MAGNETO) != BSP_ERROR_NONE)
        {
          ret = 0;
        }
        if (Is_DIL24_Sensor(&MagNameList[MagIndex][0]) == 1)
        {
          DIL24_INT1_Init();
        }
        MagInstance = MagInstanceList[MagIndex];
      }
      break;

    case SC_TEMPERATURE:
      TmpIndex = Msg->Data[5];
      if ((TmpInstance != TmpInstanceList[TmpIndex]) || (IsHybridTmpSensor != HybridTmpInstanceList[TmpIndex]))
      {
        if (IsHybridTmpSensor == HYBRID_SENSOR && TmpInstance == IKS01A3_LIS2DTW12_0)
        {
          if (IKS01A3_HYBRID_ENV_SENSOR_Disable(TmpInstance, HYBRID_TEMPERATURE) != BSP_ERROR_NONE)
          {
            ret = 0;
          }
        }
        else
        {
          if (IKS01A3_ENV_SENSOR_Disable(TmpInstance, ENV_TEMPERATURE) != BSP_ERROR_NONE)
          {
            ret = 0;
          }
        }
        IsHybridTmpSensor = HybridTmpInstanceList[TmpIndex];
        if (IsHybridTmpSensor == HYBRID_SENSOR && TmpInstanceList[TmpIndex] == IKS01A3_LIS2DTW12_0)
        {
          if (IKS01A3_HYBRID_SENSOR_Init(TmpInstanceList[TmpIndex], HYBRID_TEMPERATURE) != BSP_ERROR_NONE)
          {
            ret = 0;
          }
        }
        else
        {
          if (IKS01A3_ENV_SENSOR_Init(TmpInstanceList[TmpIndex], ENV_TEMPERATURE) != BSP_ERROR_NONE)
          {
            ret = 0;
          }
        }
        if (Is_DIL24_Sensor(&TmpNameList[TmpIndex][0]) == 1)
        {
          DIL24_INT1_Init();
        }
        TmpInstance = TmpInstanceList[TmpIndex];
      }
      break;

    case SC_HUMIDITY:
      HumIndex = Msg->Data[5];
      if (HumInstance != HumInstanceList[HumIndex])
      {
        if (IKS01A3_ENV_SENSOR_Disable(HumInstance, ENV_HUMIDITY) != BSP_ERROR_NONE)
        {
          ret = 0;
        }
        if (IKS01A3_ENV_SENSOR_Init(HumInstanceList[HumIndex], ENV_HUMIDITY) != BSP_ERROR_NONE)
        {
          ret = 0;
        }
        if (Is_DIL24_Sensor(&HumNameList[HumIndex][0]) == 1)
        {
          DIL24_INT1_Init();
        }
        HumInstance = HumInstanceList[HumIndex];
      }
      break;

    case SC_PRESSURE:
      PrsIndex = Msg->Data[5];
      if (PrsInstance != PrsInstanceList[PrsIndex])
      {
        if (IKS01A3_ENV_SENSOR_Disable(PrsInstance, ENV_PRESSURE) != BSP_ERROR_NONE)
        {
          ret = 0;
        }
        if (IKS01A3_ENV_SENSOR_Init(PrsInstanceList[PrsIndex], ENV_PRESSURE) != BSP_ERROR_NONE)
        {
          ret = 0;
        }
        if (Is_DIL24_Sensor(&PrsNameList[PrsIndex][0]) == 1)
        {
          DIL24_INT1_Init();
        }
        PrsInstance = PrsInstanceList[PrsIndex];
      }
      break;

    default:
      ret = 0;
      break;
  }

  return ret;
}

/**
 * @brief  Send sensor name
 * @param  Msg the pointer to the message to be handled
 * @param  SensorName the sensor name
 * @retval 1 if the message is correctly handled, 0 otherwise
 */
static void Send_Sensor_Name(TMsg *Msg, uint8_t *SensorName)
{
  uint32_t i = 0;
  BUILD_REPLY_HEADER(Msg);

  while (i < strlen((char const *)SensorName))
  {
    Msg->Data[5U + i] = SensorName[i];
    i++;
  }

  Msg->Len = 5U + i;
  UART_SendMsg(Msg);
}

/**
 * @brief  Send sensor full scale list
 * @param  Msg the pointer to the message to be handled
 * @param  FsList the sensor full scale list
 * @retval 1 if the message is correctly handled, 0 otherwise
 */
static void Send_Sensor_FS_List(TMsg *Msg, uint32_t *FsList)
{
  uint32_t i;
  BUILD_REPLY_HEADER(Msg);

  Serialize(&Msg->Data[5], FsList[0], 4);

  for (i = 0; i < FsList[0]; i++)
  {
    Serialize(&Msg->Data[9U + i * 4U], FsList[i + 1U], 4U);
  }

  Msg->Len = 9U + i * 4U;
  UART_SendMsg(Msg);
}

/**
 * @brief  Send sensor output data rate list
 * @param  Msg the pointer to the message to be handled
 * @param  OdrList the sensor output data rate list
 * @retval 1 if the message is correctly handled, 0 otherwise
 */
static void Send_Sensor_ODR_List(TMsg *Msg, float *OdrList)
{
  uint32_t i;
  BUILD_REPLY_HEADER(Msg);

  Serialize(&Msg->Data[5], (uint32_t)OdrList[0], 4);

  for (i = 0; i < (uint32_t)OdrList[0]; i++)
  {
    FloatToArray(&Msg->Data[9U + i * 4U], OdrList[i + 1U]);
  }

  Msg->Len = 9U + i * 4U;
  UART_SendMsg(Msg);
}

/**
 * @brief  Check if sensor is a DIL24 sensor
 * @param  SensorName the pointer to sensor name string
 * @retval 1 if sensor is a DIL24 sensor, 0 otherwise
 */
static int Is_DIL24_Sensor(uint8_t *SensorName)
{
  uint8_t string[] = " (DIL24)";
  size_t size = strlen((const char *)string);
  uint8_t *suffix = SensorName + strlen((const char *)SensorName) - size;

  if (suffix > SensorName)
  {
    return (strncmp((const char *)suffix, (const char *)string, size) == 0) ? 1 : 0;
  }
  else
  {
    return 0;
  }
}

/**
 * @brief  Configure DIL24 INT1 pin
 * @param  None
 * @retval None
 */
static void DIL24_INT1_Init(void)
{
  __HAL_RCC_GPIOC_CLK_ENABLE();

  GPIO_InitTypeDef GPIO_InitStruct;

  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}

/**
 * @brief  Copy float data to array
 * @param  Dest the pointer to the destination
 * @param  data the source data
 * @retval None
 */
static void FloatToArray(uint8_t *Dest, float data)
{
  (void)memcpy(Dest, (void *) &data, 4);
}

/**
 * @brief  Copy float data from array
 * @param  Source the pointer to the source data
 * @param  data the pointer to the destination
 * @retval None
 */
static void ArrayToFloat(uint8_t *Source, float *data)
{
  (void)memcpy((void *) data, Source, 4);
}

/**
 * @}
 */

/**
 * @}
 */
