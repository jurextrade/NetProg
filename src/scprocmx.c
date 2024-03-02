/* NetProg is an API in C language allowing an homogeneous programming of  */
/* communicating applications                                              */
/* Copyright (C) 2002,2003,2004,2005                                       */
/* Gabriel Jureidini                                                       */
/* Version 2.0 - 17 July 2005                                              */
/*-------------------------------------------------------------------------*/


#define _CRT_SECURE_NO_WARNINGS
#include "sc.h"
#include "mail.h"
#include "ftp.h"

/*------------------------------------------------------------------------*/

SCComFuncStruct* SCFindFunction (SC* psc, char* name)
{
    SCComFuncStruct* pFuncStruct;
    List* list = psc->ListComFunctions;
    while (list)
    {
        pFuncStruct = (SCComFuncStruct *)list->car;
        if (OSStrcmp (pFuncStruct->Name, name) == 0)
            return pFuncStruct;
        list = list->cdr;
    }
    return NULL;
}

/*------------------------------------------------------------------------*/



SCPROCRESULT SCMXAddPort (SC* psc,  SCAction* thisaction)
{
    SCValue* Val1 = NULL;
    SCValue* Val2 = NULL;
    int Error;
    int NbParam = 0;
    char SProtocol[30];

    strcpy (SProtocol, "DG");

    SCSETRETURNVALUETYPE(thisaction->Value, SCINTEGER);

    SCASSERTMINPARAM (thisaction, 1);
    SCASSERTMAXPARAM (thisaction, 2);


    Val1 = SCGetParamValue (psc, thisaction, 1);
    SCASSERTDEFINEDVALUE (Val1);
    SCASSERTVALUE (Val1, SCINTEGER);
    NbParam = thisaction->NbParam;
    if(NbParam > 1)
    {

        Val2 = SCGetParamValue (psc, thisaction, 2);
        SCASSERTDEFINEDVALUE (Val2);
        SCASSERTORVALUES (Val2, SCSTRING, SCCHAR);
        strcpy (SProtocol, (char*)Val2->Val);
    }


    Error = MXAddPort (psc->MX, (int)Val1->Val, MXGetProtocolFromName (SProtocol));

    if (Error >=0)  Error = 0;
    SCAffectValue (thisaction->Value, &Error, SCINTEGER);

    SCSetActionState (psc, thisaction, SCDONE, SCDEFINED, Error);

    return thisaction->Value;
}

/*------------------------------------------------------------------------*/


SCPROCRESULT SCMXOpenCom (SC* psc,  SCAction* thisaction)
{
    SCValue* Val1 = NULL;
    SCValue* Val2 = NULL;
    SCValue* Val3 = NULL;
    SCValue* Val4 = NULL;
    MXCom* pcom;
    int NbParam = 0;
    MX* pcv = psc->MX;
    int Error = 0;
    char SProtocol[30];

    strcpy (SProtocol, "DG");

    SCSETRETURNVALUETYPE(thisaction->Value, SCPOINTER);
    SCASSERTMINPARAM (thisaction, 2);
    SCASSERTMAXPARAM (thisaction, 4);



    Val1 = SCGetParamValue (psc, thisaction, 1);
    SCASSERTDEFINEDVALUE (Val1);
    SCASSERTORVALUES (Val1, SCSTRING, SCCHAR);

    Val2 = SCGetParamValue (psc, thisaction, 2);
    SCASSERTDEFINEDVALUE (Val2);

    if ((Val2->Type != SCSTRING) &&
            (Val2->Type != SCCHAR))
        SCASSERTORVALUES (Val2, SCINTEGER, SCLONG);

    NbParam = thisaction->NbParam;
    if (NbParam == 3)
    {

        Val3 = SCGetParamValue (psc, thisaction, 3);
        SCASSERTDEFINEDVALUE (Val3);
        SCASSERTORVALUES (Val3, SCSTRING, SCCHAR);
        strcpy (SProtocol, (char*)Val3->Val);
    }
    else
        if (NbParam == 4)
        {

            Val3 = SCGetParamValue (psc, thisaction, 3);
            SCASSERTDEFINEDVALUE (Val3);
            SCASSERTORVALUES (Val3, SCSTRING, SCCHAR);

            Val4 = SCGetParamValue (psc, thisaction, 4);
            SCASSERTDEFINEDVALUE (Val4);
            SCASSERTORVALUES (Val4, SCSTRING, SCCHAR);
            strcpy (SProtocol, (char*)Val4->Val);
        }


    if (NbParam == 4)
        pcom = MXOpenBase (pcv, (char*)Val1->Val, (char*)Val2->Val, (char*)Val3->Val , MXGetProtocolFromName (SProtocol), 1);
    else
    {
        if ((Val2->Type == SCSTRING) ||
                (Val2->Type == SCCHAR))
            pcom = MXOpenFile (pcv, (char*)Val1->Val, (char*)Val2->Val, MXGetProtocolFromName (SProtocol), 'D', 0);
        else
            pcom = MXOpenTCP (pcv, (char*)Val1->Val, (int)Val2->Val, MXGetProtocolFromName (SProtocol), NULL, NULL, 1);
    }

    if (pcom)
    {
        SCPutValue (thisaction->Value, pcom, SCPOINTER);
        ListNewr (&psc->Communications, pcom);
        MXAddEndingProcedure (pcom, SCOnBreakClose, psc);
        if ((Val2->Type != SCSTRING) &&
                (Val2->Type != SCCHAR))
        {
            MXAddComCallBack(pcv,  pcom, DG_SYS,  "FileReply"          , MXONSEND,   SCFileReplyCommand,      psc);
            MXAddComCallBack(pcv,  pcom, DG_SYS,  "FileReply"          , MXONRECV,   SCFileReplyCommand,      psc);
            MXAddComCallBack(pcv,  pcom, DG_SYS,  "DirReply"           , MXONRECV,   SCDirReplyCommand,       psc);
            MXAddComCallBack(pcv,  pcom, DG_SYS,  "ListReply"          , MXONRECV,   SCListReplyCommand,      psc);
            MXAddComCallBack(pcv,  pcom, DG_SYS,  "GDirReply"          , MXONRECV,   SCGDirReplyCommand,      psc);
            MXAddComCallBack(pcv,  pcom, FTP_SYS, "get_reply"          , MXONRECV,   SCGetReplyCommand,       psc);
            MXAddComCallBack(pcv,  pcom, FTP_SYS, "put_reply"          , MXONRECV,   SCPutReplyCommand,       psc);
            MXAddComCallBack(pcv,  pcom, DL_SYS,  "Login"              , MXONRECV,   SYS_Login,               psc);
            MXAddComCallBack(pcv,  pcom, DL_SYS,  "LoginReply"         , MXONRECV,   SYS_LoginReply,          psc);
            MXAddComCallBack(pcv,  pcom, DL_SYS,  "ParseString"        , MXONRECV,   SYS_ParseString,         psc);
            MXAddComCallBack(pcv,  pcom, DL_SYS,  "ParseReply"         , MXONRECV,   SYS_ParseReply,          psc);
            MXAddComCallBack(pcv,  pcom, DL_SYS,  "ShareVariable"      , MXONRECV,   SYS_ShareVariable,       psc);
            MXAddComCallBack(pcv,  pcom, DL_SYS,  "ShareReply"         , MXONRECV,   SYS_ShareReply,          psc);
            MXAddComCallBack(pcv,  pcom, DL_SYS,  "GetEntityValue"     , MXONRECV,   SYS_GetEntityValueReply, psc);
            MXAddComCallBack(pcv,  pcom, DL_SYS,  "GetEntityValueReply", MXONRECV,   SYS_GetVariableValue,    psc);
            MXAddComCallBack(pcv,  pcom, DL_SYS,  "SetEntityValue"     , MXONRECV,   SYS_SetEntityValueReply, psc);
        }
    }
    else
    {
        Error =1;
    }
    SCSetActionState (psc, thisaction, SCDONE, SCDEFINED, Error);
    return thisaction->Value;
}

/*------------------------------------------------------------------------*/

SCPROCRESULT SCMXCloseCom (SC* psc,  SCAction* thisaction)
{

    SCValue* Val1 = NULL;
    MXCom* pcom;
    int Error;
    SCEntity* Entity;

    SCSETRETURNVALUETYPE(thisaction->Value, SCPOINTER);

    SCASSERTPARAMNUMBER (thisaction, 1);

    Val1 = SCGetParamValue (psc, thisaction, 1);
    SCASSERTDEFINEDVALUE (Val1);
    SCASSERTORVALUES (Val1, SCPOINTER, SCLONG);

    pcom = (MXCom*)Val1->Val;
    SCASSERTNOTNULL (pcom);

    MXRemoveEndingProcedure (pcom, SCOnBreakClose, psc);
    Entity = SCGetParam (psc, thisaction , 1);

    MXAddEndingProcedure (pcom, SCOnClose, Entity->RootAction);
    Error = MXCloseCom (psc->MX, pcom);

    if (Error >=0)  Error = 0;
    if (!Error) SCCopyValue (psc, thisaction->Value, Val1);

    SCPutValue (Val1, NULL, SCPOINTER);

    SCSetActionState (psc, thisaction, SCDONE, SCDEFINED, Error);


    return thisaction->Value;
}


/*------------------------------------------------------------------------*/


SCPROCRESULT SCMXLoadClass (SC* psc,  SCAction* thisaction)
{
    SCValue* Val1 = NULL;
    int Error;

    SCSETRETURNVALUETYPE(thisaction->Value, SCSTRING);

    SCASSERTPARAMNUMBER (thisaction, 1);

    Val1 = SCGetParamValue (psc, thisaction, 1);
    SCASSERTDEFINEDVALUE (Val1);
    SCASSERTORVALUES (Val1, SCSTRING, SCCHAR);

    Error = MXLoadResourceFile (psc->MX, (char*)Val1->Val);

    if (Error >=0)  Error = 0;
    if (!Error) SCCopyValue (psc, thisaction->Value, Val1) ;

    SCSetActionState (psc, thisaction, SCDONE, SCDEFINED, Error);

    return thisaction->Value;
}


/*------------------------------------------------------------------------*/


SCPROCRESULT SCMXLoadResource (SC* psc,  SCAction* thisaction)
{
    SCValue* Val1 = NULL;
    int Error;

    SCSETRETURNVALUETYPE(thisaction->Value, SCSTRING);

    SCASSERTPARAMNUMBER (thisaction, 1);

    Val1 = SCGetParamValue (psc, thisaction, 1);
    SCASSERTDEFINEDVALUE (Val1);
    SCASSERTORVALUES (Val1, SCSTRING, SCCHAR);

    Error = MXLoadResourceFile (psc->MX, (char*)Val1->Val);

    if (Error >=0)  Error = 0;
    if (!Error) SCCopyValue (psc, thisaction->Value, Val1) ;

    SCSetActionState (psc, thisaction, SCDONE, SCDEFINED, Error);

    return thisaction->Value;
}

/*------------------------------------------------------------------------*/


SCPROCRESULT  SCMXGetType (SC* psc,  SCAction* thisaction)
{

    SCValue* Val1 = NULL;
    SCValue* Val2 = NULL;
    int Error = 0;
    MXMessage* pmessage;
    char CReturn[80];

    SCSETRETURNVALUETYPE(thisaction->Value, SCSTRING);

    SCASSERTPARAMNUMBER (thisaction, 2);

    Val1 = SCGetParamValue (psc, thisaction, 1);
    SCASSERTDEFINEDVALUE (Val1);
    SCASSERTORVALUES (Val1, SCPOINTER, SCLONG);
    pmessage = (MXMessage*)Val1->Val;
    SCASSERTNOTNULL (pmessage);



    Val2 = SCGetParamValue (psc, thisaction, 2);
    SCASSERTDEFINEDVALUE (Val2);
    SCASSERTVALUE (Val2, SCINTEGER);


    MXGetType (pmessage, (int)Val2->Val, CReturn);


    SCPutValue (thisaction->Value, CReturn, SCSTRING) ;
    if (strcmp (CReturn, "NULL") == 0)
        Error = -1;

    SCSetActionState (psc, thisaction, SCDONE, SCDEFINED, Error);
    return thisaction->Value;
}

SCPROCRESULT  SCMXGetName (SC* psc,  SCAction* thisaction)
{

    SCValue* Val1 = NULL;
    SCValue* Val2 = NULL;
    int Error = 0;
    char* Name;
    MXMessage* pmessage;

    SCSETRETURNVALUETYPE(thisaction->Value, SCSTRING);

    SCASSERTPARAMNUMBER (thisaction, 2);

    Val1 = SCGetParamValue (psc, thisaction, 1);
    SCASSERTDEFINEDVALUE (Val1);
    SCASSERTORVALUES (Val1, SCPOINTER, SCLONG);
    pmessage = (MXMessage*)Val1->Val;
    SCASSERTNOTNULL (pmessage);



    Val2 = SCGetParamValue (psc, thisaction, 2);
    SCASSERTDEFINEDVALUE (Val2);
    SCASSERTVALUE (Val2, SCINTEGER);


    Name =  MXGetName (pmessage, (int)Val2->Val);

    if (Name) SCPutValue (thisaction->Value, Name, SCSTRING) ;
    else Error = -1;

    SCSetActionState (psc, thisaction, SCDONE, SCDEFINED, Error);
    return thisaction->Value;
}


