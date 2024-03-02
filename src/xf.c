/* NetProg is an API in C language allowing an homogeneous programming of  */
/* communicating applications                                              */
/* Copyright (C) 2002,2003,2004,2005                                       */
/* Gabriel Jureidini                                                       */
/* Version 2.0 - 17 July 2005                                              */
/*-------------------------------------------------------------------------*/


#define _CRT_SECURE_NO_WARNINGS

#include "xf.h"

XLTable* GetXLTable(XF* pxf, char* from, char* to)
{
    XLTable* t;
    List* Tables = pxf->XlatTables;

    while (Tables)
    {
        t = (XLTable*)Tables->car;
        /* test sur from et from */
        if (strcmp(from, t->From))
        {
            /* from et from diff�rents, test crois�  */
            if (!strcmp(from, t->To) && !strcmp(to, t->From)) return t;
        }
        else /* from et from �gaux */
            if (!strcmp(to, t->To)) return t;
        Tables = Tables->cdr;
    }
    return NULL; /* pas trouve */
}

XL* InitXlat(XF* pxf, char* from, char* to)
{
    XL *ret = (XL *)calloc(1, sizeof(XL));

    if (ret == NULL)  return NULL;
    if (!strcmp(from, to))
    {
        free(ret);
        return NULL;
    }
    ret->Tab = GetXLTable(pxf, from, to);
    if (ret->Tab == NULL)
    {
        free(ret);
        return NULL;
    }
    ret->FromTo = strcmp(ret->Tab->From, from) != 0 ? 1 : 0;
    return ret;
}

/*-------------------------------------------------------------------------*/
/* fonction de chargement d'une table de conversion                        */
/* retourne OK ou NOTFOUND ou NOMEM                                        */
/*-------------------------------------------------------------------------*/

char* XLat( XL* x, char* dst, char* src, size_t len )
{
    size_t i=0;
    char* p = dst;
    char *Tbl;

    if (x == NULL) return (char *)memcpy( dst, src,len);
    Tbl = x->Tab->Tables[x->FromTo];
    for (;i<len; i++)
    {
        *p = Tbl[(unsigned char)*src];
        p++;src++;
    }
    return dst;
}

char* UnXlat( XL *x, char* dst, char* src, size_t len)
{
    size_t i = 0;
    char*  p = dst;
    char*  Tbl;

    if (x == NULL)
        return (char *)memcpy( dst, src,len);
    Tbl = x->Tab->Tables[not1(x->FromTo)];
    for (;i<len; i++)
    {
        *p = Tbl[(unsigned char)*src];
        p++;src++;
    }
    return dst;
}

char* Xlats (XL* x, char* str)
{
    char* mem = str;
    char* Tbl;

    if (x == NULL) return str;
    Tbl = x->Tab->Tables[x->FromTo];
    while (*str)
    {
        *str = Tbl[(unsigned char)*str];
        str++;
    }
    return mem;
}

char* UnXlats (XL* x, char* str)
{
    char* mem = str;
    char* Tbl;

    if (x == NULL) return str;
    Tbl = x->Tab->Tables[not1(x->FromTo)];
    while (*str)
    {
        *str = Tbl[(unsigned char)*str];
        str++;
    }
    return mem;
}

char Xlatc (XL* x, char c)
{
    return MXlatc(x,c);
}

char UnXlatc (XL* x, char c)
{
    return MUnXlatc(x,c);
}

void EndXlat (XL* x )
{
    if (x != NULL) free(x);
}

#define TABLESNUMBER   20

XFTable XFEbcdicWindows =
    {"EbcdicFrench-WindowsExtended", 1000, "297", "1004",
     "000000000009000000000000000D0000",
     "00000000000A00000000000000000000",
     "00000000000000000000000000000000",
     "00000000000000000000000000000000",
     "20A0E2E440E1E3E55CF1B02E3C282B21",
     "267BEAEB7DEDEEEFECDFA7242A293B5E",
     "2D2FC2C4C0C1C3C5C7D1F92C255F3E3F",
     "F8C9CACBC8CDCECFCCB53AA3E0273D22",
     "D8616263646566676869ABBBF0FDFEB1",
     "DD6A6B6C6D6E6F707172AABAE6B8C6A4",
     "60A8737475767778797AA1BFD05BDEAE",
     "A223A5B7A97EB6BCBDBEAC7CAF5DB4D7",
     "E9414243444546474849ADF4F6F2F3F5",
     "E84A4B4C4D4E4F505152B9FBFCA6FAFF",
     "E7F7535455565758595AB2D4D6D2D3D5",
     "30313233343536373839B3DBDCD9DA9F"
    };

