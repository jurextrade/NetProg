/* NetProg is an API in C language allowing an homogeneous programming of  */
/* communicating applications                                              */
/* Copyright (C) 2002,2003,2004,2005                                       */
/* Gabriel Jureidini                                                       */
/* Version 2.0 - 17 July 2005                                              */
/*-------------------------------------------------------------------------*/


#define _CRT_SECURE_NO_WARNINGS
#include "sc.h"

int SCAddTime(char* Val1, char* Val2)
{
    DWORD time;
    SHORT heure[2], minute[2], seconde[2];
    char timechar[7]; /* pour conversion HH:MM:SS -> HHMMSS */

    timechar[0] = Val1[0];
    timechar[1] = Val1[1];
    timechar[2] = Val1[3];
    timechar[3] = Val1[4];
    timechar[4] = Val1[6];
    timechar[5] = Val1[7];
    timechar[6] = 0;

    time = (DWORD) atol(timechar);

    heure[0]   = (SHORT)(time/10000);
    minute[0]  = (SHORT)((time - heure[0]*10000)/100);
    seconde[0] = (SHORT)(time - (heure[0]*10000 + minute[0]*100));

    timechar[0] = Val2[0];
    timechar[1] = Val2[1];
    timechar[2] = Val2[3];
    timechar[3] = Val2[4];
    timechar[4] = Val2[6];
    timechar[5] = Val2[7];
    timechar[6] = 0;

    time = (DWORD) atol(timechar);

    heure[1]   = (SHORT)(time/10000);
    minute[1]  = (SHORT)((time - heure[1]*10000)/100);
    seconde[1] = (SHORT)(time - (heure[1]*10000 + minute[1]*100));

    seconde[0] = seconde[0] + seconde[1];
    if (seconde[0] > 59)
    {
        minute[0]++;
        if (minute[0] > 59)
        {
            heure[0]++;
            if (heure[0] > 23)
                heure[0] = heure[0] - 24;
            minute[0] = minute[0] - 60;
        }

        seconde[0] = seconde[0] - 60;
    }

    minute[0] = minute[0] + minute[1];
    if (minute[0] > 59)
    {
        heure[0]++;
        if (heure[0] > 23)
            heure[0] = heure[0] - 24;
        minute[0] = minute[0] - 60;
    }

    heure[0] = heure[0] + heure[1];
    if (heure[0] > 23)
        heure[0] = heure[0] - 24;

    sprintf(Val1, "%.2d:%.2d:%.2d", heure[0], minute[0], seconde[0]);

    return 1;
}

/*------------------------------------------------------------------------*/

int SCMinusTime(char* Val1, char* Val2)
{
    DWORD time;
    SHORT heure[2], minute[2], seconde[2];
    char timechar[7]; /* pour conversion HH:MM:SS -> HHMMSS */

    timechar[0] = Val1[0];
    timechar[1] = Val1[1];
    timechar[2] = Val1[3];
    timechar[3] = Val1[4];
    timechar[4] = Val1[6];
    timechar[5] = Val1[7];
    timechar[6] = 0;

    time = (DWORD) atol(timechar);

    heure[0]   = (SHORT)(time/10000);
    minute[0]  = (SHORT)((time - heure[0]*10000)/100);
    seconde[0] = (SHORT)(time - (heure[0]*10000 + minute[0]*100));

    timechar[0] = Val2[0];
    timechar[1] = Val2[1];
    timechar[2] = Val2[3];
    timechar[3] = Val2[4];
    timechar[4] = Val2[6];
    timechar[5] = Val2[7];
    timechar[6] = 0;

    time = (DWORD) atol(timechar);

    heure[1]   = (SHORT)(time/10000);
    minute[1]  = (SHORT)((time - heure[1]*10000)/100);
    seconde[1] = (SHORT)(time - (heure[1]*10000 + minute[1]*100));

    heure[0] = heure[0] - heure[1];
    if (heure[0] < 0)
        heure[0] = heure[0] + 24;

    minute[0] = minute[0] - minute[1];
    if (minute[0] < 0)
    {
        heure[0]--;
        if (heure[0] < 0)
            heure[0] = heure[0] + 24;
        minute[0] = minute[0] + 60;
    }

    seconde[0] = seconde[0] - seconde[1];
    if (seconde[0] < 0)
    {
        minute[0]--;
        if (minute[0] < 0)
        {
            heure[0]--;
            if (heure[0] < 0)
                heure[0] = heure[0] + 24;
            minute[0] = minute[0] + 60;
        }

        seconde[0] = seconde[0] + 60;
    }

    sprintf(Val1, "%.2d:%.2d:%.2d", heure[0], minute[0], seconde[0]);

    return 1;
}

