#include "SCCB.h"

#define SCCB_SCL_H     (HAL_GPIO_WritePin(SCCB_SCL_GPIO_Port,SCCB_SCL_Pin,GPIO_PIN_SET))
#define SCCB_SCL_L     (HAL_GPIO_WritePin(SCCB_SCL_GPIO_Port,SCCB_SCL_Pin,GPIO_PIN_RESET))

#define SCCB_SDA_H     (HAL_GPIO_WritePin(SCCB_SDA_GPIO_Port,SCCB_SDA_Pin,GPIO_PIN_SET))
#define SCCB_SDA_L     (HAL_GPIO_WritePin(SCCB_SDA_GPIO_Port,SCCB_SDA_Pin,GPIO_PIN_RESET))

#define SCCB_SDA_STATE (HAL_GPIO_ReadPin(SCCB_SDA_GPIO_Port,SCCB_SDA_Pin))

void SCCB_SDA_OUT(void)
{
   GPIO_InitTypeDef GPIO_InitStruct;
	
	/*Configure GPIO pins : SCCB_SDA_Pin as output */
  GPIO_InitStruct.Pin = SCCB_SDA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
}

void SCCB_SDA_IN(void)
{
   GPIO_InitTypeDef GPIO_InitStruct;
	
	/*Configure GPIO pins : SCCB_SDA_Pin as input */
  GPIO_InitStruct.Pin = SCCB_SDA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
}

uint8_t SCCB_Start(void)
{
  volatile uint8_t i=0;
	SCCB_SDA_OUT();
	SCCB_SDA_H;
	delay_us(150);
	SCCB_SCL_H;
	delay_us(150);
	while(!SCCB_SDA_STATE){
	   i++;
		if(i>=249){
			return 1;		
		}
	}
	SCCB_SDA_L;
	delay_us(150);
	i=0;
	while((SCCB_SDA_STATE == 1) && (i<250))     
	{
		i++;                                                                                                        
		if(i==249)                                                                                        
		return (1);
	} 
	SCCB_SCL_L;
	delay_us(150);
	return 0;
}

void SCCB_Stop(void)
{
	SCCB_SDA_OUT();
	SCCB_SDA_L;
	delay_us(150);
	SCCB_SCL_H;
	delay_us(150);
	SCCB_SDA_H;
	delay_us(150);
}

void noAck(void)
{	
	SCCB_SDA_H;	
	delay_us(150);	
	SCCB_SCL_H;	
	delay_us(150);	
	SCCB_SCL_L;	
	delay_us(150);	
	SCCB_SDA_L;	
	delay_us(150);
}

uint8_t SCCB_Write(uint8_t m_data)
{
	uint8_t j,tem;

	for(j=0;j<8;j++) 
	{
		if((m_data<<j)&0x80)SCCB_SDA_H;
		else SCCB_SDA_L;
		delay_us(150);
		SCCB_SCL_H;	
		delay_us(150);
		SCCB_SCL_L;	
		delay_us(150);
	}
	delay_us(150);
	SCCB_SDA_IN();
	delay_us(50);
	SCCB_SCL_H;	
	delay_us(150);
	if(SCCB_SDA_STATE)
		tem=0;//SDA=1·
	else
		tem=1;//SDA=0
	SCCB_SCL_L;	
	delay_us(150);	
    SCCB_SDA_OUT();
	return tem;  
}

uint8_t SCCB_Read(void)
{
	uint8_t read,j;
	read=0x00;
	
	SCCB_SDA_IN();
	delay_us(150);
	for(j=8;j>0;j--) 
	{		     
		delay_us(150);
		SCCB_SCL_H;
		delay_us(150);
		read = read << 1;
		if(SCCB_SDA_STATE) read = read + 1; 
		SCCB_SCL_L;
		delay_us(150);
	}	
    SCCB_SDA_OUT();
	return read;
}