XFTable XFEbcdicEnglish =
    {"EbcdicEnglish-WindowsExtended", 1001, "285", "1004",
     "000000000009000000000000000D0000",
     "00000000000A00000000000000000000",
     "00000000000000000000000000000000",
     "00000000000000000000000000000000",
     "20A0E2E440E1E3E55CF1B02E3C282B21",
     "267BEAEB7DEDEEEFECDFA7242A293B5E",
     "2D2FC2C4C0C1C3C5C7D1F92C255F3E3F",
     "F8C9CACBC8CDCECFCCB53AA3E0273D22",
     "D8616263646566676869ABBBF0FDFEB1",
     "DD6A6B6C6D6E6F707172AABAE6B8C6A4",
     "60A8737475767778797AA1BFD05BDEAE",
     "A223A5B7A97EB6BCBDBEAC7CAF5DB4D7",
     "E9414243444546474849ADF4F6F2F3F5",
     "E84A4B4C4D4E4F505152B9FBFCA6FAFF",
     "E7F7535455565758595AB2D4D6D2D3D5",
     "30313233343536373839B3DBDCD9DA9F"
    };

XFTable XFEbcdicTurc =
    {"EbcdicTurc-WindowsTurc", 1002, "1026", "1254",
     "00000000000000000000000000000000",
     "00000000000000000000000000000000",
     "00000000000000000000000000000000",
     "00000000000000000000000000000000",
     "20A0E2E4E0E1E3E57BF1C72E3C282B21",
     "26E9EAEBE8EDEEEFECDFD0DD2A293B5E",
     "2D2FC2C4C0C1C3C55BD1FE2C255F3E3F",
     "F8C9CACBC8CDCECFCCFD3AD6DE273DDC",
     "D8616263646566676869ABBB7D60A6B1",
     "B06A6B6C6D6E6F707172AABAE6B8C6A4",
     "B5F6737475767778797AA1BF5D2440AE",
     "A2A3A5B7A9A7B6BCBDBEAC7CAFA8B4D7",
     "E7414243444546474849ADF47EF2F3F5",
     "F04A4B4C4D4E4F505152B9FB5CF9FAFF",
     "FCF7535455565758595AB2D423D2D3D5",
     "30313233343536373839B3DB22D9DA20"

    };

XFTableRepresentation XFDefinedTables[TABLESNUMBER] = {
            {XFCPWINDOWS,  "1004",                -2             },
            {XFCPWINDOWS,  "1250",                -2             },
            {XFCPWINDOWS,  "1252",                -2             },
            {XFCPWINDOWS,  "1253" ,                30            },
            {XFCPWINDOWS,  "1254",                 90            },
            {XFCPEBCDIC,   "37",                   1             },
            {XFCPEBCDIC,   "273",                  49            },
            {XFCPEBCDIC,   "227",                  45            },
            {XFCPEBCDIC,   "227",                  47            },
            {XFCPEBCDIC,   "278",                  46            },
            {XFCPEBCDIC,   "278",                  358           },
            {XFCPEBCDIC,   "280",                  39            },
            {XFCPEBCDIC,   "284",                  34            },
            {XFCPEBCDIC,   "285",                  44            },
            {XFCPEBCDIC,   "290",                  81            },
            {XFCPEBCDIC,   "297",                  33            },
            {XFCPEBCDIC,   "424",                  972           },
            {XFCPEBCDIC,   "833",                  82            },
            {XFCPEBCDIC,   "836",                  82            },
            {XFCPEBCDIC,   "1026",                 90            }
        };