/*------------------------------------------------------------------------*/

int SCMinusStr(SCValue* Value1, SCValue* Value2)
{
    char* tampon;
    int i, lengthVal1, lengthVal2;
    char* Val1 = Value1->Val;
    char* Val2 = Value2->Val;

    if ((!Val1) || (!Val2))    return 0;

    lengthVal1 = strlen(Val1);
    lengthVal2 = strlen(Val2);
    i = strlen(Val1) - lengthVal2;

    while(i >= 0)
    {
        if  (OSStrncmp (Val1 + i, Val2, lengthVal2) == 0)
        {
            tampon = (char *)malloc(lengthVal1 - lengthVal2 + 1);
            if (!tampon) return -1;

            tampon = strncpy(tampon, Val1, i);
            tampon[i] = 0;
            strcat(tampon, Val1 + i + lengthVal2);
            free(Val1);

            Val1 = (char*)malloc(strlen(tampon) + 1);
            if (!Val1) return -1;
            Value1->Val = Val1;
            strcpy(Val1, tampon);
            free(tampon);
            return 1;
        }

        i--;
    }

    return 0;
}

/*------------------------------------------------------------------------*/


SCPROCRESULT SCOpeAddProc (SC* psc,  SCAction* thisaction)
{
    SCValue Value1;
    SCValue* Value   = NULL;
    SCValue* Value2  = NULL;
    WORD NbParam     = 0;
    DWORD size       = 0;
    int Error        = 0;
    WORD i;
    char* val;

    SCASSERTMINPARAM (thisaction, 2);

    NbParam = thisaction->NbParam;

    Value = SCGetParamValue (psc, thisaction, 1);
    SCASSERTDEFINEDVALUE(Value);

    Value1.Val = NULL;
    SCCopyValue (psc, &Value1, Value);

    for (i = 2; i <= NbParam; i++)
    {
        Value2 = SCGetParamValue (psc, thisaction, i);
        SCASSERTDEFINEDVALUE(Value2);

        /* types non gérés actuellement */
        if ((Value1.Type == SCNULL)    ||
                (Value1.Type == SCFLOAT)   ||
                (Value1.Type == SCBOOLEAN) ||
                (Value1.Type == SCDATE))
            return NULL;

        if ((Value1.Type == SCSTRING) ||
                (Value1.Type == SCCHAR)   ||
                (Value1.Type == SCPOINTER))
        {
            /* les types ne sont pas egaux */
            SCASSERTORVALUES (Value2, SCSTRING, SCCHAR);

            size = strlen ((char*)Value1.Val) + strlen((char*)Value2->Val) + 1;
            val = (char*)malloc(size);
            strcpy (val, (char*)Value1.Val);
            strcat(val, (char*)Value2->Val);
            SCAffectValue (&Value1, val, Value1.Type);
        }

        if (Value1.Type == SCINTEGER)
        {
            /* les types ne sont pas egaux */
            SCASSERTORVALUES (Value2, SCINTEGER, SCLONG);
            Value1.Val = (void *)((int)Value1.Val + (int)Value2->Val);
        }
        if (Value1.Type == SCLONG)
        {
            /* les types ne sont pas egaux */
            SCASSERTORVALUES (Value2, SCINTEGER, SCLONG);
            Value1.Val = (void *)((LONG)Value1.Val + (LONG)Value2->Val);
        }
        if (Value1.Type == SCTIME)
        {
            /* les types ne sont pas egaux */
            SCASSERTVALUE (Value2, SCTIME);
            if (SCAddTime(Value1.Val, Value2->Val) < 0)
                return NULL;
        }
    }

    SCCopyValue (psc,  thisaction->Value, &Value1);
    SCSetActionState (psc, thisaction, SCDONE, SCDEFINED, Error);

    /* il faut libere Value1 */
    return thisaction->Value;
}

/*------------------------------------------------------------------------*/

