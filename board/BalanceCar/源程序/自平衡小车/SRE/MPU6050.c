#include "MPU6050.h"
#include "USART.h"
#include <math.h>    //Keil library  
#include <stdio.h>   //Keil library	
#include "OLED.h" 
void I2C_delay(void)
{
		
   u8 i=6; //?aà??éò?ó??ˉ?ù?è	￡??-2aê?×?μíμ?5?1?üD′è?
   while(i) 
   { 
     i--; 
   }  
}
void delay5ms(void)
{
		
   int i=5000;  
   while(i) 
   { 
     i--; 
   }  
}
/*******************************************************************************
* Function Name  : I2C_Start
* Description    : Master Start Simulation IIC Communication
* Input          : None
* Output         : None
* Return         : Wheather	 Start
****************************************************************************** */
Myflag I2C_Start(void)
{
	SDA_H;
	SCL_H;
	I2C_delay();
	if(!SDA_read)return FALSE;	//SDA???aμíμ????ò×ü???|,í?3?
	SDA_L;
	I2C_delay();
	if(SDA_read) return FALSE;	//SDA???a??μ????ò×ü??3?′í,í?3?
	SDA_L;
	I2C_delay();
	return TRUE;
}
/*******************************************************************************
* Function Name  : I2C_Stop
* Description    : Master Stop Simulation IIC Communication
* Input          : None
* Output         : None
* Return         : None
****************************************************************************** */
void I2C_Stop(void)
{
	SCL_L;
	I2C_delay();
	SDA_L;
	I2C_delay();
	SCL_H;
	I2C_delay();
	SDA_H;
	I2C_delay();
} 
/*******************************************************************************
* Function Name  : I2C_Ack
* Description    : Master Send Acknowledge Single
* Input          : None
* Output         : None
* Return         : None
****************************************************************************** */
void I2C_Ack(void)
{	
	SCL_L;
	I2C_delay();
	SDA_L;
	I2C_delay();
	SCL_H;
	I2C_delay();
	SCL_L;
	I2C_delay();
}   
/*******************************************************************************
* Function Name  : I2C_NoAck
* Description    : Master Send No Acknowledge Single
* Input          : None
* Output         : None
* Return         : None
****************************************************************************** */
void I2C_NoAck(void)
{	
	SCL_L;
	I2C_delay();
	SDA_H;
	I2C_delay();
	SCL_H;
	I2C_delay();
	SCL_L;
	I2C_delay();
} 
/*******************************************************************************
* Function Name  : I2C_WaitAck
* Description    : Master Reserive Slave Acknowledge Single
* Input          : None
* Output         : None
* Return         : Wheather	 Reserive Slave Acknowledge Single
****************************************************************************** */
Myflag I2C_WaitAck(void) 	 //·μ???a:=1óDACK,=0?TACK
{
	SCL_L;
	I2C_delay();
	SDA_H;			
	I2C_delay();
	SCL_H;
	I2C_delay();
	if(SDA_read)
	{
      SCL_L;
	  I2C_delay();
      return FALSE;
	}
	SCL_L;
	I2C_delay();
	return TRUE;
}
/*******************************************************************************
* Function Name  : I2C_SendByte
* Description    : Master Send a Byte to Slave
* Input          : Will Send Date
* Output         : None
* Return         : None
****************************************************************************** */
void I2C_SendByte(u8 SendByte) //êy?Y′ó????μ?μí??//
{
    u8 i=8;
    while(i--)
    {
        SCL_L;
        I2C_delay();
      if(SendByte&0x80)
        SDA_H;  
      else 
        SDA_L;   
        SendByte<<=1;
        I2C_delay();
		SCL_H;
        I2C_delay();
    }
    SCL_L;
}  
/*******************************************************************************
* Function Name  : I2C_RadeByte
* Description    : Master Reserive a Byte From Slave
* Input          : None
* Output         : None
* Return         : Date From Slave 
****************************************************************************** */
unsigned char I2C_RadeByte(void)  //êy?Y′ó????μ?μí??//
{ 
    u8 i=8;
    u8 ReceiveByte=0;

    SDA_H;				
    while(i--)
    {
      ReceiveByte<<=1;      
      SCL_L;
      I2C_delay();
	  SCL_H;
      I2C_delay();	
      if(SDA_read)
      {
        ReceiveByte|=0x01;
      }
    }
    SCL_L;
    return ReceiveByte;
} 

