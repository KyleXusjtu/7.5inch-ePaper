#ifndef _display_h
#define _display_h
#include "511.h"
#define prevfontheight (ufont.getFontAscent()-ufont.getFontDescent()+5)

/*必要组件*/
bool isleapYear(uint16_t y);
uint16_t whatDay(uint16_t year, uint16_t month);
void printcalendar(int16_t Xstart, int16_t Ystart);
void printclock(uint16_t Xstart, uint16_t Ystart, tm &pTime,bool issec);
void clockupdate();
void weathercard(uint16_t Xstart, uint16_t Ystart, int mt,temppoint &tpt, Weather &weather,int i);
void templine(temppoint tpt1, temppoint tpt2, temppoint tpt3, temppoint tpt4);
void printsimpledate();
/*编写好的页面*/
void welcomepage();
void calendarpage();
void weatherpage();
#endif