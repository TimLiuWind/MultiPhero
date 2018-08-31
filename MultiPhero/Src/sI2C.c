#include "sI2C.h"

#define sI2C_SCL_H     (HAL_GPIO_WritePin(sI2C_SCL_GPIO_Port,sI2C_SCL_Pin,GPIO_PIN_SET))
#define sI2C_SCL_L     (HAL_GPIO_WritePin(sI2C_SCL_GPIO_Port,sI2C_SCL_Pin,GPIO_PIN_RESET))

#define sI2C_SDA_H     (HAL_GPIO_WritePin(sI2C_SDA_GPIO_Port,sI2C_SDA_Pin,GPIO_PIN_SET))
#define sI2C_SDA_L     (HAL_GPIO_WritePin(sI2C_SDA_GPIO_Port,sI2C_SDA_Pin,GPIO_PIN_RESET))

#define sI2C_SDA_STATE (HAL_GPIO_ReadPin(sI2C_SDA_GPIO_Port,sI2C_SDA_Pin))

void sI2C_SDA_OUT(void)
{
   GPIO_InitTypeDef GPIO_InitStruct;
	
	/*Configure GPIO pins : sI2C_SDA_Pin as output */
  GPIO_InitStruct.Pin = sI2C_SDA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(sI2C_SDA_GPIO_Port, &GPIO_InitStruct);
}

void sI2C_SDA_IN(void)
{
   GPIO_InitTypeDef GPIO_InitStruct;
	
	/*Configure GPIO pins : sI2C_SDA_Pin as input */
  GPIO_InitStruct.Pin = sI2C_SDA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(sI2C_SDA_GPIO_Port, &GPIO_InitStruct);
}



uint8_t sI2C_Start(void)
{
  volatile uint8_t i=0;
	sI2C_SDA_OUT();
	sI2C_SDA_H;
	sI2C_SCL_H;
	delay_us(150);
	while(!sI2C_SDA_STATE){
	   i++;
		if(i>=249){
			return 1;		
		}
	}
	sI2C_SDA_L;
	delay_us(150);
	i=0;
	while((sI2C_SDA_STATE == 1) && (i<250))     
	{
		i++;                                                                                                        
		if(i==249)                                                                                        
		return 1;
	} 
	sI2C_SDA_L;
	sI2C_SCL_L;
	delay_us(150);
	return 0;
}

void sI2C_Stop(void)
{
	sI2C_SDA_L;
	delay_us(150);
	sI2C_SCL_H;
	delay_us(150);
	sI2C_SDA_H;
	delay_us(150);
}

void sI2C_Ack(void)
{
//	SCL_L;
//	delay_short(hObj->nDelay);
	sI2C_SDA_L;
	delay_us(150);
	sI2C_SCL_H;
	delay_us(150);
	sI2C_SCL_L;
	delay_us(150);
	sI2C_SDA_L;
	delay_us(150);
}

void sI2C_NoAck(void)
{	
	sI2C_SDA_H;	
	delay_us(150);	
	sI2C_SCL_H;	
	delay_us(150);	
	sI2C_SCL_L;	
	delay_us(150);	
	sI2C_SDA_L;	
	delay_us(150);
}

uint8_t sI2C_WaitAck(void)
{
	//SCL_L;
	//delay_short(hObj->nDelay);
	sI2C_SDA_H;
	delay_us(150);
	sI2C_SCL_H;
	sI2C_SDA_IN();
	delay_us(150);
	if ((sI2C_SDA_STATE))
	{
		sI2C_SCL_L;
		delay_us(150);
		return 1; //got no ack
	}
	sI2C_SCL_L;
	sI2C_SDA_OUT();
	return 0;  //got ack
}

void sI2C_Write(uint8_t data)
{
	uint8_t i=8;
	while(i--)
	{
		if(data&0x80)
			sI2C_SDA_H;  
		else 
			sI2C_SDA_L;   
		data<<=1;
		delay_us(150);
		sI2C_SCL_H;
		delay_us(150);
		sI2C_SCL_L;
	}
}

uint8_t sI2C_Read(void)
{
	uint8_t read=0x00,j;
	
	sI2C_SDA_IN();
	delay_us(150);
	for(j=8;j>0;j--) 
	{		     
		sI2C_SCL_H;
		delay_us(150);
    read=sI2C_SDA_STATE + (read<<1); 
		sI2C_SCL_L;
		delay_us(150);
	}	
  sI2C_SDA_OUT();
	return read;
}