//μ￥×??úD′è?*******************************************
Myflag Single_Write(unsigned char SlaveAddress,unsigned char REG_Address,unsigned char REG_data)		     //void
{
  	if(!I2C_Start())return FALSE;
    I2C_SendByte(SlaveAddress);   //·￠?íéè±?μ??·+D′D?o?//I2C_SendByte(((REG_Address & 0x0700) >>7) | SlaveAddress & 0xFFFE);//éè?????eê?μ??·+?÷?tμ??· 
    if(!I2C_WaitAck()){I2C_Stop(); return FALSE;}
    I2C_SendByte(REG_Address );   //éè??μí?eê?μ??·      
    I2C_WaitAck();	
    I2C_SendByte(REG_data);
    I2C_WaitAck();   
    I2C_Stop(); 
    delay5ms();
    return TRUE;
}

//μ￥×??ú?áè?*****************************************
unsigned char Single_Read(unsigned char SlaveAddress,unsigned char REG_Address)
{   
	unsigned char REG_data;     	
	if(!I2C_Start())return FALSE;
    I2C_SendByte(SlaveAddress); //I2C_SendByte(((REG_Address & 0x0700) >>7) | REG_Address & 0xFFFE);//éè?????eê?μ??·+?÷?tμ??· 
    if(!I2C_WaitAck()){I2C_Stop(); return FALSE;}
    I2C_SendByte((u8) REG_Address);   //éè??μí?eê?μ??·      
    I2C_WaitAck();
    I2C_Start();
    I2C_SendByte(SlaveAddress+1);
    I2C_WaitAck();

	REG_data= I2C_RadeByte();
    I2C_NoAck();
    I2C_Stop();
    //return TRUE;
	return REG_data;

}

void GPIO_6050_I2C_Config(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure; 
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	
//	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_8;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
//	GPIO_Init(GPIOA, &GPIO_InitStructure);
//	
//	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_9;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;  
//	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;  
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}

void MPU6050_Init(void)
{
	GPIO_6050_I2C_Config();							//3?ê??ˉ6050μ?I2C?ú
	Single_Write(MPU6050_Addr,PWR_MGMT_1, 0x00);	//?a3yDY??×′ì?,1?±????è′??D?÷
	Single_Write(MPU6050_Addr,SMPLRT_DIV, 0x07);	//1KHz/(6+1)7ms×a??ò?′?
	Single_Write(MPU6050_Addr,CONFIG, 0x06);		//μíí¨??2¨?÷μ??μ￡o0x01=188Hz 0x02=98Hz 0x03=42Hz 0x04=20Hz 0x05=10Hz 0x06=5Hz
	Single_Write(MPU6050_Addr,GYRO_CONFIG, 0x18);  	//íó?Yò?×??ì?°2aá?·??§￡?μ?Dí?μ￡o0x18(2?×??ì￡?2000deg/s)
	Single_Write(MPU6050_Addr,ACCEL_CONFIG, 0x01);	//?ó?ù??×??ì?￠2aá?·??§?°??í¨??2¨?μ?ê￡?μ?Dí?μ￡o0x01(2?×??ì￡?2G￡?5Hz)
}

