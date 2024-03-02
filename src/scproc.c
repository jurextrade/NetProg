/* NetProg is an API in C language allowing an homogeneous programming of  */
/* communicating applications                                              */
/* Copyright (C) 2002,2003,2004,2005                                       */
/* Gabriel Jureidini                                                       */
/* Version 2.0 - 17 July 2005                                              */
/*-------------------------------------------------------------------------*/


#define _CRT_SECURE_NO_WARNINGS
#include "sc.h"

#ifdef _WIN32
#include <process.h>
#endif



SCPROCRESULT SCSetqProc    (SC* psc,  SCAction* thisaction)
{
    SCValue* Value = NULL;
    SCEntity* Entity;
    SCEntity* Entity1;
    SCVariable* Var;
    SCVariable* Var1;
    int Error = 0;

    SCASSERTPARAMNUMBER (thisaction, 2);

    Entity = SCGetParam (psc, thisaction, 1);
    Var = (SCVariable*)Entity->Val;

    SCASSERTPARAMTYPE(Entity, SCVARIABLE);

    /* (setq a a) not allowed */
    Entity1 = SCGetParam (psc, thisaction, 2);
    Var1 = (SCVariable*)Entity1->Val;

    if (Entity1->Type == SCVARIABLE)
    {
        if (strcmp (Var->Name, Var1->Name) == 0)
            return NULL;
    }

    Value =  SCGetParamValue (psc, thisaction, 2);

    SCASSERTDEFINEDVALUE(Value);

    if (Value)  SCCopyValue (psc, Var->Value, Value);

    Value = SCSetParamValue (psc,  thisaction, 1, Value);

    SCCopyValue (psc,  thisaction->Value, Value);

    SCSetActionState (psc, thisaction, SCDONE, SCDEFINED, Error);

    return thisaction->Value ;
}

/*------------------------------------------------------------------------*/
/* A REVOIR */
/*------------------------------------------------------------------------*/

SCPROCRESULT SCSetProc    (SC* psc,  SCAction* thisaction)
{
    SCEntity* Entity;
    SCEntity* Entity1;
    SCValue* Value = NULL;
    SCVariable* Var;
    SCVariable* Var1;
    int Error = 0;


    SCASSERTPARAMNUMBER (thisaction, 2);

    Entity = SCGetParam (psc, thisaction, 1);

    SCASSERTPARAMTYPE(Entity, SCVARIABLE);
    Var = (SCVariable*)Entity->Val;

    /* (setq a a) not allowed */
    Entity1 = SCGetParam (psc, thisaction, 2);
    Var1 = (SCVariable*)Entity1->Val;

    if (Entity1->Type == SCVARIABLE)
    {
        if (strcmp (Var->Name, Var1->Name) == 0)
            return NULL;
    }

    Var->SetEntity = SCCopyEntity (psc, NULL, Entity1);

    SCCopyValue (psc,  thisaction->Value, Value);

    SCSetActionState (psc, thisaction, SCDONE, SCDEFINED, Error);

    return thisaction->Value ;
}

/*------------------------------------------------------------------------*/


SCPROCRESULT SCMallocProc    (SC* psc,  SCAction* thisaction)
{
    SCValue* Val1 = NULL;
    void* result;
    LONG Error = 0;

    SCSETRETURNVALUETYPE(thisaction->Value, SCPOINTER);
    SCASSERTPARAMNUMBER (thisaction, 1);

    Val1 = SCGetParamValue (psc, thisaction, 1);

    SCASSERTDEFINEDVALUE(Val1) ;
    SCASSERTORVALUES(Val1, SCINTEGER, SCLONG);

    result = malloc ((size_t)Val1->Val);
    if (!result) Error = 1;
    else
    {
        memset (result, 0, (size_t)Val1->Val);
        SCAffectValue (thisaction->Value, result, SCPOINTER);
    }

    SCSetActionState (psc, thisaction, SCDONE, SCDEFINED, Error);


    return thisaction->Value;

}


/*------------------------------------------------------------------------*/

SCPROCRESULT SCFreeProc    (SC* psc,  SCAction* thisaction)
{
    SCValue* Val1 = NULL;
    int Error = 0;

    SCSETRETURNVALUETYPE(thisaction->Value, SCPOINTER);
    SCASSERTPARAMNUMBER (thisaction, 1);

    Val1 = SCGetParamValue (psc, thisaction, 1);

    SCASSERTDEFINEDVALUE(Val1) ;
    SCASSERTORVALUES(Val1, SCPOINTER, SCLONG);

    if (Val1->Val) free ((void *)Val1->Val);


    SCPutValue (Val1, NULL, SCPOINTER);
    Val1->Defined = SCNOTDEFINED;


    SCPutValue (thisaction->Value, NULL, SCPOINTER);

    SCSetActionState (psc, thisaction, SCDONE, SCDEFINED, Error);


    return thisaction->Value;

}