BOOL XFSameSystem (XF* pxf)
{
    UINT CharFlag = pxf->CharFlag | (pxf->StreamCharFlag << 2);
    UINT Flag     = pxf->SystemFlag | (pxf->StreamSystemFlag << 1);

    if  (((Flag == 0) || (Flag == 3))  &&
            ((CharFlag == 0) || (CharFlag == 5) || (CharFlag == 10)))
        return TRUE;
    else return FALSE;
}

int XFGetIndexTableFromName (char* Name)
{
    int i;
    for (i = 0; i < TABLESNUMBER; i++)
        if (strcmp (Name, XFDefinedTables[i].Label) == 0)
            return i;
    return -1;
}

int XFInit (XF* pxf, UINT readwrite, UINT chartrans)
{
    UINT CharFlag;

#if defined(LOW_HIGH)
    pxf->SystemFlag = XFLittleEndian;
#endif
#if defined(HIGH_LOW)
    pxf->SystemFlag = XFBigEndian;
#endif

#if defined(CHAR_ASCII)
    pxf->CharFlag = XFAscii;
#endif
#if defined(CHAR_EBCDIC)
    pxf->CharFlag = XFEbcdic;
#endif
#if defined(CHAR_OS2)
    pxf->CharFlag = XFOs2;
#endif

    pxf->StreamSystemFlag = readwrite;
    pxf->StreamCharFlag = pxf->CharFlag;
    pxf->XlatTables = NULL;
  
    XFLoadSystemTable(pxf, &XFEbcdicWindows);
    XFLoadSystemTable(pxf, &XFEbcdicEnglish);
    XFLoadSystemTable(pxf, &XFEbcdicTurc);

    pxf->XL = NULL;
    CharFlag = pxf->CharFlag | (chartrans << 2);
    switch (CharFlag)
    {
    case 1 :
    case 4 :
        pxf->XL = InitXlat (pxf, XFEbcdicWindows.From, XFEbcdicWindows.To);
        if (!(pxf->XL))  XFError ("TEXF002", 98);

        strcpy (pxf->FileName, "");
        pxf->StreamCharFlag = chartrans;
        if (CharFlag == 1) pxf->XL->FromTo = 0;
        else pxf->XL->FromTo = 1;
        break;
    case 2 :
        if (XFLoadTable (pxf, "os2_asc.xlt") == NULL)          XFError ("TEXF001", 97);
        if (!(pxf->XL = InitXlat (pxf, "OS2", "ASCII"))) XFError ("TEXF002", 98);
        strcpy (pxf->FileName, "os2_asc.xlt");
        pxf->StreamCharFlag = chartrans;
        break;
    case 6 :
        if (XFLoadTable (pxf, "ebc_os2.xlt")  == NULL)         XFError ("TEXF001", 97);
        if (!(pxf->XL = InitXlat (pxf, "OS2", "EBCDI"))) XFError ("TEXF002", 98);
        strcpy (pxf->FileName, "ebc_os2.xlt");
        pxf->StreamCharFlag = chartrans;
        break;
    case 8 :
        if (XFLoadTable (pxf, "os2_asc.xlt")  == NULL)         XFError ("TEXF001", 97);
        if (!(pxf->XL = InitXlat (pxf, "ASCII", "OS2"))) XFError ("TEXF002", 98);
        strcpy (pxf->FileName, "os2_asc.xlt");
        pxf->StreamCharFlag = chartrans;
        break;
    case 9 :
        if (XFLoadTable (pxf, "ebc_os2.xlt")  == NULL)         XFError ("TEXF001", 97);
        if (!(pxf->XL = InitXlat (pxf, "EBCDI", "OS2"))) XFError ("TEXF002", 98);
        strcpy (pxf->FileName, "ebc_os2.xlt");
        pxf->StreamCharFlag = chartrans;
        break;
    default:
        break;
    }
    return 1;
}

