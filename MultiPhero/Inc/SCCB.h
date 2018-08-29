#ifndef SCCB_h
#define SCCB_h


#include "stm32f4xx_hal.h"
#include "main.h"
#include "delay.h"


void SCCB_SDA_OUT(void);
void SCCB_SDA_IN(void);
uint8_t SCCB_Start(void);
void SCCB_Stop(void);
void noAck(void);
uint8_t SCCB_Write(uint8_t m_data);
uint8_t SCCB_Read(void);

#endif