Data_6050_str	Data_6050; 	//6050êy?Y?á11
float AccX,AccY,AccZ,GyroX,GyroY,GyroZ;;
uint8_t Temp1[20];
void READ_MPU6050(void)
{  	
	unsigned char BUF[12];       //?óê?êy?Y?o′???  ///10
	
	/*???ù?è′??D?÷2aá?*/
//	BUF[0]=Single_Read(MPU6050_Addr,GYRO_XOUT_L); 
//	BUF[1]=Single_Read(MPU6050_Addr,GYRO_XOUT_H);
//	Data_6050.GX=	(BUF[1]<<8)|BUF[0];
//	Data_6050.Gryo_X = (float)Data_6050.GX /16.4;  
    /*-----------------------------------------------------------*/
	BUF[2]=Single_Read(MPU6050_Addr,GYRO_YOUT_L);
	BUF[3]=Single_Read(MPU6050_Addr,GYRO_YOUT_H);
	Data_6050.GY=	(BUF[3]<<8)|BUF[2];
	Data_6050.Gryo_Y = (float)Data_6050.GY /16.4;//?úá?3ì2000?è/S
	/*-----------------------------------------------------------*/
//	BUF[4]=Single_Read(MPU6050_Addr,GYRO_ZOUT_L);
//	BUF[5]=Single_Read(MPU6050_Addr,GYRO_ZOUT_H);
//	Data_6050.GZ=	(BUF[5]<<8)|BUF[4];
//	Data_6050.Gryo_Z = (float)Data_6050.GZ /16.4;

	/*?ó?ù?è′??D?÷2aá?*/
	BUF[6]=Single_Read(MPU6050_Addr,ACCEL_XOUT_L); 
	BUF[7]=Single_Read(MPU6050_Addr,ACCEL_XOUT_H);
	Data_6050.AX=	(BUF[7]<<8)|BUF[6];
	Data_6050.Acc_X = (float)Data_6050.AX/16384;	//Acc = AX * 9.8/0x4000;
																//Angle = asin(Acc/9.8)*57.32 ; ???è??×a??57.32 = 180/3.14
	/*-----------------------------------------------------------*/
//	BUF[8]=Single_Read(MPU6050_Addr,ACCEL_YOUT_L); 
//	BUF[9]=Single_Read(MPU6050_Addr,ACCEL_YOUT_H);
//	Data_6050.AY=	(BUF[9]<<8)|BUF[8];
//	Data_6050.Acc_Y = (float)Data_6050.AY/16384;	//Acc = AY * 9.8/0x4000;
																//Angle = asin(Acc/9.8)*57.32 ; ???è??×a??57.32 = 180/3.14
	/*-----------------------------------------------------------*/
	BUF[10]=Single_Read(MPU6050_Addr,ACCEL_ZOUT_L); 
	BUF[11]=Single_Read(MPU6050_Addr,ACCEL_ZOUT_H);
	Data_6050.AZ=	(BUF[11]<<8)|BUF[10];
	Data_6050.Acc_Z = (float)Data_6050.AZ/16384;	//Acc = AZ * 9.8/0x4000;

//   GyroX = Data_6050.Gryo_X;
//   sprintf(Temp1,"GyroX: %4.2f  \r\n", GyroX);
//   USART_printf(USART1,Temp1);

   GyroY = Data_6050.Gryo_Y;
    if(GyroY>32768)  GyroY-=65536;     
   sprintf(Temp1,"GyroY: %4.2f  \r\n", GyroY);
   USART_printf(USART1,Temp1);

//   GyroZ = Data_6050.Gryo_Z;
//   sprintf(Temp1,"GyroZ: %4.2f  \r\n", GyroZ);
//   USART_printf(USART1,Temp1);


   AccX = Data_6050.Acc_X;
   if(AccX>32768) AccX-=65536;   
   sprintf(Temp1,"AccX: %4.2f  \r\n", AccX);
   USART_printf(USART1,Temp1);
//
//   AccY = Data_6050.Acc_Y;
//   sprintf(Temp1,"AccY: %4.2f  \r\n", AccY);
//   USART_printf(USART1,Temp1);

   AccZ = Data_6050.Acc_Z;
   if(AccZ>32768) AccZ-=65536;   
   sprintf(Temp1,"AccZ: %4.2f  \r\n", AccZ);
   USART_printf(USART1,Temp1);
//	  USART_printf(USART1,Temp);																//Angle = asin(Acc/9.8)*57.32 ; ???è??×a??57.32 = 180/3.14 
}
//
//float Pitch;
float pitch_temp1;

float K1 =0.15; 
float angle, angle_dot; 	
float Q_angle=0.003;// 过程噪声的协方差
float Q_gyro=0.0003;//0.003 过程噪声的协方差 过程噪声的协方差为一个一行两列矩阵
float R_angle=0.5;// 测量噪声的协方差 既测量偏差
float dt=0.005;//                 
char  C_0 = 1;
float Q_bias, Angle_err;
float PCt_0, PCt_1, E;
float K_0, K_1, t_0, t_1;
float Pdot[4] ={0,0,0,0};
float PP[2][2] = { { 1, 0 },{ 0, 1 } };


/**************************************************************************
函数功能：一阶互补滤波
入口参数：加速度、角速度
返回  值：无
**************************************************************************/
void Yijielvbo(float angle_m, float gyro_m)
{
   angle = K1 * angle_m+ (1-K1) * (angle + gyro_m * 0.005);
}
void Angle_Calcu()
{
 	int i = 0;

	static float pitch_sum = 0.0;
    for(i=0;i<3;i++)
		{
			READ_MPU6050();		
			
			pitch_temp1 = (atan((float)AccX/AccZ)*57.2958);   //??Pitch?? 0.4??????
			
			pitch_sum += pitch_temp1;
		}
		
		pitch_temp1 = pitch_sum / 3.0;	 //?????

		pitch_sum = 0.0;
		Yijielvbo(pitch_temp1,-GyroY);

		sprintf(Temp1,"%4.2f",angle);
        OLED_ShowString(5,2,Temp1,16);
		sprintf(Temp1,"%4.2f",Midle);
        OLED_ShowString(70,2,Temp1,16);
}
float kp=525,kd=32,Midle=4.55;	
/**************************************************************************
函数功能：直立PD控制
入口参数：角度、角速度
返回  值：直立控制PWM
**************************************************************************/
int balance(float Angle,float Gyro)
{  
   float Bias;		 //525 32    535   38
	 int balance;
	 Bias=Angle-Midle;       //===求出平衡的角度中值 和机械相关
	 balance=kp*Bias+Gyro*kd;   //===计算平衡控制的电机PWM  PD控制   kp是P系数 kd是D系数 
	 return balance;
}

