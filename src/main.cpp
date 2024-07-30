#include "511.h"
String ssid="511";
String password = "guofangwei";
bool webonquest = 0;
bool weboffquest = 0;
bool isopen = 0;  // 是否开机
bool runonce = 1; // runonce
extern tm timeinfo;
extern Weather weatherinfo;
extern int calendarpic;
//构造U8g2字体库和GxEPD驱动
U8G2_FOR_ADAFRUIT_GFX ufont;
GxEPD2_3C<GxEPD2_750c_Z08, GxEPD2_750c_Z08::HEIGHT / 2> epaper(GxEPD2_750c_Z08(/*CS=D8*/ 15, /*DC=D3*/ 27, /*RST=D4*/ 26, /*BUSY=D2*/ 25));

//AsyncWebServer server(80);//打开异步服务器

/* setup --------------------------------------------------------------------*/
void setup()
{
  
  Serial.begin(115200);      // 打开串口监视
  //pinMode(LED_GPIO, OUTPUT); // 测试用LED灯

  //检测spiffs状态
  if (!SPIFFS.begin(true))
  {
    Serial.println("failed to load spiffs");
    while (1){};
  }
  // 配置SPI,初始化驱动,挂载u8g2字体库
  SPI.end();
  SPI.begin(13, 14, 14, 4);
  epaper.init(115200, true, 2, false);
  ufont.begin(epaper);
  ufont.setFontDirection(0);
  ufont.setForegroundColor(GxEPD_BLACK);     // 设置前景色
  ufont.setBackgroundColor(GxEPD_WHITE);     // 设置背景色
  ufont.setFont(u8g2_font_wqy16_t_gb2312);//设置字体为全中文库,16*16,注意ASCII间距和中文字体间距不同
  epaper.setFullWindow();
  epaper.firstPage();//清屏
  wificonnect(ssid, password);
  setTime();
  getTime(timeinfo);
  getWeather(weatherinfo, 0);
  do
  {
    calendarpage();
  } while (epaper.nextPage());
  while(1){

    getTime(timeinfo);
    if(timeinfo.tm_sec==0){
      epaper.firstPage();
        do{
          calendarpage();
      } while (epaper.nextPage());
      calendarpic = (calendarpic + 1) % 6;
    }

  }

  epaper.hibernate();
  deleteWeather(weatherinfo,0);




  //WiFi.softAP("esp32wifi", "guofangwei");//启动热点

  /*
  setup asyncwebserver
  
  server.serveStatic("/", SPIFFS, "/www/").setDefaultFile("index.html");
  server.on("/sw",HTTP_GET, [](AsyncWebServerRequest *req) {
    if(req->hasParam("epapersw")){
      String epapersw = req->getParam("epapersw")->value();
      if(epapersw=="on")
        webonquest = 1;
      else if(epapersw=="off")
        weboffquest = 1;
      
    }
  });
  server.on("/wificon", HTTP_GET, [](AsyncWebServerRequest *req) {
    if(req->hasParam("wifipass")){
      String ssid = req->getParam("ssid")->value();
      String wifipass = req->getParam("wifipass")->value();
      wificonnect(ssid, wifipass);
    }
  });
  server.onNotFound([](AsyncWebServerRequest *req)
            { req->send(200, "html/text", "<h1>404 Not Found</h1>"); });
  server.begin();
*/

}

/* The main loop -------------------------------------------------------------*/
void loop()
{
  // do nothing here
}
