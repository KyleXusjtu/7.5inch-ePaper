// 511.h实现文件
#include "511.h"
tm timeinfo;         // 时间结构体
Weather weatherinfo; // 天气结构体
int calendarpic = 0; // 选择显示的图片

/*******************网络模块**************************/
// WiFi 配置
// const char* ssid="511";
// const char* password ="guofangwei";
// api配置
String weatherurl = "http://apis.juhe.cn/simpleWeather/query";
String weathercity = "上海"; // 修改天气位置
// String key="ef670a1bc22e8ac69bdb5cd12716bd39";


bool wificonnect(String ssid, String password)
{
  Serial.println("Connecting to    ");
  Serial.print(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult(3000) != WL_CONNECTED)
  {
    Serial.println("WiFi Failed!");
    return 0;
  }

  Serial.println("Connection sucessful");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  return 1;
}

// 通过API获得最新北京时间戳,更新RTC模块
void setTime()
{
  // 发送网络请求
  HTTPClient http;
  // http.begin(url+"?city="+city+"&key="+key);
  bool isgettime = 1;
  // Serial.print("联网获取时间中\r\n");
  while (isgettime)
  {
    http.begin("http://apis.juhe.cn/fapigx/worldtime/query?key=91e19239e8956af5027136df3c5cfe64&city=上海");
    int http_code = http.GET();
    if (http_code == 200)
    {
      isgettime = 0;
      // Serial.print("获取成功\r\n");
    }
    else
    {
      Serial.print("获取失败,十秒后重新请求\r\n");
    }
    delay(10000);
  }
  String timeresponse = http.getString();
  http.end();
  // 解析json数据
  DynamicJsonDocument doc(1024);
  deserializeJson(doc, timeresponse);
  setenv("TZ", "CST-8", 1);
  tzset();
  const time_t nowtime = doc["result"]["timestamp"].as<int64_t>() + 10; // 补偿刷新时间
  struct timeval tv = {.tv_sec = nowtime};
  settimeofday(&tv, NULL);
  doc.clear();
  // Serial.print("时间联网设置成功\r\n");
}

// 联网获取天气情况,n<=5为往后预报的天数,记得deleteWeather
void getWeather(Weather &weather, int n)
{
  HTTPClient http;
  bool isgetweather = 1;
  // Serial.print("联网获取天气中\r\n");
  while (isgetweather)
  {
    http.begin(weatherurl + "?city=" + weathercity + "&key=ef670a1bc22e8ac69bdb5cd12716bd39");
    int http_code = http.GET();
    if (http_code == 200)
    {
      isgetweather = 0;
      // Serial.print("获取成功\r\n");
    }
    else
    {
      Serial.print("获取失败,十秒后重新请求\r\n");
    }
    delay(1000);
  }
  String weatherresponse = http.getString();
  http.end();
  // 解析json数据
  DynamicJsonDocument doc(1024);
  // Serial.println(weatherresponse);
  deserializeJson(doc, weatherresponse);
  // 创建Weather链表联系明后天天气
  Weather *p, *q;
  p = &weather;
  q = p;
  weather.city = doc["result"]["city"].as<String>();
  weather.info = doc["result"]["realtime"]["info"].as<String>();
  weather.temp = doc["result"]["realtime"]["temperature"].as<int16_t>();
  weather.humidity = doc["result"]["realtime"]["humidity"].as<int16_t>();
  weather.aqi = doc["result"]["realtime"]["aqi"].as<int16_t>();
  // 仅获取后两天的预报天气
  for (int i = 0; i < n; i++)
  {
    p = new Weather;
    q->nextday = p;
    q = p;
    q->predictday = doc["result"]["future"][i]["date"].as<String>();
    q->info = doc["result"]["future"][i]["weather"].as<String>();
    q->predictdaytemp = doc["result"]["future"][i]["temperature"].as<String>();
  }
  doc.clear();
  // Serial.print("天气联网设置成功\r\n");
  p = q = NULL;
}
// 析构Weather动态分配的内存,n为要析构的次数
void deleteWeather(Weather &weather, int n)
{
  Weather *p, *q;
  p = q = &weather;
  for (int i = 0; i < n; i++)
  {
    p = q->nextday;
    q = p;
    delete p;
  }
  p = q = NULL;
}

// 获取当前时间,tm格式
void getTime(tm &timeinfo_get)
{
  time_t now_get;
  time(&now_get);
  localtime_r(&now_get, &timeinfo_get);
}

