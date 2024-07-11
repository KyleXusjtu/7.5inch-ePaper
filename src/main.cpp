#include "511.h"
//pinMode(LED_GPIO,OUTPUT);//测试用LED灯
Button beginB(beginb_GPIO);//开机/关机按钮
Button nextB(nextb_GPIO);//next按钮
//Button confirmB(confirmb_GPIO);//确认按钮
//起始界面

Weather weatherinfo;//天气结构体
tm timeinfo;//时间结构体
int calendarpic=0;
bool isopen=0;
bool runonce=1;

void calendar(frame &pg){
  pg.color(red);
  pg.clear();
  pg.color(black);
  pg.clear();
  //background
  switch(calendarpic){
    case 0:pg.printpicH(gImage_0);calendarpic=1;break;//starrynight
    case 1:pg.printpicH(gImage_1);calendarpic=2;break;
    case 2:pg.printpicH(gImage_2b);pg.color(red);pg.printpicH(gImage_2a);pg.color(black);calendarpic=3;break;
    case 3:pg.printpicH(gImage_3);calendarpic=4;break;
    case 4:pg.printpicH(gImage_4);calendarpic=5;break;
    case 5:pg.printpicH(gImage_5b);pg.color(red);pg.printpicH(gImage_5a);pg.color(black);calendarpic=0;break;
  }
  
  pg.printtime(160,0,timeinfo,&Font36B,0);
  String curmonth;
  String curweekday;
  switch(timeinfo.tm_mon){
    case 0:curmonth="一月";break;case 1:curmonth="二月";break;case 2:curmonth="三月";break;case 3:curmonth="四月";break;case 4:curmonth="五月";break;case 5:curmonth="六月";break;case 6:curmonth="七月";break;case 7:curmonth="八月";break;case 8:curmonth="九月";break;case 9:curmonth="十月";break;case 10:curmonth="十一月";break;case 11:curmonth="十二月";break;
    default:;
  }
  switch(timeinfo.tm_wday){
    case 0:curweekday="星期日";break;case 1:curweekday="星期一";break;case 2:curweekday="星期二";break;case 3:curweekday="星期三";break;case 4:curweekday="星期四";break;case 5:curweekday="星期五";break;case 6:curweekday="星期六";break;
    default:;
  }


  //weather
  String weathersimpleline=weatherinfo.city + " " + weatherinfo.info +" "+ (String)weatherinfo.temp + "度" ;
  pg.printstr(10,150,weathersimpleline.c_str(),1);
  pg.printline(0,220,320,220,5,1);
  
  //===
  pg.color(red);

  if(timeinfo.tm_mday<10){        
      pg.printchar(10,0,'0',&Font72B);
      pg.printnum(74,0,timeinfo.tm_mday,&Font72B);
  }else{
      pg.printnum(10,0,timeinfo.tm_mday,&Font72B);
  }
  pg.printstr(170,60,curmonth.c_str(),1);
  pg.printstr(170,60+41,curweekday.c_str(),1);
  pg.printline(10,130,10+64*2,130,5);
  //打印月历
  printcalendar(10,270,pg,timeinfo);
  //===
  pg.display();
}


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
        //SleepMode();
        isopen=1;
        wificonnect();
        setTime();
        getWeather(weatherinfo,2);
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
        //开机后仅运行一次
        //epaperinit();
        //---
        calendar(pg);
        //---
        runonce=0;
        SleepMode();
      }

      if(timeinfo.tm_sec==0){
        //刷新时间
        epaperinit();
        //---
        calendar(pg);
        //---
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

