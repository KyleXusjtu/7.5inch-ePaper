/**
* @File      	  :	511.h
* @Author         : Qruisjtu
* @email          : 724715050@qq.com
* @Function       : Simplify and Pack the use of Paint functions&HTTP communication functions
* @Info           :
*----------------
* |	This version:   V1.1.1
* | Date        :   2024-06-28
* | Info        :   Alpha version
**/

#ifndef _511_h
#define _511_h
//all includes
#include"ImageData.h"
#include "SPIFFS.h"
#include <time.h>
#include<GxEPD2_3C.h>
#include<U8g2_for_Adafruit_GFX.h>
#include <WiFi.h>
#include <HTTPClient.h>
//#include <WebServer.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include <ArduinoJson.h>
#include <stdlib.h>
#include <cstring>

#define LED_GPIO 2
#define black GxEPD_BLACK
#define red GxEPD_RED
#define white GxEPD_WHITE
#define defaultFONT u8g2_font_wqy16_t_gb2312
//wifi relative function-----
    //注意天气有实时和预报,内容不同,用*nextday联系
    struct Weather{
        String city;
        String info;//天气情况,预报和实时都有
        int16_t temp;
        int16_t humidity;
        int16_t aqi;
        Weather *nextday;
        String predictday;String predictdaytemp;//仅预报有   
    };
    //连接wifi请求
    bool wificonnect(String ssid,String password);
    //设置系统时间
    void setTime();
    //获取当前系统时间
    void getTime(tm &timeinfo_get);
    //联网获取天气情况
    void getWeather(Weather &weather,int n);
    //析构天气结构体
    void deleteWeather(Weather &weather,int n);
//button related function-----
    #define beginb_GPIO 34
    #define nextb_GPIO 35
    #define confirmb_GPIO 32
    #define debounceDelay 30
    //按钮类
    class Button {
        private:
            uint8_t pin;
            //uint8_t buttonstate;
            //uint8_t lastbuttonstate;
            bool status;
            //static uint8_t debouncedelay;
        public:
            Button(uint8_t gpio):pin(gpio),status(0){
                pinMode(pin,INPUT_PULLDOWN);
                }
            bool isPressed();
    };

//framework related functions

bool isleapYear(uint16_t y);
uint16_t whatDay(uint16_t year,uint16_t month);

//打印一个月历
void printcalendar(int16_t Xstart, int16_t Ystart);
void printclock(uint16_t Xstart, uint16_t Ystart, tm &pTime);

//page related functions
void calendarpage();

#endif