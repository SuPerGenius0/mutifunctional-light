#include <IRremote.h>
#include <Wire.h>//调用收发数据所使用的库函数
#include "I2Cdev.h"
#include <SoftwareSerial.h>
 #include <SHT2x.h>
#include <Adafruit_NeoPixel.h>
#include "audio.h"
#define INTERVAL_Time 10
#define music_num_MAX 3 //最多存放3首歌曲
#define humanHotSensor 4//PIR传感器D4

#define PIN 6//彩灯
#include <U8glib.h>//使用OLED需要包含这个头文件
#define INTERVAL_LCD 20 //定义OLED刷新时间间隔 
unsigned long lcd_time = millis(); //OLED刷新时间计时器
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE); //设置OLED型号 
//-------字体设置，大、中、小
#define setFont_L u8g.setFont(u8g_font_7x13)
#define setFont_M u8g.setFont(u8g_font_fixed_v0r)
#define setFont_S u8g.setFont(u8g_font_fixed_v0r)
#define setFont_SS u8g.setFont(u8g_font_fub25n)
int RECV_PIN = 10;   //红外线接收器OUTPUT端接在pin 10
IRrecv irrecv(RECV_PIN);  //定义IRrecv对象来接收红外线信号
decode_results results;   //解码结果放在decode_results构造的对象results里
Adafruit_NeoPixel strip = Adafruit_NeoPixel(2, PIN, NEO_GRB + NEO_KHZ800);
#define temp1    24//#define定义常量
#define temp2    28
float sensor_tem;//float定义基本数据单精度常量
void colorSet(uint32_t);
void TemRead();
bool humanHotState = false;
boolean on_off;
boolean statusChange;
int pos = 0;
boolean music_status = false; //歌曲播放状态
int music_num = 1;    //歌曲序号
int music_vol = 30;             //音量0~30
void setup() {
  strip.begin();   //准备对灯珠进行数据发送
  strip.show(); // Initialize all pixels to 'off'
  Serial.begin(9600);
  pinMode(humanHotSensor, INPUT); 
  audio_init(4,2, music_vol);   //初始化mp3模块
  irrecv.enableIRIn(); // 启动红外解码
}

void loop() { TemRead();//调用自定义函数
  humanHotState = digitalRead(humanHotSensor);
  Serial.println(humanHotState);
  // print out the state of the button:
  //Serial.println(humanHotState);
  delay(1000);        // delay in between reads for stability
if (humanHotState) {strip.setPixelColor(0, strip.Color(255, 255, 255));//白光
  strip.show();   //LED显示 
if (irrecv.decode(&results)) {
    Serial.println(results.value,DEC);
    u8g.firstPage();
    do {
        setFont_L;
        u8g.setPrintPos(0, 10);
 u8g.print(sensor_tem);
    u8g.setPrintPos(55, 10);
 u8g.print(SHT2x.readRH());
  if (sensor_tem <= temp1)//if判断语句
   {strip.setPixelColor(0, strip.Color(0, 0, 255));//蓝光
  strip.show();   //LED显示
  u8g.setPrintPos(0, 25);
   u8g.print("Put on more clothes!");
   music_num=1;
   }
  else if (sensor_tem > temp1 && sensor_tem <= temp2)//三个逻辑运算符，>表示大于，&&表示逻辑关系“与”，<=表示大于等于
   {strip.setPixelColor(0, strip.Color(0, 255, 0));//绿光
   strip.show();
  u8g.setPrintPos(0, 25);
   u8g.print("It is just fine.");
   music_num=2;}   //LED显示
  else
  {strip.setPixelColor(0, strip.Color(255, 0, 0));//红光
  strip.show();
  u8g.setPrintPos(0, 25);
   u8g.print("Don't dress like ");
   u8g.setPrintPos(0, 40);
   u8g.print("an Eskimo");
   music_num=3;}  //LED显示
 }while( u8g.nextPage() );
    irrecv.resume();     //音频状态为工作
    music_status=false;  };  //接收下一个值
    if (music_status==false) {audio_choose(music_num);

      audio_play();

      //delay(500);      //延迟0。5秒
      music_status = true; } 
delay(5000);}
else{
    strip.setPixelColor(0, strip.Color(0, 0, 0));//关灯
    strip.show();
   } 

      }
void TemRead()
{
  sensor_tem = SHT2x.readT();//把获得的温度值赋给变量sensor_tem
  Serial.println(sensor_tem);//将数据从Arduino传递到PC且单独占据一行，此数据可在串口监视器中看到
  Serial.print("--");   
  Serial.println(SHT2x.readRH());
  delay(100);
}
