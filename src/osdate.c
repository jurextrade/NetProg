/* NetProg is an API in C language allowing an homogeneous programming of  */
/* communicating applications                                              */
/* Copyright (C) 2002,2003,2004,2005                                       */
/* Gabriel Jureidini                                                       */
/* Version 2.0 - 17 July 2005                                              */
/*-------------------------------------------------------------------------*/


#define _CRT_SECURE_NO_WARNINGS
#include "osdate.h" 


int ActualMonth;    /* month in the year  0 = janvier */
int ActualMonthDay; /* day in the current month from 1 */
int ActualYearDay;  /* day in the current year from 1 */

int ActualWeekDay;      /* day of the week 0 = Dimanche ... 6 */

int ActualYear;     /* year 1999 */
long ActualDate;    /* date in number since 70 look osio.c */
long ActualHour;


int InDay;          /* day since  septembre 98  1 = 1 septembre 1998*/
int InMonth;        /* month since  septembre 98  1 = 1 septembre 1998*/
int BeginDay = 1;   /* begining day of the actualmonth year - 1 since septembre 98*/

char ActualStrDate[200]; /* 22-Juillet-1999*/
char ActualStrHour[200]; /*13:59:39 */
char ActualStrMonth[5];
char ActualStrWeek[10];
char ActualStrDay[10];   /* Lundi */





char* Date[] =
    {
        "Janvier", "Fevrier", "Mars", "Avril", "Mai", "Juin", "Juillet", "Aout", "Septembre", "Octobre",
        "Novembre", "Decembre"
    };

char* Day[] =
    {
        "Dimanche", "Lundi", "Mardi", "Mercredi", "Jeudi", "Vendredi", "Samedi"
    };

int NbDays[] =
    {
        31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
    };



int OSGetMonthFromDate (int year, int month)   /* month from 1 = Sep */
{

    if (year == 98)
    {
        if (month < 9) return -1;
        return (month - 8);
    }
    else
        if (year < 98) return -1;
        else
            return (month + 4);
}



int OSGetDayFromDate (int year, int month,int day)   /* day from 1 */
{

    struct tm gmt;
    struct tm *gmt1;

    int rday;
    char buf[5];
    time_t time;


    if (year == 98)
    {
        if (month < 9) return -1;
        if (month == 9) return day;
        else
            if (month == 10) return (30 + day);
            else if (month == 11) return (61 + day);
            else  return (91 + day);

    }
    else
        if (year < 98) return -1;

    memset (&gmt, 0, sizeof (struct tm));
    gmt.tm_year = year;
    gmt.tm_mon = month -1;
    gmt.tm_mday = day;
    time = mktime (&gmt);
    gmt1 = localtime (&time);


    strftime (buf, 4, "%j", gmt1);
    rday = atoi(buf);


    return rday + 122;
}


int OSGetDateTime (char* SDate, char* STime)
{
#ifdef __VOS__
    time_t timer;
    struct tm *time_ptr;
    time_ptr = localtime (&timer);
    if (time_ptr)
    {
        strftime (STime, 100, "%X",time_ptr);
        strftime (SDate, 100, "%x",time_ptr);
    }
    else return -1;
#else    
#ifdef WINDOWS
    _strtime( STime );
    _strdate( SDate );
#else
#endif
#endif
    return 1;
}



char* OSFormatDate(long date, char* datcha, int WithSep)
{
    int v2, i, flagstop;
    int Qan[12] ;
    int AA, QQ;
    div_t dres ;
    char annee[3];
    char jour[4];
    char format[6];
    char month[15];

    flagstop = 0 ;

    Qan[0] = 31 ;  Qan[1] = 59 ;  Qan[2] = 90 ;
    Qan[3] = 120;  Qan[4] = 151;  Qan[5] = 181;
    Qan[6] = 212;  Qan[7] = 243;  Qan[8] = 273;
    Qan[9] = 304;  Qan[10] = 334; Qan[11] = 365;

    sprintf (format, "%.5ld", date);

    strncpy(annee, format, 2);
    annee[2] = '\0';


    strncpy(jour, format + 2, 3);
    jour[3] = '\0';

    AA = atoi (annee) + 1990;
    QQ = atoi (jour);
    dres = div (AA, 4) ;
    if (dres.rem == 0)
        for (i = 1; i < 12; i++)
            Qan[i]++ ;
    v2 = QQ ;
    if (v2 != 0)
    {
        for (i = 0; flagstop == 0; i++)
        {
            if (QQ <= Qan[i])
                flagstop = 1 ;
            else
                v2 = QQ - Qan[i] ;
        }
    }
    else i = 0 ;

    sprintf (month, "%s", Date[i -1]);
    ActualMonth = i -1 ;
    ActualMonthDay = v2;
    if (WithSep)
    {
        sprintf(datcha, "%02d-%s-%04d", v2, month, AA) ;
        datcha[8 + strlen (month)] = '\0' ;
    }
    else
    {
        sprintf(datcha, "%02d%02d%04d", v2,i,AA);
        datcha[10] = '\0' ;
    }

    return(datcha) ;
}



char* OSFormatHour(long hour, char* hourcha, int WithSep)
{
    char chour[20];
    char heure[3];
    char minute[3];
    char sec[3];

    sprintf (chour, "%06ld", hour);
    strncpy (heure, chour, 2);
    strncpy (minute, chour + 2, 2);
    strncpy (sec, chour + 4, 2);

    heure[2] = minute[2] = sec[2] = '\0';
    if (WithSep)
        sprintf (hourcha, "%.2s:%.2s:%.2s", heure, minute, sec);
    else
        sprintf (hourcha, "%.2s%.2s%.2s", heure, minute, sec);
    return hourcha;

}



void OSGetDateHour(long* dat, long* hour)
{
    time_t dh;
    char buf1[7];
    char buf2[4];
    char buf3[20];
    struct tm *gmt;
    int ann;
    int i;

    BeginDay = 1;
    time(&dh);
    gmt = localtime(&dh);



    strftime(buf1,5,"%Y",gmt);
    ann = atoi(buf1);
    ann -= 1990;

    strftime(buf2, 4, "%j", gmt);
    ActualYearDay = atoi (buf2);

    sprintf(buf1,"%02d%s", ann, buf2);
    *dat = (long)atol(buf1);

    strftime(buf1,7,"%H%M%S",gmt);
    *hour = (long)atol(buf1);

    strftime(buf3, 2, "%w", gmt);
    ActualWeekDay = atoi (buf3);

    strftime(buf3, 5, "%Y", gmt);

    ActualYear = atoi (buf3);
    OSFormatDate(*dat, ActualStrDate, 1);
    OSFormatHour (ActualHour, ActualStrHour, 1);

    InDay = OSGetDayFromDate (ActualYear - 1900, ActualMonth + 1, ActualMonthDay);
    sprintf (ActualStrDay, "%s", Day[ActualWeekDay]);
    InMonth = OSGetMonthFromDate (ActualYear - 1900, ActualMonth + 1);

    if (ActualMonth >= 8)
        for (i = 8; i < ActualMonth; i++)
            BeginDay +=  NbDays[i];
    else
    {
        for (i = 8; i <= 11; i++)
            BeginDay +=  NbDays[i];
        for (i = 0; i <= ActualMonth; i++)
            BeginDay +=  NbDays[i];
    }
    return;
}