SCPROCRESULT  SCMXGetNbrObjects (SC* psc,  SCAction* thisaction)
{

    SCValue* Val1 = NULL;
    int Error = 0;
    int NbrObjects = 0;
    MXMessage* pmessage;

    SCSETRETURNVALUETYPE(thisaction->Value, SCINTEGER);

    SCASSERTPARAMNUMBER (thisaction, 1);

    Val1 = SCGetParamValue (psc, thisaction, 1);
    SCASSERTDEFINEDVALUE (Val1);
    SCASSERTORVALUES (Val1, SCPOINTER, SCLONG);
    pmessage = (MXMessage*)Val1->Val;
    SCASSERTNOTNULL (pmessage);


    NbrObjects = MXGetNbrObjects (pmessage);

    SCAffectValue (thisaction->Value, (void *)NbrObjects, SCINTEGER);

    SCSetActionState (psc, thisaction, SCDONE, SCDEFINED, Error);

    return thisaction->Value;
}



SCPROCRESULT  SCMXGetRangeValue (SC* psc, SCAction* thisaction)
{
    MXObject* pobject;
    SCValue* Val1 = NULL;
    SCValue* Val2= NULL;
    SCValue* Val3 = NULL;
    int Error = 0;
    MXMessage* pmessage;

    SCASSERTPARAMNUMBER (thisaction, 3);

    Val1 = SCGetParamValue (psc, thisaction, 1);
    SCASSERTDEFINEDVALUE (Val1);
    SCASSERTORVALUES (Val1, SCPOINTER, SCLONG);
    pmessage = (MXMessage*)Val1->Val;
    SCASSERTNOTNULL (pmessage);


    Val2 = SCGetParamValue (psc, thisaction, 2);
    SCASSERTDEFINEDVALUE (Val2);
    SCASSERTVALUE (Val2, SCINTEGER);

    Val3 = SCGetParamValue (psc, thisaction, 3);
    SCASSERTDEFINEDVALUE (Val3);
    SCASSERTVALUE (Val3, SCINTEGER);

    pobject = (MXObject*)ListGetElt (pmessage->MessageClass->Objects,  (int)Val2->Val - 1);
    if (pobject)
    {
        switch (pobject->Type)
        {
        case  MXFILE :
        case  MXBUFFER :
            SCSETRETURNVALUETYPE(thisaction->Value, SCPOINTER);
            SCAffectValue (thisaction->Value, MXGetRangeValue ((MXMessage*)Val1->Val, (int)Val2->Val, (int)Val3->Val), SCLONG);
            break;
        case  MXWORD  :
            SCSETRETURNVALUETYPE(thisaction->Value, SCINTEGER);
            SCAffectValue (thisaction->Value, MXGetRangeValue ((MXMessage*)Val1->Val, (int)Val2->Val, (int)Val3->Val), SCLONG);
            break;
        case  MXSHORT :
            SCSETRETURNVALUETYPE(thisaction->Value, SCINTEGER);
            SCAffectValue (thisaction->Value, MXGetRangeValue ((MXMessage*)Val1->Val, (int)Val2->Val, (int)Val3->Val), SCLONG);
            break;
        case  MXDWORD :
            SCSETRETURNVALUETYPE(thisaction->Value, SCLONG);
            SCAffectValue (thisaction->Value, MXGetRangeValue ((MXMessage*)Val1->Val, (int)Val2->Val, (int)Val3->Val), SCLONG);
            break;
        case  MXLONG  :
            SCSETRETURNVALUETYPE(thisaction->Value, SCLONG);
            SCAffectValue (thisaction->Value, MXGetRangeValue ((MXMessage*)Val1->Val, (int)Val2->Val, (int)Val3->Val), SCLONG);
            break;
        case  MXDOUBLE:
            SCSETRETURNVALUETYPE(thisaction->Value, SCLONG);
            SCAffectValue (thisaction->Value, MXGetRangeValue ((MXMessage*)Val1->Val, (int)Val2->Val, (int)Val3->Val), SCLONG);
            break;
        case  MXCHAR :
        case  MXBYTE :
            {
                char* copy_result = (char*)malloc (pobject->Size + 1);
                SCSETRETURNVALUETYPE(thisaction->Value, SCCHAR);
                sprintf (copy_result, "%.*s", pobject->Size, (char*)MXGetRangeValue ((MXMessage*)Val1->Val, (int)Val2->Val, (int)Val3->Val));
                SCPutValue (thisaction->Value, copy_result, SCCHAR);
                free (copy_result);
            }
            break;
        case  MXSTRING :
            SCSETRETURNVALUETYPE(thisaction->Value, SCSTRING);
            SCPutValue (thisaction->Value, (STRING)MXGetRangeValue ((MXMessage*)Val1->Val, (int)Val2->Val, (int)Val3->Val), SCSTRING);
            break;
        }
    }
    else
    {
        Error =1;
    }

    SCSetActionState (psc, thisaction, SCDONE, SCDEFINED, Error);

    return thisaction->Value;
}



SCPROCRESULT  SCMXGetValue (SC* psc, SCAction* thisaction)
{
    MXObject* pobject;
    SCValue* Val1 = NULL;
    SCValue* Val2= NULL;
    SCValue* Val3 = NULL;
    int Error = 0;
    MXMessage* pmessage;

    SCASSERTPARAMNUMBER (thisaction, 3);

    Val1 = SCGetParamValue (psc, thisaction, 1);
    SCASSERTDEFINEDVALUE (Val1);
    SCASSERTORVALUES (Val1, SCPOINTER, SCLONG);
    pmessage = (MXMessage*)Val1->Val;
    SCASSERTNOTNULL (pmessage);


    Val2 = SCGetParamValue (psc, thisaction, 2);
    SCASSERTDEFINEDVALUE (Val2);
    SCASSERTORVALUES (Val2, SCCHAR, SCSTRING);


    Val3 = SCGetParamValue (psc, thisaction, 3);
    SCASSERTDEFINEDVALUE (Val3);
    SCASSERTVALUE (Val3, SCINTEGER);

    pobject = MXGetObjectFromName (pmessage->MessageClass, (char*)Val2->Val);
    if (pobject)
    {
        switch (pobject->Type)
        {
        case  MXFILE :
        case  MXBUFFER :
            SCSETRETURNVALUETYPE(thisaction->Value, SCPOINTER);
            SCAffectValue (thisaction->Value, MXGetValue ((MXMessage*)Val1->Val, (char*)Val2->Val, (int)Val3->Val), SCLONG);
            break;
        case  MXWORD  :
            SCSETRETURNVALUETYPE(thisaction->Value, SCINTEGER);
            SCAffectValue (thisaction->Value, MXGetValue ((MXMessage*)Val1->Val, (char*)Val2->Val, (int)Val3->Val), SCLONG);
            break;
        case  MXSHORT :
            SCSETRETURNVALUETYPE(thisaction->Value, SCINTEGER);
            SCAffectValue (thisaction->Value, MXGetValue ((MXMessage*)Val1->Val, (char*)Val2->Val, (int)Val3->Val), SCLONG);
            break;
        case  MXDWORD :
            SCSETRETURNVALUETYPE(thisaction->Value, SCLONG);
            SCAffectValue (thisaction->Value, MXGetValue ((MXMessage*)Val1->Val, (char*)Val2->Val, (int)Val3->Val), SCLONG);
            break;
        case  MXLONG  :
            SCSETRETURNVALUETYPE(thisaction->Value, SCLONG);
            SCAffectValue (thisaction->Value, MXGetValue ((MXMessage*)Val1->Val, (char*)Val2->Val, (int)Val3->Val), SCLONG);
            break;
        case  MXDOUBLE:
            SCSETRETURNVALUETYPE(thisaction->Value, SCLONG);
            SCAffectValue (thisaction->Value, MXGetValue ((MXMessage*)Val1->Val, (char*)Val2->Val, (int)Val3->Val), SCLONG);
            break;
        case  MXCHAR :
        case  MXBYTE :
            {
                char* copy_result = (char*)malloc (pobject->Size + 1);
                SCSETRETURNVALUETYPE(thisaction->Value, SCCHAR);
                sprintf (copy_result, "%.*s", pobject->Size, (char*)MXGetValue ((MXMessage*)Val1->Val, (char*)Val2->Val, (int)Val3->Val));
                SCPutValue (thisaction->Value, copy_result, SCCHAR);
                free (copy_result);
            }
            break;
        case  MXSTRING :
            SCSETRETURNVALUETYPE(thisaction->Value, SCSTRING);
            SCPutValue (thisaction->Value, (STRING)MXGetValue ((MXMessage*)Val1->Val, (char*)Val2->Val, (int)Val3->Val), SCSTRING);
            break;
        }
    }
    else
    {
        Error =1;
    }

    SCSetActionState (psc, thisaction, SCDONE, SCDEFINED, Error);

    return thisaction->Value;
}

/*------------------------------------------------------------------------*/

SCPROCRESULT  SCMXSetRangeValue (SC* psc, SCAction* thisaction)
{

    MXObject* pobject;
    SCValue* Val1 = NULL;
    SCValue* Val2 = NULL;
    SCValue* Val3 = NULL;
    SCValue* Value = NULL;
    int Error = 0;
    MXMessage* pmessage;

    SCASSERTPARAMNUMBER (thisaction, 4);

    Val1 = SCGetParamValue (psc, thisaction, 1);
    SCASSERTDEFINEDVALUE (Val1);
    SCASSERTORVALUES (Val1, SCPOINTER, SCLONG);
    pmessage = (MXMessage*)Val1->Val;
    SCASSERTNOTNULL (pmessage);


    Val2 = SCGetParamValue (psc, thisaction, 2);
    SCASSERTDEFINEDVALUE (Val2);
    SCASSERTVALUE (Val2, SCINTEGER);

    Val3 = SCGetParamValue (psc, thisaction, 3);
    SCASSERTDEFINEDVALUE (Val3);
    SCASSERTVALUE (Val3, SCINTEGER);

    Value = SCGetParamValue (psc, thisaction, 4);
    SCASSERTDEFINEDVALUE (Value);

    pobject = (MXObject*)ListGetElt (pmessage->MessageClass->Objects,  (int)Val2->Val - 1);
    if (pobject)
    {
        switch (pobject->Type)
        {
        case MXSHORT :
            {
                SHORT Val = *(SHORT*)Value->Val;
                Error = MXSetRangeValue (pmessage, (int)Val2->Val , (int)Val3->Val, &Val);
            }
            break;
        case MXWORD :
            {
                WORD Val = *(WORD*)Value->Val;
                Error = MXSetRangeValue (pmessage, (int)Val2->Val , (int)Val3->Val, &Val);
            }
            break;
        case MXDWORD :
            {
                DWORD Val = *(DWORD*)Value->Val;
                Error = MXSetRangeValue (pmessage, (int)Val2->Val , (int)Val3->Val, &Val);
            }
            break;
        case MXLONG :
            {
                LONG Val = *(LONG*)Value->Val;
                Error = MXSetRangeValue (pmessage, (int)Val2->Val , (int)Val3->Val, &Val);
            }
            break;
        case MXFILE :
            {
                FILEPARM* Val = (FILEPARM *)Value->Val;
                Error = MXSetRangeValue (pmessage, (int)Val2->Val , (int)Val3->Val, Val);
                free (Val);
            }
            break;
        case  MXBUFFER :
            {
                BUFFERPARM* Val = (BUFFERPARM *)Value->Val;
                Error = MXSetRangeValue (pmessage, (int)Val2->Val , (int)Val3->Val, Val);
                free (Val);
            }
            break;
        case MXSTRING :
            {
                STRING Val = (STRING)Value->Val;
                Error = MXSetRangeValue (pmessage, (int)Val2->Val , (int)Val3->Val, Val);
            }
            break;
        case MXCHAR :
            {
                STRING Val = (STRING)Value->Val;
                MXSetRangeValue (pmessage, (int)Val2->Val , 1, Val);
            }
            break;
        }
    }
    else
    {
        Error =1;
    }
    SCAffectValue (thisaction->Value, (void *)Error, SCINTEGER);
    SCSetActionState (psc, thisaction, SCDONE, SCDEFINED, Error);
    return thisaction->Value;
}