SCPROCRESULT SCOpeMinusProc (SC* psc,  SCAction* thisaction)
{
    SCValue Value1;
    SCValue* Value   = NULL;
    SCValue* Value2  = NULL;
    WORD NbParam     = 0;
    DWORD size       = 0;
    int Error        = 0;
    WORD i;

    SCASSERTMINPARAM (thisaction, 2);

    NbParam = thisaction->NbParam;

    Value = SCGetParamValue (psc, thisaction, 1);
    SCASSERTDEFINEDVALUE(Value);

    Value1.Val = NULL;
    SCCopyValue (psc, &Value1, Value);

    for (i = 2; i <= NbParam; i++)
    {
        Value2 = SCGetParamValue (psc, thisaction, i);
        SCASSERTDEFINEDVALUE(Value2);

        /* types non gérés actuellement */
        if ((Value1.Type == SCNULL)    ||
                (Value1.Type == SCFLOAT)   ||
                (Value1.Type == SCPOINTER) ||
                (Value1.Type == SCBOOLEAN) ||
                (Value1.Type == SCDATE))
            return NULL;

        if ((Value1.Type == SCSTRING) ||
                (Value1.Type == SCCHAR))
        {
            /* les types ne sont pas egaux */
            SCASSERTORVALUES (Value2, SCSTRING, SCCHAR);

            if (SCMinusStr(&Value1, Value2) < 0)
                return NULL;
            Value1.Size = strlen(Value1.Val) + 1;
        }

        if (Value1.Type == SCINTEGER)
        {
            /* les types ne sont pas egaux */
            SCASSERTORVALUES (Value2, SCINTEGER, SCLONG);
            Value1.Val = (void *)((int)Value1.Val - (int)Value2->Val);
        }
        if (Value1.Type == SCLONG)
        {
            /* les types ne sont pas egaux */
            SCASSERTORVALUES (Value2, SCINTEGER, SCLONG);
            Value1.Val = (void *)((LONG)Value1.Val - (LONG)Value2->Val);
        }
        if (Value1.Type == SCTIME)
        {
            /* les types ne sont pas egaux */
            SCASSERTVALUE (Value2, SCTIME);
            if (SCMinusTime(Value1.Val, Value2->Val) < 0)
                return NULL;
        }
    }

    SCCopyValue (psc, thisaction->Value, &Value1);
    SCSetActionState (psc, thisaction, SCDONE, SCDEFINED, Error);

    /* il faut libere Value1 */
    return thisaction->Value;
}

/*------------------------------------------------------------------------*/

SCPROCRESULT SCOpeMultProc (SC* psc,  SCAction* thisaction)
{
    SCValue Value1;
    SCValue* Value   = NULL;
    SCValue* Value2  = NULL;
    WORD NbParam     = 0;
    DWORD size       = 0;
    int Error        = 0;
    WORD i;

    SCASSERTMINPARAM (thisaction, 2);

    NbParam = thisaction->NbParam;

    Value = SCGetParamValue (psc, thisaction, 1);
    SCASSERTDEFINEDVALUE(Value);

    Value1.Val = NULL;
    SCCopyValue (psc, &Value1, Value);

    for (i = 2; i <= NbParam; i++)
    {
        Value2 = SCGetParamValue (psc, thisaction, i);
        SCASSERTDEFINEDVALUE(Value2);

        /* types non gérés actuellement */
        if ((Value1.Type == SCNULL)    ||
                (Value1.Type == SCFLOAT)   ||
                (Value1.Type == SCPOINTER) ||
                (Value1.Type == SCBOOLEAN) ||
                (Value1.Type == SCSTRING)  ||
                (Value1.Type == SCCHAR)    ||
                (Value1.Type == SCDATE)    ||
                (Value1.Type == SCTIME))
            return NULL;

        if (Value1.Type == SCINTEGER)
        {
            /* les types ne sont pas egaux */
            SCASSERTORVALUES (Value2, SCINTEGER, SCLONG);
            Value1.Val = (void *)((int)Value1.Val * (int)Value2->Val);
        }
        if (Value1.Type == SCLONG)
        {
            /* les types ne sont pas egaux */
            SCASSERTORVALUES (Value2, SCINTEGER, SCLONG);
            Value1.Val = (void *)((LONG)Value1.Val * (LONG)Value2->Val);
        }
    }

    SCCopyValue (psc, thisaction->Value, &Value1);
    SCSetActionState (psc, thisaction, SCDONE, SCDEFINED, Error);

    return thisaction->Value;
}