/*------------------------------------------------------------------------*/

SCPROCRESULT SCSystemProc (SC* psc,  SCAction* thisaction)
{
    SCValue* Val1 = NULL;
    int Error;

    SCSETRETURNVALUETYPE(thisaction->Value, SCINTEGER);
    SCASSERTPARAMNUMBER (thisaction, 1);

    Val1 = SCGetParamValue (psc, thisaction, 1);

    SCASSERTDEFINEDVALUE(Val1) ;
    SCASSERTORVALUES(Val1, SCSTRING, SCCHAR);

#ifdef WINDOWS
#ifdef _WIN32
    Error = system ((char*)Val1->Val);
#else
    Error = WinExec ((char*)Val1->Val, SW_SHOW);
#endif
#else
    Error = system ((char*)Val1->Val);
#endif

    thisaction->Value->Val = (void *)Error;

    SCSetActionState (psc, thisaction, SCDONE, SCDEFINED, Error);

    return thisaction->Value;
}


SCPROCRESULT SCProcessProc (SC* psc,  SCAction* thisaction)
{
    SCValue* Val1 = NULL;
    SCValue* Value[20];
    int Error;
    WORD i = 0;
    SCASSERTMINPARAM (thisaction, 1);
    SCSETRETURNVALUETYPE(thisaction->Value, SCINTEGER);

    Val1 = SCGetParamValue (psc, thisaction, 1);

    SCASSERTDEFINEDVALUE(Val1) ;
    SCASSERTORVALUES(Val1, SCSTRING, SCCHAR);

    for (i = 2; i <= thisaction->NbParam; i++)
    {
        Value[i-2] = SCGetParamValue (psc, thisaction, i);
        SCASSERTDEFINEDVALUE (Value[i - 2]);
        SCASSERTORVALUES(Value[i-2], SCSTRING, SCCHAR);
    }

#ifdef WINDOWS
    /*
    #ifdef _WIN32
      if (thisaction->NbParam == 1)
       Error = _spawnlp( _P_DETACH, (char*)Val1->Val, (char*)Val1->Val, NULL );
      else
       if (thisaction->NbParam == 2)
        Error = _spawnlp( _P_DETACH, (char*)Val1->Val, (char*)Val1->Val, (char*)Value[0]->Val, NULL );
       else
       if (thisaction->NbParam == 3)
         Error = _spawnlp( _P_DETACH, (char*)Val1->Val, (char*)Val1->Val, (char*)Value[0]->Val, (char*)Value[1]->Val, NULL );
       

    #else
    */
    Error = WinExec ((char*)Val1->Val, SW_SHOW);
    /*#endif
    */
#else
    Error = system ((char*)Val1->Val);
#endif

    thisaction->Value->Val = (void *)Error;

    SCSetActionState (psc, thisaction, SCDONE, SCDEFINED, Error);

    return thisaction->Value;
}


/*------------------------------------------------------------------------*/

SCPROCRESULT SCTimeProc    (SC* psc,  SCAction* thisaction)
{
    time_t timenow;
    struct tm *ltime;
    char StrTime[10];
    int Error = 0;

    SCSETRETURNVALUETYPE(thisaction->Value, SCTIME);
    SCASSERTPARAMNUMBER (thisaction, 0);

    timenow = time (NULL);
    ltime = localtime (&timenow);

    sprintf(StrTime, "%2.2d:%2.2d:%2.2d", ltime->tm_hour, ltime->tm_min, ltime->tm_sec);

    SCPutValue (thisaction->Value, StrTime, SCTIME);

    SCSetActionState (psc, thisaction, SCDONE, SCDEFINED, Error);

    return thisaction->Value;

}


SCPROCRESULT SCDateProc    (SC* psc,  SCAction* thisaction)
{
    DWORD Date, Hour;
    char StrTime[12];
    int Error = 0;

    SCSETRETURNVALUETYPE(thisaction->Value, SCDATE);
    SCASSERTPARAMNUMBER (thisaction, 0);

    OSDateHour (&Date, &Hour);
    OSFormateDate (Date, StrTime, TRUE);

    SCPutValue (thisaction->Value, StrTime, SCDATE);

    SCSetActionState (psc, thisaction, SCDONE, SCDEFINED, Error);

    return thisaction->Value;

}



/*------------------------------------------------------------------------*/

