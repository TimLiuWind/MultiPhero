#ifndef sI2C_h
#define sI2C_h

#include "stm32f4xx_hal.h"
#include "main.h"
#include "delay.h"






void sI2C_SDA_OUT(void);
void sI2C_SDA_IN(void);
uint8_t sI2C_Start(void);
void sI2C_Stop(void);
void noAck(void);
uint8_t sI2C_Write(uint8_t m_data);
uint8_t sI2C_Read(void);

#endif