/*------------------------------------------------------------------------*/

SCPROCRESULT SCOpeDivProc (SC* psc,  SCAction* thisaction)
{
    SCValue Value1;
    SCValue* Value   = NULL;
    SCValue* Value2  = NULL;
    WORD NbParam     = 0;
    DWORD size       = 0;
    int Error        = 0;
    WORD i;

    SCASSERTMINPARAM (thisaction, 2);

    NbParam = thisaction->NbParam;

    Value = SCGetParamValue (psc, thisaction, 1);
    SCASSERTDEFINEDVALUE(Value);

    Value1.Val = NULL;
    SCCopyValue (psc, &Value1, Value);

    for (i = 2; i <= NbParam; i++)
    {
        Value2 = SCGetParamValue (psc, thisaction, i);
        SCASSERTDEFINEDVALUE(Value2);

        /* types non gérés actuellement */
        if ((Value1.Type == SCNULL)    ||
                (Value1.Type == SCFLOAT)   ||
                (Value1.Type == SCPOINTER) ||
                (Value1.Type == SCBOOLEAN) ||
                (Value1.Type == SCSTRING)  ||
                (Value1.Type == SCCHAR)    ||
                (Value1.Type == SCDATE)    ||
                (Value1.Type == SCTIME))
            return NULL;

        if (Value1.Type == SCINTEGER)
        {
            /* les types ne sont pas egaux */
            SCASSERTORVALUES (Value2, SCINTEGER, SCLONG);
            if ((int)Value2->Val != 0)
                Value1.Val = (void *)((int)Value1.Val / (int)Value2->Val);
        }

        if (Value1.Type == SCLONG)
        {
            /* les types ne sont pas egaux */
            SCASSERTORVALUES (Value2, SCINTEGER, SCLONG);
            if ((int)Value2->Val != 0)
                Value1.Val = (void *)((LONG)Value1.Val / (LONG)Value2->Val);
        }
    }

    SCCopyValue (psc, thisaction->Value, &Value1);
    SCSetActionState (psc, thisaction, SCDONE, SCDEFINED, Error);

    return thisaction->Value;
}

/*------------------------------------------------------------------------*/

void* OperatorFunction (SC* psc, SCAction* thisaction, List* listparam, char Operator, WORD* range, int* Error)
{
    SCValue* value = NULL;

    value = SCGetParamValue (psc, thisaction, *range);

    if (value == NULL)
    {
        *Error = 1;
        return NULL;
    }
    if (!value->Defined)
    {
        *Error = 2;
        return NULL;
    }

    listparam = listparam->cdr;
    if (listparam)
    {
        (*range)++;
        if (Operator == '&')
            return (void *)(value->Val && OperatorFunction (psc, thisaction, listparam, Operator, range, Error ));
        if (Operator == '|')
            return (void *)(value->Val || OperatorFunction (psc, thisaction, listparam, Operator, range, Error));
        else
        {
            *Error = 3;
            return NULL;
        }
    }
    else return value->Val;
}

/*------------------------------------------------------------------------*/

SCPROCRESULT SCAndProc    (SC* psc,  SCAction* thisaction)
{
    List* listparam = thisaction->ListParams;
    void* Val;
    WORD range = 1;
    int result = 0;
    int Error = 0;

    SCSETRETURNVALUETYPE(thisaction->Value, SCBOOLEAN);
    SCASSERTMINPARAM (thisaction, 2);

    Val = OperatorFunction (psc, thisaction, listparam, '&', &range, &result);
    if (result == 1) return NULL;
    if (result == 2) return thisaction->Value;

    thisaction->Value->Val = Val;

    SCSetActionState (psc, thisaction, SCDONE, SCDEFINED, Error);

    return thisaction->Value;
}

/*------------------------------------------------------------------------*/

SCPROCRESULT SCOrProc    (SC* psc,  SCAction* thisaction)
{
    List* listparam = thisaction->ListParams;
    void* Val;
    WORD range = 1;
    int result = 0;
    int Error = 0;


    SCSETRETURNVALUETYPE(thisaction->Value, SCBOOLEAN);
    SCASSERTMINPARAM (thisaction, 2);

    Val = OperatorFunction (psc, thisaction, listparam, '|', &range, &result);
    if (result == 1) return NULL;
    if (result == 2) return thisaction->Value;


    thisaction->Value->Val = Val;

    SCSetActionState (psc, thisaction, SCDONE, SCDEFINED, Error);

    return thisaction->Value;
}

