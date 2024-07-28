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

/*************绘图模块 ***********************************/
// 选择帧(打印未选择的帧会导致花屏!)

// void frame::activate(){
//   Paint_NewImage(thisframe,epaperw,epaperh,0,WHITE);
// }

void frame::color(UBYTE co)
{
  if (co == 0)
  {
    Color = 0;
  }
  else if (co == 1)
  {
    Color = 1;
  }
}
void frame::clear()
{
  Color == 0 ? (Paint_SelectImage(blackframe)) : (Paint_SelectImage(redframe));
  Paint_Clear(WHITE);
}
void frame::windowsclear(UWORD Xstart, UWORD Ystart, UWORD Xend, UWORD Yend, UWORD Color)
{
  Color == 0 ? Paint_SelectImage(blackframe) : Paint_SelectImage(redframe);
  Paint_ClearWindows(Xstart, Ystart, Xend, Yend, Color);
}
void frame::printpoint(UWORD Xpoint, UWORD Ypoint, UWORD Color = BLACK, UBYTE width, UBYTE style)
{
  Color == 0 ? Paint_SelectImage(blackframe) : Paint_SelectImage(redframe);
  DOT_PIXEL Dot_Pixel;
  switch (width)
  {
  case 1:
    Dot_Pixel = DOT_PIXEL_1X1;
    break;
  case 2:
    Dot_Pixel = DOT_PIXEL_2X2;
    break;
  case 3:
    Dot_Pixel = DOT_PIXEL_3X3;
    break;
  case 4:
    Dot_Pixel = DOT_PIXEL_4X4;
    break;
  case 5:
    Dot_Pixel = DOT_PIXEL_5X5;
    break;
  case 6:
    Dot_Pixel = DOT_PIXEL_6X6;
    break;
  case 7:
    Dot_Pixel = DOT_PIXEL_7X7;
    break;
  case 8:
    Dot_Pixel = DOT_PIXEL_8X8;
    break;
  default:;
  }
  if (style == 0)
  {
    Paint_DrawPoint(Xpoint, Ypoint, Color, Dot_Pixel, DOT_FILL_RIGHTUP);
  }
  else
  {
    Paint_DrawPoint(Xpoint, Ypoint, Color, Dot_Pixel, DOT_FILL_AROUND);
  }
}
void frame::printline(UWORD Xstart, UWORD Ystart, UWORD Xend, UWORD Yend, UBYTE width, UBYTE style)
{
  Color == 0 ? Paint_SelectImage(blackframe) : Paint_SelectImage(redframe);
  DOT_PIXEL Line_width;
  switch (width)
  {
  case 1:
    Line_width = DOT_PIXEL_1X1;
    break;
  case 2:
    Line_width = DOT_PIXEL_2X2;
    break;
  case 3:
    Line_width = DOT_PIXEL_3X3;
    break;
  case 4:
    Line_width = DOT_PIXEL_4X4;
    break;
  case 5:
    Line_width = DOT_PIXEL_5X5;
    break;
  case 6:
    Line_width = DOT_PIXEL_6X6;
    break;
  case 7:
    Line_width = DOT_PIXEL_7X7;
    break;
  case 8:
    Line_width = DOT_PIXEL_8X8;
    break;
  default:;
  }
  if (style == 0)
  {
    Paint_DrawLine(Xstart, Ystart, Xend, Yend, BLACK, Line_width, LINE_STYLE_SOLID);
  }
  else
  {
    Paint_DrawLine(Xstart, Ystart, Xend, Yend, BLACK, Line_width, LINE_STYLE_DOTTED);
  }
}
void frame::printrec(UWORD Xstart, UWORD Ystart, UWORD Xend, UWORD Yend, UBYTE width, UBYTE style)
{
  Color == 0 ? Paint_SelectImage(blackframe) : Paint_SelectImage(redframe);
  DOT_PIXEL Line_width;
  switch (width)
  {
  case 1:
    Line_width = DOT_PIXEL_1X1;
    break;
  case 2:
    Line_width = DOT_PIXEL_2X2;
    break;
  case 3:
    Line_width = DOT_PIXEL_3X3;
    break;
  case 4:
    Line_width = DOT_PIXEL_4X4;
    break;
  case 5:
    Line_width = DOT_PIXEL_5X5;
    break;
  case 6:
    Line_width = DOT_PIXEL_6X6;
    break;
  case 7:
    Line_width = DOT_PIXEL_7X7;
    break;
  case 8:
    Line_width = DOT_PIXEL_8X8;
    break;
  default:;
  }
  if (style == 0)
  {
    Paint_DrawRectangle(Xstart, Ystart, Xend, Yend, BLACK, Line_width, DRAW_FILL_EMPTY);
  }
  else
  {
    Paint_DrawRectangle(Xstart, Ystart, Xend, Yend, BLACK, Line_width, DRAW_FILL_FULL);
  }
}
void frame::printchar(UWORD Xstart, UWORD Ystart, const char Acsii_Char, sFONT *Font, UBYTE style)
{
  Color == 0 ? Paint_SelectImage(blackframe) : Paint_SelectImage(redframe);
  if (style == 0)
  {
    Paint_DrawChar(Xstart, Ystart, Acsii_Char, Font, BLACK, WHITE);
  }
  else
  {
    Paint_DrawChar(Xstart, Ystart, Acsii_Char, Font, WHITE, BLACK);
  }
}
/*lang=0英文模式(默认),lang=1中英文模式(建议)*/
/***|字体对照表(fontnum)|:
英文:|0-font8(默认)|1-font12|2-font16|3-font20|4-font24|
中文:|0-font24CN(默认)|1-font12CN|*/
/*style=0白底黑字(默认),style=1黑底白字*/
void frame::printstr(UWORD Xstart, UWORD Ystart, const char *pString, UBYTE lang, UBYTE fontnum, UBYTE style)
{
  Color == 0 ? Paint_SelectImage(blackframe) : Paint_SelectImage(redframe);
  sFONT *Fonts = &Font8;
  cFONT *Fontc = &Font24CN;
  switch (fontnum)
  {
  case 0:
    Fonts = &Font8;
    Fontc = &Font24CN;
    break;
  case 1:
    Fonts = &Font12;
    Fontc = &Font12CN;
    break;
  case 2:
    Fonts = &Font16;
    break;
  case 3:
    Fonts = &Font20;
    break;
  case 4:
    Fonts = &Font24;
    break;
  case 5:
    Fonts = &Font48B;
    break;
  default:;
  }
  if (lang == 0)
  {
    if (style == 0)
    {
      Paint_DrawString_EN(Xstart, Ystart, pString, Fonts, WHITE, BLACK);
    }
    else
    {
      Paint_DrawString_EN(Xstart, Ystart, pString, Fonts, BLACK, WHITE);
    }
  }
  else if (lang == 1)
  {
    if (style == 0)
    {
      Paint_DrawString_CN(Xstart, Ystart, pString, Fontc, BLACK, WHITE);
    }
    else
    {
      Paint_DrawString_CN(Xstart, Ystart, pString, Fontc, WHITE, BLACK);
    }
  }
}

