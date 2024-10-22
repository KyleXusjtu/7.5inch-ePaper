// 511.h实现文件
#include "511.h"

/*******************网络模块**************************/
// WiFi 配置
// const char* ssid="511";
// const char* password ="guofangwei";
// api配置项目
String weatherurl = "http://apis.juhe.cn/simpleWeather/query";
String weatherlifeurl = "http://apis.juhe.cn/simpleWeather/life";
String mycity = "闵行"; // 修改天气位置
// String key="ef670a1bc22e8ac69bdb5cd12716bd39";

// 通过API获得最新北京时间戳,更新RTC模块
void setTime()
{
  
  //get time from ntp server(bug:cant display righttime in first display)
  /*
  if (time(nullptr) < 1000000000)
    {
    // 如果没有获取过时间，重新获取时间
    uint8_t i = 0;
    configTime(8*3600, 0, "ntp1.aliyun.com", "ntp2.aliyun.com");
    while ((time(nullptr) < 1000000000) & (i < 20))
    {
      i++;
      Serial.print(".");
      delay(500);
    }
    Serial.println("时间同步成功");
  }
  */
  // get time from juhe api(50times/day)
  
  HTTPClient http;
  bool isgettime = 1;
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
}

// 联网获取天气情况,n<=5为往后预报的天数,记得deleteWeather
void getWeather(Weather &weather, int n)
{
  HTTPClient http;
  bool isgetweather = 1;
  // Serial.print("联网获取天气中\r\n");
  while (isgetweather)
  {
    http.begin(weatherurl + "?city=" + mycity + "&key=ef670a1bc22e8ac69bdb5cd12716bd39");
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
  isgetweather = 1;
  // Serial.print("联网获取天气中\r\n");
  
  while (isgetweather)
  {
    http.begin(weatherlifeurl + "?city=" + mycity + "&key=ef670a1bc22e8ac69bdb5cd12716bd39");
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
  String weatherlife = http.getString();
  http.end();
  deserializeJson(doc,weatherlife);
  weather.life = doc["result"]["life"]["shushidu"]["des"].as<String>();

  doc.clear();
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

//根据mycity获取当地新闻

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

