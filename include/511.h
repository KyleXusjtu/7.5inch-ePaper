/**
* @File      	  :	511.h
* @Author         : Qruisjtu
* @email          : 724715050@qq.com
* @Function       : Simplify and Pack the use of Paint functions&HTTP communication functions
* @Info           :
*  epaperinit()    :initialize e-paper
*  ClearPage()     :Clear e-paper
*  SleepMode()     :Switch to sleep mode
*  epaperw,epaperh :screen size bits
*  imagebyte       :image c-file size
*  frame           :basic paint unit,encapsulated from GUI_Paint
*----------------
* |	This version:   V1.1.1
* | Date        :   2024-06-28
* | Info        :   Alpha version
**/

#ifndef _511_h
#define _511_h
    //all includes
    #include"DEV_Config.h"
    #include"EPD.h"
    #include"GUI_Paint.h"
    #include"ImageData.h"
    #include"fonts.h"
    #include <time.h>
    #include <WiFi.h>
    #include <HTTPClient.h>
    #include <WebServer.h>
    #include <ArduinoJson.h>
    #include <stdlib.h>
    #include <cstring>

//e-paper relative functions-----
    //initize the e-paper(5.83inch)
    #define epaperinit() DEV_Module_Init();EPD_7IN5B_V2_Init()
    //Clear e-paper
    #define ClearPage() EPD_7IN5B_V2_Clear();Serial.print("clear page\r\n")
    //Switch to sleep mode
    #define SleepMode() EPD_7IN5B_V2_Sleep();Serial.print("go to sleep\r\n")
    #define epaperw EPD_7IN5B_V2_WIDTH
    #define epaperh EPD_7IN5B_V2_HEIGHT
    #define newimage UBYTE
    #define imagebyte (((EPD_7IN5B_V2_WIDTH % 8 == 0) ? (EPD_7IN5B_V2_WIDTH / 8 ) : (EPD_7IN5B_V2_WIDTH / 8 + 1)) * EPD_7IN5B_V2_HEIGHT)
    #define LED_GPIO 2
    #define black 0
    #define red 1
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
    void wificonnect(String ssid,String password);
    void setTime();
    void getTime(tm &timeinfo_get);
    void getWeather(Weather &weather,int n);
    void deleteWeather(Weather &weather,int n);



//paint related function-----
    class frame{
        private:
            UBYTE Color;
        public:
            newimage *blackframe;
            newimage *redframe;
            frame():Color(0){
                blackframe=(newimage *)malloc(imagebyte);
                redframe=(newimage *)malloc(imagebyte);
                Paint_NewImage(redframe,epaperw,epaperh,0,WHITE);
                Paint_Clear(WHITE);
                Paint_SelectImage(blackframe);
                Paint_Clear(WHITE);

            }
            ~frame(){
                free(blackframe);
                free(redframe);
                blackframe=NULL;
                redframe=NULL;
                //Serial.print("删除帧成功\r\n");
            }
            //black=0,red=1
            void color(UBYTE co);
            void clear();
            void windowsclear(UWORD Xstart, UWORD Ystart, UWORD Xend, UWORD Yend, UWORD Color=WHITE);
            //style=0实心(默认),style=1空心
            void printpoint(UWORD Xpoint, UWORD Ypoint, UWORD Color,UBYTE width=1,UBYTE style=0);
            
            //style=0白底黑字,style=1黑底白字
            void printchar(UWORD Xstart, UWORD Ystart, const char Acsii_Char, sFONT* Font,UBYTE style=0);
            
            //style=0实线(默认),style=1虚线
            void printline(UWORD Xstart, UWORD Ystart, UWORD Xend, UWORD Yend, UBYTE width=1,UBYTE style=0);
            //style=0空心(默认),style=1填充
            void printrec(UWORD Xstart, UWORD Ystart, UWORD Xend, UWORD Yend, UBYTE width=1,UBYTE style=0);

            void printstr(UWORD Xstart, UWORD Ystart, const char * pString,UBYTE lang=0,UBYTE fontnum=0,UBYTE style=0);
            
            /*style=0白底黑字(默认),style=1黑底白字*/
            void printnum(UWORD Xstart, UWORD Ystart,int32_t Nummber,sFONT *Fonts,UBYTE style=0);
            
            //写入图片数据,全屏
            void printpicH(const unsigned char* image_buffer);
            
            //写入图片数据,任意大小,参数均为bits,一定要填写正确
            void printpicP(const unsigned char *image_buffer, UWORD xStart, UWORD yStart, UWORD W_Image, UWORD H_Image);
            
            /*style=0白底黑字(默认),style=1黑底白字*/            
            void printtime(UWORD Xstart, UWORD Ystart, tm &pTime,sFONT *Fonts,int style=0);
            
            //打印帧,包括红黑通道          
            void display();
    };
    //extern frame pg;
    //void Display(frame &blackframe,frame &redframe);

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


    extern Button beginB;
    extern Button nextB;

//framework related functions

bool isleapYear(uint16_t y);
uint16_t whatDay(uint16_t year,uint16_t month);

//打印一个月历 font16:297*160(192)
void printcalendar(UWORD Xstart,UWORD Ystart,frame &pg,tm &timeinfo);

//pg related function
void calendar(frame &pg);


#endif