SCPROCRESULT SCGetCharProc   (SC* psc,  SCAction* thisaction)
{
    int c;
    char str[100];
    int Error = 0;


    SCSETRETURNVALUETYPE(thisaction->Value, SCSTRING);
    SCASSERTPARAMNUMBER (thisaction,0);

    c = getchar ();
    sprintf ( str, "%c", c);

    SCPutValue (thisaction->Value, str, SCSTRING);

    SCSetActionState (psc, thisaction, SCDONE, SCDEFINED, Error);

    return thisaction->Value;
}

/*------------------------------------------------------------------------*/


SCPROCRESULT SCPutCharProc   (SC* psc,  SCAction* thisaction)
{
    int c;
    int result;
    char str[100];
    char* str1;
    SCValue* Val1 = NULL;
    int Error = 0;

    SCSETRETURNVALUETYPE(thisaction->Value, SCSTRING);
    SCASSERTPARAMNUMBER (thisaction,1);

    Val1 = SCGetParamValue (psc, thisaction, 1);

    SCASSERTDEFINEDVALUE(Val1) ;
    SCASSERTORVALUES(Val1, SCINTEGER, SCCHAR);

    if (Val1->Type == SCINTEGER)  c = (int)Val1->Val;
    else
    {
        str1 = (char*)Val1->Val;
        c = (int)str1[0];
    }

    result = putchar (c);
    sprintf ( str, "%c", c);

    SCPutValue (thisaction->Value, str, SCSTRING);

    SCSetActionState (psc, thisaction, SCDONE, SCDEFINED, Error);

    return thisaction->Value;
}

/*------------------------------------------------------------------------*/

SCPROCRESULT SCEqualProc   (SC* psc,  SCAction* thisaction)
{
    SCValue* Val1 = NULL;
    SCValue* Val2 = NULL;
    SCValue* Val3 = NULL;
    SCValue* Val4 = NULL;
    WORD NbParam = 0;

    int Error = 0;
    int ok = 1;

    SCASSERTMINPARAM (thisaction,2);

    SCAffectValue (thisaction->Value, NULL, SCPOINTER);

    Val1 = SCGetParamValue (psc, thisaction, 1);

    SCASSERTDEFINEDVALUE(Val1);
    Val2 = SCGetParamValue (psc, thisaction, 2);

    SCASSERTDEFINEDVALUE(Val2);

    if ((SCMallocType (Val1->Type)) &&
            (SCMallocType (Val2->Type)))
    {
        SCASSERTMAXPARAM (thisaction,4);

        SCASSERTVALUE (Val1, Val2->Type);
        NbParam = thisaction->NbParam;

        if(NbParam == 2 )
        {
            if (strcmp ((char*)Val1->Val, (char*)Val2->Val) == 0)
                SCPutValue (thisaction->Value, (char*)Val2->Val, SCSTRING);
        }
        else
            if (NbParam == 3)
            {

                Val3 = SCGetParamValue (psc, thisaction, 3);
                SCASSERTDEFINEDVALUE (Val3);
                SCASSERTVALUE (Val3, SCINTEGER);
                if (strcmp ((char*)Val1->Val + (int)Val3->Val, (char*)Val2->Val) == 0)
                    SCPutValue (thisaction->Value, (char*)Val2->Val, SCSTRING);
            }
            else
            {

                Val3 = SCGetParamValue (psc, thisaction, 3);
                SCASSERTDEFINEDVALUE (Val3);
                SCASSERTVALUE (Val3, SCINTEGER);
                Val4 = SCGetParamValue (psc, thisaction, 4);
                SCASSERTDEFINEDVALUE (Val4);
                SCASSERTVALUE (Val4, SCINTEGER);
                if (strncmp ((char*)Val1->Val + (int)Val3->Val, (char*)Val2->Val, (int)Val4->Val) == 0)
                    SCPutValue (thisaction->Value, (char*)Val2->Val, SCSTRING);
            }


    }

    if ((!SCMallocType (Val1->Type)) &&
            (!SCMallocType (Val2->Type)))
    {

        SCASSERTMAXPARAM (thisaction,2);

        if (Val1->Val == Val2->Val)
            SCAffectValue (thisaction->Value, (void*)ok, SCPOINTER);
    }

    SCSetActionState (psc, thisaction, SCDONE, SCDEFINED, Error);

    return thisaction->Value;
}

/*------------------------------------------------------------------------*/

SCPROCRESULT SCEvalProc    (SC* psc,  SCAction* thisaction)
{
    WORD i;
    SCValue* Value = NULL;
    int Error = 0;


    SCASSERTMINPARAM (thisaction, 1);

    for (i = 1; i <= thisaction->NbParam; i++)
    {
        Value = SCGetParamValue (psc, thisaction, i);
        SCASSERTDEFINEDVALUE (Value);
    }

    SCCopyValue (psc,  thisaction->Value, Value);
    SCSetActionState (psc, thisaction, SCDONE, SCDEFINED, Error);

    return thisaction->Value ;
}

