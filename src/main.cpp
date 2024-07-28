#include "511.h"
String ssid;
String password;
bool webonquest = 0;
bool weboffquest = 0;
bool isopen = 0;  // 是否开机
bool runonce = 1; // runonce
extern tm timeinfo;
extern Weather weatherinfo;
AsyncWebServer server(80);

/* setup --------------------------------------------------------------------*/
void setup()
{
  frame pg;
  Serial.begin(115200);      // 打开串口监视
  pinMode(LED_GPIO, OUTPUT); // 测试用LED灯

  //检测spiffs状态
  if (!SPIFFS.begin(true))
  {
    Serial.println("failed to load spiffs");
    while (1){};
  }

  WiFi.softAP("esp32wifi", "guofangwei");
  /*
  setup asyncwebserver
  */
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

  while (1)
  {
    if (webonquest == 1)
    {
      isopen = 1;
      webonquest = 0;
    }
    if (weboffquest == 1)
    {
      weboffquest = 0;
      // 关机的动作
      epaperinit();
      pg.color(black);
      pg.clear();
      pg.color(red);
      pg.clear();
      pg.printstr(epaperw / 2 - 10 * 14 / 2, epaperh / 2 - 20 / 2, "Quiting...", 0, 3, 1);
      pg.display();
      isopen = 0;
      runonce = 1;
      ClearPage();
      SleepMode();
    }
    if (isopen)
    {
      // epaperinit();
      getTime(timeinfo);
      if (runonce)
      {
        // 开机后仅运行一次
        epaperinit();
        //---
        calendar(pg);
        //---
        runonce = 0;
        SleepMode();
      }
      if (timeinfo.tm_sec == 0)
      {
        // 刷新时间
        epaperinit();
        //---
        calendar(pg);
        //---
        SleepMode();
        runonce = 0;
      }
    }
    delay(100);
  }
}

/* The main loop -------------------------------------------------------------*/
void loop()
{
  // do nothing here
}
