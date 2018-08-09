#ifndef sI2C_h
#define sI2C_h

#include "stm32f4xx_hal.h"
#include "main.h"
#include "delay.h"



#define SCL_H     (HAL_GPIO_WritePin(sI2C_SCL_GPIO_Port,sI2C_SCL_Pin,GPIO_PIN_SET))
#define SCL_L     (HAL_GPIO_WritePin(sI2C_SCL_GPIO_Port,sI2C_SCL_Pin,GPIO_PIN_RESET))

#define SDA_H     (HAL_GPIO_WritePin(sI2C_SDA_GPIO_Port,sI2C_SDA_Pin,GPIO_PIN_SET))
#define SDA_L     (HAL_GPIO_WritePin(sI2C_SDA_GPIO_Port,sI2C_SDA_Pin,GPIO_PIN_RESET))

#define SDA_STATE (HAL_GPIO_ReadPin(sI2C_SDA_GPIO_Port,sI2C_SDA_Pin))


void sI2C_SDA_OUT(void);
void sI2C_SDA_IN(void);
uint8_t sI2C_Start(void);
void sI2C_Stop(void);
void noAck(void);
uint8_t sI2C_Write(uint8_t m_data);
uint8_t sI2C_Read(void);

#endif

