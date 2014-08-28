
/*
    RTC-Setup for GEOS 2.0 64/128
    (DS12C887 RTC chip)

    Maciej 'YTM/Elysium' Witkowiak <ytm@elysium.pl>
    17.07.2002

    You need cc65 package version later than 2.8.5
    from http://www.cc65.org

    This program is freeware (look at README for more)
*/

#include <geos.h>
#include <ctype.h>
/* This is just to get CIA1 structure */
#define __C64__
#include <c64.h>

/* low-level RTC functions */
#include "rtclow.h"

/* for user interaction */
char answer[11];
/* local time variables */
unsigned char m_hour,m_minute,m_second;
unsigned char m_day,m_month, m_year,m_century;
unsigned char m_dayofweek;
unsigned int base_addy;

/* convert two digits into BCD value */
unsigned int makeBCD (char d, char m)
{
  return ((d-'0')*16+(m-'0'));
}

/* get value of a hex digit (0-9,A-F,a-f) */
unsigned int normXDigit (char d)
{
  if (d>='a') d&=0x5f;
  if (d>='A') d-=7;
  d-='0';
  return d;
}

/* setup local GEOS time and CIA1 TOD clock */
void PutLocalTime(void) {
  system_date.s_year = fromBCD(m_year);
  system_date.s_month = fromBCD(m_month);
  system_date.s_day = fromBCD(m_day);
  system_date.s_hour = fromBCD(m_hour);
  system_date.s_minutes = fromBCD(m_minute);
  system_date.s_seconds = fromBCD(m_second);
  if (m_hour>0x12) { m_hour = (m_hour - 0x12) | 0x80; }	// AM/PM shit
  InitForIO();
  CIA1.tod_hour = m_hour;
  CIA1.tod_min = m_minute;
  CIA1.tod_sec = m_second;
  CIA1.tod_10 = 0;
  DoneWithIO();
}

/* get time and date from RTC @ base_addy into variables */
void GetTimeDateFromRTC(void) {
  InitForIO();
  m_second = RTC_Peek(0);
  m_minute = RTC_Peek(2);
  m_hour   = RTC_Peek(4);
  m_dayofweek = RTC_Peek(6);
  m_day	   = RTC_Peek(7);
  m_month  = RTC_Peek(8);
  m_year   = RTC_Peek(9);
  m_century = RTC_Peek(50);
  DoneWithIO();
}

/* figure out base address according to string at tab */
void makeBaseAddy (const char *tab)
{
    base_addy = (normXDigit(tab[0]) << 12) + (normXDigit(tab[1]) << 8) +
		(normXDigit(tab[2]) << 4) + (normXDigit(tab[3]));
}

/* try to get base address from fileHeader note */
char getBaseAddy (void) {
    if (isxdigit(fileHeader.note[0]) && isxdigit(fileHeader.note[1]) &&
	isxdigit(fileHeader.note[2]) && isxdigit(fileHeader.note[3])) {
	makeBaseAddy (fileHeader.note);
	return 1;
    } else {
	return 0;
    }
}

void main (void)
{
char x,y;

  if (firstBoot != 0xff) {
	/* Auto-Exec here */
    if (getBaseAddy()==0) goto endnow;
    GetTimeDateFromRTC();
    PutLocalTime();
    goto endnow;

  } else {
	/* user executed here */
    y=0;
    /* if base address of RTC hasn't been set - ask for it */
    if (getBaseAddy()==0) {
	y=1;
        do {
	    CopyString(answer,"D200\0");
	    x = DlgBoxGetString(answer,4,CPLAINTEXT "What is the base address of RTC?", "(in hexadecimal, 0-9, A-F)");
	    if (x==CANCEL) goto endnow;
	} while (!(isxdigit(answer[0]) && isxdigit(answer[1]) && isxdigit(answer[2]) && isxdigit(answer[3])));
	makeBaseAddy(answer);
    }

    /* if user supplied base address of RTC - store it on disk */
    if (y==1) {
	CopyFString(4,fileHeader.note,answer);
	PutBlock(&dirEntryBuf.header,
	(char*)&fileHeader);
    }

    /* now ask for time and date */
    do {
	CopyString(answer,"12:00");
	x = DlgBoxGetString(answer,5,CPLAINTEXT "Enter time","(hh:mm 24-hour format)");
        if (x==CANCEL) goto endnow;
    } while (!(isdigit(answer[0]) && isdigit(answer[1]) && (answer[2]==':') &&
		isdigit(answer[3]) && isdigit(answer[4])));
    m_hour = makeBCD(answer[0],answer[1]);
    m_minute = makeBCD(answer[3],answer[4]);
    m_second = 0;

    do {
	CopyString(answer,"17/07/2002");
	x = DlgBoxGetString(answer,10,CPLAINTEXT "Enter date","(dd/mm/yyyy format)");
        if (x==CANCEL) goto endnow;
    } while (!(isdigit(answer[0]) && isdigit(answer[1]) && (answer[2]=='/') &&
               isdigit(answer[3]) && isdigit(answer[4]) && (answer[5]=='/') &&
               isdigit(answer[6]) && isdigit(answer[7]) && isdigit(answer[8]) &&
	       isdigit(answer[9])));

    m_day = makeBCD(answer[0],answer[1]);
    m_month = makeBCD(answer[3],answer[4]);
    m_century = makeBCD(answer[6],answer[7]);
    m_year = makeBCD(answer[8],answer[9]);

    /* day of week isn't supported by GEOS, but RTC has this field */
    do {
	answer[0]='\0';
	DlgBoxGetString(answer,1,CPLAINTEXT "What is the day of week?", "(1-7, Sunday=1)");
        if (x==CANCEL) goto endnow;
    } while (!(isdigit(answer[0])));

    m_dayofweek = answer[0]-'0';

    PutTimeDateToRTC();
    PutLocalTime();

endnow:
    EnterDeskTop();
    }
}