SCPROCRESULT  SCMXSetValue (SC* psc, SCAction* thisaction)
{
    MXObject* pobject;
    SCValue* Val1 = NULL;
    SCValue* Val2 = NULL;
    SCValue* Val3 = NULL;
    SCValue* Value = NULL;
    int Error = 0;
    MXMessage* pmessage;

    SCASSERTPARAMNUMBER (thisaction, 4);

    Val1 = SCGetParamValue (psc, thisaction, 1);
    SCASSERTDEFINEDVALUE (Val1);
    SCASSERTORVALUES (Val1, SCPOINTER, SCLONG);
    pmessage = (MXMessage*)Val1->Val;
    SCASSERTNOTNULL (pmessage);


    Val2 = SCGetParamValue (psc, thisaction, 2);
    SCASSERTDEFINEDVALUE (Val2);
    SCASSERTORVALUES (Val2, SCCHAR, SCSTRING);

    Val3 = SCGetParamValue (psc, thisaction, 3);
    SCASSERTDEFINEDVALUE (Val3);
    SCASSERTVALUE (Val3, SCINTEGER);

    Value = SCGetParamValue (psc, thisaction, 4);
    SCASSERTDEFINEDVALUE (Value);

    pobject = MXGetObjectFromName (pmessage->MessageClass, (char*)Val2->Val);
    if (pobject)
    {
        switch (pobject->Type)
        {
        case MXSHORT :
            {
                SHORT Val = *(SHORT*)Value->Val;
                Error = MXSetValue (pmessage, pobject->Name , (int)Val3->Val, &Val);
            }
            break;
        case MXWORD :
            {
                WORD Val = *(WORD*)Value->Val;
                Error = MXSetValue (pmessage, pobject->Name , (int)Val3->Val, &Val);
            }
            break;
        case MXDWORD :
            {
                DWORD Val = *(DWORD*)Value->Val;
                Error = MXSetValue (pmessage, pobject->Name , (int)Val3->Val, &Val);
            }
            break;
        case MXLONG :
            {
                LONG Val = *(LONG*)Value->Val;
                Error = MXSetValue (pmessage, pobject->Name , (int)Val3->Val, &Val);
            }
            break;
        case MXFILE :
            {
                FILEPARM* Val = (FILEPARM *)Value->Val;
                Error = MXSetValue (pmessage, pobject->Name , (int)Val3->Val, Val);
                free (Val);
            }
            break;
        case  MXBUFFER :
            {
                BUFFERPARM* Val = (BUFFERPARM *)Value->Val;
                Error = MXSetValue (pmessage, pobject->Name , (int)Val3->Val, Val);
                free (Val);
            }
            break;
        case MXSTRING :
            {
                STRING Val = (STRING)Value->Val;
                Error = MXSetValue (pmessage, pobject->Name , (int)Val3->Val, Val);
            }
            break;
        case MXCHAR :
        case MXBYTE :
            {
                STRING Val = (STRING)Value->Val;
                MXSetValue (pmessage, pobject->Name , 1, Val);
            }
            break;
        }
    }
    else
    {
        Error =1;
    }
    SCAffectValue (thisaction->Value, (void *)Error, SCINTEGER);
    SCSetActionState (psc, thisaction, SCDONE, SCDEFINED, Error);
    return thisaction->Value;
}

/*------------------------------------------------------------------------*/


SCPROCRESULT  SCMXSend (SC* psc, SCAction* thisaction)
{
    SCValue* Val1 = NULL;
    SCValue* Val2 = NULL;
    MXCom* pcom;
    MXMessage* pmessage;
    int Error = 0;
    int result = 0;

    SCSETRETURNVALUETYPE(thisaction->Value, SCINTEGER);
    SCASSERTPARAMNUMBER (thisaction, 2);

    Val1 = SCGetParamValue (psc, thisaction, 1);
    SCASSERTDEFINEDVALUE (Val1);
    SCASSERTORVALUES (Val1, SCPOINTER, SCLONG);
    pcom = (MXCom*)Val1->Val;
    SCASSERTNOTNULL (pcom);
    SCASSERTNOTNULL (pcom->Client);


    Val2 = SCGetParamValue (psc, thisaction, 2);
    SCASSERTDEFINEDVALUE (Val2);
    SCASSERTORVALUES (Val2, SCPOINTER, SCLONG);
    pmessage = (MXMessage*)Val2->Val;
    SCASSERTNOTNULL (pmessage);

    result = MXSend (pcom->MX, pcom, pmessage);
    if (result <= 0) Error = 1;

    SCAffectValue (thisaction->Value, (void*)result, SCINTEGER);
    SCSetActionState (psc, thisaction, SCDONE, SCDEFINED, Error);
    return thisaction->Value;
}


SCPROCRESULT  SCMXSetCGIFile (SC* psc, SCAction* thisaction)
{
    SCValue* Val1 = NULL;
    SCValue* Val2 = NULL;
    int Error = 0;
    int result = 0;
    int NbParam = 0;

    SCSETRETURNVALUETYPE(thisaction->Value, SCINTEGER);
    SCASSERTMAXPARAM (thisaction, 2);
    SCASSERTMINPARAM (thisaction, 1);

    NbParam = thisaction->NbParam;


    Val1 = SCGetParamValue (psc, thisaction, 1);
    SCASSERTDEFINEDVALUE (Val1);
    SCASSERTORVALUES (Val1, SCSTRING, SCCHAR);

    if (NbParam > 1)
    {
        Val2 = SCGetParamValue (psc, thisaction, 2);
        SCASSERTDEFINEDVALUE (Val2);
        SCASSERTORVALUES (Val2, SCPOINTER, SCLONG);
        result = MXSetCGIFile (psc->MX, (char*)Val1->Val, (FILEID*)Val2->Val);
    }
    else
        result = MXSetCGIFile (psc->MX, (char*)Val1->Val, NULL);
    if (result <= 0) Error = 1;

    SCAffectValue (thisaction->Value, (void*)result, SCINTEGER);
    SCSetActionState (psc, thisaction, SCDONE, SCDEFINED, Error);
    return thisaction->Value;
}


SCPROCRESULT  SCCGIGetValueFormEntry (SC* psc, SCAction* thisaction)
{
    SCValue* Val1 = NULL;
    int Error = 0;
    char* result = NULL;
    int NbParam = 0;

    SCSETRETURNVALUETYPE(thisaction->Value, SCSTRING);
    SCASSERTPARAMNUMBER (thisaction, 1);

    NbParam = thisaction->NbParam;


    Val1 = SCGetParamValue (psc, thisaction, 1);
    SCASSERTDEFINEDVALUE (Val1);
    SCASSERTORVALUES (Val1, SCSTRING, SCCHAR);
    if (psc->MX->CGI)
        result = CGIGetValueFormEntry (psc->MX->CGI, (char*)Val1->Val);
    else
        Error = -1;

    SCPutValue (thisaction->Value, result, SCSTRING);

    SCSetActionState (psc, thisaction, SCDONE, SCDEFINED, Error);
    return thisaction->Value;
}




SCPROCRESULT  SCCGIAddValue (SC* psc, SCAction* thisaction)
{
    SCValue* Val1 = NULL;
    SCValue* Val2 = NULL;
    CGIValue* Value = NULL;
    int Error = 0;
    char* result = NULL;
    int NbParam = 0;

    SCSETRETURNVALUETYPE(thisaction->Value, SCPOINTER);
    SCASSERTPARAMNUMBER (thisaction, 2);

    NbParam = thisaction->NbParam;

    Val1 = SCGetParamValue (psc, thisaction, 1);
    SCASSERTDEFINEDVALUE (Val1);
    SCASSERTORVALUES (Val1, SCSTRING, SCCHAR);

    Val2 = SCGetParamValue (psc, thisaction, 2);
    SCASSERTDEFINEDVALUE (Val2);
    SCASSERTORVALUES (Val2, SCSTRING, SCCHAR);

    if ((psc->MX->CGI) &&
            (Value = CGIAddValue (psc->MX->CGI, (char*)Val1->Val, (char*)Val2->Val)) != NULL)
        Error = 0;
    else
        Error = -1;

    SCPutValue (thisaction->Value, Value, SCPOINTER);

    SCSetActionState (psc, thisaction, SCDONE, SCDEFINED, Error);
    return thisaction->Value;
}


SCPROCRESULT  SCCGIAddColumn (SC* psc, SCAction* thisaction)
{

    SCValue* Val1 = NULL;
    SCValue* Val2 = NULL;
    SCValue* Val3 = NULL;
    SCValue* Val4 = NULL;
    CGIValue* Value = NULL;
    int Error = 0;
    char* result = NULL;
    int NbParam = 0;

    SCSETRETURNVALUETYPE(thisaction->Value, SCINTEGER);
    SCASSERTPARAMNUMBER (thisaction, 3);

    NbParam = thisaction->NbParam;

    Val1 = SCGetParamValue (psc, thisaction, 1);
    SCASSERTDEFINEDVALUE (Val1);
    SCASSERTORVALUES (Val1, SCSTRING, SCCHAR);

    Val2 = SCGetParamValue (psc, thisaction, 2);
    SCASSERTDEFINEDVALUE (Val2);
    SCASSERTVALUE (Val2, SCINTEGER);

    Val3 = SCGetParamValue (psc, thisaction, 3);
    SCASSERTDEFINEDVALUE (Val3);
    SCASSERTVALUE (Val3, SCINTEGER);



    if (psc->MX->CGI)
        CGIAddColumn (psc->MX->CGI, (char*)Val1->Val, (int)Val2->Val, (int)Val3->Val, NULL);
    else
        Error = -1;

    SCAffectValue (thisaction->Value, (void*)Error, SCINTEGER);
    SCSetActionState (psc, thisaction, SCDONE, SCDEFINED, Error);
    return thisaction->Value;
}







/*------------------------------------------------------------------------*/


SCPROCRESULT  SCMXRecv (SC* psc, SCAction* thisaction)
{
    SCValue* Val1 = NULL;
    MXCom* pcom;
    MXMessage* pmessage;
    int Error = 0;

    SCSETRETURNVALUETYPE(thisaction->Value, SCPOINTER);
    SCASSERTPARAMNUMBER (thisaction, 1);

    Val1 = SCGetParamValue (psc, thisaction, 1);
    SCASSERTDEFINEDVALUE (Val1);
    SCASSERTVALUE (Val1, SCPOINTER);

    pcom = (MXCom*)Val1->Val;
    SCASSERTNOTNULL (pcom);
    SCASSERTNOTNULL (pcom->Client);


    pmessage = MXRecv (pcom->MX, pcom);
    if (!pmessage) Error = 1;
    else SCPutValue (thisaction->Value, pmessage, SCPOINTER);
    SCSetActionState (psc, thisaction, SCDONE, SCDEFINED, Error);
    return thisaction->Value;
}

/*------------------------------------------------------------------------*/


SCPROCRESULT  SCMXCreateMessage (SC* psc, SCAction* thisaction)
{
    SCValue* Val1 = NULL;
    SCValue* Val2 = NULL;
    int Error = 0;
    MXDialogClass* pclass;
    MXMessageClass* pmessclass;
    MXMessage* pmessage;

    SCSETRETURNVALUETYPE(thisaction->Value, SCPOINTER);
    SCASSERTPARAMNUMBER (thisaction, 2);

    Val1 = SCGetParamValue (psc, thisaction, 1);
    SCASSERTDEFINEDVALUE (Val1);
    SCASSERTORVALUES (Val1, SCCHAR, SCSTRING);

    Val2 = SCGetParamValue (psc, thisaction, 2);
    SCASSERTDEFINEDVALUE (Val2);
    SCASSERTORVALUES (Val2, SCCHAR, SCSTRING);

    pclass = MXGetDialogClassFromName (psc->MX, (char*)Val1->Val);
    if (!pclass)
    {
        Error = -1;
        SCSetActionState (psc, thisaction, SCDONE, SCDEFINED, Error);
        return thisaction->Value;
    }

    pmessclass = MXGetMessageClassFromName (pclass, (char*)Val2->Val);
    if (!pmessclass)
    {
        Error = -1;
        SCSetActionState (psc, thisaction, SCDONE, SCDEFINED, Error);
        return thisaction->Value;
    }
    pmessage = MXCreateMessage (psc->MX, pclass->Name, pmessclass->Name);
    if (!pmessage) Error = -1;
    else SCPutValue (thisaction->Value, pmessage, SCPOINTER);
    SCSetActionState (psc, thisaction, SCDONE, SCDEFINED, Error);
    return thisaction->Value;
}

/*------------------------------------------------------------------------*/


SCPROCRESULT  SCMXFreeMessage (SC* psc, SCAction* thisaction)
{
    SCValue* Val1 = NULL;
    MXMessage* pmessage;
    int Error = 0;

    SCSETRETURNVALUETYPE(thisaction->Value, SCINTEGER);
    SCASSERTPARAMNUMBER (thisaction, 1);

    Val1 = SCGetParamValue (psc, thisaction, 1);
    SCASSERTDEFINEDVALUE (Val1);
    SCASSERTVALUE (Val1, SCPOINTER);

    pmessage = (MXMessage*)Val1->Val;

    if (pmessage)
        MXFreeMessage (psc->MX, pmessage);
    else
        Error = 1;

    SCAffectValue (thisaction->Value, (void*)Error, SCINTEGER);
    SCSetActionState (psc, thisaction, SCDONE, SCDEFINED, Error);
    return thisaction->Value;
}


/*------------------------------------------------------------------------*/


