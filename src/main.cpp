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

void calendar(frame &pg){
  pg.color(black);
  pg.clear();
  pg.printpicH(gImage_StarrynightF);//starrynight
  pg.printtime(160,0,timeinfo,&Font36B,0);
  String month="一月";
  String city="上海";
  switch(timeinfo.tm_mon){
    case 0:month="一月";break;case 1:month="二月";break;case 2:month="三月";break;case 3:month="四月";break;case 4:month="五月";break;case 5:month="六月";break;case 6:month="七月";break;case 7:month="八月";break;case 8:month="九月";break;case 9:month="十月";break;case 10:month="十一月";break;case 11:month="十二月";break;
    default:;
  }
  pg.printstr(160,70,city.c_str(),1);
  pg.printline(0,150,320,150,5,1);
  //===
  pg.color(red);
  pg.clear();
  if(timeinfo.tm_mday<10){        
      pg.printchar(10,0,'0',&Font72B);
      pg.printnum(74,0,timeinfo.tm_mday,&Font72B);
  }else{
      pg.printnum(10,0,timeinfo.tm_mday,&Font72B);
  }
  pg.printline(10,130,10+64*2,130,5);
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
        //epaperinit();
        //---
        calendar(pg);
        //---
        runonce=0;
        SleepMode();
      }

      if(timeinfo.tm_sec==0){
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