/*------------------------------------------------------------------------*/

SCPROCRESULT SCAtoiProc   (SC* psc,  SCAction* thisaction)
{
    SCValue* Value;
    int Error = 0;

    SCSETRETURNVALUETYPE(thisaction->Value, SCINTEGER);
    SCASSERTMINPARAM (thisaction, 1);

    Value = SCGetParamValue (psc, thisaction, 1);
    SCASSERTDEFINEDVALUE (Value);

    SCPutValue (thisaction->Value, Value->Val, SCINTEGER);

    SCSetActionState (psc, thisaction, SCDONE, SCDEFINED, Error);

    return thisaction->Value;
}

/*------------------------------------------------------------------------*/

SCPROCRESULT SCStringProc   (SC* psc,  SCAction* thisaction)
{
    SCValue* Value1;
    SCValue* Value2;
    int Error = 0;
    char Buf[250];
    char precision[50];
    char format[50];
    WORD NbParam;
    WORD range = 1;

    precision[0] = 0;

    SCSETRETURNVALUETYPE(thisaction->Value, SCSTRING);
    SCASSERTMINPARAM (thisaction, 1);
    SCASSERTMAXPARAM (thisaction, 2);

    Value1 = SCGetParamValue (psc, thisaction, range++);
    SCASSERTDEFINEDVALUE (Value1);

    NbParam = thisaction->NbParam;
    if (NbParam >= range)
    {
        Value2 = SCGetParamValue (psc, thisaction, range++);
        SCASSERTDEFINEDVALUE (Value2);
        SCASSERTORVALUES (Value2, SCCHAR, SCSTRING);

        strcpy(precision, (char*)Value2->Val);
    }

    switch (Value1->Type) {
        break;
    case SCNULL    :
    case SCBOOLEAN :
    case SCINTEGER :
        strcpy(format, "%");
        strcat(format, precision);
        strcat(format, "d");
        sprintf (Buf, format, Value1->Val);
        SCPutValue (thisaction->Value, Buf, SCSTRING);
        break;
    case SCLONG    :
        strcpy(format, "%");
        strcat(format, precision);
        strcat(format, "ld");
        sprintf (Buf, format, Value1->Val);
        SCPutValue (thisaction->Value, Buf, SCSTRING);
        break;
    case SCFLOAT   :
        break;
    case SCPOINTER :
        strcpy(format, "%");
        strcat(format, precision);
        strcat(format, "p");
        sprintf (Buf, format, Value1->Val);
        SCPutValue (thisaction->Value, Buf, SCSTRING);
        break;
    case SCSTRING  :
    case SCDATE    :
    case SCTIME    :
    case SCCHAR    :
        SCPutValue (thisaction->Value, Value1->Val, SCSTRING);
        break;
    }

    SCSetActionState (psc, thisaction, SCDONE, SCDEFINED, Error);

    return thisaction->Value;
}



/*------------------------------------------------------------------------*/

SCPROCRESULT SCMaxIntProc    (SC* psc,  SCAction* thisaction)
{
    SCValue* Value1;
    SCValue* Value2;
    char Buf[250];
    char precision[50];
    char format[50];
    int Error = 0;

    precision[0] = 0;

    SCASSERTPARAMNUMBER(thisaction, 2);

    Value1 = SCGetParamValue (psc, thisaction, 1);
    SCASSERTDEFINEDVALUE(Value1) ;
    SCASSERTORVALUES (Value1, SCINTEGER, SCLONG);

    Value2 = SCGetParamValue (psc, thisaction, 2);
    SCASSERTDEFINEDVALUE(Value2) ;
    SCASSERTORVALUES (Value2, SCINTEGER, SCLONG);

    strcpy(format, "%");
    strcat(format, precision);

    if (Value1->Type == SCINTEGER)
    {
        strcat(format, "d");

        if ((int)Value1->Val > (int)Value2->Val)
            sprintf (Buf, format, Value1->Val);
        else
            sprintf (Buf, format, Value2->Val);

        SCPutValue (thisaction->Value, Buf, SCINTEGER);
    }

    if (Value1->Type == SCLONG)
    {
        strcat(format, "ld");

        if ((LONG)Value1->Val > (LONG)Value2->Val)
            sprintf (Buf, format, Value1->Val);
        else
            sprintf (Buf, format, Value2->Val);

        SCPutValue (thisaction->Value, Buf, SCLONG);
    }


    SCSetActionState (psc, thisaction, SCDONE, SCDEFINED, Error);

    return thisaction->Value;
}