/*------------------------------------------------------------------------*/

SCPROCRESULT SCIfProc    (SC* psc,  SCAction* thisaction)
{
    SCValue* Value = NULL;
    SCValue* Value1 = NULL;
    WORD NbParam;
    int Error = 0;

    SCASSERTMINPARAM (thisaction, 2);
    SCASSERTMAXPARAM (thisaction, 3);

    Value = SCGetParamValue (psc, thisaction, 1);
    SCASSERTDEFINEDVALUE(Value);

    if (Value->Val)
    {
        Value1 = SCGetParamValue (psc, thisaction, 2);
        SCASSERTDEFINEDVALUE(Value1);
    }
    else
    {
        NbParam = thisaction->NbParam;
        if(NbParam > 2)
        {
            Value1 = SCGetParamValue (psc, thisaction, 3);
            SCASSERTDEFINEDVALUE(Value1);
        }
    }
    SCCopyValue (psc,  thisaction->Value, Value1);

    SCSetActionState (psc, thisaction, SCDONE, SCDEFINED, Error);

    return thisaction->Value;
}

/*------------------------------------------------------------------------*/

SCPROCRESULT SCWhileProc    (SC* psc,  SCAction* thisaction)
{
    SCValue* Value = NULL;
    SCValue* Value1 = NULL;
    BOOL condition = TRUE;
    int  Error = 0;

    SCASSERTPARAMNUMBER (thisaction, 2);

    while (condition)
    {
        if (thisaction->ExecuteCondition)
        {
            Value = SCGetParamValue (psc, thisaction, 1);
            SCASSERTDEFINEDVALUE(Value);
        }
        if ((!thisaction->ExecuteCondition) ||
                (thisaction->ExecuteCondition && Value->Val))
        {

            thisaction->ExecuteCondition = FALSE;

            Value1 = SCGetParamValue (psc, thisaction, 2);
            SCASSERTDEFINEDVALUE(Value1);

            thisaction->ExecuteCondition = TRUE;
            SCSetEntityState (psc, SCGetParam (psc, thisaction, 1), SCTODO);
            SCSetEntityState (psc, SCGetParam (psc, thisaction, 2), SCTODO);
        }
        else
        {
            SCSetEntityState (psc, SCGetParam (psc, thisaction, 2), SCDONE);
            condition = FALSE;
        }
    }
    SCSetEntityState (psc, SCGetParam (psc, thisaction, 1), SCDONE);
    SCCopyValue (psc,  thisaction->Value, Value1);
    SCSetActionState (psc, thisaction, SCDONE, SCDEFINED, Error);
    return thisaction->Value;
}

/*------------------------------------------------------------------------*/

SCPROCRESULT SCNotProc    (SC* psc,  SCAction* thisaction)
{
    SCValue* Value = NULL;
    int Error = 0;


    SCASSERTPARAMNUMBER (thisaction, 1);

    Value = SCGetParamValue (psc, thisaction, 1);
    SCASSERTDEFINEDVALUE(Value);

    thisaction->Value->Val = (void *)(!Value->Val);

    SCSetActionState (psc, thisaction, SCDONE, SCDEFINED, Error);

    return thisaction->Value;
}

/*------------------------------------------------------------------------*/

SCPROCRESULT SCParseProc (SC* psc, SCAction* thisaction)
{
    SCValue* Value = NULL;
    List* listentities;
    SCEntity* pentity;
    int Error = 0;

    SCSETRETURNVALUETYPE(thisaction->Value, SCSTRING);
    SCASSERTPARAMNUMBER (thisaction, 1);

    Value = SCGetParamValue (psc, thisaction, 1);

    SCASSERTDEFINEDVALUE (Value);
    SCASSERTORVALUES (Value, SCSTRING, SCCHAR);

    pentity = SCParseFile(psc, (char *)Value->Val, &Error);
    if (!pentity )
    {
        thisaction->State          = SCDONE;
        thisaction->Value->Defined = SCDEFINED;
        thisaction->Error          = Error;
        return thisaction->Value;
    }

    listentities = SCFindDebListEntity(psc, pentity);
    SCCopyFromListEntity (psc, thisaction, listentities);

    SCCopyValue (psc, thisaction->Value, Value);

    SCSetActionState (psc, thisaction, SCDONE, SCDEFINED, Error);

    return thisaction->Value;
}

/*------------------------------------------------------------------------*/


