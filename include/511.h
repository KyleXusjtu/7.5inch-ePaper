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
    #include <ArduinoJson.h>
    #include <stdlib.h>
    #include <cstring>

//e-paper relative functions-----
    //initize the e-paper(5.83inch)
    #define epaperinit() DEV_Module_Init();EPD_7IN5B_V2_Init()
    //Clear e-paper
    #define ClearPage() EPD_7IN5B_V2_Clear()
    //Switch to sleep mode
    #define SleepMode() EPD_7IN5B_V2_Sleep()
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
        int8_t temp;
        int8_t humidity;
        int8_t aqi;
        Weather *nextday;
        String predictday;String daytemp;//仅预报有   
    };
    void wificonnect();
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
                Serial.print(1);
                blackframe=(newimage *)malloc(imagebyte);Serial.print(2);
                redframe=(newimage *)malloc(imagebyte);Serial.print(3);
                Paint_NewImage(redframe,epaperw,epaperh,0,WHITE);Serial.print(4);
                Paint_Clear(WHITE);Serial.print(5);
                Paint_SelectImage(blackframe);Serial.print(6);
                Paint_Clear(WHITE);Serial.print(7);
            }
            ~frame(){
                free(blackframe);
                free(redframe);
                blackframe=NULL;
                redframe=NULL;
                //Serial.print("删除帧成功\r\n");
            }
            //friend void Display(frame &blackframe,frame &redframe);
            //friend void EPD_7IN5B_V2_Display(const UBYTE *blackimage, const UBYTE *ryimage);
            //friend void Paint_NewImage(UBYTE *image, UWORD Width, UWORD Height, UWORD Rotate, UWORD Color);
            //friend void Paint_SelectImage(UBYTE *image);
            //void activate();
            void color(UBYTE co);
            void clear();
            void windowsclear(UWORD Xstart, UWORD Ystart, UWORD Xend, UWORD Yend, UWORD Color=WHITE);
            void point(UWORD Xpoint, UWORD Ypoint, UWORD Color, DOT_PIXEL Dot_Pixel=DOT_PIXEL_1X1, DOT_STYLE Dot_FillWay=DOT_FILL_RIGHTUP);
            
            //style=0白底黑字,style=1黑底白字
            void printchar(UWORD Xstart, UWORD Ystart, const char Acsii_Char, sFONT* Font,UBYTE style=0);
            
            /*lang=0英文模式(默认),lang=1中英文模式(建议)*/
            /***|字体对照表(fontnum)|:
            英文:|0-font8(默认)|1-font12|2-font16|3-font20|4-font24|
            中文:|0-font24CN(默认)|1-font12CN|*/
            /*style=0白底黑字(默认),style=1黑底白字*/
            void printstr(UWORD Xstart, UWORD Ystart, const char * pString,UBYTE lang=0,UBYTE fontnum=0,UBYTE style=0);
            
            /*|字体对照表(fontnum)|:
            |0-font8(默认)|1-font12|2-font16|3-font20|4-font24|*/
            /*style=0白底黑字(默认),style=1黑底白字*/
            void printnum(UWORD Xstart, UWORD Ystart,int32_t Nummber,UBYTE fontnum,UBYTE style=0);
            
            //写入图片数据,全屏
            void printpicH(const unsigned char* image_buffer);
            
            //写入图片数据,任意大小,参数均为bits,一定要填写正确
            void printpicP(const unsigned char *image_buffer, UWORD xStart, UWORD yStart, UWORD W_Image, UWORD H_Image);
            
            /*|字体对照表(fontnum)|:
            |0-font8(默认)|1-font12|2-font16|3-font20|4-font24|*/
            /*style=0白底黑字(默认),style=1黑底白字*/            
            void printtime(UWORD Xstart, UWORD Ystart, tm *pTime,int fontnum=0,int style=0);
            
            //打印帧,包括红黑通道          
            void display();
    };
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

#endif