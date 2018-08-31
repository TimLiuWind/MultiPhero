#include "MPU9250.h"
#include "sI2C.h"

//uint8_t MPU_WriteReg(uint8_t regID, uint8_t regDat)
//{
//	uint8_t a=4;
//	a = sI2C_Start();
//	if(!a)
//	{
//		if(sI2C_Write(0xD0)==0)
//		{
//			sI2C_Stop();
//			return 1;
//		}
//		delay_us(150);
//		if(sI2C_Write(regID)==0)
//		{
//			sI2C_Stop();
//			return 2;
//		}
//		delay_us(150);
//		if(sI2C_Write(regDat)==0)
//		{
//			sI2C_Stop();
//			return 3;
//		}
//		sI2C_Stop();
//		return 0;
//	}
//	else
//		return 1;
//}

//uint8_t MPU_ReadReg(uint8_t regID, __IO uint8_t *regDat)
//{
//	
//	sI2C_Start();
//	if(sI2C_Write(0xD0)==0)
//	{
//		sI2C_Stop();
//		return 1;
//	}
//	delay_us(150);
//  if(sI2C_Write(regID)==0)
//	{
//		sI2C_Stop();
//		return 2;
//	}
////	sI2C_Stop();
//	delay_us(150);	
//	
//	sI2C_Start();
//	if(sI2C_Write(0xD1)==0)
//	{
//		sI2C_Stop();
//		return 3;
//	}
//	delay_us(150);
//	*regDat=sI2C_Read();
//	sI2C_noAck();
//	sI2C_Stop();
//	return 0;
//}

uint8_t MPU_WriteReg(uint8_t regID, uint8_t regDat)
{
	if(sI2C_Start())
	{
		sI2C_Stop();
		
		return 1;
	}
	sI2C_Write(0xD0);   //I2C_SendByte(((REG_Address & 0x0700) >>7) | SlaveAddress & 0xFFFE);
	if(sI2C_WaitAck())
	{
		sI2C_Stop(); //got no ack, busy or error
		return 2;
	}
	sI2C_Write(regID);
	if(sI2C_WaitAck())
	{
		sI2C_Stop(); //got no ack, busy or error
		return 2;
	}
	sI2C_Write(regDat);
	if(sI2C_WaitAck())
	{
		sI2C_Stop(); //got no ack, busy or error
		return 2;
	}
	sI2C_Stop();

	return 0;
}

uint8_t MPU_ReadReg(uint8_t regID, __IO uint8_t *regDat)
{
	
	if(sI2C_Start())
	{
		sI2C_Stop();

		return 1;
	}
	sI2C_Write(0xD0);   //I2C_SendByte(((REG_Address & 0x0700) >>7) | SlaveAddress & 0xFFFE);
	if(sI2C_WaitAck())
	{
		sI2C_Stop(); //got no ack, busy or error
		return 2;
	}
	sI2C_Write(regID);
	if(sI2C_WaitAck())
	{
		sI2C_Stop(); //got no ack, busy or error
		return 2;
	}
	
	if(sI2C_Start())
	{
		sI2C_Stop();
		return 1;
	}
	sI2C_Write(0xD0+1);
	if(sI2C_WaitAck())
	{
		sI2C_Stop(); //got no ack, busy or error
		return 2;
	}
	*regDat = sI2C_Read();
	sI2C_NoAck();
	sI2C_Stop();
	return 0;
}

uint8_t MPU_WriteBits(uint8_t reg, uint8_t bitStart, uint8_t length, uint8_t data)
{
  uint8_t b;
	if(MPU_ReadReg(reg, &b) ==0)
	{
		uint8_t mask = (0xFF << (bitStart + 1)) | 0xFF >> ((8 - bitStart) + length - 1);	
		data <<= (8 - length);
		data >>= (7 - bitStart);
		b &= mask;
		b |= data;
	return MPU_WriteReg(reg, b);
	}else {
		return 1;
	}
}

uint8_t MPU_WriteBit(uint8_t reg, uint8_t bitNum, uint8_t data)
{
	uint8_t b;
	if(MPU_ReadReg(reg, &b)) return 1;
	b = (data != 0) ? (b | (1 << bitNum)) : (b & ~(1 << bitNum));

	return MPU_WriteReg(reg, b);	
}