void  XFChangeSystemFlag (XF* pxf, UINT xdr, UINT chartrans)
{
    UINT CharFlag;

    XFEnd (pxf);

    pxf->SystemFlag = xdr;

    pxf->CharFlag = chartrans;

    pxf->XL = NULL;
    CharFlag = pxf->CharFlag | (pxf->StreamCharFlag << 2);
    switch (CharFlag)
    {
    case 1 :
        if (XFLoadTable (pxf, "ebc_asc.xlt")  == NULL)           XFError ("TEXF001", 97);
        if (!(pxf->XL = InitXlat (pxf, "EBCDI", "ASCII"))) XFError ("TEXF002", 98);
        break;
    case 2 :
        if (XFLoadTable (pxf, "os2_asc.xlt")  == NULL)           XFError ("TEXF001", 97);
        if (!(pxf->XL = InitXlat (pxf, "OS2", "ASCII")))   XFError ("TEXF002", 98);
        break;
    case 4 :
        if (XFLoadTable (pxf, "ebc_asc.xlt")  == NULL)           XFError ("TEXF001", 97);
        if (!(pxf->XL = InitXlat (pxf, "ASCII", "EBCDI"))) XFError ("TEXF002", 98);
        break;
    case 6 :
        if (XFLoadTable (pxf, "ebc_os2.xlt")  == NULL)           XFError ("TEXF001", 97);
        if (!(pxf->XL = InitXlat (pxf, "OS2", "EBCDI")))   XFError ("TEXF002", 98);
        break;
    case 8 :
        if (XFLoadTable (pxf, "os2_asc.xlt")  == NULL)           XFError ("TEXF001", 97);
        if (!(pxf->XL = InitXlat (pxf, "ASCII", "OS2")))   XFError ("TEXF002", 98);
        break;
    case 9 :
        if (XFLoadTable (pxf, "ebc_os2.xlt")  == NULL)           XFError ("TEXF001", 97);
        if (!(pxf->XL = InitXlat (pxf, "EBCDI", "OS2")))   XFError ("TEXF002", 98);
        break;
    default:
        break;
    }
}

XLTable* XFGetXlatFromName(XF* pxf, char* Name)
{
    XLTable* t;
    List* Tables = pxf->XlatTables;


    while (Tables)
    {
        t = (XLTable*)Tables->car;
        if (strcmp(Name, t->Name) == 0)    return t;
        Tables = Tables->cdr;
    }
    return NULL;
}

/* copie s2 dans s1, jusqu'� une taille de n, ajoute le nul de fin */
char *strmcpy( char *s1, char *s2 , int n)
{
    /* retourne un pointeur sur s1 */
    char *p = s1;

    for ( ; n ; n-- )
    {
        if (*s2 == '\0') break;
        *s1 = *s2;
        s1++;s2++;
    }
    *s1 = 0;
    return p;
}
/* Delete blanks from beginning and end of string*/

char* stripe (char *src)
{
    char *dest = src;
    char *ptr = dest;
    char *last = dest;

    if ( src == NULL )
        return NULL;

    while( *src == ' ')
        src++;

    while(*src)
    {
        if (*src != ' ')
        {
            last = dest+1 ;
        }
        *dest = *src;
        dest++; src++;
    }
    *last = 0;
    return ptr;
}

XLTable* XFLoadTable(XF* pxf, char* nomfic)
{

    return NULL;
}

XLTable* XFLoadSystemTable(XF* pxf, XFTable *pTable)
{
    int rc = 0;
    int Etat = 0;
    int i, j, k;

    XLTable *new1, *sav;
    char buf[250];
    int inverse = 0;

    if ((sav = XFGetXlatFromName(pxf, pTable->Name)) != NULL)
    {
        new1 = sav;
    }
    else
    {
        new1 = (XLTable *)malloc (sizeof(XLTable));
        if (new1 == NULL)  return NULL;
        ListInsert (&pxf->XlatTables, new1);
        strcpy (new1->Name, pTable->Name);
    }
    strcpy(new1->From, pTable->From);
    strcpy(new1->To, pTable->To);
    i = 0;
    k = 0;
    while (i < 16)
    {
        int n;
        int i1;
        strcpy (buf, pTable->Table[i]);
        for (j = 0; j < 16; j++)
        {
            n = sscanf(buf + (2*j) , "%2x", &i1);
            new1->Tables[0][k] = i1;
            if (i1 != 0) new1->Tables[1][i1] = k;
            k++;
        }
        i++;
    }

    new1->IsBiject = 1;
    for(i = 0; i < 256; i++)
        if ((unsigned char)new1->Tables[1][(unsigned char)new1->Tables[0][i]] != i)
        {
            new1->IsBiject = 0;
            break;
        }
    return new1;
}