SCPROCRESULT  SCMXSetIOMessageClassProc (SC* psc, SCAction* thisaction)
{
    SCValue* Val1 = NULL;
    SCValue* Val2 = NULL;
    MXMessageClass* pmessageclass;
    MXCom* pcom;
    int Error = 0;

    SCSETRETURNVALUETYPE(thisaction->Value, SCINTEGER);
    SCASSERTPARAMNUMBER (thisaction, 2);

    Val1 = SCGetParamValue (psc, thisaction, 1);
    SCASSERTDEFINEDVALUE (Val1);
    SCASSERTVALUE (Val1, SCPOINTER);
    pcom = (MXCom*)Val1->Val;

    SCASSERTNOTNULL (pcom);
    SCASSERTNOTNULL (pcom->Client);


    Val2 = SCGetParamValue (psc, thisaction, 2);
    SCASSERTDEFINEDVALUE (Val2);
    SCASSERTORVALUES (Val2, SCPOINTER, SCLONG);

    pmessageclass = (MXMessageClass*)Val2->Val;
    SCASSERTNOTNULL (pmessageclass);

    MXSetIOMessageClass (pcom, pmessageclass);
    SCAffectValue (thisaction->Value, (void*)Error, SCINTEGER);
    SCSetActionState (psc, thisaction, SCDONE, SCDEFINED, Error);
    return thisaction->Value;
}


SCPROCRESULT  SCMXGetMessageClassFromNames (SC* psc, SCAction* thisaction)
{
    SCValue* Val1 = NULL;
    SCValue* Val2 = NULL;
    MXMessageClass* pmessageclass;
    int Error = 0;

    SCSETRETURNVALUETYPE(thisaction->Value, SCPOINTER);
    SCASSERTPARAMNUMBER (thisaction, 2);

    Val1 = SCGetParamValue (psc, thisaction, 1);
    SCASSERTDEFINEDVALUE (Val1);
    SCASSERTORVALUES (Val1, SCSTRING, SCCHAR);
    

    Val2 = SCGetParamValue (psc, thisaction, 2);
    SCASSERTDEFINEDVALUE (Val2);
    SCASSERTORVALUES (Val2, SCSTRING, SCCHAR);

    pmessageclass = MXGetMessageClassFromNames (psc->MX, Val1->Val, Val2->Val);

    SCPutValue(thisaction->Value, pmessageclass, SCPOINTER);

    SCSetActionState (psc, thisaction, SCDONE, SCDEFINED, Error);

    return thisaction->Value;
}

/*------------------------------------------------------------------------*/


SCPROCRESULT  SCMXAddComCallBack (SC* psc, SCAction* thisaction)
{
    SCValue* Val1 = NULL;
    SCValue* Val2 = NULL;
    SCValue* Val3 = NULL;
    SCValue* Val4 = NULL;
    SCValue* Val5 = NULL;
    int Error = 0;
    SCComFuncStruct* FuncStruct;
    MXDialogClass* pclass;
    MXMessageClass* pmessclass;
    MXCom* pcom;
    BYTE Type;

    SCSETRETURNVALUETYPE(thisaction->Value, SCINTEGER);

    SCASSERTPARAMNUMBER (thisaction, 5);


    Val1 = SCGetParamValue (psc, thisaction, 1);
    SCASSERTDEFINEDVALUE (Val1);
    SCASSERTORVALUES (Val1, SCPOINTER, SCLONG);
    pcom = (MXCom*)Val1->Val;

    SCASSERTNOTNULL (pcom);
    SCASSERTNOTNULL (pcom->Client);


    Val2 = SCGetParamValue (psc, thisaction, 2);
    SCASSERTDEFINEDVALUE (Val2);
    SCASSERTORVALUES (Val2, SCCHAR, SCSTRING);

    Val3 = SCGetParamValue (psc, thisaction, 3);
    SCASSERTDEFINEDVALUE (Val3);
    SCASSERTORVALUES (Val3, SCCHAR, SCSTRING);

    Val4 = SCGetParamValue (psc, thisaction, 4);
    SCASSERTDEFINEDVALUE (Val4);
    SCASSERTORVALUES (Val4, SCCHAR, SCSTRING);

    Val5 = SCGetParamValue (psc, thisaction, 5);
    SCASSERTDEFINEDVALUE (Val5);
    SCASSERTORVALUES (Val5, SCCHAR, SCSTRING);

    pclass = MXGetDialogClassFromName (psc->MX, (char*)Val2->Val);
    if (!pclass)
    {
        Error = -1;
        SCSetActionState (psc, thisaction, SCDONE, SCDEFINED, Error);
        return thisaction->Value;
    }

    pmessclass = MXGetMessageClassFromName (pclass, (char*)Val3->Val);
    if (!pmessclass)
    {
        if ((OSStrcmp (pclass->Name, DB_SYS) != 0) &&
                (OSStrcmp (pclass->Name, FIOP_SYS) != 0))
        {
            Error = -1;
            SCSetActionState (psc, thisaction, SCDONE, SCDEFINED, Error);
            return thisaction->Value;
        }
        else
            pmessclass = MXCreateMessageClass(psc->MX, pclass, (char*)Val3->Val, 1, 0);
    }
    if (OSStrcmp (Val4->Val, "ONSEND") == 0)
        Type = MXONSEND;
    else
        if (OSStrcmp (Val4->Val, "ONRECV") == 0)
            Type = MXONRECV;
        else
        {
            Error = -1;
            SCSetActionState (psc, thisaction, SCDONE, SCDEFINED, Error);
            return thisaction->Value;
        }


    FuncStruct = SCFindFunction (psc, (char*)Val5->Val);

    if (!FuncStruct)
    {
        FuncStruct = (SCComFuncStruct *)malloc (sizeof (SCComFuncStruct));
        ListInsert(&psc->ListComFunctions, FuncStruct);
    }

    strcpy (FuncStruct->Name, (char*)Val5->Val);
    strcpy (FuncStruct->ClassName, pclass->Name);
    strcpy (FuncStruct->MessageClassName, pmessclass->Name);
    FuncStruct->Type = Type;
    FuncStruct->Par1 = psc;

    Error = MXAddComCallBack (pcom->MX, pcom, FuncStruct->ClassName, FuncStruct->MessageClassName, FuncStruct->Type, SCReceiveMessage, FuncStruct);
    if (Error > 0) Error = 0;

    SCAffectValue (thisaction->Value, (void *)Error, SCINTEGER);
    SCSetActionState (psc, thisaction, SCDONE, SCDEFINED, Error);

    return thisaction->Value;
}

/*------------------------------------------------------------------------*/

SCPROCRESULT  SCMXRemoveComCallBack(SC* psc, SCAction* thisaction)
{
    SCValue* Val1 = NULL;
    SCValue* Val2 = NULL;
    SCValue* Val3 = NULL;
    SCValue* Val4 = NULL;
    SCValue* Val5 = NULL;
    int Error = 0;
    SCComFuncStruct* FuncStruct;
    MXDialogClass* pclass;
    MXMessageClass* pmessclass;
    MXCom* pcom;
    BYTE Type;

    SCSETRETURNVALUETYPE(thisaction->Value, SCINTEGER);

    SCASSERTPARAMNUMBER (thisaction, 5);


    Val1 = SCGetParamValue (psc, thisaction, 1);
    SCASSERTDEFINEDVALUE (Val1);
    SCASSERTVALUE (Val1, SCPOINTER);
    pcom = (MXCom*)Val1->Val;

    SCASSERTNOTNULL (pcom);
    SCASSERTNOTNULL (pcom->Client);


    Val2 = SCGetParamValue (psc, thisaction, 2);
    SCASSERTDEFINEDVALUE (Val2);
    SCASSERTORVALUES (Val2, SCCHAR, SCSTRING);

    Val3 = SCGetParamValue (psc, thisaction, 3);
    SCASSERTDEFINEDVALUE (Val3);
    SCASSERTORVALUES (Val3, SCCHAR, SCSTRING);

    Val4 = SCGetParamValue (psc, thisaction, 4);
    SCASSERTDEFINEDVALUE (Val4);
    SCASSERTORVALUES (Val4, SCCHAR, SCSTRING);

    Val5 = SCGetParamValue (psc, thisaction, 5);
    SCASSERTDEFINEDVALUE (Val5);
    SCASSERTORVALUES (Val5, SCCHAR, SCSTRING);

    pclass = MXGetDialogClassFromName (psc->MX, (char*)Val2->Val);
    if (!pclass)
    {
        Error = -1;
        SCSetActionState (psc, thisaction, SCDONE, SCDEFINED, Error);
        return thisaction->Value;
    }

    pmessclass = MXGetMessageClassFromName (pclass, (char*)Val3->Val);
    if (!pmessclass)
    {
        Error = -1;
        SCSetActionState (psc, thisaction, SCDONE, SCDEFINED, Error);
        return thisaction->Value;
    }

    if (OSStrcmp (Val4->Val, "ONSEND") == 0)
        Type = MXONSEND;
    else
        if (OSStrcmp (Val4->Val, "ONRECV") == 0)
            Type = MXONRECV;
        else
        {
            Error = -1;
            SCSetActionState (psc, thisaction, SCDONE, SCDEFINED, Error);
            return thisaction->Value;
        }


    FuncStruct = SCFindFunction (psc, (char*)Val5->Val);
    if (FuncStruct)
    {
        ListRemove(&psc->ListComFunctions, FuncStruct);
        Error = MXRemoveComCallBack (pcom->MX, pcom, FuncStruct->ClassName, FuncStruct->MessageClassName, FuncStruct->Type, SCReceiveMessage, FuncStruct);
        if (Error > 0) Error = 0;
        free (FuncStruct);
    }
    else
    {
        Error = -1;
        SCSetActionState (psc, thisaction, SCDONE, SCDEFINED, Error);
        return thisaction->Value;
    }

    SCAffectValue (thisaction->Value, (void *)Error, SCINTEGER);
    SCSetActionState (psc, thisaction, SCDONE, SCDEFINED, Error);

    return thisaction->Value;
}

/*------------------------------------------------------------------------*/

SCPROCRESULT  SCMXAddEndingProcedure (SC* psc, SCAction* thisaction)
{
    return thisaction->Value;
}
/*------------------------------------------------------------------------*/

SCPROCRESULT  SCMXRemoveEndingProcedure (SC* psc, SCAction* thisaction)
{
    return thisaction->Value;
}
/*------------------------------------------------------------------------*/

SCPROCRESULT  SCMXAddGeneralConnectCallBack (SC* psc, SCAction* thisaction)
{
    SCValue* Val1 = NULL;
    SCValue* Val2 = NULL;
    int Error = 0;
    SCComFuncStruct* FuncStruct;
    BYTE Type;

    SCSETRETURNVALUETYPE(thisaction->Value, SCINTEGER);

    SCASSERTPARAMNUMBER (thisaction, 2);

   
    Val1 = SCGetParamValue (psc, thisaction, 1);
    SCASSERTDEFINEDVALUE (Val1);
    SCASSERTORVALUES (Val1, SCCHAR, SCSTRING);

    Val2 = SCGetParamValue (psc, thisaction, 2);
    SCASSERTDEFINEDVALUE (Val2);
    SCASSERTORVALUES (Val2, SCCHAR, SCSTRING);

    
    if (OSStrcmp (Val1->Val, "ONCONNECT") == 0)
        Type = MXONCONNECT;
    else
        if (OSStrcmp (Val1->Val, "ONCLOSE") == 0)
            Type = MXONCLOSE;
        else
        {
            Error = -1;
            SCSetActionState (psc, thisaction, SCDONE, SCDEFINED, Error);
            return thisaction->Value;
        }


    FuncStruct = SCFindFunction (psc, (char*)Val2->Val);

    if (!FuncStruct)
    {
        FuncStruct = (SCComFuncStruct *)malloc (sizeof (SCComFuncStruct));
        ListInsert(&psc->ListComFunctions, FuncStruct);
    }

    strcpy (FuncStruct->Name, (char*)Val2->Val);
    FuncStruct->Type = Type;
    FuncStruct->Par1 = psc;

    Error = MXAddGeneralConnectCallBack (psc->MX, FuncStruct->Type, SCGeneralConnect, FuncStruct);
    if (Error > 0) Error = 0;

    SCAffectValue (thisaction->Value, (void *)Error, SCINTEGER);
    SCSetActionState (psc, thisaction, SCDONE, SCDEFINED, Error);

    return thisaction->Value;
}
/*------------------------------------------------------------------------*/

