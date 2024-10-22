#include "display.h"
extern U8G2_FOR_ADAFRUIT_GFX ufont;
extern GxEPD2_3C<GxEPD2_750c_Z08, GxEPD2_750c_Z08::HEIGHT / 2> epaper;
tm timeinfo;         // 时间结构体
Weather weatherinfo; // 天气结构体
int calendarpic = 2; // 选择显示的图片
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

void printcalendar(int16_t Xstart, int16_t Ystart)
{
    // ufont.setFont("u8g2_font_crox1h_tr");
    uint8_t fontw = prevfontheight*0.5;
    uint8_t fonth = prevfontheight;
    uint8_t months[12] = {31, 0, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    isleapYear(timeinfo.tm_year) ? months[1] = 29 : months[1] = 28;
    ufont.setCursor(Xstart, Ystart + fonth);
    ufont.print("一");
    ufont.setCursor(Xstart + fontw * 3 * 1, Ystart + fonth);
    ufont.print("二");
    ufont.setCursor(Xstart + fontw * 3 * 2, Ystart + fonth);
    ufont.print("三");
    ufont.setCursor(Xstart + fontw * 3 * 3, Ystart + fonth);
    ufont.print("四");
    ufont.setCursor(Xstart + fontw * 3 * 4, Ystart + fonth);
    ufont.print("五");
    ufont.setCursor(Xstart + fontw * 3 * 5, Ystart + fonth);
    ufont.print("六");
    ufont.setCursor(Xstart + fontw * 3 * 6, Ystart + fonth);
    ufont.print("日");
    uint8_t fweek = whatDay(timeinfo.tm_year + 1900, timeinfo.tm_mon + 1);
    uint16_t Xshift = (0 + 3 * (fweek - 2)) * fontw;
    uint16_t Yshift = fonth * 3;

    for (int i = 1; i <= months[timeinfo.tm_mon]; i++)
    {
        Xshift += 3 * fontw;
        if ((fweek - 2 + i) % 7 == 0&&i!=1)
        {
            Yshift += fonth*1.2;
            Xshift = 0 * fontw;
        }
        if (i == timeinfo.tm_mday)
        {
            ufont.setBackgroundColor(white);
            ufont.setForegroundColor(red);
            ufont.setCursor(Xstart + Xshift, Ystart + Yshift);
            ufont.print(i);
        }
        else
        {
            ufont.setBackgroundColor(white);
            ufont.setForegroundColor(black);
            ufont.setCursor(Xstart + Xshift, Ystart + Yshift );
            ufont.print(i);
        }
    }
}
void printclock(uint16_t Xstart, uint16_t Ystart, tm &pTime,bool issec)
{
    char timeString[20];
    ufont.setForegroundColor(GxEPD_BLACK);
    ufont.setBackgroundColor(GxEPD_WHITE);
    if(issec){
        sprintf(timeString, "%02d:%02d:%02d", pTime.tm_hour, pTime.tm_min,pTime.tm_sec); // 将时间格式化为字符串
        ufont.setCursor(Xstart, Ystart+(ufont.getFontAscent()-ufont.getFontDescent()));
        ufont.print(timeString); // 显示时间字符串 
    }else{
        sprintf(timeString, "%02d:%02d", pTime.tm_hour, pTime.tm_min); // 将时间格式化为字符串
        ufont.setCursor(Xstart, Ystart + (ufont.getFontAscent() - ufont.getFontDescent()));
        ufont.print(timeString); // 显示时间字符串
    }

}
/*************页面相关函数 **********************************/
void welcomepage(){
    epaper.setFullWindow();
    epaper.firstPage();
    do{
    ufont.setBackgroundColor(white);
    ufont.setForegroundColor(red);
    ufont.setFont(u8g2_font_sticker_mel_tr);
    ufont.setCursor((epaper.width()-ufont.getUTF8Width("Epaper Calender Displayer"))/2,(epaper.height()-prevfontheight*4)/2);
    ufont.print("Epaper Calender Displayer");
    ufont.setForegroundColor(black);
    for (int i = 0; i < 6;i++){
        //epaper.setPartialWindow((epaper.width() - ufont.getUTF8Width("Epaper Calender Displayer")) / 2 - 5 -);
        epaper.drawRect((epaper.width() - ufont.getUTF8Width("EEpEpaper Calender Displayer")) / 2 - 5 + i*3, (epaper.height() - prevfontheight * 6) / 2-2*i , ufont.getUTF8Width("EpEpaper Calender Displayer") + 10 , prevfontheight * 1.8 + i, black);
    }
    ufont.setFont(u8g2_font_crox2c_tf);
    ufont.setCursor((epaper.width() - ufont.getUTF8Width("Please Connect my AP and setup WLAN in 192.168.4.1")) / 2, (epaper.height() / 2 + prevfontheight));
    ufont.print("Please Connect my AP and setup WLAN in 192.168.4.1");
    ufont.setCursor((epaper.width() - ufont.getUTF8Width("AP:esp32ap;password:esp32pass")) / 2, (epaper.height() / 2 + 2*prevfontheight));
    ufont.print("AP:esp32ap;password:esp32pass");
    ufont.setCursor((epaper.width() - ufont.getUTF8Width("Waiting for connection...")) / 2, (epaper.height() / 2 +3* prevfontheight));
    ufont.print("Waiting for connection...");

    } while (epaper.nextPage());


}
void calendarpage()
{
    epaper.setFullWindow();
    epaper.firstPage();
    do
    {
        switch (calendarpic)
        {
        case 0:
            epaper.drawBitmap(0, 0, gImage_0, epaper.width(), epaper.height(), black);
            break; // starrynight
        case 1:
            epaper.drawBitmap(0, 0, gImage_1, epaper.width(), epaper.height(), black);
            break;
        case 2:
            epaper.drawBitmap(0, 0, gImage_2b, epaper.width(), epaper.height(), black);
            epaper.drawBitmap(0, 0, gImage_2a, epaper.width(), epaper.height(), red);
            break;
        case 3:
            epaper.drawBitmap(0, 0, gImage_3, epaper.width(), epaper.height(), black);
            break;
        case 4:
            epaper.drawBitmap(0, 0, gImage_4, epaper.width(), epaper.height(), black);
            break;
        case 5:
            epaper.drawBitmap(0, 0, gImage_5b, epaper.width(), epaper.height(), black);
            epaper.drawBitmap(0, 0, gImage_5a, epaper.width(), epaper.height(), red);
            break;
        }
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
        String weathersimpleline = weatherinfo.city + ":" + weatherinfo.info + " " + (String)weatherinfo.temp + "度.";
        ufont.setFont(u8g2_font_logisoso50_tn);
        ufont.setForegroundColor(black);
        printclock(160, 0, timeinfo, 0); // clock
        
        ufont.setFont(defaultFONT);
        ufont.drawUTF8(0, 160, weathersimpleline.c_str());
        int i1 = 0;
        for (i1 = 0; i1 < weatherinfo.life.length(); i1++)
        {
            if (ufont.getUTF8Width(weatherinfo.life.substring(0, i1).c_str()) > 310)
            {
                break;
            }
        }
        ufont.drawUTF8(0, 160 + prevfontheight, weatherinfo.life.substring(0,i1).c_str());
        ufont.drawUTF8(0, 160 + 2 * prevfontheight, weatherinfo.life.substring(i1).c_str());               //weather
        ufont.setFont(my_yahei_pB_32);
        ufont.drawUTF8(160, 100, curmonth.c_str());                                                      // month
        ufont.drawUTF8(160, 100 + (ufont.getFontAscent() - ufont.getFontDescent()), curweekday.c_str()); // week
        epaper.fillRect(0, 208, 315, 5, black);
        epaper.fillRect(1, 209, 313, 3, white); // dividing line
        epaper.fillRect(0, 208, 1, 280, black);
        epaper.fillRect(314, 208, 1, 280, black);
        ufont.setFont(u8g2_font_logisoso92_tn);
        ufont.setForegroundColor(red);
        ufont.setCursor(10, 0 + prevfontheight * 0.9);
        ufont.printf("%02d", timeinfo.tm_mday);                                         // date
        epaper.fillRect(0, 0 + prevfontheight, ufont.getUTF8Width("88") * 1.2, 5, red); // dateunderline
        ufont.setFont(my_yahei_pB_24);
        ufont.setForegroundColor(black);
        printcalendar(20, 215); // calender
    } while (epaper.nextPage());
}
void clockupdate(){
    ufont.setFont(u8g2_font_logisoso50_tn);
    epaper.setPartialWindow(155, 0, 155,prevfontheight );
    epaper.firstPage();
    do
    {
        ufont.setForegroundColor(black);
        printclock(160, 0, timeinfo, 0);
    } while (epaper.nextPageBW());
    
}
void weathercard(uint16_t Xstart, uint16_t Ystart, int mt,temppoint &tpt,Weather &weather,int i)
{
    //process weather string
    //temppoint tpt;
    String date = weather.predictday.substring(weather.predictday.indexOf('-')+1);
    String info = weather.info;
    int lowtemp = weather.predictdaytemp.substring(0, weather.predictdaytemp.indexOf('/')).toInt();
    int hightemp = weather.predictdaytemp.substring(weather.predictdaytemp.indexOf('/')+1).toInt();
    int weatherico = 0;
    if(strstr(info.c_str(),"雨")!=NULL)
        weatherico = 3;
    else if (strstr(info.c_str(), "晴") != NULL && (strstr(info.c_str(), "云") != NULL || strstr(info.c_str(), "阴") != NULL))
        weatherico = 1;
    else if (strstr(info.c_str(), "晴") != NULL)
        weatherico = 5;
    else if ((strstr(info.c_str(), "云") != NULL || strstr(info.c_str(), "阴") != NULL))
        weatherico = 0;
    //date+info
    ufont.setFont(u8g2_font_lubI18_tr);
    ufont.setCursor(Xstart, Ystart+prevfontheight);
    ufont.printf((date+"(+%d)").c_str(),i);
    ufont.setFont(defaultFONT);
    ufont.setCursor(ufont.getCursorX() - (ufont.getUTF8Width(info.c_str())), ufont.getCursorY() + prevfontheight);
    ufont.print(info);
    ufont.setFont(u8g2_font_open_iconic_weather_6x_t);
    ufont.setCursor(ufont.getCursorX() - (ufont.getUTF8Width(info.c_str())) / 2, ufont.getCursorY() + prevfontheight);
    //ico
    switch (weatherico)
    {
    case 0:
        ufont.drawGlyph(ufont.getCursorX() - (ufont.getUTF8Width(info.c_str())), ufont.getCursorY(), 0x0040);
        break;
    case 1:
        ufont.drawGlyph(ufont.getCursorX() - (ufont.getUTF8Width(info.c_str())), ufont.getCursorY(), 0x0041);
        break;
    case 2:
        ufont.drawGlyph(ufont.getCursorX() - (ufont.getUTF8Width(info.c_str())), ufont.getCursorY(), 0x0042);
        break;
    case 3:
        ufont.drawGlyph(ufont.getCursorX() - (ufont.getUTF8Width(info.c_str())), ufont.getCursorY(), 0x0043);
        break;
    case 4:
        ufont.drawGlyph(ufont.getCursorX() - (ufont.getUTF8Width(info.c_str())), ufont.getCursorY(), 0x0044);
        break;
    case 5:
        ufont.drawGlyph(ufont.getCursorX() - (ufont.getUTF8Width(info.c_str())), ufont.getCursorY(), 0x0045);
        break;
    default:
        break;
    }
    //graph
    int16_t OpointY = ufont.getCursorY()+50;
    ufont.setFont(u8g2_font_lubI18_tr);
    tpt.highX = tpt.lowX = Xstart+ufont.getUTF8Width(date.c_str());
    tpt.highY = OpointY + (mt + 20 -hightemp)  * 8;
    tpt.lowY = OpointY + (mt + 20 -lowtemp)  * 8;
    ufont.setFont(defaultFONT);
    epaper.drawCircle(tpt.highX, tpt.lowY, 3, black);
    epaper.drawCircle(tpt.highX, tpt.highY, 3, red);
    epaper.drawCircle(tpt.highX, tpt.lowY, 2, black);
    epaper.drawCircle(tpt.highX, tpt.highY, 2, red);
    epaper.drawCircle(tpt.highX, tpt.lowY, 1, black);
    epaper.drawCircle(tpt.highX, tpt.highY, 1, red);

    ufont.setCursor(tpt.lowX -10, tpt.lowY + 5+prevfontheight);
    ufont.print(lowtemp);
    ufont.setForegroundColor(red);
    ufont.setCursor(tpt.highX-10, tpt.highY-10);
    ufont.print(hightemp);
    ufont.setForegroundColor(black);
    //return tpt;
}
void templine(temppoint tpt1, temppoint tpt2, temppoint tpt3, temppoint tpt4)
{
    //Serial.printf("1h坐标(%d,%d),1l坐标(%d,%d)\n", tpt1.highX, tpt1.highY,tpt1.lowX, tpt1.lowY);
    //Serial.printf("2h坐标(%d,%d),1l坐标(%d,%d)\n", tpt2.highX, tpt2.highY, tpt2.lowX, tpt2.lowY);
    //Serial.printf("3h坐标(%d,%d),1l坐标(%d,%d)\n", tpt3.highX, tpt3.highY, tpt3.lowX, tpt3.lowY);
    for (int i:{-1,0,1}){
        epaper.drawLine(tpt1.highX, tpt1.highY+i, tpt2.highX, tpt2.highY+i, red);
        epaper.drawLine(tpt1.lowX, tpt1.lowY+i, tpt2.lowX, tpt2.lowY+i, black);
        epaper.drawLine(tpt2.highX, tpt2.highY+i, tpt3.highX, tpt3.highY+i, red);
        epaper.drawLine(tpt2.lowX, tpt2.lowY+i, tpt3.lowX, tpt3.lowY+i, black);
        epaper.drawLine(tpt3.highX, tpt3.highY + i, tpt4.highX, tpt4.highY + i, red);
        epaper.drawLine(tpt3.lowX, tpt3.lowY + i, tpt4.lowX, tpt4.lowY + i, black);
    }
}
void printsimpledate(){
    String curmonth;
    String curweekday;
    String curaqi;
    char timestring[20];
    sprintf(timestring, "%02d:%02d:%02d", timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
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
        curweekday = " 星期日";
        break;
    case 1:
        curweekday = " 星期一";
        break;
    case 2:
        curweekday = " 星期二";
        break;
    case 3:
        curweekday = " 星期三";
        break;
    case 4:
        curweekday = " 星期四";
        break;
    case 5:
        curweekday = " 星期五";
        break;
    case 6:
        curweekday = " 星期六";
        break;
    default:;
    }
    
    ufont.setFont(u8g2_font_open_iconic_all_2x_t);
    ufont.setForegroundColor(red);
    ufont.drawGlyph(20, 20 + prevfontheight, 0x006b);
    ufont.setForegroundColor(black);
    ufont.setFont(defaultFONT);
    ufont.setCursor(40, 20 + prevfontheight);
    ufont.print( (curmonth + curweekday).c_str());
    ufont.setCursor(20, ufont.getCursorY() + prevfontheight);
    ufont.print(weatherinfo.city+": "+weatherinfo.info+" 温度："+weatherinfo.temp+"℃");
    //aqi
    ufont.setCursor(20, ufont.getCursorY() + prevfontheight);
    ufont.print("AQI:");
    switch (weatherinfo.aqi/50)
    {
    case 0:
        curaqi="优秀,出门透透气吧";
        break;
    case 1:
        curaqi="良好，可以正常室外活动";
        break;
    case 2:
        curaqi="轻度污染敏感人群应注意";
        break;
    case 3:
        curaqi="中度污染，适量减少活动";
        break;
    case 4:
    case 5:     
        curaqi="重度污染，应减少室外活动";
        break;
    case 6:
    case 7:
        curaqi="严重污染，应避免室外活动";
        break;

    }        
    epaper.drawRect(60, ufont.getCursorY()-prevfontheight+5, 120, prevfontheight, black);
    for (int i = 60; i < weatherinfo.aqi*120/300+60; i++)
    {
        epaper.drawFastVLine(i, ufont.getCursorY() - prevfontheight+5, prevfontheight, black);
    }
    ufont.setCursor(185, ufont.getCursorY());
    ufont.print(weatherinfo.aqi);
    ufont.setCursor(20, ufont.getCursorY() + prevfontheight+5);
    ufont.print(curaqi);
}
void weatherpage(){
    // init temp graph
    int midtemp = (weatherinfo.nextday->predictdaytemp.substring(0, weatherinfo.predictdaytemp.indexOf('/')).toInt() + weatherinfo.nextday->predictdaytemp.substring(weatherinfo.predictdaytemp.indexOf('/') + 1).toInt()) / 2;
    temppoint tpt1, tpt2, tpt3, tpt4;
    epaper.setFullWindow();
    epaper.firstPage();
    do
    {
        weathercard(20, 100, midtemp, tpt1, *weatherinfo.nextday, 0);
        weathercard(220, 100, midtemp, tpt2, *weatherinfo.nextday->nextday, 1);
        weathercard(420, 100, midtemp, tpt3, *weatherinfo.nextday->nextday->nextday, 2);
        weathercard(620, 100, midtemp, tpt4, *weatherinfo.nextday->nextday->nextday->nextday, 3);
        templine(tpt1, tpt2, tpt3, tpt4);
        epaper.drawRoundRect(10, 10, 200, 210, 10, red);
        epaper.drawRoundRect(9, 9, 202, 212, 10, red);
        printsimpledate();
        ufont.setFont(u8g2_font_tenfatguys_tf);
        ufont.setForegroundColor(red);
        ufont.drawGlyph(10, 220 + prevfontheight, 0x007b);
        ufont.drawUTF8(30, 220 + prevfontheight, "Today");
        ufont.setForegroundColor(black);
        ufont.drawGlyph(220, 80 + prevfontheight, 0x00bb);
        ufont.drawUTF8(240, 80 + prevfontheight, "Forecast");
        ufont.drawGlyph(10, 220 + 2.5 * prevfontheight, 0x007d);
        ufont.drawUTF8(30, 220 + 2.5 * prevfontheight, "Temperature Graph");
    } while (epaper.nextPage());
}