void XFError (char* name, int index)

{
    OSError (name, NULL);
}

DWORD XFStringRead (XF* pxf, char* lpbuffer, LONG* ReadBufferPos, char* BeginPointer, int Size)
{
    int j;
    DWORD loffset = 0;
    char* bpointer;
    char* bbuffer;
    for (j = 0; j < Size; j++)
    {
        bpointer = (char *)(BeginPointer + loffset);
        bbuffer  = (char *)(lpbuffer + *ReadBufferPos);
        UnXlat (pxf->XL, bbuffer , bpointer, strlen (bpointer) + 1);
        *ReadBufferPos += (int)strlen (bpointer) + 1;
        loffset += (int)strlen (bpointer) + 1;
    }
    return loffset;
}

DWORD XFStringWrite (XF* pxf, char* lpbuffer, LONG* ReadBufferPos, char* BeginPointer, int Size)
{
    int j;
    DWORD loffset = 0;
    char* bpointer;
    char* bbuffer;

    for (j = 0; j < Size; j++)
    {
        bpointer = (char *)(BeginPointer + loffset);
        bbuffer = (char*)(lpbuffer + *ReadBufferPos);
        XLat (pxf->XL, bpointer, bbuffer , strlen (bbuffer) + 1);
        *ReadBufferPos += (int)strlen (bpointer) + 1;
        loffset += (int)strlen (bpointer) + 1;
    }
    return loffset;
}

DWORD XFByteRead (XF* pxf, char* lpbuffer, LONG* ReadBufferPos, char* BeginPointer, int Size)
{
    int j = 0;

    for (j = 0; j < Size; j++)
    {
        *(char *)(lpbuffer + *ReadBufferPos) = *(char*)(BeginPointer + j);
        (*ReadBufferPos)++;
    }
    return (DWORD)j;
}

DWORD XFByteWrite (XF* pxf, char* lpbuffer, LONG* ReadBufferPos, char* BeginPointer, int Size)
{
    int i = 0;

    for (i = 0; i < Size; i++)
    {
        *(BYTE *)(BeginPointer + i) = *(char *)(lpbuffer + *ReadBufferPos);
        (*ReadBufferPos)++;
    }
    return (DWORD)i;
}

DWORD XFCharRead (XF* pxf, char* lpbuffer, LONG* ReadBufferPos, char* BeginPointer,  int Size)
{

    char* bbuffer  = (char *)(lpbuffer + *ReadBufferPos);
    UnXlat (pxf->XL, bbuffer , BeginPointer, Size);
    *ReadBufferPos += Size;
    return (DWORD)Size;
}

DWORD XFCharWrite (XF* pxf, char* lpbuffer, LONG* ReadBufferPos, char* BeginPointer,int Size)
{

    char* bbuffer  = (char *)(lpbuffer + *ReadBufferPos);
    XLat (pxf->XL, BeginPointer, bbuffer, Size);
    *ReadBufferPos += Size;
    return (DWORD)Size;
}

DWORD XFFileRead (XF* pxf, char* lpbuffer, LONG* ReadBufferPos, char* BeginPointer, int Size)
{
    DWORD  loffset = 0;
    loffset += XFCharRead (pxf, lpbuffer, ReadBufferPos, BeginPointer, FILENAMESIZE);
    loffset += XFCharRead (pxf, lpbuffer, ReadBufferPos, (char *) (BeginPointer + loffset), FILENAMESIZE);
    loffset += XFCharRead (pxf, lpbuffer, ReadBufferPos, (char *) (BeginPointer + loffset), 3);
    loffset += XFDWordRead (pxf, lpbuffer, ReadBufferPos, (char *) (BeginPointer + loffset), 1);
    loffset += XFLongRead (pxf, lpbuffer, ReadBufferPos, (char *) (BeginPointer + loffset), 1);
    return loffset;
}