SCPROCRESULT  SCMXRemoveGeneralConnectCallBack (SC* psc, SCAction* thisaction)
{
    SCValue* Val1 = NULL;
    SCValue* Val2 = NULL;
    int Error = 0;
    SCComFuncStruct* FuncStruct;
    BYTE Type;

    SCSETRETURNVALUETYPE(thisaction->Value, SCINTEGER);

    SCASSERTPARAMNUMBER (thisaction, 2);


    Val1 = SCGetParamValue (psc, thisaction, 1);
    SCASSERTDEFINEDVALUE (Val1);
    SCASSERTORVALUES (Val1, SCCHAR, SCSTRING);

    
    Val2 = SCGetParamValue (psc, thisaction, 2);
    SCASSERTDEFINEDVALUE (Val2);
    SCASSERTORVALUES (Val2, SCCHAR, SCSTRING);


    if (OSStrcmp (Val2->Val, "ONCONNECT") == 0)
        Type = MXONCONNECT;
    else
        if (OSStrcmp (Val2->Val, "ONCLOSE") == 0)
            Type = MXONCLOSE;
        else
        {
            Error = -1;
            SCSetActionState (psc, thisaction, SCDONE, SCDEFINED, Error);
            return thisaction->Value;
        }


    FuncStruct = SCFindFunction (psc, (char*)Val1->Val);
    if (FuncStruct)
    {
        ListRemove(&psc->ListComFunctions, FuncStruct);
        Error = MXRemoveGeneralConnectCallBack (psc->MX, FuncStruct->Type, SCGeneralConnect, FuncStruct);
        if (Error > 0) Error = 0;
        free (FuncStruct);
    }
    else
    {
        Error = -1;
        SCSetActionState (psc, thisaction, SCDONE, SCDEFINED, Error);
        return thisaction->Value;
    }

    SCAffectValue (thisaction->Value, (void *)Error, SCINTEGER);
    SCSetActionState (psc, thisaction, SCDONE, SCDEFINED, Error);
   
    return thisaction->Value;
}
/*------------------------------------------------------------------------*/



SCPROCRESULT SCInterruptedCom (SC* psc,  SCAction* thisaction)
{

    SCValue* Val1 = NULL;
    int interrupt = 0;
    MXCom* pcom;
    int Error = 0;

    SCSETRETURNVALUETYPE(thisaction->Value, SCINTEGER);

    SCASSERTPARAMNUMBER (thisaction, 1);

    Val1 = SCGetParamValue (psc, thisaction, 1);
    SCASSERTDEFINEDVALUE (Val1);
    SCASSERTORVALUES (Val1, SCPOINTER, SCLONG);

    pcom = (MXCom*)Val1->Val;
    SCASSERTNOTNULL (pcom);

    if ((pcom) && (!pcom->Client))
    {
        interrupt = 1;
        SCAffectValue (thisaction->Value, (void *)interrupt, SCINTEGER);
    }

    SCSetActionState (psc, thisaction, SCDONE, SCDEFINED, Error);

    return thisaction->Value;
}

/*------------------------------------------------------------------------*/


SCPROCRESULT SCMXJoinCom (SC* psc,  SCAction* thisaction)
{

    SCValue* Val1 = NULL;
    SCValue* Val2 = NULL;
    int Error;


    SCSETRETURNVALUETYPE(thisaction->Value, SCINTEGER);

    SCASSERTPARAMNUMBER (thisaction, 2);

    Val1 = SCGetParamValue (psc, thisaction, 1);
    SCASSERTDEFINEDVALUE (Val1);
    SCASSERTORVALUES (Val1, SCPOINTER, SCLONG);
    SCASSERTNOTNULL (Val1->Val);


    Val2 = SCGetParamValue (psc, thisaction, 2);
    SCASSERTDEFINEDVALUE (Val2);
    SCASSERTORVALUES (Val2, SCPOINTER, SCLONG);
    SCASSERTNOTNULL (Val2->Val);



    Error = MXJoinCom (psc->MX, (MXCom*)Val1->Val, (MXCom*)Val2->Val);

    SCAffectValue (thisaction->Value, (void*)Error, SCINTEGER);

    SCSetActionState (psc, thisaction, SCDONE, SCDEFINED, Error);

    return thisaction->Value;
}



/*------------------------------------------------------------------------*/

SCPROCRESULT SCMXCreateMessageClass (SC* psc,  SCAction* thisaction)
{
    SCValue* Val1 = NULL;
    SCValue* Val2 = NULL;
    SCValue* Val3 = NULL;
    SCValue* Val4 = NULL;
    SCValue* Val5 = NULL;
    int Error = 0;
    MXDialogClass* pclass;
    MXMessageClass* pclassmess;
    int nb_param = 0;
    int i, ret;
    WORD offset = 0;
    WORD range   = 1;

    SCASSERTMINPARAM (thisaction, 2);
    nb_param  = thisaction->NbParam - 2;

    SCSETRETURNVALUETYPE(thisaction->Value, SCPOINTER);


    Val1 = SCGetParamValue (psc, thisaction, range++);
    SCASSERTDEFINEDVALUE(Val1) ;
    SCASSERTORVALUES(Val1, SCSTRING, SCCHAR);

    pclass = MXGetDialogClassFromName (psc->MX, (char *)Val1->Val);
    if (!pclass) pclass = MXCreateDialogClass(psc->MX, (char *)Val1->Val, DL_SYS_CODE);

    pclassmess = (MXMessageClass *)malloc (sizeof (MXMessageClass));

    Val2 = SCGetParamValue (psc, thisaction, range++);
    SCASSERTDEFINEDVALUE(Val2) ;
    SCASSERTORVALUES(Val2, SCSTRING, SCCHAR);

    strcpy(pclassmess->Name, (char *)Val2->Val);
    pclassmess->Code            = ListNbElt (pclass->MessageClasses) + 1;
    pclassmess->Size            = 0;
    pclassmess->ExecuteAck      = NULL;
    pclassmess->ExecuteOutputs  = NULL;
    pclassmess->ExecuteInputs   = NULL;
    pclassmess->BlockedOnInput  = FALSE;
    pclassmess->BlockedOnOutput = FALSE;
    pclassmess->Objects         = NULL;

    for (i = 1; i <= (nb_param / 3); i++)
    {
        MXObject* pobject = (MXObject *)malloc (sizeof (MXObject));
        pobject->Size   = 1;

        Val3 = SCGetParamValue (psc, thisaction, range++);
        SCASSERTDEFINEDVALUE(Val3) ;
        SCASSERTORVALUES(Val3, SCSTRING, SCCHAR);

        ret = MXCheckType2 ((char *)Val3->Val);
        pobject->Type   = ret;

        Val4 = SCGetParamValue (psc, thisaction, range++);
        SCASSERTDEFINEDVALUE(Val4);
        SCASSERTVALUE(Val4, SCINTEGER);

        pobject->Size   = *(WORD*)Val4->Val;

        Val5 = SCGetParamValue (psc, thisaction, range++);
        SCASSERTDEFINEDVALUE(Val5) ;
        SCASSERTORVALUES(Val5, SCSTRING, SCCHAR);

        strcpy(pobject->Name, (char *)Val5->Val);
        pobject->Offset = offset;
        offset += pobject->Size;
        ListNewr (&pclassmess->Objects, pobject);
    }

    pclassmess->Size   = offset;
    ListInsert (&pclass->MessageClasses, pclassmess);

    SCPutValue(thisaction->Value, pclassmess, SCPOINTER);


    SCSetActionState (psc, thisaction, SCDONE, SCDEFINED, Error);


    return thisaction->Value;
}

/*------------------------------------------------------------------------*/


SCPROCRESULT SCMXPutMessage (SC* psc,  SCAction* thisaction)
{
    SCValue* Val1 = NULL;
    SCValue* Val2 = NULL;
    SCValue* Val3 = NULL;
    int Error = 0;
    MXDialogClass* pclass;
    MXMessageClass* pmessclass;
    MXMessage* pmessage;
    MXCom* pcom;


    SCSETRETURNVALUETYPE(thisaction->Value, SCPOINTER);
    SCASSERTPARAMNUMBER (thisaction, 3);

    Val1 = SCGetParamValue (psc, thisaction, 1);
    SCASSERTDEFINEDVALUE (Val1);
    SCASSERTORVALUES (Val1, SCPOINTER, SCLONG);
    pcom = (MXCom*)Val1->Val;
    SCASSERTNOTNULL (pcom);
    SCASSERTNOTNULL (pcom->Client);

    Val2 = SCGetParamValue (psc, thisaction, 2);
    SCASSERTDEFINEDVALUE (Val2);
    SCASSERTORVALUES (Val2, SCSTRING, SCCHAR);

    Val3 = SCGetParamValue (psc, thisaction, 3);
    SCASSERTDEFINEDVALUE (Val3);
    SCASSERTORVALUES (Val3, SCSTRING, SCCHAR);


    pclass = MXGetDialogClassFromName (psc->MX, (char*)Val2->Val);
    if (!pclass)
    {
        Error = -1;
        SCSetActionState (psc, thisaction, SCDONE, SCDEFINED, Error);
        return thisaction->Value;
    }

    pmessclass = MXGetMessageClassFromName (pclass, (char*)Val3->Val);
    if (!pmessclass)
    {
        Error = -1;
        SCSetActionState (psc, thisaction, SCDONE, SCDEFINED, Error);
        return thisaction->Value;
    }
    pmessage = MXPutMessage (pcom, pclass->Name, pmessclass->Name);
    if (!pmessage) Error = -1;
    else SCPutValue (thisaction->Value, pmessage, SCPOINTER);
    SCSetActionState (psc, thisaction, SCDONE, SCDEFINED, Error);
    return thisaction->Value;
}


SCPROCRESULT SCDGSendMessage (SC* psc,  SCAction* thisaction)
{
    SCValue* Val1 = NULL;
    SCValue* Val2 = NULL;
    SCValue* Val3 = NULL;
    SCValue* Value = NULL;
    int Error = 0;


    MXMessage*      pmessage = NULL;
    MXMessageClass* pclassmess;
    MXObject* pobject;
    MXCom*    pcom;

    SCEntity* Entity;
    SCAction* RootAction;
    SCComFuncStruct* pfunc;

    WORD   range = 1;
    int    nbparamnow;
    WORD    i;
    List* listfunctions = NULL;
    List* list;


    RootAction = SCGetRootActionFromAction (psc, thisaction);

    SCSETRETURNVALUETYPE(thisaction->Value, SCPOINTER);

    SCASSERTMINPARAM (thisaction, 3);

    Val1 = SCGetParamValue (psc, thisaction, range++);
    SCASSERTDEFINEDVALUE (Val1);
    SCASSERTORVALUES (Val1, SCPOINTER, SCLONG);


    Val2 = SCGetParamValue (psc, thisaction, range++);
    SCASSERTDEFINEDVALUE (Val2);
    SCASSERTORVALUES (Val2, SCSTRING, SCCHAR);

    Val3 = SCGetParamValue (psc, thisaction, range++);
    SCASSERTDEFINEDVALUE (Val3);
    SCASSERTORVALUES (Val3, SCSTRING, SCCHAR);

    pcom = (MXCom*)Val1->Val;
    SCASSERTNOTNULL (pcom);


    thisaction->Com = pcom;


    for (i = 0; i < thisaction->NbParam - 3; i++)
    {
        Value = SCGetParamValue (psc, thisaction, range++);
        SCASSERTDEFINEDVALUE (Value);
    }
    range -= thisaction->NbParam - 3;

    nbparamnow = ListNbElt (thisaction->ListParams);

    if (nbparamnow == thisaction->NbParam)
    {
        pmessage = MXCreateMessage (pcom->MX, (char*)Val2->Val, (char*)Val3->Val);
        if (!pmessage)
        {
            Error = -1;
            SCSetActionState (psc, thisaction, SCDONE, SCDEFINED, Error);
            return thisaction->Value;
        }
        pmessage->Com = pcom;
        pclassmess = pmessage->MessageClass;
        list = pclassmess->Objects;
        for (i = 0; i < thisaction->NbParam - 3; i++)
        {
            pobject = (MXObject *)list->car;
            Value =    SCGetParamValue (psc, thisaction, range++);
            switch(Value->Type)
            {
            case SCINTEGER :
            case SCLONG :
            case SCFLOAT :
            case SCBOOLEAN :
                switch (pobject->Type)
                {
                case MXSHORT :
                    {
                        SHORT Val = *(SHORT*)Value->Val;
                        MXSetValue (pmessage, pobject->Name , 1, &Val);
                    }
                    break;
                case MXWORD :
                    {
                        WORD Val = *(WORD*)Value->Val;
                        MXSetValue (pmessage, pobject->Name , 1, &Val);
                    }
                    break;
                case MXDWORD :
                    {
                        DWORD Val = *(DWORD*)Value->Val;
                        MXSetValue (pmessage, pobject->Name , 1, &Val);
                    }
                    break;
                case MXLONG :
                    {
                        LONG Val = *(LONG*)Value->Val;
                        MXSetValue (pmessage, pobject->Name , 1, &Val);
                    }
                    break;
                }
                break;
            case SCSTRING :
            case SCDATE   :
            case SCTIME   :
            case SCCHAR   :
                switch (pobject->Type)
                {
                case MXBUFFER :
                    {
                        BUFFERPARM Buffer;
                        Buffer.BufferSize = strlen (Value->Val);
                        Buffer.BufferContent = malloc (strlen (Value->Val));
                        strncpy (Buffer.BufferContent, Value->Val, strlen (Value->Val));
                        Buffer.BufferType = 'T';
                        MXSetValue (pmessage, pobject->Name , 1, &Buffer);
                        free (Buffer.BufferContent);
                    }
                    break;
                default :
                    MXSetValue (pmessage, pobject->Name , 1, Value->Val);
                    break;
                }
                break;
            case SCNULL :
                break;
            default:
                break;
            }
            list = list->cdr;
        }
        listfunctions = psc->ListComFunctions;

        while (listfunctions)
        {
            pfunc = (SCComFuncStruct *)listfunctions->car;
            if ((pfunc->Par1 != pmessage->MessageClass) ||
                    (pfunc->Type != MXONSEND))
            {
                listfunctions = listfunctions->cdr;
                continue;
            }
            Entity = SCGetEntityFromFuncName (psc, pmessage, pfunc->Name);
            if (!Entity)
            {
                listfunctions = listfunctions->cdr;
                continue;
            }
            SCAddFunctionToInstance (psc, (SCAction*)Entity->Val, SCBEFORE, SCAffectMessageValueToEntityVar,  pmessage);
            SCAddFunctionToInstance (psc, (SCAction*)Entity->Val, SCAFTER,  SCAffectEntityVarToMessageValue,  pmessage);
            ListNewr (&thisaction->ListParams, Entity);
            nbparamnow++;
            SCSetEntityRootAction (psc, Entity, RootAction);
            listfunctions = listfunctions->cdr;
        }
    }

    for (i = thisaction->NbParam + 1; i <= nbparamnow; i++)
    {
        Value = SCGetParamValue (psc, thisaction, i);
        SCASSERTDEFINEDVALUE (Value);
    }

    if (pmessage)
    {
        pmessage->Identity = pcom->MessagesId;
        pcom->MessagesId++;
        ListNewr (&pcom->OutputMessages, pmessage);
    }


    SCPutValue(thisaction->Value, pmessage, SCPOINTER);

    SCSetActionState (psc, thisaction, SCDONE, SCDEFINED, Error);

    return thisaction->Value;
}




