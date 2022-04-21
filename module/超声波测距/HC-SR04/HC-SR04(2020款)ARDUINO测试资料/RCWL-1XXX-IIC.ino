
/*
 模块：          带IIC功能的HC-SR04（2020款）系列超声波测距模块  
 主控芯片：      RCWL-9300系列
 功能：          IIC模式下读取HC-SR04（2020款）系列超声波测距模块距离数据并串口显示
 注意：          HC-SR04（2020款）模块选择成IIC模式，即模式选择IIC部分接10K电阻。
 编写：          无锡日晨物联科技有限公司
 方案定制：      18851501095
 串口数据格式：  RCWL-1661的IIC输出3位8BIT数据，距离MM值=24位数据转换成10进制/10000。
 连线：
   -VCC          = 5V /3.3V/3-40V  
   -TRIG/RX/SCL  = A5
   -ECHO/TX/SDA  = A4
   -GND          = GND 
*/

#include "SoftwareSerial.h"
#include <Wire.h> 
float               distance = 0;                       //距离数据十进制值
float               ds[3]; 

void setup()
{
  Serial.begin(9600);                                  //定义串口波特率9600 出厂默认波特率9600
  Wire.begin();
  Serial.println("HC-SR04（2020款）系列-IIC 测距开始："); 
}


void loop() 
{
   char i = 0;
    ds[0]=0;
    ds[1]=0;
    ds[2]=0;
   Wire.beginTransmission(0x57);                        //地址为0X57 写8位数据为AE，读8位数据位AF
   Wire.write(1);                                       //写命令0X01,0X01为开始测量命令 
   Wire.endTransmission();            
   delay(120);                                          //测量周期延时，一个周期为100mS,设置120MS,留余量    
   Wire.requestFrom(0x57,3);                            //地址为0X57 读3个8位距离数据       
    while (Wire.available())
    {
     ds[i++] = Wire.read();
    }          
   distance=(ds[0]*65536+ds[1]*256+ds[2])/10000;        //计算成CM值     
   Serial.print("距离 : "); 
   if ((1<=distance)&&(900>=distance))                  //1CM-9M之间数值显示 
    {
   Serial.print(distance);
   Serial.print(" CM ");  
    }
   else 
    {
   Serial.print(" - - - - ");                          //无效数值数值显示 - - - - 
    }
   Serial.println();    
   delay(100);                                          //调节测量周期  
}


