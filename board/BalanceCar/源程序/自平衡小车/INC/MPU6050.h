#ifndef _MPU6050_H_
#define _MPU6050_H_

#include "stm32f10x.h"
#include "math.h"    //Keil library 
#include "stdio.h" 	

#define I2C_SCL		  GPIO_Pin_10
#define I2C_SDA		  GPIO_Pin_11
#define I2C_SCL_PORT      GPIOC
#define I2C_SDA_PORT      GPIOC

#define SCL_H         I2C_SCL_PORT->BSRR |= I2C_SCL
#define SCL_L         I2C_SCL_PORT->BRR  |= I2C_SCL 
   
#define SDA_H         I2C_SDA_PORT->BSRR |= I2C_SDA
#define SDA_L         I2C_SDA_PORT->BRR  |= I2C_SDA

#define SCL_read      I2C_SCL_PORT->IDR  & I2C_SCL
#define SDA_read      I2C_SDA_PORT->IDR  & I2C_SDA
typedef enum {TRUE=1,FALSE=0}Myflag;
// ?����?MPU6050?��2?��??��
//****************************************
#define	SMPLRT_DIV		0x19	//����?Y��?2��?��?����?��?D��?�̡�o0x07(125Hz)
#define	CONFIG			0x1A	//�̨�����??2��?��?����?��?D��?�̡�o0x06(5Hz)
#define	GYRO_CONFIG		0x1B	//����?Y��?��??��?��2a��?��??���?��?D��?�̡�o0x18(2?��??����?2000deg/s)
#define	ACCEL_CONFIG	0x1C	//?��?��??��??��?��2a��?��??��?��??����??2��?��?����?��?D��?�̡�o0x01(2?��??����?2G��?5Hz)

#define	PWR_MGMT_1		0x6B	//��??��1��������?��?D��?�̡�o0x00(?y3��??��?)
#define	WHO_AM_I		0x75	//IIC�䨮��??��??��??��(??��?��y?��0x68��????��)

#define	ACCEL_XOUT_H	0x3B
#define	ACCEL_XOUT_L	0x3C
#define	ACCEL_YOUT_H	0x3D
#define	ACCEL_YOUT_L	0x3E
#define	ACCEL_ZOUT_H	0x3F
#define	ACCEL_ZOUT_L	0x40
#define	TEMP_OUT_H		0x41
#define	TEMP_OUT_L		0x42

#define	GYRO_XOUT_H		0x43
#define	GYRO_XOUT_L		0x44	
#define	GYRO_YOUT_H		0x45
#define	GYRO_YOUT_L		0x46
#define	GYRO_ZOUT_H		0x47
#define	GYRO_ZOUT_L		0x48

#define	MPU6050_Addr   	0xD0	  //?����??��?t?��IIC����???D��?�䨮��??��,?��?YALT  ADDRESS��??����y??2?��?DT??
//?����?��??D?�¨�y?Y��?DT?y?��
#define ACCX_ADJ		6.5
#define ACCY_ADJ		0
#define ACCZ_ADJ		0
#define GRYOX_ADJ		0
#define GRYOY_ADJ		0.09
#define GRYOZ_ADJ		0

typedef struct{
	int16_t GX;	int16_t GY;	int16_t GZ;					   //x,y,z?��???��?��??��??��?��
	float Gryo_X;float Gryo_Y;float Gryo_Z;	   //X,Y,Z?��???��?����?3??��
	int16_t AX;	int16_t AY;	int16_t AZ;					   //x,y,z?��?��?��?��??��??��?��
	float Acc_X; float Acc_Y; float Acc_Z;	   //x,y,z?��?��?��?����?3??��
	}Data_6050_str;

extern float AccX,AccY,AccZ,GyroX,GyroY,GyroZ;;
extern float angle;
extern float pitch_temp1;
extern float kp,kd,Midle;
/* o����y����?�� -----------------------------------------------*/
void MPU6050_Init(void);
void READ_MPU6050(void);
void Angle_Calcu(void);
int balance(float Angle,float Gyro);

#endif