/*------------------------------------------------------------------------*/

int SCGeneralConnect (MXCom* pcom, void* par)
{
    char FuncName[100];
    char* ExecString;
    char* VarString;
    DWORD Size = 300;
    DWORD Read = 0;
    SCSection* Section;
    int Error = 0;
    SCComFuncStruct* FuncStruct = (SCComFuncStruct*)par;
    SC* psc = (SC*)FuncStruct->Par1;

    strcpy (FuncName,  FuncStruct->Name);

    Section = SCGetSectionFromList(psc->ListSections, FuncName);

    if (!Section)
    {
        return -1;
    }
    ExecString = (char*)malloc (Size);
    memset (ExecString , 0, Size);
    VarString = ExecString;
    sprintf (VarString ,"(%s %p",  FuncName, pcom);
    Read += strlen (VarString);

    strcat (ExecString + Read, ")");
    SCExecuteString (psc, ExecString, NULL, 0, 0);
    free (ExecString);

    return 1;
}

int SCReceiveMessage (MXMessage* pmessage, MXCom* pcom, void* par)
{
    char FuncName[100];
    char* ExecString;
    char* VarString;
    DWORD Size = 300;
    DWORD Read = 0;
    SCSection* Section;
    int Error = 0;
    SCComFuncStruct* FuncStruct = (SCComFuncStruct*)par;
    SC* psc = (SC*)FuncStruct->Par1;

    strcpy (FuncName,  FuncStruct->Name);

    Section = SCGetSectionFromList(psc->ListSections, FuncName);

    if (!Section)
    {
        return -1;
    }

    ExecString = (char*)malloc (Size);
    memset (ExecString , 0, Size);
    VarString = ExecString;
    sprintf (VarString ,"(%s %p %p",  FuncName, MXGetComFromMessage (pmessage), pmessage);
    Read += strlen (VarString);

    strcat (ExecString + Read, ")");
    SCExecuteString (psc, ExecString, NULL, 0, 0);
    free (ExecString);

    return 1;
}


/*------------------------------------------------------------------------*/

SCPROCRESULT SCMXOnRecvMessage (SC* psc,  SCAction* thisaction)
{
    SCValue* Val1 = NULL;
    SCValue* Val2 = NULL;
    SCValue* Val3 = NULL;
    int Error = 0;
    SCComFuncStruct* FuncStruct;

    SCSETRETURNVALUETYPE(thisaction->Value, SCINTEGER);

    SCASSERTPARAMNUMBER (thisaction, 3);

    Val1 = SCGetParamValue (psc, thisaction, 1);
    SCASSERTDEFINEDVALUE (Val1);
    SCASSERTVALUE (Val1, SCSTRING);

    Val2 = SCGetParamValue (psc, thisaction, 2);
    SCASSERTDEFINEDVALUE (Val2);
    SCASSERTVALUE (Val2, SCSTRING);

    Val3 = SCGetParamValue (psc, thisaction, 3);
    SCASSERTDEFINEDVALUE (Val3);
    SCASSERTORVALUES (Val3, SCCHAR, SCSTRING);



    FuncStruct = SCFindFunction (psc, (char*)Val3->Val);
    if (!FuncStruct)
    {
        FuncStruct = (SCComFuncStruct *)malloc (sizeof (SCComFuncStruct));
        ListInsert(&psc->ListComFunctions, FuncStruct);
    }
    strcpy (FuncStruct->Name, (char*)Val3->Val);
    FuncStruct->Par1 = psc;
    strcpy (FuncStruct->ClassName, (char*)Val1->Val);
    strcpy (FuncStruct->MessageClassName, (char*)Val2->Val);
    FuncStruct->Type = MXONRECV;

    Error = MXAddGeneralCallBack (psc->MX, (char*)Val1->Val, (char*)Val2->Val, MXONRECV, SCReceiveMessage, FuncStruct);
    SCAffectValue (thisaction->Value, (void *)Error, SCINTEGER);

    SCSetActionState (psc, thisaction, SCDONE, SCDEFINED, Error);

    return thisaction->Value;
}

/*------------------------------------------------------------------------*/


SCPROCRESULT SCMXOnSendMessage (SC* psc,  SCAction* thisaction)
{
    SCValue* Val1 = NULL;
    SCValue* Val2 = NULL;
    SCValue* Val3 = NULL;
    int Error = 0;
    SCComFuncStruct* FuncStruct;
    MXDialogClass* pclass;
    MXMessageClass* pmessclass;

    SCSETRETURNVALUETYPE(thisaction->Value, SCINTEGER);


    SCASSERTPARAMNUMBER (thisaction, 3);

    Val1 = SCGetParamValue (psc, thisaction, 1);
    SCASSERTDEFINEDVALUE (Val1);
    SCASSERTVALUE (Val1, SCSTRING);

    Val2 = SCGetParamValue (psc, thisaction, 2);
    SCASSERTDEFINEDVALUE (Val2);
    SCASSERTVALUE (Val2, SCSTRING);

    Val3 = SCGetParamValue (psc, thisaction, 3);
    SCASSERTDEFINEDVALUE (Val3);
    SCASSERTORVALUES (Val3, SCCHAR, SCSTRING);

    pclass = MXGetDialogClassFromName (psc->MX, (char*)Val1->Val);
    if (!pclass)
    {
        Error = -1;
        SCSetActionState (psc, thisaction, SCDONE, SCDEFINED, Error);
        return thisaction->Value;
    }
    pmessclass = MXGetMessageClassFromName (pclass, (char*)Val2->Val);
    if (!pmessclass)
    {
        Error = -1;
        SCSetActionState (psc, thisaction, SCDONE, SCDEFINED, Error);
        return thisaction->Value;
    }

    FuncStruct = SCFindFunction (psc, (char*)Val3->Val);
    if (!FuncStruct)
    {
        FuncStruct = (SCComFuncStruct *)malloc (sizeof (SCComFuncStruct));
        ListInsert(&psc->ListComFunctions, FuncStruct);
    }

    strcpy (FuncStruct->Name, (char*)Val3->Val);
    FuncStruct->Par1 = pmessclass;
    FuncStruct->Type = MXONSEND;
    strcpy (FuncStruct->ClassName, (char*)Val1->Val);
    strcpy (FuncStruct->MessageClassName, (char*)Val2->Val);
    SCAffectValue (thisaction->Value, (void *)Error, SCINTEGER);

    SCSetActionState (psc, thisaction, SCDONE, SCDEFINED, Error);
    return thisaction->Value;

}


/*------------------------------------------------------------------------*/
/* MXSYS                                                                  */
/*------------------------------------------------------------------------*/


SCPROCRESULT SCListProc  (SC* psc,  SCAction* thisaction)
{
    SCValue* Val = NULL;
    SCValue* Val1 = NULL;
    SCValue* Val2 = NULL;

    MXCom* pcom = psc->Com;
    WORD NbParam;
    char FileLoc;
    char* From = NULL;
    WORD range = 1;
    int Error= 0;

    FileLoc = LOCDISQU;

    SCSETRETURNVALUETYPE(thisaction->Value, SCINTEGER);

    SCASSERTMINPARAM (thisaction, 0);
    SCASSERTMAXPARAM (thisaction, 3);

    Val = SCGetParamValue (psc, thisaction, range++);

    if (Val && (Val->Type != SCSTRING) && (Val->Type != SCCHAR))
    {
        SCASSERTDEFINEDVALUE (Val);
        SCASSERTORVALUES (Val, SCPOINTER, SCLONG);
        pcom = (MXCom *)Val->Val;

        thisaction->Com = pcom;
        SCASSERTNOTNULL (pcom);
        SCASSERTNOTNULL (pcom->Client);
    }
    else range--;


    NbParam = thisaction->NbParam;
    if (NbParam >= range)
    {
        Val1 = SCGetParamValue (psc, thisaction, range++);
        SCASSERTDEFINEDVALUE (Val1);
        SCASSERTORVALUES (Val1, SCSTRING, SCCHAR);
    }
    if (NbParam >= range)
    {
        Val2 = SCGetParamValue (psc, thisaction, range++);
        SCASSERTDEFINEDVALUE (Val2);
        SCASSERTORVALUES (Val1, SCSTRING, SCCHAR);
    }
    if (Val2) FileLoc        = *(char*)Val2->Val;

    if (pcom)
        if (Val1)
            DGGetList (pcom,  thisaction->Identity, (char*)Val1->Val,  FileLoc);
        else
            DGGetList (pcom,  thisaction->Identity, NULL,  FileLoc);

    else
    {
        List* ListFiles = NULL;
        int NbFile = 0;
        char Buf[400];

        if (Val1)
            ListFiles = OSList ((char*)Val1->Val, FileLoc);
        else
            ListFiles = OSList ("", FileLoc);
        while (ListFiles)
        {
            FILEDATA* fileparam = (FILEDATA*)ListFiles->car;
            sprintf (Buf, "%d %s %s %d\n", NbFile, fileparam->Name, ((fileparam->Type == TYPEDIRECTORY) ? "<Dir>" : ((fileparam->Type == TYPELINK) ? "<Link>" : "")), fileparam->Size);
            SCTrace(psc, NbFile, Buf);
            free (ListFiles->car);
            ListRemove (&ListFiles, ListFiles->car);
            NbFile++;
        }


        SCAffectValue (thisaction->Value, (void *)NbFile, SCINTEGER);
        SCSetActionState (psc, thisaction, SCDONE, SCDEFINED, Error);
    }

    return thisaction->Value;
}


SCPROCRESULT SCDirProc  (SC* psc,  SCAction* thisaction)
{
    SCValue* Val = NULL;
    SCValue* Val1 = NULL;
    SCValue* Val2 = NULL;

    MXCom* pcom = psc->Com;
    WORD NbParam;
    char FileLoc;
    char* From = NULL;
    WORD range = 1;
    int Error= 0;
    BUFFERPARM Buffer;

    FileLoc = LOCDISQU;

    SCSETRETURNVALUETYPE(thisaction->Value, SCINTEGER);

    SCASSERTMINPARAM (thisaction, 0);
    SCASSERTMAXPARAM (thisaction, 3);

    Val = SCGetParamValue (psc, thisaction, range++);

    if (Val && (Val->Type != SCSTRING) && (Val->Type != SCCHAR))
    {
        SCASSERTDEFINEDVALUE (Val);
        SCASSERTORVALUES (Val, SCPOINTER, SCLONG);
        pcom = (MXCom *)Val->Val;

        thisaction->Com = pcom;
        SCASSERTNOTNULL (pcom);
        SCASSERTNOTNULL (pcom->Client);
    }
    else range--;


    NbParam = thisaction->NbParam;
    if (NbParam >= range)
    {
        Val1 = SCGetParamValue (psc, thisaction, range++);
        SCASSERTDEFINEDVALUE (Val1);
        SCASSERTORVALUES (Val1, SCSTRING, SCCHAR);
    }
    if (NbParam >= range)
    {
        Val2 = SCGetParamValue (psc, thisaction, range++);
        SCASSERTDEFINEDVALUE (Val2);
        SCASSERTORVALUES (Val1, SCSTRING, SCCHAR);
    }
    if (Val2) FileLoc        = *(char*)Val2->Val;

    if (pcom)
        if (Val1)
            DGDir (pcom,  thisaction->Identity, (char*)Val1->Val,  FileLoc);
        else
            DGDir (pcom,  thisaction->Identity, NULL,  FileLoc);
    else
    {
        char* Buf = 0;
        int NbFile = 0;

        if (Val1)
            Error = OSDir ((char*)Val1->Val, FileLoc, &Buffer);
        else
            Error = OSDir ("", FileLoc, &Buffer);
        if (Error)
        {
            Buf = strtok(Buffer.BufferContent, "\n\r");
            Buf[Buffer.BufferSize] = 0;
            while (Buf)
            {
                printf ("%d %s\n", NbFile, Buf);
                Buf = strtok (NULL, "\n\r");
                NbFile++;
            }
            free (Buffer.BufferContent);
        }
        SCAffectValue (thisaction->Value, (void *)Error, SCINTEGER);
        SCSetActionState (psc, thisaction, SCDONE, SCDEFINED, Error);
    }

    return thisaction->Value;
}




