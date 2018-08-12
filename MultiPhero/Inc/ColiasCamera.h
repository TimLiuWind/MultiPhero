#ifndef ColiasCamera_h
#define ColiasCamera_h

#include "stm32f4xx_hal.h"
#include "main.h"
#include "delay.h"
#include "sI2C.h"

#define Camera_ID 0x73

#define Res_QQCIF 	6

#define Image_Width 99
#define Image_Height 72



uint8_t Camera_WriteReg(uint8_t regID, uint8_t regDat);
uint8_t Camera_ReadReg(uint8_t regID, __IO uint8_t *regDat);
void Camera_Reset(void);
uint8_t Camera_ReadID(void);
uint8_t Camera_Init(uint8_t resolution);

#endif