SCPROCRESULT SCGetVersionProc (SC* psc, SCAction* thisaction)
{
    SCValue* Value = NULL;
    int Error = 0;


    SCSETRETURNVALUETYPE(thisaction->Value, SCSTRING);
    SCASSERTPARAMNUMBER (thisaction, 0);


    SCPutValue (thisaction->Value, psc->Version, SCSTRING);

    SCSetActionState (psc, thisaction, SCDONE, SCDEFINED, Error);

    return thisaction->Value;
}



/*------------------------------------------------------------------------*/

SCPROCRESULT SCExitProc  (SC* psc,  SCAction* thisaction)
{

    int Error = 0;
    SCASSERTPARAMNUMBER (thisaction, 0);
    exit (0);

    SCSetActionState (psc, thisaction, SCDONE, SCDEFINED, Error);

    return thisaction->Value;
}

/*------------------------------------------------------------------------*/

SCPROCRESULT SCPingProc   (SC* psc,  SCAction* thisaction)
{
    SCValue* Val = NULL;
    TPClient* pclient;
    WORD range = 1;
    int Error = 0;

    SCSETRETURNVALUETYPE(thisaction->Value, SCSTRING);

    SCASSERTPARAMNUMBER (thisaction, 1);

    Val = SCGetParamValue (psc, thisaction, 1);
    SCASSERTDEFINEDVALUE (Val);
    SCASSERTORVALUES (Val, SCSTRING, SCCHAR);

    if ((pclient = ping (psc->MX->TP, (char*)Val->Val)) == NULL)
    {
        printf ("icmp not supported\n");
        return NULL;
    }

    pclient->UserField = (List*)thisaction;

    /*  if (pclient) SCPutValue (thisaction->Value, pclient, SCPOINTER);
      SCSetActionState (psc, thisaction, SCDONE, SCDEFINED, Error); */

    return thisaction->Value;
}

/*------------------------------------------------------------------------*/

SCPROCRESULT SCWhenProc (SC* psc,  SCAction* thisaction)
{
    SCValue* Value = NULL;
    SCValue* Value1 = NULL;
    SCEntity* Entity;
    SCAction* paction;
    int       Error = 0;
    thisaction->Persistency = 1;


    SCASSERTPARAMNUMBER (thisaction, 2);

    Entity = SCGetParam (psc, thisaction, 1);

    Value = SCGetParamValue (psc, thisaction, 1);
    SCASSERTDEFINEDVALUE(Value);

    /* all variables  are set to dependency exept those who are affected in expression by setq or set */
    /* if entity is in a defun what to do ?*/
    SCAddDependencyValuesToVariables (Entity, thisaction->Identity, thisaction->Persistency, NULL);

    if (Value->Val)
    {
        Entity = SCGetParam (psc, thisaction, 2);
        SCInitEntity (psc, Entity);
        if (Entity->Type == SCOPERATOR)
        {
            paction = (SCAction *) Entity->Val;
            SCSetEntityRootAction (psc, Entity, paction);   /* we only execute the second part */
        }
        /* we should execute it all the time except .....*/
        Value1 = SCGetParamValue (psc, thisaction, 2);
        SCASSERTDEFINEDVALUE(Value1);
        thisaction->State= SCDONE;
    }

    SCCopyValue (psc,  thisaction->Value, Value1);
    SCSetActionState (psc, thisaction, SCDONE, SCDEFINED, Error);
    return thisaction->Value;
}

/*------------------------------------------------------------------------*/
/* A REVOIR */
/*------------------------------------------------------------------------*/


SCPROCRESULT SCEveryTimeProc    (SC* psc,  SCAction* thisaction)
{

    SCValue* Value = NULL;
    SCValue* Value1 = NULL;
    int Error = 0;

    SCASSERTMINPARAM (thisaction, 2);
    SCASSERTMAXPARAM (thisaction, 3);

    Value = SCGetParamValue (psc, thisaction, 1);
    SCASSERTDEFINEDVALUE(Value);

    if (Value->Val)
        Value1 = SCGetParamValue (psc, thisaction, 2);
    else
        Value1 = SCGetParamValue (psc, thisaction, 3);

    SCASSERTDEFINEDVALUE(Value1);

    SCCopyValue (psc,  thisaction->Value, Value1);

    thisaction->Persistency = 1;
    SCSetActionState (psc, thisaction, SCDONE, SCDEFINED, Error);

    return thisaction->Value;

}

/*------------------------------------------------------------------------*/
/* A REVOIR */
/*------------------------------------------------------------------------*/