SCPROCRESULT SCPutProc   (SC* psc,  SCAction* thisaction)
{
    SCValue* Val = NULL;
    SCValue* Val1 = NULL;
    SCValue* Val2 = NULL;
    SCValue* Val3 = NULL;
    SCValue* Val4 = NULL;
    SCValue* Val5 = NULL;
    MXCom* pcom = psc->Com;
    WORD NbParam;
    char LocalFileLoc;
    char DistantFileLoc;
    char FileType;
    char* InTo = NULL;
    WORD range = 1;

    FileType = 'b';
    LocalFileLoc = DistantFileLoc = LOCDISQU;

    SCSETRETURNVALUETYPE(thisaction->Value, SCSTRING);

    SCASSERTMINPARAM (thisaction, 1);
    SCASSERTMAXPARAM (thisaction, 6);


    Val = SCGetParamValue (psc, thisaction, range++);
    SCASSERTDEFINEDVALUE (Val);
    SCASSERTORVALUES (Val, SCPOINTER, SCLONG);

    pcom = (MXCom *)Val->Val;
    SCASSERTMINPARAM (thisaction, 2);

    Val1 = SCGetParamValue (psc, thisaction, range++);
    SCASSERTDEFINEDVALUE (Val1);
    SCASSERTORVALUES (Val1, SCSTRING, SCCHAR);

    thisaction->Com = pcom;
    SCASSERTNOTNULL (pcom);
    SCASSERTNOTNULL (pcom->Client);

    NbParam = thisaction->NbParam;
    if (NbParam >= range)
    {
        Val2 = SCGetParamValue (psc, thisaction, range++);
        SCASSERTDEFINEDVALUE (Val2);
        SCASSERTORVALUES (Val2, SCCHAR, SCSTRING);
        if (Val2->Type == SCSTRING)
        {
            InTo = (char*)Val2->Val;
            if (NbParam >= range)
            {
                Val3 = SCGetParamValue (psc, thisaction, range++);
                SCASSERTDEFINEDVALUE (Val3);
            }
        }
        else
        {
            SCASSERTVALUESIZE(Val2, 2);
            Val3 = Val2;
        }
    }

    if (NbParam >= range)
    {
        Val4 = SCGetParamValue (psc, thisaction, range++);
        SCASSERTDEFINEDVALUE (Val4);
    }

    if (NbParam >= range)
    {
        Val5 = SCGetParamValue (psc, thisaction, range++);
        SCASSERTDEFINEDVALUE (Val5);
    }


    if (Val3) FileType       = *(char*)Val3->Val;
    if (Val4) LocalFileLoc   = *(char*)Val4->Val;
    if (Val5) DistantFileLoc = *(char*)Val5->Val;

    if (pcom->Protocol == IPPROTO_FTP)
    {
        if (FTPSendFile (pcom, thisaction->Identity, (char*)Val1->Val, InTo, FileType, NULL, 0) < 0)
        {
            int Error = -1;
            SCPutValue (thisaction->Value, "File do not exists !!" , SCSTRING);
            SCSetActionState (psc, thisaction, SCDONE, SCDEFINED, Error);
        }
    }
    else
        DGSendFile (pcom,  thisaction->Identity, (char*)Val1->Val, InTo, FileType, LocalFileLoc, DistantFileLoc);


    return thisaction->Value;
}

/*------------------------------------------------------------------------*/


SCPROCRESULT SCMputProc  (SC* psc,  SCAction* thisaction)
{
    SCValue* Val = NULL;
    SCValue* Val1 = NULL;
    SCValue* Val2 = NULL;
    SCValue* Val3 = NULL;
    SCValue* Val4 = NULL;
    SCValue* Val5 = NULL;
    MXCom* pcom = psc->Com;
    WORD NbParam;
    char LocalFileLoc;
    char DistantFileLoc;
    char FileType;
    char* InTo = NULL;
    WORD range = 1;

    FileType = 'b';
    LocalFileLoc = DistantFileLoc = LOCDISQU;

    SCSETRETURNVALUETYPE(thisaction->Value, SCSTRING);

    SCASSERTMINPARAM (thisaction, 1);
    SCASSERTMAXPARAM (thisaction, 6);


    Val = SCGetParamValue (psc, thisaction, range++);
    SCASSERTDEFINEDVALUE (Val);
    SCASSERTORVALUES (Val, SCPOINTER, SCLONG);

    pcom = (MXCom *)Val->Val;
    SCASSERTMINPARAM (thisaction, 2);
    Val1 = SCGetParamValue (psc, thisaction, range++);
    SCASSERTDEFINEDVALUE (Val1);
    SCASSERTORVALUES (Val1, SCSTRING, SCCHAR);

    thisaction->Com = pcom;
    SCASSERTNOTNULL (pcom);
    SCASSERTNOTNULL (pcom->Client);

    NbParam = thisaction->NbParam;
    if (NbParam >= range)
    {
        Val2 = SCGetParamValue (psc, thisaction, range++);
        SCASSERTDEFINEDVALUE (Val2);
        SCASSERTORVALUES (Val2, SCCHAR, SCSTRING);
        if (Val2->Type == SCSTRING)
        {
            InTo = (char*)Val2->Val;
            if (NbParam >= range)
            {
                Val3 = SCGetParamValue (psc, thisaction, range++);
                SCASSERTDEFINEDVALUE (Val3);
            }

        }
        else
        {
            SCASSERTVALUESIZE(Val2, 2);
            Val3 = Val2;
        }
    }

    if (NbParam >= range)
    {
        Val4 = SCGetParamValue (psc, thisaction, range++);
        SCASSERTDEFINEDVALUE (Val4);
    }

    if (NbParam >= range)
    {
        Val5 = SCGetParamValue (psc, thisaction, range++);
        SCASSERTDEFINEDVALUE (Val5);
    }

    if (Val3) FileType       = *(char*)Val3->Val;
    if (Val4) LocalFileLoc   = *(char*)Val4->Val;
    if (Val5) DistantFileLoc = *(char*)Val5->Val;



    DGSendFiles (pcom,  thisaction->Identity, (char*)Val1->Val, InTo, FileType, LocalFileLoc, DistantFileLoc);

    return thisaction->Value;

}

/*------------------------------------------------------------------------*/


SCPROCRESULT SCGetProc   (SC* psc,  SCAction* thisaction)
{
    SCValue* Val = NULL;
    SCValue* Val1 = NULL;
    SCValue* Val2 = NULL;
    SCValue* Val3 = NULL;
    SCValue* Val4 = NULL;
    SCValue* Val5 = NULL;
    MXCom* pcom = psc->Com;
    WORD NbParam;
    char LocalFileLoc;
    char DistantFileLoc;
    char FileType;
    char* From = NULL;
    WORD range = 1;


    FileType = 'b';
    LocalFileLoc = DistantFileLoc = LOCDISQU;

    SCSETRETURNVALUETYPE(thisaction->Value, SCSTRING);

    SCASSERTMINPARAM (thisaction, 1);
    SCASSERTMAXPARAM (thisaction, 6);


    Val = SCGetParamValue (psc, thisaction, range++);
    SCASSERTDEFINEDVALUE (Val);
    SCASSERTORVALUES (Val, SCPOINTER, SCLONG);

    pcom = (MXCom *)Val->Val;
    SCASSERTMINPARAM (thisaction, 2);
    Val1 = SCGetParamValue (psc, thisaction, range++);
    SCASSERTDEFINEDVALUE (Val1);
    SCASSERTORVALUES (Val1, SCSTRING, SCCHAR);

    thisaction->Com = pcom;
    SCASSERTNOTNULL (pcom);
    SCASSERTNOTNULL (pcom->Client);

    NbParam = thisaction->NbParam;
    if (NbParam >= range)
    {
        Val2 = SCGetParamValue (psc, thisaction, range++);
        SCASSERTDEFINEDVALUE (Val2);
        SCASSERTORVALUES (Val2, SCCHAR, SCSTRING);
        if (Val2->Type == SCSTRING)
        {
            From = (char*)Val2->Val;
            if (NbParam >= range)
            {
                Val3 = SCGetParamValue (psc, thisaction, range++);
                SCASSERTDEFINEDVALUE (Val3);
            }

        }
        else
        {
            SCASSERTVALUESIZE(Val2, 2);
            Val3 = Val2;
        }
    }

    if (NbParam >= range)
    {
        Val4 = SCGetParamValue (psc, thisaction, range++);
        SCASSERTDEFINEDVALUE (Val4);
    }

    if (NbParam >= range)
    {
        Val5 = SCGetParamValue (psc, thisaction, range++);
        SCASSERTDEFINEDVALUE (Val5);
    }

    if (Val3) FileType       = *(char*)Val3->Val;
    if (Val4) LocalFileLoc   = *(char*)Val4->Val;
    if (Val5) DistantFileLoc = *(char*)Val5->Val;


    if (pcom->Protocol == IPPROTO_FTP)
        FTPGetFile (pcom, thisaction->Identity, (char*)Val1->Val, From, FileType, NULL, 0);
    else
        DGGetFile (pcom,  thisaction->Identity, (char *)Val1->Val, From, FileType, LocalFileLoc, DistantFileLoc);

    return thisaction->Value;
}

/*------------------------------------------------------------------------*/


SCPROCRESULT SCMgetProc  (SC* psc,  SCAction* thisaction)
{
    SCValue* Val = NULL;
    SCValue* Val1 = NULL;
    SCValue* Val2 = NULL;
    SCValue* Val3 = NULL;
    SCValue* Val4 = NULL;
    SCValue* Val5 = NULL;
    MXCom* pcom = psc->Com;
    WORD NbParam;
    char LocalFileLoc;
    char DistantFileLoc;
    char FileType;
    char* From = NULL;
    WORD range = 1;

    FileType = 'b';
    LocalFileLoc = DistantFileLoc = LOCDISQU;

    SCSETRETURNVALUETYPE(thisaction->Value, SCSTRING);

    SCASSERTMINPARAM (thisaction, 1);
    SCASSERTMAXPARAM (thisaction, 6);


    Val = SCGetParamValue (psc, thisaction, range++);
    SCASSERTDEFINEDVALUE (Val);
    SCASSERTORVALUES (Val, SCPOINTER, SCLONG);

    pcom = (MXCom *)Val->Val;
    SCASSERTMINPARAM (thisaction, 2);
    Val1 = SCGetParamValue (psc, thisaction, range++);
    SCASSERTDEFINEDVALUE (Val1);
    SCASSERTORVALUES (Val1, SCSTRING, SCCHAR);

    thisaction->Com = pcom;
    SCASSERTNOTNULL (pcom);
    SCASSERTNOTNULL (pcom->Client);

    NbParam = thisaction->NbParam;
    if (NbParam >= range)
    {
        Val2 = SCGetParamValue (psc, thisaction, range++);
        SCASSERTDEFINEDVALUE (Val2);
        SCASSERTORVALUES (Val2, SCCHAR, SCSTRING);
        if (Val2->Type == SCSTRING)
        {
            From = (char*)Val2->Val;
            if (NbParam >= range)
            {
                Val3 = SCGetParamValue (psc, thisaction, range++);
                SCASSERTDEFINEDVALUE (Val3);
            }
        }
        else
        {
            SCASSERTVALUESIZE(Val2, 2);
            Val3 = Val2;
        }
    }

    if (NbParam >= range)
    {
        Val4 = SCGetParamValue (psc, thisaction, range++);
        SCASSERTDEFINEDVALUE (Val4);
    }

    if (NbParam >= range)
    {
        Val5 = SCGetParamValue (psc, thisaction, range++);
        SCASSERTDEFINEDVALUE (Val5);
    }

    if (Val3) FileType       = *(char*)Val3->Val;
    if (Val4) LocalFileLoc   = *(char*)Val4->Val;
    if (Val5) DistantFileLoc = *(char*)Val5->Val;


    DGGetFiles (pcom,  thisaction->Identity, (char*)Val1->Val, From, FileType, LocalFileLoc, DistantFileLoc);

    return thisaction->Value;
}



/*------------------------------------------------------------------------*/
/* REMOTE                                                                 */
/*------------------------------------------------------------------------*/