/*************按钮模块 ************** *********************/
bool Button::isPressed()
{
  if (digitalRead(pin))
  {
    delay(debounceDelay);
    if (digitalRead(pin) && !status)
    {
      status = !status;
      return 1;
    }
    else if (!digitalRead(pin))
    {
      status = false;
    }
  }
  else
  {
    status = false;
  }
  return 0;
}

Button beginB(beginb_GPIO); // 开机/关机按钮
Button nextB(nextb_GPIO);   // next按钮
// Button confirmB(confirmb_GPIO);//确认按钮

/*************自订显示框架程式 **********************************/
extern U8G2_FOR_ADAFRUIT_GFX ufont;
extern GxEPD2_3C<GxEPD2_750c_Z08, GxEPD2_750c_Z08::HEIGHT / 2> epaper;

bool isleapYear(uint16_t y)
{
  if (y % 4 == 0 && y % 100 != 0 || y % 400 == 0)
    return 1;
  return 0;
}

uint16_t whatDay(uint16_t year, uint16_t month)
{
  int dyear = 0, nd = 0, w, lyear = 0;
  if (year == 1)
    nd = 0;
  else
  {
    for (int i = 1; i < year; i++)
    {
      if (isleapYear(i))
        lyear += 1;
      else
        dyear += 1;
    }
    nd = dyear * 365 + lyear * 366;
  }
  if (month == 1)
    nd += 1;
  else
  {
    int monthl[] = {1, 31, 0, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    for (int j = 0; j <= month - 1; j++)
    {
      nd += monthl[j];
    }
    if (isleapYear(year) && month >= 3)
      nd += 29;
    if (isleapYear(year) == 0 && month >= 3)
      nd += 28;
  }
  w = 0;
  w += nd;
  w = w % 7;
  if (w == 0)
    w = 7;
  return w;
}

void printcalendar(int16_t Xstart, int16_t Ystart)
{
  //ufont.setFont("u8g2_font_crox1h_tr");
  uint8_t fontw = 9;
  uint8_t fonth = 12;
  uint8_t months[12] = {31, 0, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  isleapYear(timeinfo.tm_year) ? months[1] = 29 : months[1] = 28;
  ufont.setCursor(Xstart, Ystart+fonth+4);
  ufont.print("MON TUE WED THU FRI SAT SUN");
  uint8_t fweek = whatDay(timeinfo.tm_year + 1900, timeinfo.tm_mon + 1);
  uint16_t Xshift = (1 + 4 * (fweek - 2)) * fontw;
  uint16_t Yshift = fonth * 3;

  for (int i = 1; i <= months[timeinfo.tm_mon]; i++)
  {
    Xshift += 4 * fontw;
    if ((fweek - 2 + i) % 7 == 0)
    {
      Yshift += fonth * 2;
      Xshift = 1 * fontw;
    }
    if (i == timeinfo.tm_mday)
    {
      ufont.setBackgroundColor(white);
      ufont.setForegroundColor(red);
      ufont.setCursor(Xstart + Xshift, Ystart + Yshift + fonth);
      ufont.print(i);
    }
    else
    {
      ufont.setBackgroundColor(white);
      ufont.setForegroundColor(black);
      ufont.setCursor(Xstart + Xshift, Ystart + Yshift + fonth);
      ufont.print(i);
    }
  }
}
void printclock(uint16_t Xstart,uint16_t Ystart,tm& pTime){
  uint16_t Dx = ufont.getFontAscent();
  Ystart += Dx;
  ufont.setCursor(Xstart, Ystart);
  ufont.print(pTime.tm_hour / 10);
  ufont.setCursor(Xstart + Dx, Ystart);
  ufont.print(pTime.tm_hour % 10);
  ufont.setCursor(Xstart + Dx*2, Ystart);
  ufont.print(':');
  ufont.setCursor(Xstart + Dx*2  + Dx / 2, Ystart);
  ufont.print(pTime.tm_min / 10);
  ufont.setCursor(Xstart + Dx * 3 + Dx / 2, Ystart);
  ufont.print(pTime.tm_min % 10);
  /*
  Paint_DrawChar(Xstart, Ystart, value[pTime->tm_hour / 10], Font, Color_Background, Color_Foreground);
  Paint_DrawChar(Xstart + Dx, Ystart, value[pTime->tm_hour % 10], Font, Color_Background, Color_Foreground);
  Paint_DrawChar(Xstart + Dx + Dx / 4 + Dx / 2, Ystart, ':', Font, Color_Background, Color_Foreground);
  Paint_DrawChar(Xstart + Dx * 2 + Dx / 2, Ystart, value[pTime->tm_min / 10], Font, Color_Background, Color_Foreground);
  Paint_DrawChar(Xstart + Dx * 3 + Dx / 2, Ystart, value[pTime->tm_min % 10], Font, Color_Background, Color_Foreground);
  // 没有打印秒数的需求
  // Paint_DrawChar(Xstart + Dx * 4 + Dx / 2 - Dx / 4, Ystart, ':'                    , Font, Color_Background, Color_Foreground);
  // Paint_DrawChar(Xstart + Dx * 5                  , Ystart, value[pTime->tm_sec / 10] , Font, Color_Background, Color_Foreground);
  // Paint_DrawChar(Xstart + Dx * 6                  , Ystart, value[pTime->tm_sec % 10] , Font, Color_Background, Color_Foreground);*/
}
/*************页面相关函数 **********************************/

void calendarpage()
{
  // backgroundpic
  switch (calendarpic)
  {
  case 0:
    epaper.drawBitmap(0, 0,  gImage_0,epaper.width(), epaper.height(),black);
    break; // starrynight
  case 1:
    epaper.drawBitmap(0, 0, gImage_1, epaper.width(), epaper.height(), black);
    break;
  case 2:
    epaper.drawBitmap(0, 0, gImage_2b, epaper.width(), epaper.height(), black);
    epaper.drawBitmap(0, 0, gImage_2a, epaper.width(), epaper.height(), red);
    break;
  case 3:
    epaper.drawBitmap(0, 0, gImage_3, epaper.width(), epaper.height(), black);
    break;
  case 4:
    epaper.drawBitmap(0, 0, gImage_4, epaper.width(), epaper.height(), black);
    break;
  case 5:
    epaper.drawBitmap(0, 0, gImage_5b, epaper.width(), epaper.height(), black);
    epaper.drawBitmap(0, 0, gImage_5a, epaper.width(), epaper.height(), red);
    break;
  }
  //ufont.setFont(u8g2_font_inb38_mn);
  ufont.setFont(my_yahei_pB_32);
  printclock(150, 20, timeinfo);
  String curmonth;
  String curweekday;
  switch (timeinfo.tm_mon)
  {
  case 0:
    curmonth = "一月";
    break;
  case 1:
    curmonth = "二月";
    break;
  case 2:
    curmonth = "三月";
    break;
  case 3:
    curmonth = "四月";
    break;
  case 4:
    curmonth = "五月";
    break;
  case 5:
    curmonth = "六月";
    break;
  case 6:
    curmonth = "七月";
    break;
  case 7:
    curmonth = "八月";
    break;
  case 8:
    curmonth = "九月";
    break;
  case 9:
    curmonth = "十月";
    break;
  case 10:
    curmonth = "十一月";
    break;
  case 11:
    curmonth = "十二月";
    break;
  default:;
  }
  switch (timeinfo.tm_wday)
  {
  case 0:
    curweekday = "星期日";
    break;
  case 1:
    curweekday = "星期一";
    break;
  case 2:
    curweekday = "星期二";
    break;
  case 3:
    curweekday = "星期三";
    break;
  case 4:
    curweekday = "星期四";
    break;
  case 5:
    curweekday = "星期五";
    break;
  case 6:
    curweekday = "星期六";
    break;
  default:;
  }

  // weather month mday
  String weathersimpleline = weatherinfo.city + " " + weatherinfo.info + " " + (String)weatherinfo.temp + "度";
  ufont.setFont(my_yahei_pB_32);
  ufont.drawUTF8(10, 170, weathersimpleline.c_str());
  ufont.drawUTF8(150, 15 + 2 * (ufont.getFontAscent() - ufont.getFontDescent()), curmonth.c_str());
  ufont.drawUTF8(150, 15 +3*(ufont.getFontAscent()-ufont.getFontDescent()), curweekday.c_str());
  //pg.printstr(10, 150, weathersimpleline.c_str(), 1);
  epaper.fillRect(0, 200, 315, 5, black);
  //pg.printline(0, 220, 320, 220, 5, 1);

  //===
  //pg.color(red);
  ufont.setFont(my_yahei_aB_70);ufont.setForegroundColor(red);
  if (timeinfo.tm_mday < 10)
  {
    
    ufont.setCursor(22, 27 + ufont.getFontAscent());
    ufont.print("0");
    ufont.setCursor(67, 27 + ufont.getFontAscent());
    ufont.print(timeinfo.tm_mday);
  }
  else
  {
    ufont.setCursor(22, 27 + ufont.getFontAscent());
    ufont.print(timeinfo.tm_mday);
  }
  epaper.fillRect(14,92,50*2,5,red);
  ufont.setFont(defaultFONT);
  ufont.setForegroundColor(black);
  printcalendar(30, 260);
  
}
