#include "display.h"
bool webonquest = 0;
bool weboffquest = 0;
bool isopen = 0;  // 是否开机
bool runonce = 1; // runonce
bool loadmainpage = 0;
extern tm timeinfo;
extern Weather weatherinfo;
extern int calendarpic;
extern String mycity;
#define coverheight 30
//AsyncWebServer server(80);//打开异步服务器
WebServer server(80);//开启Webserver服务
U8G2_FOR_ADAFRUIT_GFX ufont;
GxEPD2_3C<GxEPD2_750c_Z08, GxEPD2_750c_Z08::HEIGHT / 2> epaper(GxEPD2_750c_Z08(/*CS=D8*/ 15, /*DC=D3*/ 27, /*RST=D4*/ 26, /*BUSY=D2*/ 25));//构造U8g2字体库和GxEPD驱动
void setupepaper();
bool wificonnect(String ssid, String password);
void catchinfo();
void setupserver();
// init checking and setting
void setupepaper()
{
  Serial.begin(115200); // 打开串口监视
  if (!SPIFFS.begin(true))
  {
    Serial.println("failed to load spiffs");
    while (1){};
  }// 检测spiffs状态
  SPI.end();
  SPI.begin(13, 14, 14, 4);// 配置SPI,初始化驱动,挂载u8g2字体库
  epaper.init(115200, true, 2, false);
  epaper.setTextWrap(1);
  epaper.setTextColor(black);
  ufont.setFontMode(1);
  ufont.begin(epaper);
  ufont.setFontDirection(0);
  ufont.setForegroundColor(GxEPD_BLACK); // 设置前景色
  ufont.setBackgroundColor(GxEPD_WHITE); // 设置背景色
  ufont.setFont(defaultFONT);         // 设置字体为全中文库,16*16,注意ASCII间距和中文字体间距不同
}
bool wificonnect(String ssid, String password)
{
  Serial.println("Connecting to    ");
  Serial.print(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult(5000) != WL_CONNECTED)
  {
    Serial.println("WiFi Failed!");
    WiFi.mode(WIFI_AP);
    epaper.setPartialWindow(200, 300, 400, 100);
    ufont.setFont(u8g2_font_crox2c_tf);
    epaper.firstPage();
    do
    { 
      ufont.setCursor((800 - ufont.getUTF8Width("WiFi Failed!")) / 2, 300 + 2*prevfontheight);
      ufont.print("WiFi Failed");
    } while (epaper.nextPageBW());
    return 0;
  }
  epaper.setPartialWindow(200, 300, 400, 100);
  ufont.setFont(u8g2_font_crox2c_tf);
  epaper.firstPage();
  do
  {
    ufont.setCursor((800 - ufont.getUTF8Width("Connect successful!")) / 2, 300 + 2 * prevfontheight);
    ufont.print("Connect successful!");
    ufont.setFont(defaultFONT);
    ufont.setCursor((800 - ufont.getUTF8Width("获取网络信息中...")) / 2, 300 + 3 * prevfontheight);
    ufont.print("获取网络信息中...");
  } while (epaper.nextPageBW());
  Serial.println("Connection sucessful");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  catchinfo();//抓取联网时间和天气信息
  return 1;
}

//setup wifi and fetch internet info
void catchinfo(){
 // wificonnect(ssid, password);
  setTime();
  getTime(timeinfo);
  getWeather(weatherinfo, 4);
  server.stop();
  loadmainpage = 1;//在loop中显示主界面并根据时间刷新
}
//build AP server
void setupserver(){
  server.serveStatic("/", SPIFFS, "/www/index.html");
  server.on("/wificon", HTTP_GET, []()
            {
              
    if(!server.arg("wifipass").isEmpty()&&!server.arg("ssid").isEmpty()){
      String ssid = server.arg("ssid");
      String wifipass = server.arg("wifipass");
      String reply = "CONNECTING TO "+ssid;
      epaper.setPartialWindow(200, 300, 400, 100);
      ufont.setFont(u8g2_font_crox2c_tf);
      epaper.firstPage();
      do
      {
        ufont.setCursor((800-ufont.getUTF8Width(reply.c_str()))/2, 300 + prevfontheight);
        ufont.print(reply);
      } while (epaper.nextPageBW());
      delay(3000);
      wificonnect(ssid, wifipass);
      
    } 
    if(!server.arg("city").isEmpty()){
      mycity = server.arg("city");
    }
    });

  server.onNotFound([]()
                    { server.send(200, "html/text", "<h1>404 Not Found</h1>"); });
      server.begin();
}

void setup()
{
  setupepaper();
  int _mode = 2;
  switch(_mode){
    case 0://clearpage
      epaper.setFullWindow();
      epaper.firstPage();
      do
      {
        epaper.clearScreen();
      } while (epaper.nextPage());
      break;
    case 1://main
      WiFi.softAP("esp32ap", "esp32pass"); // 启动热点
      setupserver();
      welcomepage();
      break;
    case 2://dev test
      wificonnect("511", "guofangwei");
      epaper.setFullWindow();
      //init temp graph
      int midtemp = (weatherinfo.nextday->predictdaytemp.substring(0, weatherinfo.predictdaytemp.indexOf('/')).toInt() + weatherinfo.nextday->predictdaytemp.substring(weatherinfo.predictdaytemp.indexOf('/')+1).toInt())/2;
      temppoint tpt1, tpt2, tpt3;
      epaper.firstPage();
      String monthstr;
      switch (timeinfo.tm_mon)
      {
      case 0:
        monthstr = "January ";
        break;
      case 1:
        monthstr = "February ";
        break;
      case 2:
        monthstr = "March ";
        break;
      case 3:
        monthstr = "April ";
        break;
      case 4:
        monthstr = "May ";
        break;
      case 5:
        monthstr = "June ";
        break;
      case 6:
        monthstr = "July ";
        break;
      case 7:
        monthstr = "Augest ";
        break;
      case 8:
        monthstr = "September ";
        break;
      case 9:
        monthstr = "October ";
        break;
      case 10:
        monthstr = "November ";
        break;
      case 11:
        monthstr = "December ";
        break;
      default:;
      }
      monthstr += timeinfo.tm_mday;
      do
      {
        ufont.setFont(u8g2_font_lubB18_tr);
        ufont.drawStr((epaper.width() - ufont.getUTF8Width(monthstr.c_str())) / 2, coverheight + prevfontheight, monthstr.c_str());
        weathercard(110, 100, midtemp, tpt1, *weatherinfo.nextday->nextday,1);
        weathercard(340, 100, midtemp, tpt2, *weatherinfo.nextday->nextday->nextday,2);
        weathercard(570, 100, midtemp, tpt3, *weatherinfo.nextday->nextday->nextday->nextday,3);
        templine(tpt1, tpt2, tpt3);
      } while (epaper.nextPage());
      while(1){}
      break;
  }
}

void loop()
{ 
  server.handleClient();
  if(loadmainpage){
    getTime(timeinfo);
    if(runonce){
      calendarpage();
      runonce = 0;
    }
    if(timeinfo.tm_sec==0&&timeinfo.tm_min%5!=0){
      clockupdate();
    }
    if (timeinfo.tm_min % 10 == 0&&timeinfo.tm_sec==0){
      calendarpic++;calendarpic %= 6;
      calendarpage();
    }
  }
  delay(200);
}
