/*
 模块：          带UART功能的HC-SR04（2020款）系列超声波测距模块   
 主控芯片：      RCWL-9300系列
 功能：          串口模式下读取HC-SR04（2020款）系列超声波测距模块距离数据并串口显示
 注意：          HC-SR04（2020款）系列模块选择成UART模式，即模式选择UART。
 编写：          无锡日晨物联科技有限公司
 方案定制：      18851501095
 串口数据格式：  RCWL-1661串口输出3位8BIT数据，距离MM值=24位数据转换成10进制/10000。
 连线：
   -VCC          = 5V /3.3V/3-40V   
   -TRIG/RX/SCL  = A5
   -ECHO/TX/SDA  = A4
   -GND          = GND 
*/
#include "SoftwareSerial.h"
SoftwareSerial mySerial(A4, A5);    // A4为RX, A5为TX;  A4接模块TX,A5接模块RX
float     Data_h = 0;                 //高8位数据
float     Data_m = 0;                 //中8位数据
float     Data_l = 0;                 //低8位数据
float     distance = 0;               //距离数据十进制值

void setup()
{
  Serial.begin(9600);                //定义串口波特率9600 出厂默认波特率9600
  mySerial.begin(9600);               //定义模拟串口波特率
  Serial.println("HC-SR04（2020款） 测距开始："); 
}

void loop() 
{
    Data_h = 0;
    Data_m = 0;   
    Data_l = 0;
    distance=0;
    mySerial.flush();                     //清模拟串口缓存数据
    mySerial.write(0XA0);                 //发送开始测试数据0XA0，0XA0为开始测试命令数据
   delay(120);                            //测量周期延时，一个周期为100mS,设置120MS,留余量    
   if (mySerial.available()>0)            //等待接收完3个数据
   {
    Data_h= mySerial.read();              //读取缓存数据
    Data_m= mySerial.read(); 
    Data_l= mySerial.read(); 
   }
  else
   {
    Data_h= 0;                             //读取缓存数据
    Data_m= 0; 
    Data_l= 0;     
    }
   distance=(Data_h*65536+Data_m*256+Data_l)/10000;   //计算成CM值     
   Serial.print("距离 : "); 
   if ((1<=distance)&&(900>=distance))     //1CM-9M之间数值显示 
    {
   Serial.print(distance);
   Serial.print(" CM ");  
    }
   else 
    {
   Serial.print(" - - - - ");             //无效数值数值显示 - - - - 
    }
   Serial.println();    
//   delay(100);                             //调节测量周期  
}


