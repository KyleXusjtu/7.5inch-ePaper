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
/*编写好的页面*/
void welcomepage();
void calendarpage();

#endif