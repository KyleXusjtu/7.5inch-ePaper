#include "511.h"
//pinMode(LED_GPIO,OUTPUT);//测试用LED灯
Button beginB(beginb_GPIO);//开机/关机按钮
Button nextB(nextb_GPIO);//next按钮
//Button confirmB(confirmb_GPIO);//确认按钮
//起始界面

Weather weatherinfo;//天气结构体
tm timeinfo;//时间结构体
int menupage=0;
bool isopen=0;
bool runonce=1;

/* setup --------------------------------------------------------------------*/
void setup()
{
  Serial.begin(115200);//打开串口监视
  frame pg;
  while(1){
    if(beginB.isPressed()){
      if(!isopen){
        //开机之后的动作
        epaperinit();
        pg.color(black);
        pg.clear();
        pg.printstr(epaperw/2-26*14/2,epaperh/2-20,"Welcome!connecting to wifi",0,3);
        pg.color(red);
        pg.clear();
        pg.printstr(epaperw/2-16*14/2,epaperh/2+20,"Powered by 511.h",0,3);
        pg.display();
        SleepMode();
        isopen=1;
        wificonnect();
        setTime();
      }
      else{
        //关机的动作
        epaperinit();
        pg.color(black);
        pg.clear();
        pg.color(red);
        pg.clear();
        pg.printstr(epaperw/2-10*14/2,epaperh/2-20/2,"Quiting...",0,3,1);
        pg.display();
        isopen=0;
        delay(1000);
        ClearPage();
        SleepMode();
      }
    }
    if(isopen){
      getTime(timeinfo);
      if(runonce){ 
        epaperinit();
        pg.color(black);
        pg.clear();
        pg.printpicP(gImage_starrynight606480,194,0,606,480);
        pg.color(red);
        pg.clear();
        pg.printtime(0,0,timeinfo,&Font48B,0);
        pg.display();
        runonce=0;
        SleepMode();
      }

      if(timeinfo.tm_sec==0){
        epaperinit();
        pg.color(black);
        pg.clear();
        pg.printpicP(gImage_starrynight606480,194,0,606,480);
        pg.color(red);
        pg.clear();
        pg.printtime(0,0,timeinfo,&Font48B,0);
        pg.display();
        SleepMode();
        runonce=0;
      }
    }
    delay(100);
  }
}

/* The main loop -------------------------------------------------------------*/
void loop()
{ 
  //do nothing here
}