uint8_t MPU9250_init()
{
//	 printf("{initing MPU9250. Keep the board steady.}\r\n");

	//setClockSource(MPU9250_CLOCK_PLL_XGYRO)
	 if(MPU_WriteBits(MPU9250_RA_PWR_MGMT_1, MPU9250_PWR1_CLKSEL_BIT, MPU9250_PWR1_CLKSEL_LENGTH, MPU9250_CLOCK_PLL_XGYRO))return 1;
	//setFullScaleGyroRange(MPU9250_GYRO_FS_1000);
	 MPU_WriteBits(MPU9250_RA_GYRO_CONFIG, MPU9250_GCONFIG_FS_SEL_BIT, MPU9250_GCONFIG_FS_SEL_LENGTH, MPU9250_GYRO_FS_1000);
	//setFullScaleAccelRange(MPU9250_ACCEL_FS_2);
	 MPU_WriteBits(MPU9250_RA_ACCEL_CONFIG, MPU9250_ACONFIG_AFS_SEL_BIT, MPU9250_ACONFIG_AFS_SEL_LENGTH, MPU9250_ACCEL_FS_2);
//setSleepEnabled(0);
	 MPU_WriteBit(MPU9250_RA_PWR_MGMT_1, MPU9250_PWR1_SLEEP_BIT, 0);

//	MPU_selfCalibration_run(hObj);
	
  return 0;
}

float MPU9250_getAx(void)
{
	uint8_t accxouth,accxoutl;
	float x;
	short xx;
	MPU_ReadReg(MPU9250_RA_ACCEL_XOUT_H,&accxouth);
	MPU_ReadReg(MPU9250_RA_ACCEL_XOUT_L,&accxoutl);
	xx=((accxouth<<8)|accxoutl);
	x=xx/16384.0f;
	return x;
}

float MPU9250_getAy(void)
{
	uint8_t accxouth,accxoutl;
	float x;
	short xx;
	MPU_ReadReg(MPU9250_RA_ACCEL_YOUT_H,&accxouth);
	MPU_ReadReg(MPU9250_RA_ACCEL_YOUT_L,&accxoutl);
	xx=((accxouth<<8)|accxoutl);
	x=xx/16384.0f;
	return x;
}
float MPU9250_getAz(void)
{
	uint8_t accxouth,accxoutl;
	float x;
	short xx;
	MPU_ReadReg(MPU9250_RA_ACCEL_ZOUT_H,&accxouth);
	MPU_ReadReg(MPU9250_RA_ACCEL_ZOUT_L,&accxoutl);
	xx=((accxouth<<8)|accxoutl);
	x=xx/16384.0f;
	return x;
}

float MPU9250_getGx(void)
{
	uint8_t accxouth,accxoutl;
	float x;
	short xx;
	MPU_ReadReg(MPU9250_RA_GYRO_XOUT_H,&accxouth);
	MPU_ReadReg(MPU9250_RA_GYRO_XOUT_L,&accxoutl);
	xx=((accxouth<<8)|accxoutl);
	x=xx/656.0f;
	return x;
}

float MPU9250_getGy(void)
{
	uint8_t accxouth,accxoutl;
	float x;
	short xx;
	MPU_ReadReg(MPU9250_RA_GYRO_YOUT_H,&accxouth);
	MPU_ReadReg(MPU9250_RA_GYRO_YOUT_L,&accxoutl);
	xx=((accxouth<<8)|accxoutl);
	x=xx/656.0f;
	return x;
}
float MPU9250_getGz(void)
{
	uint8_t accxouth,accxoutl;
	float x;
	short xx;
	MPU_ReadReg(MPU9250_RA_GYRO_ZOUT_H,&accxouth);
	MPU_ReadReg(MPU9250_RA_GYRO_ZOUT_L,&accxoutl);
	xx=((accxouth<<8)|accxoutl);
	x=xx/656.0f;
	return x;
}

float MPU9250_getTemp(void)
{
	uint8_t accxouth,accxoutl;
	float x;
	short xx;
	MPU_ReadReg(MPU9250_RA_TEMP_OUT_H,&accxouth);
	MPU_ReadReg(MPU9250_RA_TEMP_OUT_L,&accxoutl);
	xx=((accxouth<<8)|accxoutl);
	x=xx/340.0f+36.53f;
	return x;
}

void IMU_getValues(IMUDataTypedef* hObj) 
{
	hObj->ax = MPU9250_getAx();
	hObj->ay = MPU9250_getAy();
	hObj->az = MPU9250_getAz();
	hObj->gx = MPU9250_getGx() - hObj->gxoffset;
	hObj->gy = MPU9250_getGy() - hObj->gyoffset;
	hObj->gz = MPU9250_getGz() - hObj->gzoffset;
	hObj->tempC = MPU9250_getTemp();
	hObj->gxsum+=hObj->gx;
	hObj->gysum+=hObj->gy;
	hObj->gzsum+=hObj->gz;
	hObj->sum_count++;
//    acc_gyro_mag[6] = 0;
//    acc_gyro_mag[7] = 0;
//    acc_gyro_mag[8] = 0;
}