SCPROCRESULT SCWaitProc    (SC* psc,  SCAction* thisaction)
{
    SCAction* ExecAction;
    SCEntity* Entity1;
    SCEntity* Entity2;

    thisaction->State= SCDONE;

    SCSETRETURNVALUETYPE(thisaction->Value, SCPOINTER);

    SCASSERTPARAMNUMBER (thisaction, 2);

    Entity1 = SCGetParam (psc, thisaction , 1);
    Entity2 = SCGetParam (psc, thisaction , 2);

    SCASSERTPARAMTYPE (Entity2, SCOPERATOR);

    ExecAction = SCCopyAction (psc, NULL, (SCAction *)Entity2->Val);
    ExecAction->Block = TRUE;
    ExecAction->ExecCondition = SCCopyEntity (psc, ExecAction, Entity1);

    ListInsert (&psc->ListActions, ExecAction);

    return thisaction->Value;

}

/*------------------------------------------------------------------------*/
/* A REVOIR */
/*------------------------------------------------------------------------*/


SCPROCRESULT SCJavaProc    (SC* psc,  SCAction* thisaction)
{
    SCValue* Val1 = NULL;
    int result = 0;
    char Reper[250];
    int Error = 0;

    thisaction->State= SCDONE;

    SCSETRETURNVALUETYPE(thisaction->Value, SCINTEGER);

    SCASSERTPARAMNUMBER (thisaction, 1);

    Val1 = SCGetParamValue (psc, thisaction, 1);
    SCASSERTVALUE(Val1, SCSTRING);

    strcpy (Reper, "d:\\win95\\book\\applets\\");
    strcat (Reper, (char*)Val1->Val);


    OSChangeDir (Reper, 'd', &Error);

#ifdef WIN32
    result = _spawnlp( _P_OVERLAY, "d:\\win95\\jdk\\bin\\appletviewer", "d:\\win95\\jdk\\bin\\appletviewer" , "index.html", NULL );
#endif
    thisaction->Error = result;
    thisaction->Value->Val = (void *)result;

    return thisaction->Value;

}

/*------------------------------------------------------------------------*/
/* A REVOIR */
/*------------------------------------------------------------------------*/


SCPROCRESULT SCAtProc    (SC* psc,  SCAction* thisaction)
{
    SCValue* Val1 = NULL;
    DWORD Time;
    SCEntity* Entity;
    struct tm stime;
    struct tm *ltime;
    time_t timenow;
    int i = 1;
    SCAction* paction;

    List* listparam = thisaction->ListParams;

    SCASSERTMINPARAM(thisaction, 2);
    Val1 = SCGetParamValue (psc, thisaction, 1);
    SCASSERTVALUE(Val1, SCSTRING);


    timenow = time (NULL);
    ltime = localtime (&timenow);

    thisaction->State= SCDONE;
    stime.tm_year = ltime->tm_year;
    stime.tm_mon  = ltime->tm_mon;
    stime.tm_mday = ltime->tm_mday;
    stime.tm_isdst = ltime->tm_isdst;
    sscanf(Val1->Val, "%d:%d:%d", &stime.tm_hour, &stime.tm_min, &stime.tm_sec);
    Time = mktime(&stime);

    printf (SCTraceAction (thisaction));

    while(listparam->cdr)
    {
        i++;
        Entity = (SCEntity*)listparam->cdr->car;
        if (Entity->Type == SCOPERATOR )
        {
            ListInsertAfter (&psc->ListWaitingActions, (paction = SCCopyAction (psc, NULL, (SCAction *)Entity->Val)) , thisaction);
            paction->Block = FALSE;
        }
        listparam = listparam->cdr;
    }
    return thisaction->Value;
}

/*------------------------------------------------------------------------*/
/* A REVOIR */
/*------------------------------------------------------------------------*/


int SCSynchro (SCAction* paction, void* Time)
{
    SCAction* paction1;
    DWORD newtime = (DWORD) Time;
    ListNewr (&paction->ActionClass->SC->ListActions, (paction1 = SCCopyAction (paction->ActionClass->SC, NULL, paction)));
    paction1->Block = FALSE;
    return 1;
}


SCPROCRESULT SCEveryProc    (SC* psc,  SCAction* thisaction)
{
    void* Val1;
    DWORD EveryTime;
    SCEntity* Entity;
    DWORD Time;
    int i = 1;
    SCAction* paction;
    int Error = 0;

    List* listparam = thisaction->ListParams;

    thisaction->State= SCDONE;

    Val1 = SCGetParamValue (psc, thisaction, 1);
    if (!Val1) return NULL;
    EveryTime = (DWORD)Val1;
    Time = time (NULL);

    while(listparam->cdr)
    {
        i++;
        Entity = (SCEntity*)listparam->cdr->car;
        if (Entity->Type == SCOPERATOR)
        {
            ListInsert (&psc->ListWaitingActions, (paction = SCCopyAction (psc, NULL, (SCAction *)Entity->Val)));
            SCAddFunctionToInstance (psc, paction, SCAFTER, SCSynchro, (void*)EveryTime);
            paction->Block = FALSE;
        }
        listparam = listparam->cdr;
    }
    SCSetActionState (psc, thisaction, SCDONE, SCDEFINED, Error);
    return thisaction->Value;
}

