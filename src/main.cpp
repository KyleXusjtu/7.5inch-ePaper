#include "511.h"
Weather weatherinfo;//天气结构体
tm timeinfo;//时间结构体
int calendarpic=0;//选择显示的图片
bool isopen=0;//是否开机
bool runonce=1;//runonce

WebServer server(80);

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

void handleroot(){
	String HTML="<!DOCTYPE html>\
    <html>\
        <head>\
            <meta charset='utf-8'>\
            <meta http-equiv='X-UA-Compatible' content='IE=edge'>\
            <title></title>\
            <meta name='description' content=''>\
            <meta name='viewport' content='width=device-width, initial-scale=1'>\
            <link rel='stylesheet' href=''>\
        </head>\
        <body>\
            <h1>Hello world!this is a test website</h1>\
            <!-- 两个按钮 -->\
            <button onmousedown=sw('on')>开灯</button>\
            <button onmousedown=sw('off')>关灯</button>\
            <!-- script里面写函数和js -->\
            <script>\
                var xhttp=new XMLHttpRequest();\
                function sw(arg){\
                    xhttp.open('GET','/511?led='+arg,true);\
                    xhttp.send();\
                }\
            </script>\
        </body>\
    </html>";//将html代码放进这个字符串里面
    server.send(200,"text/html",HTML);
    
}
void ledswitch(){
	String state=server.arg("led");//<key>
	if(state=="on"){
		digitalWrite(LED_GPIO,HIGH);
    server.send(200,"text/html","led is on");
	}
else if(state=="off"){
	digitalWrite(LED_GPIO,LOW);
  server.send(200,"text/html","led is off");
}
}
/* setup --------------------------------------------------------------------*/
void setup()
{
  Serial.begin(115200);//打开串口监视
  pinMode(LED_GPIO,OUTPUT);//测试用LED灯


  WiFi.softAP("esp32wifi","guofangwei");
	server.on("/",handleroot);
  server.on("/511",ledswitch);//这里使用了一个匿名函数
	server.onNotFound([](){server.send(200,"text/html","<h1>404 Not Found</h1>");});
	server.begin(); 



  /*v2.5.1 setup
  frame pg;//frane必须声明在setup中
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
        getWeather(weatherinfo,0);
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
  */
}

/* The main loop -------------------------------------------------------------*/
void loop()
{ 
  server.handleClient();
  delay(100);
  //do nothing here
}

