#include "Debug.h"
#include "main.h"
#include "ColiasCamera.h"

extern uint32_t __IO FrameCounter;
extern uint32_t __IO ImageCounter;
extern uint8_t __IO CurrentImageSlot;
extern UART_HandleTypeDef huart1;
//extern uint16_t Image[3][Image_Height][Image_Width];
extern uint16_t Image[Image_Height][Image_Width];
	
uint8_t I_START[2] = {0x01,0xfe};
uint8_t I_END[2] = {0xfe,0x01};


void SendFrames(uint8_t imagetype)
{
	uint16_t i,j;
	uint8_t temp8;
	uint16_t temp16;
	
//  HAL_UART_Transmit(&huart1, (uint8_t *)&I_START, 2, 0xFFFF);
	for(i=0;i<Image_Height;i=i+1)
	{
		for(j=0;j<Image_Width;j=j+1)
		{

//		temp8 = Image[i][j]&0xff;
//			temp8 = Image[i][j]>>8;
//			if(temp8==0xff)
//			{
//				temp8 = 0xfe;
//			}
//			HAL_UART_Transmit(&huart1, (uint8_t *)&temp8, 1, 0xFFFF);
//			temp8 = Image[i][j]>>8;
			temp8 = Image[i][j]&0xff;
			if(temp8==0xff)
			{
				temp8 = 0xfe;
			}
			HAL_UART_Transmit(&huart1, (uint8_t *)&temp8, 1, 0xFFFF);
		}		
	}
	temp8 = 0xff;
	HAL_UART_Transmit(&huart1, (uint8_t *)&temp8, 1, 0xFFFF);
//	HAL_UART_Transmit(&huart1, (uint8_t *)&I_END, 2, 0xFFFF);
	
//	HAL_UART_Transmit(&huart1, (uint8_t *)&I_START, 2, 0xFFFF);
//	for(i=0;i<Image_Height;i++)
//	{
//		for(j=0;j<Image_Width;j++)
//		{
//			temp16 = Image[i][j];
//			HAL_UART_Transmit(&huart1, (uint8_t *)&temp16, 2, 0xFFFF);
////			HAL_UART_Transmit(&huart1, (uint8_t *)&temp16 + 1, 1, 0xFFFF);
////			HAL_UART_Transmit(&huart1, (uint8_t *)&temp16, 1, 0xFFFF);
//		}		
//	}
//	HAL_UART_Transmit(&huart1, (uint8_t *)&I_END, 2, 0xFFFF);
}