/*------------------------------------------------------------------------*/

SCPROCRESULT SCPrintProc   (SC* psc,  SCAction* thisaction)
{
    WORD i;
    char Buf[5250];
    SCValue* Value = NULL;
    int Error = 0;
    SCASSERTMINPARAM (thisaction, 1);

    for (i = 1; i <= thisaction->NbParam; i++)
    {
        Value = SCGetParamValue (psc, thisaction, i);
        SCASSERTDEFINEDVALUE (Value);
        SCCopyValue (psc, thisaction->Value, Value);
        SCPrintValue  (psc, Value, Buf);
        SCTrace (psc, thisaction->LineNumber, Buf);
    }

    SCSetActionState (psc, thisaction, SCDONE, SCDEFINED, Error);
    return thisaction->Value;
}

/*------------------------------------------------------------------------*/

SCPROCRESULT SCTraceProc   (SC* psc,  SCAction* thisaction)
{
    SCValue* Value = NULL;
    SCAction* paction = NULL;
    int Error = 0;


    SCSETRETURNVALUETYPE(thisaction->Value, SCSTRING);
    SCASSERTPARAMNUMBER (thisaction, 1);

    Value = SCGetParamValue (psc, thisaction, 1);
    SCASSERTDEFINEDVALUE (Value);

    SCASSERTORVALUES (Value, SCSTRING, SCCHAR);

    Error = OSTraceMessage (Value->Val);

    SCCopyValue (psc, thisaction->Value, Value);

    SCSetActionState (psc, thisaction, SCDONE, SCDEFINED, Error);
    return thisaction->Value;
}


SCPROCRESULT SCSetTraceFileProc   (SC* psc,  SCAction* thisaction)
{
    SCValue* Value = NULL;
    SCAction* paction = NULL;
    int Error = 0;

    SCSETRETURNVALUETYPE(thisaction->Value, SCSTRING);
    SCASSERTPARAMNUMBER (thisaction, 1);

    Value = SCGetParamValue (psc, thisaction, 1);
    SCASSERTDEFINEDVALUE (Value);

    SCASSERTORVALUES (Value, SCSTRING, SCCHAR);

    OSSetTraceFile (Value->Val);

    SCCopyValue (psc, thisaction->Value, Value);

    SCSetActionState (psc, thisaction, SCDONE, SCDEFINED, Error);


    return thisaction->Value;
}



/*------------------------------------------------------------------------*/
/* A REVOIR */
/*------------------------------------------------------------------------*/


SCPROCRESULT SCExecuteProc   (SC* psc,  SCAction* thisaction)
{
    WORD        i;
    SCValue*    Value = NULL;
    SCValue*       ValueTemp;
    SCSection*  psection;
    SCAction*   RootAction = NULL;
    SCEntity*   pentity;
    SCEntity*   Entitytemp;
    List*       listentities;
    List*       ListVariables;
    SCVariable* pvariable;
    int         nbparamnow;
    int         Error = 0;
    /*!! A REVOIR  RootAction*/


    RootAction = SCGetRootActionFromAction (psc, thisaction);
    Value = SCGetParamValue (psc, thisaction, 1);
    SCASSERTDEFINEDVALUE (Value);
    SCASSERTVALUE(Value, SCSTRING);

    psection = SCIsSectionDefinedInList(psc->ListSections, (char*)Value->Val);
    if (!psection)
    {
        Error = 15;
        SCPutValue (thisaction->Value, Value->Val, SCSTRING);
        SCSetActionState (psc, thisaction, SCDONE, SCDEFINED, Error);
        return thisaction->Value;
    }

    listentities = psection->ListEntities;
    ListVariables = psection->ListVariables;

    nbparamnow = ListNbElt (thisaction->ListParams);

    if (nbparamnow == thisaction->NbParam)
    {
        SCASSERTPARAMNUMBER (thisaction, (psection->NbVariables + 1));
        for (i = 2; i <= thisaction->NbParam; i++)
        {
            pvariable = (SCVariable *)ListVariables->car;
            ValueTemp = SCGetParamValue (psc, thisaction, i);
            SCASSERTDEFINEDVALUE (ValueTemp);
            SCCopyValue (psc, pvariable->Value, ValueTemp);
            ListVariables = ListVariables->cdr;
        }

        while (listentities)
        {
            pentity = (SCEntity *)listentities->car;
            if (!RootAction)   Entitytemp = SCCopyEntity(psc, thisaction, pentity);
            else               Entitytemp = SCCopyEntity(psc, RootAction, pentity);

            Entitytemp->FatherAction = thisaction;

            ListNewr (&thisaction->ListParams, Entitytemp);
            listentities = listentities->cdr;
            nbparamnow++;
        }
    }
    for (i = thisaction->NbParam + 1; i <= nbparamnow; i++)
    {
        Value = SCGetParamValue (psc, thisaction, i);
        SCASSERTDEFINEDVALUE (Value);
    }

    SCCopyValue (psc, thisaction->Value, Value);

    SCSetActionState (psc, thisaction, SCDONE, SCDEFINED, Error);


    return thisaction->Value;
}