/*------------------------------------------------------------------------*/

SCPROCRESULT SCMinIntProc    (SC* psc,  SCAction* thisaction)
{
    SCValue* Value1;
    SCValue* Value2;
    char Buf[250];
    char precision[50];
    char format[50];
    int Error = 0;

    precision[0] = 0;

    SCASSERTPARAMNUMBER(thisaction, 2);

    Value1 = SCGetParamValue (psc, thisaction, 1);
    SCASSERTDEFINEDVALUE(Value1) ;
    SCASSERTORVALUES (Value1, SCINTEGER, SCLONG);

    Value2 = SCGetParamValue (psc, thisaction, 2);
    SCASSERTDEFINEDVALUE(Value2) ;
    SCASSERTORVALUES (Value2, SCINTEGER, SCLONG);

    strcpy(format, "%");
    strcat(format, precision);

    if (Value1->Type == SCINTEGER)
    {
        strcat(format, "d");

        if ((int)Value1->Val < (int)Value2->Val)
            sprintf (Buf, format, Value1->Val);
        else
            sprintf (Buf, format, Value2->Val);

        SCPutValue (thisaction->Value, Buf, SCINTEGER);
    }

    if (Value1->Type == SCLONG)
    {
        strcat(format, "ld");

        if ((LONG)Value1->Val < (LONG)Value2->Val)
            sprintf (Buf, format, Value1->Val);
        else
            sprintf (Buf, format, Value2->Val);

        SCPutValue (thisaction->Value, Buf, SCLONG);
    }


    SCSetActionState (psc, thisaction, SCDONE, SCDEFINED, Error);

    return thisaction->Value;
}



/*------------------------------------------------------------------------*/

SCPROCRESULT SCGreaterProc    (SC* psc,  SCAction* thisaction)
{
    SCValue* Value1;
    SCValue* Value2;
    char Buf[250];
    char precision[50];
    char format[50];
    int Error = 0;

    precision[0] = 0;

    SCASSERTPARAMNUMBER(thisaction, 2);

    Value1 = SCGetParamValue (psc, thisaction, 1);
    SCASSERTDEFINEDVALUE(Value1) ;
    SCASSERTORVALUES (Value1, SCINTEGER, SCLONG);

    Value2 = SCGetParamValue (psc, thisaction, 2);
    SCASSERTDEFINEDVALUE(Value2) ;
    SCASSERTORVALUES (Value2, SCINTEGER, SCLONG);

    strcpy(format, "%");
    strcat(format, precision);

    if (Value1->Type == SCINTEGER)
    {
        strcat(format, "d");

        if ((int)Value1->Val > (int)Value2->Val)
            sprintf (Buf, format, 1);
        else
            sprintf (Buf, format, 0);

        SCPutValue (thisaction->Value, Buf, SCINTEGER);
    }

    if (Value1->Type == SCLONG)
    {
        strcat(format, "ld");

        if ((LONG)Value1->Val > (LONG)Value2->Val)
            sprintf (Buf, format, 1);
        else
            sprintf (Buf, format, 0);

        SCPutValue (thisaction->Value, Buf, SCLONG);
    }

    SCSetActionState (psc, thisaction, SCDONE, SCDEFINED, Error);

    return thisaction->Value;
}



/*------------------------------------------------------------------------*/

