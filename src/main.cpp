#include "511.h"
String ssid;
String password;
bool webonquest = 0;
bool weboffquest = 0;
bool isopen = 0;  // 是否开机
bool runonce = 1; // runonce
extern tm timeinfo;
extern Weather weatherinfo;
WebServer server(80);
void handleroot()
{
  String HTML = "<!DOCTYPE html>\
<html lang='en'>\
<head>\
    <meta charset='UTF-8'>\
    <meta name='viewport' content='width=device-width, initial-scale=1.0'>\
    <title>EpaperSetup</title>\
</head>\
<body>\
    <h1>Welcome to Epaper511</h1>\
    <button onclick='sw(1)'>开机</button>\
    <button onclick='sw(0)'>关机</button>\
    <br>\
    <br>\
<span style='color: yellowgreen; font-size: larger;''>请先进行配网:</span>\
    <form action='' method='get' style='display: inline-block;'>\
        <label for='ssid'>WiFi名:</label>\
        <input type='text' name='ssid' id='' placeholder='input wifi ssid'>\
        <label for='password'>密码:</label>\
        <input type='text' name='password' placeholder='input wifi password'>\
        <input type='submit'>\
    </form>\
    <script>\
        let xhttp=new XMLHttpRequest();\
        function sw(key) {\
            if(key==1){\
                xhttp.open('GET','?switch=on',true);\
                xhttp.send();\
            }else if(key==0){\
                xhttp.open('GET','?switch=off',true);\
                xhttp.send();\
            }\
        }\
    </script>\
</body>\
</html>"; // 将html代码放进这个字符串里面
  server.send(200, "text/html", HTML);
  String state = server.arg("switch");
  if (state == "on")
  {
    webonquest = 1;
    server.send(200, "text/html", "epaper is on");
  }
  else if (state == "off")
  {
    weboffquest = 1;
    server.send(200, "text/html", "epaper is off");
  }
  ssid = server.arg("ssid");
  password = server.arg("password");
  Serial.println(ssid);
  Serial.println(password);
  if (!ssid.isEmpty())
  {
    server.send(200, "text/html", "Connecting to wifi");
    wificonnect(ssid, password);
    setTime();
    getWeather(weatherinfo, 2);
    webonquest = 1;
  }
}

void ledswitch()
{
  String state = server.arg("led"); //<key>
  if (state == "on")
  {
    digitalWrite(LED_GPIO, HIGH);
    server.send(200, "text/html", "led is on");
  }
  else if (state == "off")
  {
    digitalWrite(LED_GPIO, LOW);
    server.send(200, "text/html", "led is off");
  }
}
/* setup --------------------------------------------------------------------*/
void setup()
{
  frame pg;
  Serial.begin(115200);      // 打开串口监视
  pinMode(LED_GPIO, OUTPUT); // 测试用LED灯
  WiFi.softAP("esp32wifi", "guofangwei");
  server.on("/", handleroot);
  server.on("/led", ledswitch);
  server.onNotFound([]()
                    { server.send(200, "text/html", "<h1>404 Not Found</h1>"); });
  server.begin();

  while (1)
  {

    server.handleClient();
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
      delay(1000);
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
  server.handleClient();
  // do nothing here
}
