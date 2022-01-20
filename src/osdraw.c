/* NetProg is an API in C language allowing an homogeneous programming of  */
/* communicating applications                                              */
/* Copyright (C) 2002,2003,2004,2005                                       */
/* Gabriel Jureidini                                                       */
/* Version 2.0 - 17 July 2005                                              */
/*-------------------------------------------------------------------------*/


#define _CRT_SECURE_NO_WARNINGS
#include "osdraw.h"


HFONT OSAllocatedFonts[OSMAXFONTS];


void OSDrawInit ()
{
    int i;
    for (i = 0; i < OSMAXFONTS; i++)
        OSAllocatedFonts[i] = NULL;
    return;
}



void OSDrawEnd ()
{
    int i;
#ifdef WINDOWS
    for (i = 0; i < OSMAXFONTS; i++)
        if (OSAllocatedFonts[i])
            DeleteObject(OSAllocatedFonts[i]);
    return;
#endif
}


void OSReturnFont (LONG dc, int Font)
{
#ifdef WINDOWS
    HDC hdc = (HDC)dc;
    HFONT hfont;
    HFONT hfont1;
    LOGFONT lf;
    int FontSize = 11;
    int Flag = 0;
    hfont = OSAllocatedFonts[Font];
    if (!hfont)
    {
        switch (Font)
        {
        case      OSFONT8  :
            FontSize = 8;
            Flag = 0;
            break;
        case      OSFONT10 :
            FontSize = 10;
            Flag = 0;
            break;
        case      OSFONT12 :
            FontSize = 12;
            Flag = 0;
            break;
        case      OSFONT14 :
            FontSize = 14;
            Flag = 0;
            break;
        case      OSFONT16 :
            FontSize = 16;
            Flag = 0;
            break;
        case      OSBOLD :
            Flag = FW_BOLD;
            break;
        case      OSBOLDFONT8  :
            FontSize = 8;
            Flag = FW_BOLD;
            break;
        case      OSBOLDFONT10 :
            FontSize = 10;
            Flag = FW_BOLD;
            break;
        case      OSBOLDFONT12 :
            FontSize = 12;
            Flag = FW_BOLD;
            break;
        case      OSBOLDFONT14 :
            FontSize = 14;
            Flag = FW_BOLD;
            break;
        case      OSBOLDFONT16 :
            FontSize = 16;
            Flag = FW_BOLD;
            break;
        default :
            return;
        }

        hfont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
        if (hfont == NULL)
            hfont = (HFONT)GetStockObject(ANSI_VAR_FONT);
        GetObject(hfont, sizeof(LOGFONT), &lf);
        if (Flag == FW_BOLD) lf.lfWeight = 700;
        if (FontSize != 11)	lf.lfHeight = FontSize;
        hfont1 = CreateFontIndirect (&lf);
        OSAllocatedFonts[Font] = hfont1;
    }
    SelectObject(hdc, hfont);
#endif
}




void OSDrawLine (int first_x, int first_y, int last_x, int last_y, LONG dc)
{
    HDC hdc = (HDC)dc;
    MoveToEx (hdc, first_x, first_y, NULL);
#ifdef WINDOWS
    LineTo (hdc, last_x, last_y);
#endif
}

void OSDrawRectangle (OSRect* prect, LONG dc)
{
    HDC hdc = (HDC)dc;
#ifdef WINDOWS
    Rectangle (hdc, prect->Left, prect->Top, prect->Right, prect->Bottom);
#endif
}


void OSDrawEllipse (OSRect* prect, LONG dc)
{
    HDC hdc = (HDC)dc;
#ifdef WINDOWS
    Ellipse (hdc, prect->Left, prect->Top, prect->Right, prect->Bottom);
#endif
}



void OSFillRectangle (OSRect* prect, LONG dc, DWORD pattern)
{

#ifdef WINDOWS
    HDC hdc = (HDC)dc;
    HBRUSH holdbrush;
    HBRUSH hbrush = GetStockObject ((int)pattern);

    holdbrush = SelectObject (hdc, hbrush);
    FillRect (hdc, (RECT*)prect, hbrush);
    SelectObject(hdc, holdbrush) ;
#endif
}



void OSInvertRectangle (OSRect* prect, LONG dc)
{
    HDC hdc = (HDC)dc;
    RECT rect;

    rect.left     =  prect->Left;
    rect.top      =  prect->Top ;
    rect.right    =  prect->Right ;
    rect.bottom   =  prect->Bottom ;
#ifdef WINDOWS
    InvertRect (hdc, &rect);
#endif
}


void OSDrawText (char* s, int Font, LONG dc, OSRect* prect, int format)
{
    HDC hdc = (HDC)dc;
    RECT rect;

//    OSReturnFont (dc, Font);
    rect.left = prect->Left;
    rect.top = prect->Top;
    rect.right = prect->Right  ;
    rect.bottom = prect->Bottom;
#ifdef WINDOWS
    DrawText (hdc, s, strlen (s), &rect, DT_VCENTER | DT_CENTER | DT_SINGLELINE);
#endif
}


void OSGetWindowSize (HWND win,  OSRect* rect)
{
    RECT Rect;

    Rect.left     =  rect->Left;
    Rect.top      =  rect->Top ;
    Rect.right    =  rect->Right ;
    Rect.bottom   =  rect->Bottom ;
#ifdef WINDOWS
    GetClientRect (win, &Rect);
#endif
}



void OSGetTextExtent (char* text, int Font, LONG dc, LONG* width, LONG* height)
{
    DWORD dwExtent;
    HDC hdc = (HDC)dc;
    SIZE lsize;
  //  OSReturnFont (dc, Font);

#ifdef WINDOWS
    dwExtent = GetTextExtentPoint32(hdc, text, strlen(text), &lsize);
    *width = (LONG)lsize.cx;
    *height = (LONG)lsize.cy;
#endif
}