DWORD XFFileWrite (XF* pxf, char* lpbuffer, LONG* ReadBufferPos, char* BeginPointer, int Size)
{

    DWORD  loffset = 0;
    loffset += XFCharWrite (pxf,  lpbuffer,  ReadBufferPos, BeginPointer, FILENAMESIZE);
    loffset += XFCharWrite (pxf,  lpbuffer,  ReadBufferPos, (char *) (BeginPointer + loffset), FILENAMESIZE);
    loffset += XFCharWrite (pxf,  lpbuffer,  ReadBufferPos, (char *) (BeginPointer + loffset), 3);
    loffset += XFDWordWrite (pxf, lpbuffer,  ReadBufferPos, (char *) (BeginPointer + loffset), 1);
    loffset += XFLongWrite (pxf,  lpbuffer,  ReadBufferPos, (char *) (BeginPointer + loffset), 1);
    return loffset;
}

DWORD XFWordRead (XF* pxf, char* lpbuffer, LONG* ReadBufferPos, char* BeginPointer, int Size)
{
    UINT Flag = pxf->SystemFlag | (pxf->StreamSystemFlag << 1);
    int i, j;
    DWORD loffset = 0;

    switch (Flag) {
    case 0 :
    case 3 :
        for (j = 0; j < Size; j++)
        {

            for (i = 0; i < 2; i++)
            {
                *(char *)(lpbuffer +*ReadBufferPos) = *(char*)(BeginPointer + loffset);
                (*ReadBufferPos)++;
                loffset ++;
            }
        }
        break;
    case 1 :
    case 2 :
        for (j = 0; j < Size; j++)
            for (i = 0; i < 2; i++)
            {
                *(char *)(lpbuffer +*ReadBufferPos) = *(char*)(BeginPointer + (++loffset - (2*i)));
                (*ReadBufferPos)++;
            }
        break;
    }
    return (DWORD)(2 * Size);
}

DWORD XFShortRead (XF* pxf, char* lpbuffer, LONG* ReadBufferPos, char* BeginPointer, int Size)
{
    UINT Flag = pxf->SystemFlag | (pxf->StreamSystemFlag << 1);
    DWORD loffset = 0;
    int i, j;

    switch (Flag) {
    case 0 :
    case 3 :
        for (j = 0; j < Size; j++)
        {
            for (i = 0; i < 2; i++)
            {
                *(char*) (lpbuffer + *ReadBufferPos) = *(char*)(BeginPointer + loffset);
                (*ReadBufferPos)++;
                loffset ++;
            }
        }
        break;
    case 1 :
    case 2 :
        for (j = 0; j < Size; j++)
            for (i = 0; i < 2; i++)
            {
                *(char*) (lpbuffer + *ReadBufferPos) = *(char*)(BeginPointer + (++loffset - (2*i)));
                (*ReadBufferPos)++;
            }
        break;
    }
    return (DWORD)(2 * Size);
}





DWORD XFLongRead (XF* pxf, char* lpbuffer, LONG* ReadBufferPos, char* BeginPointer, int Size)
{
    UINT Flag = pxf->SystemFlag | (pxf->StreamSystemFlag << 1);
    DWORD loffset = 0;
    int i, j;

    switch (Flag) {
    case 0 :
    case 3 :
        for (j = 0; j < Size; j++)
        {
            for (i = 0; i < 4; i++)
            {
                *(char*) (lpbuffer + *ReadBufferPos) = *(char*)(BeginPointer + loffset);
                (*ReadBufferPos)++;
                loffset ++;
            }
        }
        break;
    case 1 :
    case 2 :
        for (j = 0; j < Size; j++)
            for (i = 0; i < 4; i++)
            {
                *(char*)(lpbuffer + *ReadBufferPos) = *(char*)(BeginPointer + (++loffset + 2 - 2*i));
                (*ReadBufferPos)++;
            }
        break;
    }
    return (DWORD)(4 * Size);
}