SCPROCRESULT SCRemoteProc    (SC* psc,  SCAction* thisaction)
{
    SCValue* Val   = NULL;
    SCValue* Val1   = NULL;
    SCValue* Value   = NULL;
    SCEntity* Entity = NULL;
    SCEntity* Entity1 = NULL;
    WORD NbParam   = 0;
    MXCom* pcom    = psc->Com;
    WORD range     = 1;
    char *buf      = NULL;

    NbParam = thisaction->NbParam;


    SCSETRETURNVALUETYPE(thisaction->Value, SCPOINTER);

    SCASSERTMINPARAM (thisaction, 1);
    SCASSERTMAXPARAM (thisaction, 2);

    NbParam = thisaction->NbParam;
    if(NbParam > 1)
    {
        Val = SCGetParamValue (psc, thisaction, range++);

        SCASSERTDEFINEDVALUE (Val);

        SCASSERTORVALUES (Val, SCPOINTER, SCLONG);

        pcom = (MXCom *)Val->Val;
    }

    thisaction->Com = pcom;
    SCASSERTNOTNULL (pcom);
    SCASSERTNOTNULL (pcom->Client);

    Entity = SCGetParam (psc, thisaction , range++);
    Value = (SCValue*)Entity->Val;
    SCASSERTVALUE (Value, SCSTRING);


    SCSendString (pcom,  thisaction->Identity, Entity->RootAction->Persistency, (char*)Value->Val);

    return thisaction->Value;


}

/*------------------------------------------------------------------------*/

/*------------------------------------------------------------------------*/
/* A REVOIR */
/*------------------------------------------------------------------------*/


SCPROCRESULT SCShareProc    (SC* psc,  SCAction* thisaction)
{
    SCValue* Val   = NULL;
    SCEntity* Entity;
    WORD NbParam   = 0;
    MXCom* pcom    = psc->Com;
    WORD range     = 1;

    SCSETRETURNVALUETYPE(thisaction->Value, SCPOINTER);

    SCASSERTMINPARAM (thisaction, 1);
    SCASSERTMAXPARAM (thisaction, 2);

    NbParam = thisaction->NbParam;
    if(NbParam > 1)
    {
        Val = SCGetParamValue (psc, thisaction, range++);
        SCASSERTDEFINEDVALUE (Val);
        SCASSERTORVALUES (Val, SCPOINTER, SCLONG);
        pcom = (MXCom *)Val->Val;
    }

    thisaction->Com = pcom;
    SCASSERTNOTNULL (pcom);
    SCASSERTNOTNULL (pcom->Client);

    Entity = SCGetParam (psc, thisaction, range++);

    SCASSERTPARAMTYPE(Entity, SCVARIABLE);

    SCShareVariable (pcom, (char*)((SCVariable*)Entity->Val)->Name, thisaction->Identity);

    return thisaction->Value;
}



SCPROCRESULT SCSendMailProc   (SC* psc,  SCAction* thisaction)
{
    SCValue* Val = NULL;
    SCValue* Val1 = NULL;
    SCValue* Val2 = NULL;
    SCValue* Val3 = NULL;
    SCValue* Val4 = NULL;
    SCValue* Val5 = NULL;
    SCValue* Val6 = NULL;
    SCValue* Val7 = NULL;
    EMailMessage* Message = NULL;
    int Error = 0;
    WORD NbParam   = 0;

    WORD range = 1;


    SCSETRETURNVALUETYPE(thisaction->Value, SCINTEGER);

    SCASSERTMINPARAM (thisaction, 7);
    SCASSERTMAXPARAM (thisaction, 8);


    Val = SCGetParamValue (psc, thisaction, range++);
    SCASSERTDEFINEDVALUE (Val);
    SCASSERTORVALUES (Val, SCSTRING, SCCHAR);

    Val1 = SCGetParamValue (psc, thisaction, range++);
    SCASSERTDEFINEDVALUE (Val1);
    SCASSERTORVALUES (Val1, SCCHAR, SCSTRING);

    Val2 = SCGetParamValue (psc, thisaction, range++);
    SCASSERTDEFINEDVALUE (Val2);
    SCASSERTORVALUES (Val2, SCCHAR,SCSTRING);

    Val3 = SCGetParamValue (psc, thisaction, range++);
    SCASSERTDEFINEDVALUE (Val3);
    SCASSERTORVALUES (Val3, SCCHAR,SCSTRING);

    Val4 = SCGetParamValue (psc, thisaction, range++);
    SCASSERTDEFINEDVALUE (Val4);
    SCASSERTORVALUES (Val4, SCCHAR,SCSTRING);

    Val5 = SCGetParamValue (psc, thisaction, range++);
    SCASSERTDEFINEDVALUE (Val5);
    SCASSERTORVALUES (Val5, SCCHAR,SCSTRING);

    Val6 = SCGetParamValue (psc, thisaction, range++);
    SCASSERTDEFINEDVALUE (Val6);
    SCASSERTORVALUES (Val6, SCCHAR,SCSTRING);



    Message = EMailCreateMessage  ((char*)Val1->Val, (char*)Val2->Val, (char*)Val3->Val, (char*)Val4->Val, (char*)Val5->Val, (char*)Val6->Val);
    if (Message)
    {
        NbParam = thisaction->NbParam;

        if (NbParam > 7)
        {
            Val7 = SCGetParamValue (psc, thisaction, range++);
            SCASSERTDEFINEDVALUE (Val7);
            SCASSERTVALUE (Val7, SCSTRING);
            EMailAddMimePart(Message, (char*)Val7->Val, APPLICATION_OCTETSTREAM,"" ,BASE64, FALSE );
        }

        if (SendEmail (psc->MX->TP, (char*)Val->Val, Message) < 0)
            Error = 1;
        EMailFreeMessage (Message);
    }
    else Error = 1;

    SCAffectValue (thisaction->Value, (void *)Error, SCINTEGER);
    SCSetActionState (psc, thisaction, SCDONE, SCDEFINED, Error);

    return thisaction->Value;
}



/*-----------------------------------------------------------------------------------*/
SCPROCRESULT  SCGetNameProc   (SC* psc, SCAction* thisaction)
{
    SCValue* Val1 = NULL;
    int Error = 0;
    FILEPARM* FilePar;

    SCSETRETURNVALUETYPE(thisaction->Value, SCSTRING);

    SCASSERTPARAMNUMBER (thisaction, 1);

    Val1 = SCGetParamValue (psc, thisaction, 1);
    SCASSERTDEFINEDVALUE (Val1);
    FilePar = (FILEPARM *)Val1->Val;;
    SCPutValue (thisaction->Value, (char *)FilePar->FileOri, SCSTRING);
    SCSetActionState (psc, thisaction, SCDONE, SCDEFINED, Error);
    return thisaction->Value;
}


SCPROCRESULT  SCGetFromNameProc  (SC* psc, SCAction* thisaction)
{
    SCValue* Val1 = NULL;
    int Error = 0;
    FILEPARM* FilePar;

    SCSETRETURNVALUETYPE(thisaction->Value, SCSTRING);

    SCASSERTPARAMNUMBER (thisaction, 1);

    Val1 = SCGetParamValue (psc, thisaction, 1);
    SCASSERTDEFINEDVALUE (Val1);
    FilePar = (FILEPARM *)Val1->Val;;

    SCPutValue (thisaction->Value, (char *)FilePar->FileDest, SCSTRING);
    SCSetActionState (psc, thisaction, SCDONE, SCDEFINED, Error);
    return thisaction->Value;
}



SCPROCRESULT  SCGetTypeProc   (SC* psc, SCAction* thisaction)
{
    SCValue* Val1 = NULL;
    int Error = 0;
    FILEPARM* FilePar;
    char Type[2];

    SCSETRETURNVALUETYPE(thisaction->Value, SCCHAR);

    SCASSERTPARAMNUMBER (thisaction, 1);

    Val1 = SCGetParamValue (psc, thisaction, 1);
    SCASSERTDEFINEDVALUE (Val1);
    FilePar = (FILEPARM *)Val1->Val;
    Type[0] = FilePar->FileType;
    Type[1] = 0;
    SCPutValue (thisaction->Value, (void *)Type, SCCHAR);
    SCSetActionState (psc, thisaction, SCDONE, SCDEFINED, Error);
    return thisaction->Value;
}

SCPROCRESULT  SCGetSizeProc   (SC* psc, SCAction* thisaction)
{

    SCValue* Val1 = NULL;
    int Error = 0;
    FILEPARM* FilePar;

    SCSETRETURNVALUETYPE(thisaction->Value, SCLONG);

    SCASSERTPARAMNUMBER (thisaction, 1);

    Val1 = SCGetParamValue (psc, thisaction, 1);
    SCASSERTDEFINEDVALUE (Val1);
    FilePar = (FILEPARM *)Val1->Val;;
    SCAffectValue (thisaction->Value,(void *) FilePar->FileSize, SCLONG);
    SCSetActionState (psc, thisaction, SCDONE, SCDEFINED, Error);
    return thisaction->Value;

}

SCPROCRESULT  SCGetErrorProc  (SC* psc, SCAction* thisaction)
{

    SCValue* Val1 = NULL;
    int Error = 0;
    FILEPARM* FilePar;

    SCSETRETURNVALUETYPE(thisaction->Value, SCLONG);

    SCASSERTPARAMNUMBER (thisaction, 1);

    Val1 = SCGetParamValue (psc, thisaction, 1);
    SCASSERTDEFINEDVALUE (Val1);
    FilePar = (FILEPARM *)Val1->Val;;
    SCPutValue (thisaction->Value, (void *)FilePar->FileError, SCLONG);
    SCSetActionState (psc, thisaction, SCDONE, SCDEFINED, Error);
    return thisaction->Value;
}


SCPROCRESULT  SCSetNameProc   (SC* psc, SCAction* thisaction)
{
    SCValue* Val1 = NULL;
    SCValue* Val2 = NULL;
    int Error = 0;
    FILEPARM* FilePar;

    SCSETRETURNVALUETYPE(thisaction->Value, SCSTRING);

    SCASSERTPARAMNUMBER (thisaction, 2);

    Val1 = SCGetParamValue (psc, thisaction, 1);
    SCASSERTDEFINEDVALUE (Val1);
    Val2 = SCGetParamValue (psc, thisaction, 2);
    SCASSERTDEFINEDVALUE (Val2);
    SCASSERTORVALUES (Val2, SCCHAR,SCSTRING);

    FilePar = (FILEPARM *)Val1->Val;
    strcpy (FilePar->FileOri, Val2->Val);
    SCPutValue (thisaction->Value, (char *)Val2->Val, SCSTRING);
    SCSetActionState (psc, thisaction, SCDONE, SCDEFINED, Error);
    return thisaction->Value;
}


SCPROCRESULT  SCSetFromNameProc  (SC* psc, SCAction* thisaction)
{
    SCValue* Val1 = NULL;
    SCValue* Val2 = NULL;
    int Error = 0;
    FILEPARM* FilePar;

    SCSETRETURNVALUETYPE(thisaction->Value, SCSTRING);

    SCASSERTPARAMNUMBER (thisaction, 2);

    Val1 = SCGetParamValue (psc, thisaction, 1);
    SCASSERTDEFINEDVALUE (Val1);
    Val2 = SCGetParamValue (psc, thisaction, 2);
    SCASSERTDEFINEDVALUE (Val2);
    SCASSERTVALUE (Val2, SCSTRING);

    FilePar = (FILEPARM *)Val1->Val;
    strcpy (FilePar->FileDest, Val2->Val);
    SCPutValue (thisaction->Value, (char *)Val2->Val, SCSTRING);
    SCSetActionState (psc, thisaction, SCDONE, SCDEFINED, Error);
    return thisaction->Value;
}

SCPROCRESULT  SCSetTypeProc   (SC* psc, SCAction* thisaction)
{
    SCValue* Val1 = NULL;
    SCValue* Val2 = NULL;
    int Error = 0;
    FILEPARM* FilePar;

    SCSETRETURNVALUETYPE(thisaction->Value, SCCHAR);

    SCASSERTPARAMNUMBER (thisaction, 2);

    Val1 = SCGetParamValue (psc, thisaction, 1);
    SCASSERTDEFINEDVALUE (Val1);
    Val2 = SCGetParamValue (psc, thisaction, 2);
    SCASSERTDEFINEDVALUE (Val2);
    SCASSERTVALUE (Val2, SCCHAR);

    FilePar = (FILEPARM *)Val1->Val;
    FilePar->FileType = *(char *)Val2->Val;

    SCPutValue (thisaction->Value, (void *)Val2->Val, SCCHAR);

    SCSetActionState (psc, thisaction, SCDONE, SCDEFINED, Error);
    return thisaction->Value;
}



SCPROCRESULT SCGetBufferProc   (SC* psc,  SCAction* thisaction)
{
    return thisaction->Value;
}
SCPROCRESULT SCGetBTypeProc    (SC* psc,  SCAction* thisaction)
{
    return thisaction->Value;
}
SCPROCRESULT SCGetBSizeProc    (SC* psc,  SCAction* thisaction)
{
    return thisaction->Value;
}
SCPROCRESULT SCSetBufferProc   (SC* psc,  SCAction* thisaction)
{
    return thisaction->Value;
}
SCPROCRESULT SCSetBTypeProc    (SC* psc,  SCAction* thisaction)
{
    return thisaction->Value;
}
SCPROCRESULT SCSetBSizeProc    (SC* psc,  SCAction* thisaction)
{
    return thisaction->Value;
}