void frame::printnum(UWORD Xstart, UWORD Ystart, int32_t Nummber, sFONT *Fonts, UBYTE style)
{
  Color == 0 ? Paint_SelectImage(blackframe) : Paint_SelectImage(redframe);
  if (style == 0)
  {
    Paint_DrawNum(Xstart, Ystart, Nummber, Fonts, BLACK, WHITE);
  }
  else
  {
    Paint_DrawNum(Xstart, Ystart, Nummber, Fonts, WHITE, BLACK);
  }
}
void frame::printpicH(const unsigned char *image_buffer)
{
  Color == 0 ? Paint_SelectImage(blackframe) : Paint_SelectImage(redframe);
  Paint_DrawBitMap(image_buffer);
}
void frame::printpicP(const unsigned char *image_buffer, UWORD xStart, UWORD yStart, UWORD W_Image, UWORD H_Image)
{
  Color == 0 ? Paint_SelectImage(blackframe) : Paint_SelectImage(redframe);
  Paint_DrawImage(image_buffer, xStart, yStart, W_Image, H_Image);
}

void frame::printtime(UWORD Xstart, UWORD Ystart, tm &pTime, sFONT *Fonts, int style)
{
  Color == 0 ? Paint_SelectImage(blackframe) : Paint_SelectImage(redframe);
  if (style == 0)
  {
    Paint_DrawTime(Xstart, Ystart, &pTime, Fonts, WHITE, BLACK);
  }
  else
  {
    Paint_DrawTime(Xstart, Ystart, &pTime, Fonts, BLACK, WHITE);
  }
}