DWORD XFDWordRead (XF* pxf, char* lpbuffer, LONG* ReadBufferPos, char* BeginPointer, int Size)
{
    UINT Flag = pxf->SystemFlag | (pxf->StreamSystemFlag << 1);
    DWORD loffset = 0;
    int i, j;

    switch (Flag) {
    case 0 :
    case 3 :
        for (j = 0; j < Size; j++)
        {
            for (i = 0; i < 4; i++)
            {
                *(char*) (lpbuffer + *ReadBufferPos) = *(char*)(BeginPointer + loffset);
                (*ReadBufferPos)++;
                loffset ++;
            }
        }
        break;
    case 1 :
    case 2 :
        for (j = 0; j < Size; j++)
            for (i = 0; i < 4; i++)
            {
                *(char *) (lpbuffer + *ReadBufferPos) = *(char*)(BeginPointer + (++loffset + 2 - 2*i));
                (*ReadBufferPos)++;
            }
        break;
    }
    return (DWORD)(4 * Size);
}

DWORD XFDoubleRead (XF* pxf, char* lpbuffer, LONG* ReadBufferPos, char* BeginPointer, int Size)
{
    UINT Flag = pxf->SystemFlag | (pxf->StreamSystemFlag << 1);
    DWORD loffset = 0;
    int i, j;

    switch (Flag)    {
    case 0 :
    case 3 :
        for (j = 0; j < Size; j++)
        {
            for (i = 0; i < 8; i++)
            {
                *(char*) (lpbuffer + *ReadBufferPos) = *(char*)(BeginPointer + loffset);
                (*ReadBufferPos)++;
                loffset ++;
            }
        }
        break;
    case 1 :
    case 2 :
        for (j = 0; j < Size; j++)
            for (i = 0; i < 8; i++)
            {
                *(char*)(lpbuffer + *ReadBufferPos) = *(char*)(BeginPointer + (++loffset + 6 - 2*i));
                (*ReadBufferPos)++;
            }
        break;
    }
    return (DWORD)(8 * Size);
}


DWORD XFDoubleWrite (XF* pxf , char* lpbuffer, LONG* ReadBufferPos, char* BeginPointer, int Size)
{
    int i, j;
    DWORD loffset = 0;
    UINT Flag = pxf->SystemFlag | (pxf->StreamSystemFlag << 1);
    switch (Flag) {
    case 0 :
    case 3 :
        for (i = 0; i < Size; i++)
        {
            for (j = 0; j < 8; j++)
            {
                *(char*)(BeginPointer + loffset) = *(char *) (lpbuffer + *ReadBufferPos) ;
                *ReadBufferPos += 1;
                loffset += 1;
            }
        }
        break;
    case 1 :
    case 2 :
        for (i = 0; i < Size; i++)
        {
            for (j = 0; j < 8; j++)
                *(char*)(BeginPointer + j + loffset) = *(char *) (lpbuffer + (*ReadBufferPos + (7 - j))) ;
            *ReadBufferPos += 8;
            loffset += 8;
        }
        break;
    }
    return (DWORD) (8 * Size);
}





DWORD XFLongWrite (XF* pxf , char* lpbuffer, LONG* ReadBufferPos, char* BeginPointer, int Size)
{
    int i, j;
    DWORD loffset = 0;
    UINT Flag = pxf->SystemFlag | (pxf->StreamSystemFlag << 1);
    switch (Flag) {
    case 0 :
    case 3 :
        for (i = 0; i < Size; i++)
        {
            for (j = 0; j < 4; j++)
            {
                *(char*)(BeginPointer + loffset) = *(char *) (lpbuffer + *ReadBufferPos) ;
                *ReadBufferPos += 1;
                loffset += 1;
            }
        }
        break;
    case 1 :
    case 2 :
        for (i = 0; i < Size; i++)
        {
            for (j = 0; j < 4; j++)
                *(char*)(BeginPointer + j + loffset) = *(char *) (lpbuffer + (*ReadBufferPos + (3 - j))) ;
            *ReadBufferPos += 4;
            loffset += 4;
        }
        break;
    }
    return (DWORD) (4 * Size);
}




