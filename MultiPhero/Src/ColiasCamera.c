#include "ColiasCamera.h"

extern DCMI_HandleTypeDef hdcmi;

uint32_t __IO FrameCounter = 1;
uint32_t __IO ImageCounter = 1;
uint8_t __IO CurrentImageSlot = 1;
uint16_t Image[3][Image_Height][Image_Width];
//uint16_t Image[Image_Height][Image_Width];

//const uint8_t CameraInitReg[][2]= 
//{   
//     /* OV7670 QVGA RGB56 */
//  {0x3a, 0x04},//
//	{0x40, 0x10},
//	{0x12, 0xc0},//QVGA,RGB output 0x14

//	//??????
//	{0x32, 0x80},
//	{0x17, 0x16},         
//	{0x18, 0x04},//5
//	{0x19, 0x02},
//	{0x1a, 0x7a},//0x7a,
// 	{0x03, 0x0a},//0x0a,


//	{0x0c, 0x0c},
//	{0x15, 0x00},
//	{0x3e, 0x12},//10
//	{0x70, 0x00},
//	{0x71, 0x01},
//	{0x72, 0x11},
//	{0x73, 0xF2},//
//        
//	{0xa2, 0x2a},//15
//	{0x11, 0x80},//0
//	{0x7a, 0x20},
//	{0x7b, 0x1c},
//	{0x7c, 0x28},
//        
//	{0x7d, 0x3c},//20
//	{0x7e, 0x55},
//	{0x7f, 0x68},
//	{0x80, 0x76},
//	{0x81, 0x80},
//        
//	{0x82, 0x88},
//	{0x83, 0x8f},
//	{0x84, 0x96},
//	{0x85, 0xa3},
//	{0x86, 0xaf},
//        
//	{0x87, 0xc4},//30
//	{0x88, 0xd7},
//	{0x89, 0xe8},
//	{0x13, 0xe0},
//	{0x00, 0x00},//AGC
//        
//	{0x10, 0x00},
//	{0x0d, 0x00}, 
//	{0x14, 0x20},//0x38, limit the max gain
//	{0xa5, 0x05},
//	{0xab, 0x07},
//        
//	{0x24, 0x75},//40
//	{0x25, 0x63},
//	{0x26, 0xA5},
//	{0x9f, 0x78},
//	{0xa0, 0x68},
//        
//	{0xa1, 0x03},//0x0b,
//	{0xa6, 0xdf},//0xd8,
//	{0xa7, 0xdf},//0xd8,
//	{0xa8, 0xf0},
//	{0xa9, 0x90},
//        
//	{0xaa, 0x94},//50
//	{0x13, 0xe5},
//	{0x0e, 0x61},
//	{0x0f, 0x4b},
//	{0x16, 0x02},
//        
//	{0x1e, 0x31},//????????.0x07,
//	{0x21, 0x02},
//	{0x22, 0x91},
//	{0x29, 0x07},
//	{0x33, 0x0b},
//        
//	{0x35, 0x0b},//60
//	{0x37, 0x1d},
//	{0x38, 0x71},
//	{0x39, 0x2a},
//	{0x3c, 0x78},
//        
//	{0x4d, 0x40},
//	{0x4e, 0x20},
//	{0x69, 0x5d},
//	{0x6b, 0x40},//PLL*4=48Mhz
//	{0x74, 0x19},
//	{0x8d, 0x4f},
//        
//	{0x8e, 0x00},//70
//	{0x8f, 0x00},
//	{0x90, 0x00},
//	{0x91, 0x00},
//	{0x92, 0x00},//0x19,//0x66
//        
//	{0x96, 0x00},
//	{0x9a, 0x80},
//	{0xb0, 0x84},
//	{0xb1, 0x0c},
//	{0xb2, 0x0e},
//        
//	{0xb3, 0x82},//80
//	{0xb8, 0x0a},
//	{0x43, 0x14},
//	{0x44, 0xf0},
//	{0x45, 0x34},
//        
//	{0x46, 0x58},
//	{0x47, 0x28},
//	{0x48, 0x3a},
//	{0x59, 0x88},
//	{0x5a, 0x88},
//        
//	{0x5b, 0x44},//90
//	{0x5c, 0x67},
//	{0x5d, 0x49},
//	{0x5e, 0x0e},
//	{0x64, 0x04},
//	{0x65, 0x20},
//        
//	{0x66, 0x05},
//	{0x94, 0x04},
//	{0x95, 0x08},
//	{0x6c, 0x0a},
//	{0x6d, 0x55},
//        
//        
//	{0x4f, 0x80},
//	{0x50, 0x80},
//	{0x51, 0x00},
//	{0x52, 0x22},
//	{0x53, 0x5e},
//	{0x54, 0x80},
//       
//	{0x09, 0x03},//??????