void frame::display()
{
  EPD_7IN5B_V2_Display(blackframe, redframe);
  // Serial.print("打印帧成功\r\n");
}
// frame pg;
// 显示函数,接受两个frame分别代表红黑通道
// void Display(frame &blackframe,frame &redframe){
//   EPD_7IN5B_V2_Display(blackframe.thisframe,redframe.thisframe);
// }

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

/*************排版相关函数 **********************************/

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

void printcalendar(UWORD Xstart, UWORD Ystart, frame &pg, tm &timeinfo)
{
  uint8_t fontw = 11;
  uint8_t fonth = 16;
  uint8_t months[12] = {31, 0, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  isleapYear(timeinfo.tm_year) ? months[1] = 29 : months[1] = 28;
  pg.color(black);
  pg.printstr(Xstart, Ystart, "MON TUE WED THU FRI SAT SUN", 0, 2, 0);
  uint8_t fweek = whatDay(timeinfo.tm_year + 1900, timeinfo.tm_mon + 1);
  uint16_t Xshift = (1 + 4 * (fweek - 2)) * fontw;
  uint16_t Yshift = fonth * 1;

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
      pg.color(red);
      pg.printnum(Xstart + Xshift, Ystart + Yshift, i, &Font16, 1);
    }
    else
    {
      pg.color(black);
      pg.printnum(Xstart + Xshift, Ystart + Yshift, i, &Font16, 0);
    }
  }
}

/*************页面相关函数 **********************************/
void calendar(frame &pg)
{
  pg.color(red);
  pg.clear();
  pg.color(black);
  pg.clear();
  // background
  switch (calendarpic)
  {
  case 0:
    pg.printpicH(gImage_0);
    calendarpic = 1;
    break; // starrynight
  case 1:
    pg.printpicH(gImage_1);
    calendarpic = 2;
    break;
  case 2:
    pg.printpicH(gImage_2b);
    pg.color(red);
    pg.printpicH(gImage_2a);
    pg.color(black);
    calendarpic = 3;
    break;
  case 3:
    pg.printpicH(gImage_3);
    calendarpic = 4;
    break;
  case 4:
    pg.printpicH(gImage_4);
    calendarpic = 5;
    break;
  case 5:
    pg.printpicH(gImage_5b);
    pg.color(red);
    pg.printpicH(gImage_5a);
    pg.color(black);
    calendarpic = 0;
    break;
  }

  pg.printtime(160, 0, timeinfo, &Font36B, 0);
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

  // weather
  String weathersimpleline = weatherinfo.city + " " + weatherinfo.info + " " + (String)weatherinfo.temp + "度";
  pg.printstr(10, 150, weathersimpleline.c_str(), 1);
  pg.printline(0, 220, 320, 220, 5, 1);

  //===
  pg.color(red);

  if (timeinfo.tm_mday < 10)
  {
    pg.printchar(10, 0, '0', &Font72B);
    pg.printnum(74, 0, timeinfo.tm_mday, &Font72B);
  }
  else
  {
    pg.printnum(10, 0, timeinfo.tm_mday, &Font72B);
  }
  pg.printstr(170, 60, curmonth.c_str(), 1);
  pg.printstr(170, 60 + 41, curweekday.c_str(), 1);
  pg.printline(10, 130, 10 + 64 * 2, 130, 5);
  // 打印月历
  printcalendar(10, 270, pg, timeinfo);
  //===
  pg.display();
}
