/*
 模块：          HC-SR04（2020款）系列超声波测距模块   
 主控芯片：      RCWL-9300系列
 功能：          GPIO模式下HC-SR04（2020款）系列超声波测距模块距离数据并串口显示
 注意：          HC-SR04（2020款）系列超声波测距模块默认GPIO模式，兼容HC-SR04。
 编写：          无锡日晨物联科技有限公司
 方案定制：      18851501095
 连线：
   -VCC          = 5V /3.3V/3-40V   //HC-SR04（2020款）可工作到40V，测试时需外接电源，注意12V以上要应用要加限流电阻。
   -TRIG/RX/SCL  = A5
   -ECHO/TX/SDA  = A4
   -GND          = GND
*/
float       distance;
const int   echo=A4;                         //echO接A4脚
const int   trig=A5;                         //echO接A5脚
void setup()
{
  Serial.begin(9600);                       //波特率9600
  pinMode(echo,INPUT);                       //设置echo为输入脚
  pinMode(trig,OUTPUT);                      //设置trig为输出脚
  Serial.println(" RCWL-1XXX-TTL 测距开始：");
}
void loop()
{
 digitalWrite(trig,HIGH);
 delayMicroseconds(1);
 digitalWrite(trig,LOW); 
 distance  = pulseIn(echo,HIGH);             //计数接收高电平时间
 distance  = distance*340/2/10000;           //计算距离 1：声速：340M/S  2：实际距离为1/2声速距离 3：计数时钟为1US//温补公式：c=(331.45+0.61t/℃)m•s-1 (其中331.45是在0度）
 Serial.print("距离: ");
 Serial.print(distance);
 Serial.println("CM");
 delay(30);                                  //单次测离完成后加30mS的延时再进行下次测量。防止近距离测量时，测量到上次余波，导致测量不准确。
 delay(100);                                 //延时100mS再次测量，延时可不要
} 


