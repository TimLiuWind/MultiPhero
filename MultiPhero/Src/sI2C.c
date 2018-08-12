#include "sI2C.h"


void sI2C_SDA_OUT(void)
{
   GPIO_InitTypeDef GPIO_InitStruct;
	
	/*Configure GPIO pins : sI2C_SDA_Pin as output */
  GPIO_InitStruct.Pin = sI2C_SDA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
}

void sI2C_SDA_IN(void)
{
   GPIO_InitTypeDef GPIO_InitStruct;
	
	/*Configure GPIO pins : sI2C_SDA_Pin as input */
  GPIO_InitStruct.Pin = sI2C_SDA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
}

uint8_t sI2C_Start(void)
{
  volatile uint8_t i=0;
	sI2C_SDA_OUT();
	SDA_H;
	delay_us(150);
	SCL_H;
	delay_us(150);
	while(!SDA_STATE){
	   i++;
		if(i>=249){
			return 1;		
		}
	}
	SDA_L;
	delay_us(150);
	i=0;
	while((SDA_STATE == 1) && (i<250))     
	{
		i++;                                                                                                        
		if(i==249)                                                                                        
		return (1);
	} 
	SCL_L;
	delay_us(150);
	return 0;
}

void sI2C_Stop(void)
{
	sI2C_SDA_OUT();
	SDA_L;
	delay_us(150);
	SCL_H;
	delay_us(150);
	SDA_H;
	delay_us(150);
}

void noAck(void)
{	
	SDA_H;	
	delay_us(150);	
	SCL_H;	
	delay_us(150);	
	SCL_L;	
	delay_us(150);	
	SDA_L;	
	delay_us(150);
}

uint8_t sI2C_Write(uint8_t m_data)
{
	uint8_t j,tem;

	for(j=0;j<8;j++) 
	{
		if((m_data<<j)&0x80)SDA_H;
		else SDA_L;
		delay_us(150);
		SCL_H;	
		delay_us(150);
		SCL_L;	
		delay_us(150);
	}
	delay_us(150);
	sI2C_SDA_IN();
	delay_us(50);
	SCL_H;	
	delay_us(150);
	if(SDA_STATE)
		tem=0;//SDA=1·
	else
		tem=1;//SDA=0
	SCL_L;	
	delay_us(150);	
    sI2C_SDA_OUT();
	return tem;  
}

uint8_t sI2C_Read(void)
{
	uint8_t read,j;
	read=0x00;
	
	sI2C_SDA_IN();
	delay_us(150);
	for(j=8;j>0;j--) 
	{		     
		delay_us(150);
		SCL_H;
		delay_us(150);
		read = read << 1;
		if(SDA_STATE) read = read + 1; 
		SCL_L;
		delay_us(150);
	}	
    sI2C_SDA_OUT();
	return read;
}

