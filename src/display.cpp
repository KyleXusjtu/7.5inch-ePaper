#include "display.h"
extern U8G2_FOR_ADAFRUIT_GFX ufont;
extern GxEPD2_3C<GxEPD2_750c_Z08, GxEPD2_750c_Z08::HEIGHT / 2> epaper;
tm timeinfo;         // 时间结构体
Weather weatherinfo; // 天气结构体
int calendarpic = 0; // 选择显示的图片
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
        if ((fweek - 2 + i) % 7 == 0)
        {
            Yshift += fonth*1.5;
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
    ufont.setCursor((epaper.width() - ufont.getUTF8Width("AP:esp32ap;password:guofangwei")) / 2, (epaper.height() / 2 + 2*prevfontheight));
    ufont.print("AP:esp32ap;password:guofangwei");
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
        String weathersimpleline = weatherinfo.city + " " + weatherinfo.info + " " + (String)weatherinfo.temp + "度";
        ufont.setFont(u8g2_font_logisoso50_tn);
        ufont.setForegroundColor(black);
        printclock(160, 0, timeinfo, 0); // clock
        ufont.setFont(my_yahei_pB_32);
        ufont.drawUTF8(10, 180, weathersimpleline.c_str());                                              // weather
        ufont.drawUTF8(160, 100, curmonth.c_str());                                                      // month
        ufont.drawUTF8(160, 100 + (ufont.getFontAscent() - ufont.getFontDescent()), curweekday.c_str()); // week
        epaper.fillRect(0, 200, 315, 5, black);
        epaper.fillRect(1, 201, 313, 3, white); // dividing line
        ufont.setFont(u8g2_font_logisoso92_tn);
        ufont.setForegroundColor(red);
        ufont.setCursor(0, 0 + prevfontheight * 0.9);
        ufont.printf("%02d", timeinfo.tm_mday);                                         // date
        epaper.fillRect(0, 0 + prevfontheight, ufont.getUTF8Width("88") * 1.2, 5, red); // dateunderline
        ufont.setFont(my_yahei_pB_24);
        ufont.setForegroundColor(black);
        printcalendar(20, 220); // calender
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