SCPROCRESULT SCSmallerProc    (SC* psc,  SCAction* thisaction)
{
    SCValue* Value1;
    SCValue* Value2;
    char Buf[250];
    char precision[50];
    char format[50];
    int Error = 0;

    precision[0] = 0;

    SCASSERTPARAMNUMBER(thisaction, 2);

    Value1 = SCGetParamValue (psc, thisaction, 1);
    SCASSERTDEFINEDVALUE(Value1) ;
    SCASSERTORVALUES (Value1, SCINTEGER, SCLONG);

    Value2 = SCGetParamValue (psc, thisaction, 2);
    SCASSERTDEFINEDVALUE(Value2) ;
    SCASSERTORVALUES (Value2, SCINTEGER, SCLONG);

    strcpy(format, "%");
    strcat(format, precision);

    if (Value1->Type == SCINTEGER)
    {
        strcat(format, "d");

        if ((int)Value1->Val < (int)Value2->Val)
            sprintf (Buf, format, 1);
        else
            sprintf (Buf, format, 0);

        SCPutValue (thisaction->Value, Buf, SCINTEGER);
    }

    if (Value1->Type == SCLONG)
    {
        strcat(format, "ld");

        if ((LONG)Value1->Val < (LONG)Value2->Val)
            sprintf (Buf, format, 1);
        else
            sprintf (Buf, format, 0);

        SCPutValue (thisaction->Value, Buf, SCLONG);
    }

    SCSetActionState (psc, thisaction, SCDONE, SCDEFINED, Error);

    return thisaction->Value;
}



/*------------------------------------------------------------------------*/

SCPROCRESULT SCGreatEquProc    (SC* psc,  SCAction* thisaction)
{
    SCValue* Value1;
    SCValue* Value2;
    char Buf[250];
    char precision[50];
    char format[50];
    int Error = 0;

    precision[0] = 0;

    SCASSERTPARAMNUMBER(thisaction, 2);

    Value1 = SCGetParamValue (psc, thisaction, 1);
    SCASSERTDEFINEDVALUE(Value1) ;
    SCASSERTORVALUES (Value1, SCINTEGER, SCLONG);

    Value2 = SCGetParamValue (psc, thisaction, 2);
    SCASSERTDEFINEDVALUE(Value2) ;
    SCASSERTORVALUES (Value2, SCINTEGER, SCLONG);

    strcpy(format, "%");
    strcat(format, precision);

    if (Value1->Type == SCINTEGER)
    {
        strcat(format, "d");

        if ((int)Value1->Val >= (int)Value2->Val)
            sprintf (Buf, format, 1);
        else
            sprintf (Buf, format, 0);

        SCPutValue (thisaction->Value, Buf, SCINTEGER);
    }

    if (Value1->Type == SCLONG)
    {
        strcat(format, "ld");

        if ((LONG)Value1->Val >= (LONG)Value2->Val)
            sprintf (Buf, format, 1);
        else
            sprintf (Buf, format, 0);

        SCPutValue (thisaction->Value, Buf, SCLONG);
    }

    SCSetActionState (psc, thisaction, SCDONE, SCDEFINED, Error);

    return thisaction->Value;
}



/*------------------------------------------------------------------------*/

SCPROCRESULT SCSmallEquProc    (SC* psc,  SCAction* thisaction)
{
    SCValue* Value1;
    SCValue* Value2;
    char Buf[250];
    char precision[50];
    char format[50];
    int Error = 0;

    precision[0] = 0;

    SCASSERTPARAMNUMBER(thisaction, 2);

    Value1 = SCGetParamValue (psc, thisaction, 1);
    SCASSERTDEFINEDVALUE(Value1) ;
    SCASSERTORVALUES (Value1, SCINTEGER, SCLONG);

    Value2 = SCGetParamValue (psc, thisaction, 2);
    SCASSERTDEFINEDVALUE(Value2) ;
    SCASSERTORVALUES (Value2, SCINTEGER, SCLONG);

    strcpy(format, "%");
    strcat(format, precision);

    if (Value1->Type == SCINTEGER)
    {
        strcat(format, "d");

        if ((int)Value1->Val <= (int)Value2->Val)
            sprintf (Buf, format, 1);
        else
            sprintf (Buf, format, 0);

        SCPutValue (thisaction->Value, Buf, SCINTEGER);
    }

    if (Value1->Type == SCLONG)
    {
        strcat(format, "ld");

        if ((LONG)Value1->Val <= (LONG)Value2->Val)
            sprintf (Buf, format, 1);
        else
            sprintf (Buf, format, 0);

        SCPutValue (thisaction->Value, Buf, SCLONG);
    }

    SCSetActionState (psc, thisaction, SCDONE, SCDEFINED, Error);

    return thisaction->Value;
}