//	{0x6e, 0x11},//100
//	{0x6f, 0x9f},//0x9e for advance AWB
//  {0x55, 0x00},//??
//  {0x56, 0x40},//???
//  {0x57, 0x80},//0x40,  change according to Jim's request	
//}; 


//const uint8_t CameraInitReg[]={
//	0x11,0x80,//CLKRC
//	0x12,0x08,//color bar YUV
//	0x13,0x8b,//Disable Auto Gain Control
//	0x14,0x6a,//Set auto gain ceiling to 128x(was 32x for 0x4a)
//	0x0C,0x0C,//COM3 OUTPUT/SCALE
//	0x3E,0x12,//COM14  PCLK,scale,DCW
//	0x70,0x3A,//SCALING_XSC testpattern 
//	0x71,0x35,//35 SCALING_YSC testpattern 
//	0x72,0x22,//SCALING_DCWCTR
//	0x73,0xF2,//SCALING_PCLK_DIV
//	0xa2,0x2a,//SCALING_PCLK_DELAY
//	0x1e,0x11,//MVFP mirror/flip
//	0x15,0x00,//COM10 HREF/PCLK/VSYNC pattern
//	0x3A,0x05,//TSLB Line Buffer Test Option
//	0x67,0xaa,//MANU
//	0x68,0x55,//MANV
//	0x00,0x80
//	
//};

const uint8_t CameraInitReg[]={
	0x11,0x80,//CLKRC
	0x12,0x08,//color bar YUV
	0x13,0x8b,//Disable Auto Gain Control
	0x14,0x6a,//Set auto gain ceiling to 128x(was 32x for 0x4a)
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
	0x00,0x80
	
};

uint8_t Camera_WriteReg(uint8_t regID, uint8_t regDat)
{
	uint8_t a=4;
	a = sI2C_Start();
	if(!a)
	{
		if(sI2C_Write(0x42)==0)
		{
			sI2C_Stop();
			return 1;
		}
		delay_us(150);
		if(sI2C_Write(regID)==0)
		{
			sI2C_Stop();
			return 2;
		}
		delay_us(150);
		if(sI2C_Write(regDat)==0)
		{
			sI2C_Stop();
			return 3;
		}
		sI2C_Stop();
		return 0;
	}
	else
		return 1;
}

uint8_t Camera_ReadReg(uint8_t regID, __IO uint8_t *regDat)
{
	
	sI2C_Start();
	if(sI2C_Write(0x42)==0)
	{
		sI2C_Stop();
		return 1;
	}
	delay_us(150);
  	if(sI2C_Write(regID)==0)
	{
		sI2C_Stop();
		return 2;
	}
	sI2C_Stop();
	delay_us(150);	
	
	sI2C_Start();
	if(sI2C_Write(0x43)==0)
	{
		sI2C_Stop();
		return 3;
	}
	delay_us(150);
  	*regDat=sI2C_Read();
  	noAck();
  	sI2C_Stop();
  	return 0;
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
//uint8_t Camera_Init(uint8_t resolution)
//{
//  uint16_t i=0;
//	Camera_Reset();
//	if(Camera_ReadID() != Camera_ID)
//	{
//		return 1;//not OV7670, return error
//	}
//	for(i=0;i<sizeof(CameraInitReg)/sizeof(CameraInitReg[0]);i++)
//	{
//	  Camera_WriteReg(CameraInitReg[i][0],CameraInitReg[i][1]);
//		delay_ms(2);
// 	}
//  return 0x00;
//}

HAL_StatusTypeDef Camera_StartCapture(void)
{
  return HAL_DCMI_Start_DMA(&hdcmi, DCMI_MODE_CONTINUOUS, (uint32_t)Image, DMA_BufferSize);
}

void Camera_Window_Set(uint16_t sx, uint16_t sy, uint16_t width, uint16_t height)
{
	uint16_t endx;
	uint16_t endy;
	uint8_t temp; 
	endx=sx+width*2;	//V*2
 	endy=sy+height*2;
	if(endy>784)endy-=784;
	Camera_ReadReg(0X03,&temp);				//READ Vref
	temp&=0XF0;
	temp|=((endx&0X03)<<2)|(sx&0X03);
	Camera_WriteReg(0X03,temp);				//??Vref?start?end???2?
	Camera_WriteReg(0X19,sx>>2);			//??Vref?start?8?
	Camera_WriteReg(0X1A,endx>>2);			//??Vref?end??8?

	Camera_ReadReg(0X32,&temp);				//??Href????
	temp&=0XC0;
	temp|=((endy&0X07)<<3)|(sy&0X07);
	Camera_WriteReg(0X32,temp);				//??Vref?start?end???2?
	Camera_WriteReg(0X17,sy>>3);			//??Href?start?8?
	Camera_WriteReg(0X18,endy>>3);			//??Href?end??8?
}