DWORD XFWordWrite (XF* pxf, char* lpbuffer, LONG* ReadBufferPos, char* BeginPointer, int Size)
{
    UINT Flag = pxf->SystemFlag | (pxf->StreamSystemFlag << 1);
    int i, j;
    DWORD loffset = 0;
    switch (Flag) {
    case 0 :
    case 3 :
        for (i = 0; i < Size; i++)
        {
            for (j = 0; j < 2; j++)
            {
                *(char*)(BeginPointer + loffset) = *(char *) (lpbuffer + *ReadBufferPos) ;
                *ReadBufferPos += 1;
                loffset += 1;
            }
        }
        break;
    case 1 :
    case 2 :
        for (i = 0; i < Size; i++)
        {
            for (j = 0; j < 2; j++)
                *(char*)(BeginPointer + j + loffset) =
                    *(char *)(lpbuffer +*ReadBufferPos + (1 - j));
            *ReadBufferPos += 2;
            loffset += 2;
        }
        break;
    }
    return (DWORD)(2 * Size);
}






DWORD XFShortWrite (XF* pxf, char* lpbuffer, LONG* ReadBufferPos, char* BeginPointer, int Size)
{
    UINT Flag = pxf->SystemFlag | (pxf->StreamSystemFlag << 1);
    int i, j;
    DWORD loffset = 0;
    switch (Flag) {
    case 0 :
    case 3 :
        for (i = 0; i < Size; i++)
        {
            for (j = 0; j < 2; j++)
            {
                *(char*)(BeginPointer + loffset) = *(char *) (lpbuffer + *ReadBufferPos) ;
                *ReadBufferPos += 1;
                loffset += 1;
            }
        }
        break;
    case 1 :
    case 2 :
        for (i = 0; i < Size; i++)
        {
            for (j = 0; j < 2; j++)
                *(char*)(BeginPointer + j + loffset) =
                    *(char*)(lpbuffer + *ReadBufferPos + (1 - j));
            *ReadBufferPos += 2;
            loffset += 2;
        }
        break;
    }
    return (DWORD)(2 * Size);
}


DWORD XFDWordWrite (XF* pxf , char* lpbuffer, LONG* ReadBufferPos, char* BeginPointer, int Size)
{
    int i, j;
    DWORD loffset = 0;
    UINT Flag = pxf->SystemFlag | (pxf->StreamSystemFlag << 1);

    switch (Flag) {
    case 0 :
    case 3 :
        for (i = 0; i < Size; i++)
        {
            for (j = 0; j < 4; j++)
            {
                *(char*)(BeginPointer + loffset) = *(char *) (lpbuffer + *ReadBufferPos) ;
                *ReadBufferPos += 1;
                loffset += 1;
            }
        }
        break;
    case 1 :
    case 2 :
        for (i = 0; i < Size; i++)
        {
            for (j = 0; j < 4; j++)
                *(char*)(BeginPointer + j + loffset) =  * (char*) (lpbuffer + *ReadBufferPos + (3 - j)) ;
            *ReadBufferPos += 4;
            loffset += 4;
        }
        break;
    }
    return (DWORD) (4 * Size);
}



void XFEnd (XF* pxf)
{
    XLTable* xlat;
    while (pxf->XlatTables)
    {
        xlat = (XLTable*)pxf->XlatTables->car;
        ListRemove (&pxf->XlatTables, pxf->XlatTables->car);
        free (xlat);
    }
    EndXlat (pxf->XL);
}
size_t RdSGnrl(FILE* f, char* s, int max, int option)
{
    int lus = 0;  /* nb d'octets lus */
    int c;
    int nocoul = !(option & S_COLOR);

    while ((c = getc(f)) != EOF)
    {
        if (nocoul || ((char)c != COLORCAR)) lus++;
        if (c == '\n')  /* fin de ligne cr   */
        {
            if (option & S_FILL)
                for (; lus <= max; lus++)
                {
                    *s = ' ';
                    s++;
                }
            break;
        }
        if (lus > max)
        {
            if (option & (S_TRUNC | S_FILL))
            {
                if (c != '\n') ungetc(c, f);
                break;
            }
        }
        else
        {
            *s = c;
            s++;
        }
    }
    if (option & S_FILL)
        for (; lus <= max; lus++)
        {
            *s = ' ';
            s++;
        }
    *s = 0;
    return lus;
}
