#ifndef ColiasCamera_h
#define ColiasCamera_h

#include "stm32f4xx_hal.h"
#include "main.h"
#include "delay.h"
#include "sI2C.h"

#define Camera_ID       0x7673
#define Camera_REG_NUM 	121

#define YUV 0
#define RGB565 1

#define Res_QQCIF 	6

//#define Image_Width 99//99
//#define Image_Height 72//72

//#define DMA_BufferSize 10692 // W*H*3*3/4 = 10692

//#define Image_Width 154//99
//#define Image_Height 144//72

//#define DMA_BufferSize 11088 // 

#define Image_Width 320//99
#define Image_Height 240//72

#define DMA_BufferSize 38400 // 

uint8_t Camera_WriteReg(uint8_t regID, uint8_t regDat);
uint8_t Camera_ReadReg(uint8_t regID, __IO uint8_t *regDat);
void Camera_Reset(void);
uint16_t Camera_ReadID(void);
uint8_t Camera_Init(uint8_t resolution);
HAL_StatusTypeDef Camera_StartCapture(void);
HAL_StatusTypeDef Camera_StopCapture(void);
void Camera_Window_Set(uint16_t sx, uint16_t sy, uint16_t width, uint16_t height);
#endif

