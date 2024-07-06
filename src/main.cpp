#include "511.h"
//pinMode(LED_GPIO,OUTPUT);//测试用LED灯
//Button beginB(beginb_GPIO);//开机/关机按钮
//Button nextB(nextb_GPIO);//next按钮
//Button confirmB(confirmb_GPIO);//确认按钮
//起始界面
Weather weatherinfo;//天气结构体
tm timeinfo;//时间结构体
int menupage=0;
bool isopen=0;
bool runonce=0;
newimage *testframe;

/* setup --------------------------------------------------------------------*/
void setup()
{
  Serial.begin(115200);//打开串口监视
      epaperinit();
      frame startpageB;
      frame startpageR;
      startpageB.color(black);
      startpageB.printstr(epaperw/2-26*14/2,epaperh/2-20,"Welcome!connecting to wifi",0,3);
      //testframe=(newimage *)malloc(imagebyte);
      //Paint_NewImage(testframe,epaperw,epaperh,0,WHITE);
      //Paint_Clear(WHITE);
      //Paint_DrawString_EN(epaperw/2-16*14/2,epaperh/2+20,"Powered by 511.h",&Font24,BLACK,WHITE);
      startpageB.color(red);
      startpageB.printstr(epaperw/2-16*14/2,epaperh/2+20,"Powered by 511.h",0,3);
      startpageB.display();
      //Display(startpageB,startpageR);
      //EPD_7IN5B_V2_Display(startpageB.thisframe,testframe);
      isopen=1;
      //wificonnect();
      //ClearPage();
      SleepMode();
}

/* The main loop -------------------------------------------------------------*/
void loop()
{ 
  /*
  if(beginB.isPressed()){
    if(!isopen){
      epaperinit();
      startpageB.clear();
      startpageB.printstr(epaperw/2-26*14/2,epaperh/2-20,"Welcome!connecting to wifi",0,3);
      startpageR.clear();
      startpageR.printstr(epaperw/2-16*14/2,epaperh/2+20,"Powered by 511.h",0,3);
      Display(startpageB,startpageR);
      isopen=1;
      wificonnect();
      SleepMode();
    }
    else{
      startpageB.clear();
      startpageR.printstr(epaperw/2-10*14/2,epaperh/2-20/2,"Quiting...",0,3);
      Display(startpageB,startpageR);
      //析构位置
      delay(1000);
      ClearPage();
      SleepMode();
    }
  }
  delay(200);
  */
}