SCPROCRESULT SCDefunProc   (SC* psc,  SCAction* thisaction)
{
    int Error = 0;
    SCValue*   Value = NULL;

    Value = SCGetParamValue (psc, thisaction, 1);
    SCASSERTDEFINEDVALUE (Value);
    SCASSERTVALUE(Value, SCSTRING);

    SCPutValue (thisaction->Value, Value->Val, SCSTRING);

    SCSetActionState (psc, thisaction, SCDONE, SCDEFINED, Error);

    return thisaction->Value;
}




/*------------------------------------------------------------------------*/

SCPROCRESULT SCGetLastError   (SC* psc,  SCAction* thisaction)
{
    char str[6];
    int Error = 0;


    SCSETRETURNVALUETYPE(thisaction->Value, SCINTEGER);
    SCASSERTPARAMNUMBER (thisaction, 0);

    sprintf(str, "%d", psc->LastError);

    SCPutValue (thisaction->Value, str, SCINTEGER);

    SCSetActionState (psc, thisaction, SCDONE, SCDEFINED, Error);


    return thisaction->Value;
}



/*------------------------------------------------------------------------*/

SCPROCRESULT SCLook   (SC* psc,  SCAction* thisaction)
{
    List* plistaction;
    List* plistwaitaction;
    List* plistvariables;
    SCAction* paction;
    SCVariable* pvariable;
    WORD NbParam;
    SCValue* Value = NULL;
    char Buf[250];
    int Error = 0;
    int i;

    SCSETRETURNVALUETYPE(thisaction->Value, SCSTRING);
    SCASSERTMAXPARAM (thisaction, 1);

    NbParam = thisaction->NbParam;

    if (NbParam > 0)
    {
        Value = SCGetParamValue (psc, thisaction, 1);
        SCASSERTDEFINEDVALUE (Value);
        SCASSERTVALUE (Value, SCINTEGER);
        paction = SCGetActionFromIdentity (psc, (LONG)Value->Val);
        SCDeleteAction (psc, paction);
    }

    sprintf (Buf, "%s", "WAITING ACTIONS :");
    SCTrace (psc, -1, Buf);
    plistaction           = psc->ListActions;
    plistwaitaction       = psc->ListWaitingActions;
    plistvariables        = psc->ListVariables;

    i = 1;
    while (plistwaitaction)
    {
        paction = plistwaitaction->car;
        sprintf (Buf, "%d- [%s, %ld]", i++, paction->ActionClass->Name, paction->Identity);
        SCTrace (psc, -1, Buf);
        plistwaitaction = plistwaitaction->cdr;
    }
    sprintf (Buf, "%s", "BLOCKING ACTIONS :");
    SCTrace (psc,-1, Buf);

    i = 1;
    while (plistaction)
    {
        paction = plistaction->car;
        sprintf (Buf, "%d- [%s, %ld]", i++, paction->ActionClass->Name, paction->Identity);
        SCTrace (psc, -1, Buf);
        plistaction = plistaction->cdr;

    }
    sprintf (Buf, "%s", "VARIABLES :");
    SCTrace (psc,-1, Buf);

    i = 1;
    while (plistvariables)
    {
        char bufvalue[300];
        pvariable = plistvariables->car;
        SCPrintValue (psc, pvariable->Value, bufvalue);
        sprintf (Buf, "%d- %s %s ", i++, pvariable->Name, bufvalue);
        SCTrace (psc, -1, Buf);
        plistvariables = plistvariables->cdr;
    }

    SCPutValue (thisaction->Value, "DONE", SCSTRING);

    SCSetActionState (psc, thisaction, SCDONE, SCDEFINED, Error);

    return thisaction->Value;
}