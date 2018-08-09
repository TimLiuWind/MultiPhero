#include "ColiasCamera.h"

const uint8_t CameraInitReg[]={
	0x11,0x80,//CLKRC
	0x12,0x08,//color bar YUV
	0x13,0x8b,//Disable Auto Gain Control
	0x0C,0x0C,//COM3 OUTPUT/SCALE
	0x3E,0x12,//COM14  PCLK,scale,DCW
	0x70,0x3A,//SCALING_XSC testpattern 
	0x71,0x35,//35 SCALING_YSC testpattern 
	0x72,0x22,//SCALING_DCWCTR
	0x73,0xF2,//SCALING_PCLK_DIV
	0xa2,0x2a,//SCALING_PCLK_DELAY
	0x1e,0x31,//MVFP mirror/flip
	0x15,0x00,//COM10 HREF/PCLK/VSYNC pattern
	0x3A,0x15,//TSLB Line Buffer Test Option
	0x67,0xaa,//MANU
	0x68,0x55,//MANV
	
};

uint8_t Camera_WriteReg(uint8_t regID, uint8_t regDat)
{
	uint8_t a=4;
	a = sI2C_Start();//·
	if(!a)
	{
		if(sI2C_Write(0x42)==0)//
		{
			sI2C_Stop();//·
			return 1;//´
		}
		delay_us(150);
		if(sI2C_Write(regID)==0)//»ı´æÆ÷ID
		{
			sI2C_Stop();//·¢ËÍSCCB ×ÜÏßÍ£Ö¹´«ÊäÃüÁî
			return 2;//´íÎó·µ»Ø
		}
		delay_us(150);
		if(sI2C_Write(regDat)==0)//Ğ´Êı¾İµ½»ı´æÆ÷
		{
			sI2C_Stop();//·¢ËÍSCCB ×ÜÏßÍ£Ö¹´«ÊäÃüÁî
			return 3;//´íÎó·µ»Ø
		}
		sI2C_Stop();//·¢ËÍSCCB ×ÜÏßÍ£Ö¹´«ÊäÃüÁî	
		return 0;//³É¹¦·µ»Ø
	}
	else
		return 1;
}

uint8_t Camera_ReadReg(uint8_t regID, __IO uint8_t *regDat)
{
	//Í¨¹ıĞ´²Ù×÷ÉèÖÃ¼Ä´æÆ÷µØÖ·
	sI2C_Start();
	if(sI2C_Write(0x42)==0)//Ğ´µØÖ·
	{
		sI2C_Stop();//·¢ËÍSCCB ×ÜÏßÍ£Ö¹´«ÊäÃüÁî
		return 1;//´íÎó·µ»Ø
	}
	delay_us(150);
  	if(sI2C_Write(regID)==0)//»ı´æÆ÷ID
	{
		sI2C_Stop();//·¢ËÍSCCB ×ÜÏßÍ£Ö¹´«ÊäÃüÁî
		return 2;//´íÎó·µ»Ø
	}
	sI2C_Stop();//·¢ËÍSCCB ×ÜÏßÍ£Ö¹´«ÊäÃüÁî	
	delay_us(150);	
	//ÉèÖÃ¼Ä´æÆ÷µØÖ·ºó£¬²ÅÊÇ¶Á
	sI2C_Start();
	if(sI2C_Write(0x43)==0)//¶ÁµØÖ·
	{
		sI2C_Stop();//·¢ËÍSCCB ×ÜÏßÍ£Ö¹´«ÊäÃüÁî
		return 3;//´íÎó·µ»Ø
	}
	delay_us(150);
  	*regDat=sI2C_Read();//·µ»Ø¶Áµ½µÄÖµ
  	noAck();//·¢ËÍNACKÃüÁî
  	sI2C_Stop();//·¢ËÍSCCB ×ÜÏßÍ£Ö¹´«ÊäÃüÁî
  	return 0;//³É¹¦·µ»Ø
}


void Camera_Reset(void)
{
	Camera_WriteReg(0x12,0x80);
}

uint8_t Camera_ReadID(void)
{
	__IO uint8_t temp=0,sta=0;
	sta = Camera_ReadReg(0x0b,&temp);
  	return temp;
}

uint8_t Camera_Init(uint8_t resolution)
{
	uint8_t i,n=0,ad,val;
//	uint8_t* pl;
	
	//Cam_GPIO_Init_1();//traditional GPIO and interrupt method
	Camera_Reset();
	if(Camera_ReadID() != Camera_ID)
	{
//		printf("OV7670 not found!\r\n");
//		printf("Instead, found something with ID %X\r\n",OV_ReadID());
//		printf("But this program supports OV7670 ONLY(0x0A)\r\n");
		return 1;//not OV7670, return error
	}
	else//Write registers of OV7670
	{
//		printf("OV7670 found\r\n");
//		printf("Writing registers of OV7670\r\n");	
//		printf("Registers has been written:\r\n");
		if(resolution == Res_QQCIF)
		{
			Camera_Reset();
			i=sizeof(CameraInitReg)/2;
			
			for(n=0;n<i;n++)
			{
				ad = CameraInitReg[n<<1];
				val= CameraInitReg[1+(n<<1)];
				Camera_WriteReg(ad,val);
//				printf("A:0x%02X\tD:0x%02X\r\n",ad,val);
			}
			
			/*
			OV_WriteReg(0x11,0x80);//CLKRC
			OV_WriteReg(0x12,0x08);//color bar YUV
			OV_WriteReg(0x0C,0x0C);//COM3 OUTPUT/SCALE
			OV_WriteReg(0x3E,0x12);//COM14  PCLK,scale,DCW
			OV_WriteReg(0x70,0x3A);//SCALING_XSC testpattern 
			OV_WriteReg(0x71,0x35);//35 SCALING_YSC testpattern 
			OV_WriteReg(0x72,0x22);//SCALING_DCWCTR
			OV_WriteReg(0x73,0xF2);//SCALING_PCLK_DIV
			OV_WriteReg(0xa2,0x2a);//SCALING_PCLK_DELAY
			OV_WriteReg(0x1e,0x01);//MVFP mirror/flip
			OV_WriteReg(0x15,0x00);//COM10 HREF/PCLK/VSYNC pattern
			OV_WriteReg(0x3A,0x15);//TSLB Line Buffer Test Option
			OV_WriteReg(0x67,0xaa);//MANU
			OV_WriteReg(0x68,0x55);//MANV
			*/
		}
	/*	
		for(i=0;i<=0xc9;i++)
		{
			u8 add_temp;
			OV_ReadReg(i,&add_temp);
			//OV_WriteReg(OV7670_Reg[i][0],OV7670_Reg[i][1]);
			printf("A:%X, D:%X  \r\n",i,add_temp);
			LED_SYS_Toggle;
		}
		*/
//		printf("OV7670 Register Writting Done.\r\n");
		
//		if (1)
//		{
//			Cam_DCMI_GPIO_Init();
//			Cam_DCMI_Init();
//			DCMI_IT_config();
//		}
		return 0;
	}
}
