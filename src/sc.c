/* NetProg is an API in C language allowing an homogeneous programming of  */
/* communicating applications                                              */
/* Copyright (C) 2002,2003,2004,2005                                       */
/* Gabriel Jureidini                                                       */
/* Version 2.0 - 17 July 2005                                              */
/*-------------------------------------------------------------------------*/


#define _CRT_SECURE_NO_WARNINGS
#include "sc.h"

BOOL   SCDEBUG     = FALSE;
BOOL   SCSTEP      = FALSE;
BOOL   SCINSTEP    = FALSE;
BOOL   SCCONTINUE  = FALSE;
MXCom* SCDEBUGCOM  = NULL;

SC* PSC;


extern OSPrintProcedure GlbPrintProcedureStruct;


int SCInit (SC* psc, MX* pmx, BOOL Interactive, char* filename)
{
    SCVariable* var_this;

    strcpy (psc->Version, "V2.00");
    strcpy (psc->CurrentFileName, "");
    psc->ListEntities       = NULL;
    psc->ListActions        = NULL;
    psc->ListWaitingActions = NULL;
    psc->ListDoingActions   = NULL;
    psc->TraceProcedure     = NULL;
    psc->ApplicationField   = NULL;
    psc->EndingProcedure    = NULL;

    psc->ListVariables      = NULL;
    psc->ListSystemActions  = NULL;
	psc->ListAddedActions   = NULL;
	psc->ListComFunctions     = NULL;
    psc->ListClientFunctions   = NULL;
    psc->LastActionIdentity = 0;
    psc->InteractiveSection = NULL;
    psc->NbSections         = 0;
    psc->ListSections       = NULL;
    psc->CurrentSection     = NULL;
    psc->Com                = NULL;
    psc->Clients            = NULL;
    psc->Communications     = NULL;
    psc->BreakPoints        = NULL;
    psc->LastError          = 0;
    if (GlbPrintProcedureStruct.PrintProcedure == NULL)
        OSSetPrintProcedure(SCDefaultPrint, NULL);
    SCSetTraceProcedure (psc, SCDefaultTrace, NULL);
    if (!pmx)
    {
        pmx = (MX*)malloc (sizeof (MX));
        MXInit (pmx, MXSERVER, NULL, NULL, 0, NULL);
    }

    MXAddGeneralConnectCallBack (pmx, MXONCONNECT, SCOnConnect, psc);
    MXAddApplicationProcedure (pmx, SCEngine, psc);
    psc->MX                 = pmx;


    PSC = psc;

    SCAddSystemActions (psc);
    var_this = SCCreateVariable (psc, "this");
    if (var_this) ListNewr(&psc->ListVariables, var_this);

    if (filename)
        SCInsertAction (psc,  "PARSE", 1, SCSTRING, filename);

    if (Interactive)
        SCCreateInteractiveSection (psc);
    SCAddScriptCommands (psc);
    return(0);
}

/*------------------------------------------------------------------------*/


void SCClear (SC* psc)
{
    SCSection*     pSection;
    SCEntity *pEntity;
	SCVariable*    pVariable;
    SCComFuncStruct*  pFuncStruct;
    SCVariable* var_this;
    int Error  = 0;

    while (psc->ListSections)
    {
        pSection = (SCSection *)psc->ListSections->car;
        ListRemove(&psc->ListSections, pSection);
        SCFreeSection(pSection);
    }
    while (psc->ListEntities)
    {
        pEntity = (SCEntity *)psc->ListEntities->car;
        ListRemove(&psc->ListEntities, pEntity);
        SCFreeEntity(pEntity);
    }

    while (psc->ListVariables)
    {
        pVariable = (SCVariable *)psc->ListVariables->car;
        ListRemove(&psc->ListVariables, pVariable);
        SCFreeVariable(pVariable);
    }


    var_this = SCCreateVariable (psc, "this");
    if (var_this) ListNewr(&psc->ListVariables, var_this);


    while (psc->ListComFunctions)
    {
        pFuncStruct = (SCComFuncStruct *)psc->ListComFunctions->car;
        ListRemove(&psc->ListComFunctions, pFuncStruct);
/* A REVOIR */
        if ((pFuncStruct->Type == MXONRECV) && (psc->MX))
            MXRemoveGeneralCallBack (psc->MX, pFuncStruct->ClassName, pFuncStruct->MessageClassName, pFuncStruct->Type, SCReceiveMessage,pFuncStruct);
        free (pFuncStruct);
    }

}


/*------------------------------------------------------------------------*/

void SCEnd (SC* psc)
{
    SCSysAction*   pSysAction;
    SCSection*     pSection;
    SCVariable*    pVariable;
    SCComFuncStruct*  pFuncStruct;
    MXCom* pcom;
    int Error = 0;

    if (psc->MX)
    {
        MXRemoveGeneralConnectCallBack (psc->MX, MXONCONNECT, SCOnConnect, psc);
        MXRemoveApplicationProcedure (psc->MX, SCEngine, psc);
    }

	while (psc->ListAddedActions)
    {
        pSysAction = (SCSysAction *)psc->ListAddedActions->car;
        ListRemove (&psc->ListAddedActions, pSysAction);
        SCFreeSysAction (pSysAction);
    }
    while (psc->ListSystemActions)
    {
        pSysAction = (SCSysAction *)psc->ListSystemActions->car;
        ListRemove (&psc->ListSystemActions, pSysAction);
        SCFreeSysAction (pSysAction);
    }

    while (psc->Communications)
    {
        pcom = (MXCom *)psc->Communications->car;
        ListRemove (&psc->Communications, pcom);
        MXCloseCom (psc->MX, pcom);
    }

    while (psc->ListSections)
    {
        pSection = (SCSection *)psc->ListSections->car;
        ListRemove(&psc->ListSections, pSection);
        SCFreeSection(pSection);
    }
    while (psc->ListVariables)
    {
        pVariable = (SCVariable *)psc->ListVariables->car;
        ListRemove(&psc->ListVariables, pVariable);
        SCFreeVariable(pVariable);
    }

    while (psc->ListComFunctions)
    {
        pFuncStruct = (SCComFuncStruct *)psc->ListComFunctions->car;
        ListRemove(&psc->ListComFunctions, pFuncStruct);
        if ((pFuncStruct->Type == MXONRECV) && (psc->MX))
            MXRemoveGeneralCallBack (psc->MX, pFuncStruct->ClassName, pFuncStruct->MessageClassName, pFuncStruct->Type, SCReceiveMessage, pFuncStruct);
        free (pFuncStruct);
    }

    /*
      MXRemoveConnectionProcedure (psc->MX, SCOnConnect, psc);
      MXRemoveApplicationProcedure (psc->MX, SCEngine, psc);

       
       MXEnd (psc->MX);
      if (psc->DB) DBEnd (psc->DB);
     */
    return;
}

/*------------------------------------------------------------------------*/

void SCError (SC* psc, int id, char* charparam, ...)
{
    char Name[9];

    psc->LastError = id;
    sprintf(Name, "TESC%04d", id);
    OSError (Name, charparam);
}

void SCSetUserField (SCAction* paction, void* userfield)
{
	paction->UserField = userfield;
}
/*------------------------------------------------------------------------*/
/* all actions are SCNOTDEFINED */

void SCSetEntityState (SC* psc, SCEntity* Entity, BYTE State)
{
    List* ListParams;
    SCAction* paction;
    SCEntity* pentity;
    WORD i;

    if (!Entity) return;
    if (Entity->Type == SCOPERATOR)
    {

        paction = (SCAction*)Entity->Val;
        if (paction->State != SCNOTTODO)
            paction->State = State;
        paction->Value->Defined = SCNOTDEFINED;

        ListParams     = paction->ListParams;

        /*free actions with expanded action values */
        if (State == SCTODO)
            for (i = paction->NbParam + 1; i <= ListNbElt (ListParams); i++)
            {
                pentity = SCGetParam (psc, paction, i);
                ListRemove (&paction->ListParams, pentity);
                SCFreeEntity (pentity);
            }

        while (ListParams)
        {
            Entity = (SCEntity *)ListParams->car;
            SCSetEntityState (psc, Entity, State);
            ListParams = ListParams->cdr;
        }
    }
}

/*------------------------------------------------------------------------*/

SCEntity* SCCopyEntity (SC* psc, SCAction* RootAction, SCEntity* Entity)
{
    SCEntity*    pEntity;
    SCAction*    paction;
    if (!Entity) return NULL;

    pEntity = (SCEntity *)malloc(sizeof(SCEntity));
    if (!pEntity) return(NULL);

    pEntity->Type       = Entity->Type;
    pEntity->Val        = Entity->Val;
    pEntity->RootAction = RootAction;


    /* if value of Entity is of type variable nothing to do*/

    if (Entity->Type == SCOPERATOR)
    {
        paction = SCCopyAction(psc, RootAction, (SCAction*)Entity->Val);
        pEntity->Val = paction;
    }

    if (Entity->Type == SCATOM)
    {
        pEntity->Val = SCAllocValue (psc, NULL, SCPOINTER);
        SCCopyValue (psc, (SCValue *)pEntity->Val, (SCValue*)Entity->Val);
    }

    return(pEntity);
}

SCEntity* SCAllocEntity (SC* psc, void* Val, SHORT Type)
{
    SCEntity*    pentity;

    pentity = (SCEntity *)malloc(sizeof(SCEntity));
    if (!pentity) return(NULL);

    pentity->Val        =   NULL;
    pentity->RootAction =   NULL;
    pentity->FatherAction = NULL;



    switch(Type)
    {
    case SCOPERATOR :
        pentity->Type = Type;
        pentity->Val = (SCAction *)Val;

        break;
    case SCVARIABLE_BYNAME :
        pentity->Type = SCVARIABLE;
        pentity->Val = SCCreateVariable(psc, (char*)Val);
        if(!pentity->Val)
        {
            free (pentity);
            return(NULL);
        }
        break;
    case SCVARIABLE_BYVAL :
        pentity->Type = SCVARIABLE;
        pentity->Val = (SCVariable*)Val;
        if(!pentity->Val)
        {
            free (pentity);
            return(NULL);
        }
        break;
    default:
        pentity->Type = SCATOM;
        if (Val)
        {
            pentity->Val = SCAllocValue (psc, Val, Type);
            ((SCValue *)pentity->Val)->Defined = SCDEFINED; /* atom is always defined */
        }
        if(!pentity->Val)
        {
            free (pentity);
            return(NULL);
        }
        break;
    }

    return(pentity);
}

void SCFreeEntity (SCEntity *pEntity)
{

    if (!pEntity) return;

    switch(pEntity->Type)
    {
    case SCOPERATOR :
        SCFreeAction((SCAction*)pEntity->Val);
        break;
    case SCATOM :
    case SCVARIABLE :
        break;
    default:
        break;
    }
    free(pEntity);
    return;
}

/*------------------------------------------------------------------------*/

List* SCFindDebListEntity(SC* psc, SCEntity* pentity)
{
    List* listentities;
    SCEntity* pentity_temp;

    if (!pentity)     return(NULL);

    listentities = psc->ListEntities;

    while(listentities)
    {
        pentity_temp = (SCEntity*)listentities->car;
        if (pentity_temp == pentity) return listentities;
        listentities = listentities->cdr;
    }

    return(NULL);
}
/*------------------------------------------------------------------------*/

void SCCopyFromListEntity (SC* psc, SCAction* thisaction, List* listentities)
{
    SCAction* lastnoblockaction = NULL;
    SCAction* lastblockaction   = NULL;
    SCEntity* pentity_temp;
    SCAction* paction;

    (thisaction->Block) ? (lastblockaction = thisaction) : (lastnoblockaction = thisaction);

    while (listentities)
    {
        pentity_temp = (SCEntity*)listentities->car;
        if (pentity_temp->Type == SCOPERATOR)
        {
            paction = (SCAction *)pentity_temp->Val;
            paction = SCPrepareToExecute (psc, paction, ((paction->Block) ? lastblockaction : lastnoblockaction));
            (paction->Block) ? (lastblockaction = paction) : (lastnoblockaction = paction);
        }
        listentities = listentities->cdr;
    }

}

/*------------------------------------------------------------------------*/

SCAction* SCPrepareToExecute (SC* psc, SCAction* paction, SCAction* curraction)
{
    if (!paction) return NULL;

    if (paction->Block)
        if ((!psc->ListActions) || (!curraction))
            ListNewr (&psc->ListActions, (paction = SCCopyAction (psc, NULL, paction)));
        else
            ListInsertAfter (&psc->ListActions, (paction = SCCopyAction (psc, NULL, paction)) , curraction);

    else
        if ((!psc->ListWaitingActions) || (!curraction))
            ListNewr (&psc->ListWaitingActions, (paction = SCCopyAction (psc, NULL, paction)));
        else  ListInsertAfter (&psc->ListWaitingActions, (paction = SCCopyAction (psc, NULL, paction)) , curraction);
    return paction;
}

/*------------------------------------------------------------------------*/

SCAction* SCGetNextActionToExecute (SC* psc, BYTE WaitingActions)
{
    List* plistaction;
    SCAction* paction;
    List* plistwaitaction;

    plistaction           = psc->ListActions;
    plistwaitaction       = psc->ListWaitingActions;

    if ((WaitingActions == SCWAITINGACTIONS) ||
            (WaitingActions == SCALLACTIONS))

        while (plistwaitaction)   /* non blocking actions*/
        {
            paction = plistwaitaction->car;
            plistwaitaction = plistwaitaction->cdr;

            if (paction->State ==  SCNOTTODO)  paction->State = SCTODO;

            if (SCActionDone (paction))
            {
                if (!paction->Persistency) /* persistent actions can never be bloqued */
                {
                    SCDeleteAction (psc, paction);
                }
            }
            else
                if (paction->State == SCTODO)     return paction;

        }

    if ((WaitingActions == SCLISTACTIONS) ||
            (WaitingActions == SCALLACTIONS))

        while (plistaction)   /* non blocking actions*/
        {
            paction = (SCAction*)plistaction->car;
            plistaction = plistaction->cdr;

            if (paction->State ==  SCNOTTODO)  paction->State = SCTODO;

            if (!paction->Block)
            {
                ListRemove (&psc->ListActions, paction);
                ListNewr (&psc->ListWaitingActions, paction);
            }
            else
            {
                if (paction->State == SCDOING) return NULL;

                if (paction->State == SCTODO)  return paction;
                if (SCActionDone (paction))
                {
                    SCDeleteAction (psc, paction);
                }
            }
        }
    return NULL;
}

/*------------------------------------------------------------------------*/

int SCEngine (MX* pmx, void* par)
{
    SCAction* paction;
    SC* psc = (SC*)par;
    SCValue*  Value;
    SCEntity* pentity;

    paction = SCGetNextActionToExecute (psc, SCWAITINGACTIONS);
    if (!paction)
    {
        paction = SCGetNextActionToExecute (psc, SCLISTACTIONS);
        if (!paction)
        {
            SCINSTEP = SCSTEP = FALSE;
            if ((!psc->ListActions) && (psc->InteractiveSection))
            {
                /* need timer here */

                char Buffer[500];
                putchar ('S');putchar ('C');putchar ('>');
                gets (Buffer);
                if (Buffer[0] == '#') {psc->InteractiveSection = NULL; return 1;}
                if (Buffer[0] == 0) return 1;
                SCExecuteString (psc, Buffer, NULL, 0,0);
                return 1;
            }
            if (!psc->ListWaitingActions)    return 0;
            else                             return 1;
        }
    }

    /* Action ENTRY */

    SCAddFunctionToInstance (psc, paction, SCAFTER, SCDefaultWhenDone, NULL);
    /* ready to execute Tree action */
    Value = SCExecuteAction (psc, paction);
    if (!Value)
    {

        if (OSStrcmp (paction->ActionClass->Name, "EXECUTE") == 0)
        {
            pentity = (SCEntity *)paction->ListParams->car;
            Value = (SCValue *)pentity->Val;
            SCError (psc, 14, (char*)Value->Val);
        }
        else
            SCError(psc, 14,  paction->ActionClass->Name);

        SCExecuteWhenDone (psc, paction);
        SCDeleteAction (psc, paction);
    }

    return 1;
}

SCAction* SCExecuteString (SC* psc, char* Buffer, MXCom* pcom, LONG Identity, WORD Persistency)
{
    SCEntity* DebEntity;
    List* listentities;
    SCEntity* Entity;
    SCEntity* pentity;
    SCEntity* ExecEntity;
    SCAction* Action = NULL;
    SCValue* Value;
    WORD Defined = 1;

    int Error = 0;

    DebEntity = SCParseString (psc, Buffer, &Error);

    if (!DebEntity)
    {
        if (pcom)
        {
            MXMessage* pmessage;
            pmessage = MXPutMessage(pcom, DL_SYS, "ParseReply") ;
            MXSetValue (pmessage, "Error",      1, &Error);
            MXSetValue (pmessage, "Identity",   1, &Identity);
            if (!Error) MXSetValue (pmessage , "Defined", 1, &Defined);
        }
        return NULL;
    }
    listentities = SCFindDebListEntity(psc, DebEntity);
    while (listentities)
    {
        Entity = (SCEntity *) listentities->car;
        listentities = listentities->cdr;
        if (Entity->Type == SCOPERATOR)
        {
            Action = (SCAction*)Entity->Val;
            Action = SCCopyAction (psc, NULL, Action);
            SCAddFunctionToInstance (psc, Action, SCAFTER, SCDefaultWhenDone, NULL);
            if (pcom) Action->Persistency = Persistency;
            if (Action->Block) ListNewr (&psc->ListActions , Action);
            else  ListNewr (&psc->ListWaitingActions , Action);

            ExecEntity = SCAllocEntity (psc, Action , SCOPERATOR);
            ExecEntity->RootAction = Action;
        }
        else ExecEntity = Entity;
        /* Action ENTRY */

        Value = SCGetEntityValue (psc, ExecEntity); /* execute Entity to get value !!the entity for set is not free*/
        if (!Value)                                 /*if (!value) it means an action */
        {
            Action = (SCAction*)ExecEntity->Val;
            if (OSStrcmp (Action->ActionClass->Name, "EXECUTE") == 0)
            {
                pentity = (SCEntity *)Action->ListParams->car;
                Value = (SCValue *)pentity->Val;
                SCError (psc, 14, (char*)Value->Val);
            }
            else
                SCError(psc, 14,  Action->ActionClass->Name);

            SCExecuteWhenDone (psc, Action);
            SCDeleteAction (psc, Action);
            if (pcom)
            {
                MXMessage* pmessage;
                Error = 14;
                pmessage = MXPutMessage(pcom, DL_SYS, "ParseReply") ;
                MXSetValue (pmessage, "Error",   1, &Error);
                MXSetValue (pmessage, "Identity",   1, &Identity);
            }
            return NULL;
        }
        else
        {
            char Buf[5250];
            SCPrintValue (psc, Value, Buf);
        }
        if (pcom)
        {
            /*dependency here shows that we only put it in the undefined entity */
            if (!Value->Defined)
            {
                if (ExecEntity->Type == SCOPERATOR)
                    SCAddDependencyValue (Action->Value, Identity, Persistency, pcom);
                else
                    SCAddDependencyValue (Value, Identity, Persistency, pcom);
            }
            else SCSendValue (Value, Identity, Persistency, ((Action) ? Action->Error : 0), pcom, FALSE);

            if (Persistency)
                SCAddDependencyValuesToVariables (Entity, Identity, Persistency, pcom);
        }
        if (ExecEntity->Type == SCOPERATOR) free (ExecEntity);
        ListRemove (&psc->ListEntities, Entity);
        SCFreeEntity (Entity);
    }
    return Action;
}



SCAction* SCExecuteFile (SC* psc, char* FileName, MXCom* pcom, LONG Identity, WORD Persistency)
{
    SCEntity* DebEntity;
    List* listentities;
    SCEntity* Entity;
    SCEntity* pentity;
    SCEntity* ExecEntity;
    SCAction* Action = NULL;
    SCValue* Value;
    WORD Defined = 1;

    int Error = 0;

    DebEntity = SCParseFile (psc, FileName, &Error);

    if (!DebEntity)
    {
        if (pcom)
        {
            MXMessage* pmessage;
            pmessage = MXPutMessage(pcom, DL_SYS, "ParseReply") ;
            MXSetValue (pmessage, "Error",      1, &Error);
            MXSetValue (pmessage, "Identity",   1, &Identity);
            if (!Error) MXSetValue (pmessage , "Defined", 1, &Defined);
        }
        return NULL;
    }
    listentities = SCFindDebListEntity(psc, DebEntity);
    while (listentities)
    {
        Entity = (SCEntity *) listentities->car;
        listentities = listentities->cdr;
        if (Entity->Type == SCOPERATOR)
        {
            Action = (SCAction*)Entity->Val;
            Action = SCCopyAction (psc, NULL, Action);
            SCAddFunctionToInstance (psc, Action, SCAFTER, SCDefaultWhenDone, NULL);
            if (pcom) Action->Persistency = Persistency;
            if (Action->Block) ListNewr (&psc->ListActions , Action);
            else  ListNewr (&psc->ListWaitingActions , Action);

            ExecEntity = SCAllocEntity (psc, Action , SCOPERATOR);
            ExecEntity->RootAction = Action;
        }
        else ExecEntity = Entity;
        /* Action ENTRY */

        Value = SCGetEntityValue (psc, ExecEntity); /* execute Entity to get value !!the entity for set is not free*/
        if (!Value)                                 /*if (!value) it means an action */
        {
            Action = (SCAction*)ExecEntity->Val;
            if (OSStrcmp (Action->ActionClass->Name, "EXECUTE") == 0)
            {
                pentity = (SCEntity *)Action->ListParams->car;
                Value = (SCValue *)pentity->Val;
                SCError (psc, 14, (char*)Value->Val);
            }
            else
                SCError(psc, 14,  Action->ActionClass->Name);

            SCExecuteWhenDone (psc, Action);
            SCDeleteAction (psc, Action);
            if (pcom)
            {
                MXMessage* pmessage;
                Error = 14;
                pmessage = MXPutMessage(pcom, DL_SYS, "ParseReply") ;
                MXSetValue (pmessage, "Error",   1, &Error);
                MXSetValue (pmessage, "Identity",   1, &Identity);
            }
            return NULL;
        }
        else
        {
            char Buf[5250];
            SCPrintValue (psc, Value, Buf);
        }
        if (pcom)
        {
            /*dependency here shows that we only put it in the undefined entity */
            if (!Value->Defined)
            {
                if (ExecEntity->Type == SCOPERATOR)
                    SCAddDependencyValue (Action->Value, Identity, Persistency, pcom);
                else
                    SCAddDependencyValue (Value, Identity, Persistency, pcom);
            }
            else
                SCSendValue (Value, Identity, Persistency, Action->Error, pcom, FALSE);

            if (Persistency)
                SCAddDependencyValuesToVariables (Entity, Identity, Persistency, pcom);
        }
        if (ExecEntity->Type == SCOPERATOR) free (ExecEntity);
        ListRemove (&psc->ListEntities, Entity);
        SCFreeEntity (Entity);

    }
    return Action;
}





/* assume only one expression in buffer */
SCEntity* SCGetEntityFromString (SC* psc, char * Buffer)
{
    int Error = 0;
    SCEntity* Entity;

    Entity = SCParseString (psc, Buffer, &Error);
    if (!Entity) return NULL;
    ListRemove (&psc->ListEntities, Entity);
    return Entity;
}


SCEntity* SCGetEntityFromFuncName (SC* psc, MXMessage* pmessage, char* FuncName)
{
    char ExecString[300];
    char VarString[300];
    MXMessageClass * pmessclass ;
    MXObject* pobject;
    SCSection* Section;
    WORD i;
    int NbObjects;

    Section = SCGetSectionFromList(psc->ListSections, FuncName);
    if (!Section) return NULL;

    sprintf (ExecString ,"(%s %ldL ",  FuncName, MXGetComFromMessage (pmessage));



    pmessclass = pmessage->MessageClass;
    NbObjects = ListNbElt (pmessclass->Objects);
    for (i = 0; i < Section->NbVariables - 1; i++)
    {
        if (i > NbObjects)
            strcat  (ExecString, "0 ");
        else
        {
            pobject = (MXObject *)ListGetElt (pmessclass->Objects, i);
            switch (pobject->Type) {
            case MXSHORT :
                sprintf (VarString, "%d ",  *(SHORT*)MXGetValue (pmessage, pobject->Name, 1));
                strcat (ExecString , VarString);
                break;
            case MXWORD :
                sprintf (VarString, "%u ",  *(WORD*)MXGetValue (pmessage, pobject->Name, 1));
                strcat (ExecString , VarString);
                break;
            case MXDWORD :
                sprintf (VarString, "%luL ",  *(DWORD*)MXGetValue (pmessage, pobject->Name, 1));
                strcat (ExecString , VarString);
                break;
            case MXLONG :
                sprintf (VarString, "%ldL ",  *(LONG*)MXGetValue (pmessage, pobject->Name, 1));
                strcat (ExecString , VarString);
                break;
            case MXDOUBLE :
                sprintf (VarString, "%e ",  *(DOUBLE *)MXGetValue (pmessage, pobject->Name, 1));
                strcat (ExecString , VarString);
                break;
            case MXCHAR :
            case MXBYTE :
                sprintf (VarString, "'%.*c' ",  pobject->Size, (char *)MXGetValue (pmessage, pobject->Name, 1));
                strcat (ExecString , VarString);
                break;
            case MXSTRING :
                sprintf (VarString, "\"%s\" ",  (char *)MXGetValue (pmessage, pobject->Name, 1));
                strcat (ExecString , VarString);
                break;
            case MXFILE :
            case MXBUFFER :
                sprintf (VarString, "%ldL ",  (void *)MXGetValue (pmessage, pobject->Name, 1));
                strcat (ExecString , VarString);
                break;
            default:
                break;
            }
        }
    }
    strcat (ExecString, ")");
    return SCGetEntityFromString (psc, ExecString);
}




SCValue* SCExecuteAction (SC* psc, SCAction* paction)
{
    SCValue* Value = NULL;

    if (!paction) return NULL;
    if (paction->State == SCDONE)
    {
        Value = paction->Value;
        return Value;
    }
    paction->State = SCDOING;
    SCExecuteBefore (psc, paction);

    Value =  paction->ActionClass->ActionProcedure (psc, paction);

    if ((Value) && (paction->State == SCDONE))
        SCSetValue (psc, paction->Value, paction->Value);
    return Value;
}

/*------------------------------------------------------------------------*/

BOOL SCActionDone (SCAction* Action)
{

    List*  ListParams;
    SCEntity* Entity;
    ListParams = Action->ListParams;

    if ((Action->State != SCNOTTODO) &&
            (Action->State != SCDONE)) return FALSE;

    while (ListParams)
    {
        Entity = (SCEntity *)ListParams->car;

        if (Entity->Type == SCOPERATOR)
            if (SCActionDone ((SCAction*)Entity->Val) == FALSE)
                return FALSE;
        ListParams = ListParams->cdr;
    }
    return TRUE;
}




SCEntity* SCGetParam (SC* psc, SCAction* Action , WORD range)
{
    WORD i = 1;
    SCEntity* Entity;
    List* listparam = Action->ListParams;

    if (range == 0) return NULL;
    if (!listparam) return NULL;

    while (i++ < range)
    {
        listparam = listparam->cdr;
        if (!listparam) return NULL;
    }
    Entity = (SCEntity*)listparam->car;
    return Entity;
}



SCValue* SCGetEntityValue (SC* psc, SCEntity* Entity)
{
    SCValue* Value = NULL;
    SCAction* paction;
    SCVariable*  pVariable;
    SCEntity* Entity1;

    if (!Entity) return NULL;

    if (Entity->Type == SCOPERATOR)
    {
        paction = (SCAction *)Entity->Val;
        Value   = SCExecuteAction (psc, paction);
        if (Value && (!Value->Defined) && (paction->Value == Value))
            SCAddDependencyValue (paction->Value, Entity->RootAction->Identity, Entity->RootAction->Persistency, NULL);
    }
    else
        if (Entity->Type == SCVARIABLE)
        {
            pVariable = (SCVariable *)Entity->Val;
            Value = (SCValue *)pVariable->Value;
            if (pVariable->SetEntity)
            {
                /* replace pVariable by corresponding entity */

                Entity1 = SCCopyEntity(psc, Entity->RootAction, pVariable->SetEntity);
                if (Entity->FatherAction)
                {
                    ListInsertAfter (&(Entity->FatherAction->ListParams), Entity1, Entity);
                    ListRemove (&(Entity->FatherAction->ListParams), Entity);

                }

                /* il faut copier les dependances */
                SCCopyDependency (psc, Entity1, Value);

                Entity1->FatherAction = Entity->FatherAction;
                if (Entity1->Type == SCOPERATOR)
                {
                    if (!Entity1->FatherAction)
                    {
                        paction = (SCAction *)Entity1->Val;
                        if (paction->Block) ListNewr (&psc->ListActions , paction);
                        else  ListNewr (&psc->ListWaitingActions , paction);
                    }
                    Entity1->RootAction = ((Entity->RootAction) ? Entity->RootAction : (SCAction*)Entity1->Val);
                }

                /*SCFreeEntity (Entity);*/
                return SCGetEntityValue (psc, Entity1);
            }
            else
                if ((Entity->RootAction) && (!Value->Defined))
                    SCAddDependencyValue (Value, Entity->RootAction->Identity, Entity->RootAction->Persistency, NULL);

        }
        else Value = (SCValue*)Entity->Val;
    return Value;
}






SCValue* SCSetEntityValue (SC* psc, SCEntity* Entity, SCValue* pvalue)
{
    SCValue* Value = NULL;
    SCAction* paction;
    SCVariable*  pVariable;

    if (!Entity) return NULL;

    if (Entity->Type == SCOPERATOR)
    {
        paction = (SCAction *)Entity->Val;
        Value = paction->Value;
        SCSetValue (psc, Value, pvalue);
    }
    else
        if (Entity->Type == SCVARIABLE)
        {
            pVariable = (SCVariable *)Entity->Val;
            Value = (SCValue *)pVariable->Value;

            if (pVariable->SetEntity)      /* in case of setq on a set variable */
            {
                SCFreeEntity (pVariable->SetEntity);
                pVariable->SetEntity = NULL;
            }
            else
            {
                SCSetValue (psc, Value, pvalue);
            }
        }
    return Value;
}



SCValue* SCGetParamValue (SC* psc, SCAction* Action , WORD range)
{
    SCEntity* Entity;
    SCValue* Value;

    Entity = SCGetParam (psc, Action, range);
    Value  = SCGetEntityValue (psc, Entity);
    return Value;
}



SCValue* SCSetParamValue (SC* psc, SCAction* Action , WORD range, SCValue* pvalue)
{
    SCEntity* Entity;
    SCValue* Value;

    Entity = SCGetParam(psc, Action, range);
    Value = SCSetEntityValue (psc, Entity, pvalue);
    return Value;
}


/*------------------------------------------------------------------------*/



char* SCTraceAction (SCAction* paction)
{
    char* Buf = NULL;
    char *BufTemp = NULL;
    List* listparam = paction->ListParams;
    SCEntity* Entity;
    SCValue* Value;
    char InName[100];
    char* SName = NULL;

    Buf =(char *) realloc(Buf, strlen(paction->ActionClass->Name) + 3);
    if (!Buf) return(NULL);

    sprintf(Buf, "(%s", paction->ActionClass->Name);

    while(listparam)
    {
        Entity = (SCEntity*)listparam->car;
        Value = (SCValue *)Entity->Val;
        if ((Entity->Type == SCATOM) ||(Entity->Type == SCVARIABLE))
        {
            char* Name = "";

            if (Entity->Type == SCATOM)
            {
                switch (Value->Type) {
                case SCNULL :
                    {
                        Name = "NULL";
                    }
                    break;
                case SCINTEGER :
                    {
                        sprintf (InName, "%d", (int)Value->Val);
                        Name = InName;
                    }
                    break;
                case SCLONG :
                    {
                        sprintf (InName, "%ldL", (long)Value->Val);
                        Name = InName;
                    }
                    break;
                case SCFLOAT :
                    break;
                    /*case SCPOINTER :
                     {
                       sprintf (InName, "%p", Value->Val);
                       Name = InName;
                     }
                    break;*/
                case SCBOOLEAN :
                    {
                        if (OSStrcmp ((char*)Value->Val, "FALSE") == 0)
                            Name = "FALSE";
                        else     Name = "TRUE";
                    }
                    break;
                case SCDATE   :
                case SCTIME   :
                    Name = (char *)Value->Val;
                    break;
                case SCSTRING :
                    Name = (char *)Value->Val;
                    SName = (char*)malloc (strlen (Name) + 3);
                    sprintf (SName, "\"%s\"", Name);
                    Name = SName;
                    break;
                case SCCHAR   :
                    Name = (char *)Value->Val;
                    SName = (char*)malloc (strlen (Name) + 3);
                    sprintf (SName, "'%s'", Name);
                    Name = SName;

                    break;
                default :
                    break;
                }
            }
            else Name = (char*)((SCVariable *)Entity->Val)->Name;

            Buf = (char *)realloc(Buf, strlen(Buf) + 2 + strlen(Name) + 1);
            if (!Buf) return(NULL);
            strcat (Buf, " ");
            strcat (Buf, Name);
            if (SName) free (SName);
            SName = NULL;
        }
        else
            if (Entity->Type == SCOPERATOR)
            {
                BufTemp = SCTraceAction ((SCAction*)Entity->Val);
                if (!BufTemp) return(NULL);

                Buf = (char *)realloc(Buf, strlen(Buf) + 2 + strlen(BufTemp) + 1);
                if (!Buf) return(NULL);
                strcat (Buf, " ");
                strcat(Buf, BufTemp);
                free(BufTemp);
            }
        listparam = listparam->cdr;
    }
    strcat(Buf,")");
    return(Buf);
}


void SCAddSystemActions (SC* psc)
{
    /* A REVOIR 
    ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "SHARE",        SCNoBlock,   SCShareProc, 1, 1, 1));
    ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "WHEN",         SCNoBlock,   SCWhenProc, 1, 1, 1        ));
    ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "JAVA",         SCNoBlock,   SCJavaProc , 1, 1, 1       ));

    ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "EVERY",        SCNoBlock,   SCEveryProc, 1, 1, 1       ));
    ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "AT",           SCNoBlock,   SCAtProc, 1, 1, 1          ));
    ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "EVERYTIME",    SCNoBlock,   SCEveryTimeProc, 1, 1, 1   ));
    ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "WAIT",         SCBlock,     SCWaitProc, 1, 1, 1        ));
    */
	/* DIALING 

    ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "DIAL",         SCNoBlock,   SCDialProc, 1, 1, 1        ));
    ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "HANGUP",       SCNoBlock,   SCHangUpProc, 1, 1, 1      ));
    ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "DIALSTATUS",   SCNoBlock,      SCDialStatusProc, 1, 1, 1));
    ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "SETDIALENTRY", SCNoBlock,    SCSetDialEntryProc, 1, 1, 1));
     END DIALING */

    /*
    ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "SETTRACEFILE", SCNoBlock,   SCSetTraceFileProc, 1, 1, 1));
    ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "TRACE",        SCNoBlock,   SCTraceProc, 1, 1, 1       ));
    ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "PING",         SCNoBlock,   SCPingProc, 1, 1, 1        ));
    ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "REMOTE",       SCNoBlock,   SCRemoteProc, 1, 1, 1      ));
    ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "SYSTEM",       SCBlock,     SCSystemProc, 1, 1, 1      ));
    ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "PROCESS",      SCNoBlock,   SCProcessProc, 1, 1, 1     ));
    ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "EVAL",         SCBlock,     SCEvalProc, 1, -1, -1        ));
    ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "TIME",         SCBlock,     SCTimeProc, 1, 1, 1        ));
    ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "DATE",         SCBlock,     SCDateProc, 1, 1, 1        ));
    ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "GETCHAR",      SCBlock,     SCGetCharProc, 1, 1, 1     ));
    ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "PUTCHAR",      SCBlock,     SCPutCharProc, 1, 1, 1     ));
   
    ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "EXIST",        SCNoBlock,   SCExistProc, 1, 1, 1       ));


    ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "WHILE",        SCNoBlock,   SCWhileProc, 2, -1, -1         ));
    ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "PRINT",        SCNoBlock,   SCPrintProc, 1, 1, 1       ));
	ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "PARSE",        SCBlock,     SCParseProc, 2, -1, -1         ));
    ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "EXIT",         SCBlock,     SCExitProc, 2, -1, -1          ));
    ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "CD"  ,         SCBlock,     SCCdProc, 2, -1, -1            ));
    ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "RCD" ,         SCNoBlock,   SCRcdProc, 2, -1, -1           ));
    ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "PWD" ,         SCBlock,     SCPwdProc, 2, -1, -1           ));
    ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "PUT" ,         SCBlock,     SCPutProc, 2, -1, -1           ));
    ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "MPUT",         SCBlock,     SCMputProc, 2, -1, -1          ));
    ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "GET" ,         SCBlock,     SCGetProc, 2, -1, -1           ));
    ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "MGET",         SCNoBlock,   SCMgetProc, 2, -1, -1          ));
    ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "DIR",          SCNoBlock,   SCDirProc, 2, -1, -1           ));
    ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "LIST",         SCNoBlock,   SCListProc, 2, -1, -1          ));
    ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "MKDIR",        SCBlock,     SCMkdirProc, 2, -1, -1         ));
    ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "RMKDIR",       SCNoBlock,   SCRmkdirProc, 2, -1, -1        ));
    ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "RMDIR",        SCBlock,     SCRmdirProc, 2, -1, -1         ));
    ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "RRMDIR",       SCNoBlock,   SCRrmdirProc, 2, -1, -1        ));
    ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "DELETE",       SCBlock,     SCDeleteProc, 2, -1, -1        ));
    ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "RDELETE",      SCNoBlock,   SCRdeleteProc, 2, -1, -1       ));

    ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "atoi",         SCNoBlock,   SCAtoiProc        , 2, -1, -1));
	ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "malloc",       SCNoBlock,   SCMallocProc      , 2, -1, -1));
    ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "free",         SCNoBlock,   SCFreeProc        , 2, -1, -1));
    ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "strlen",       SCNoBlock,   SCStrLengthProc   , 2, -1, -1));
    ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "trim",         SCNoBlock,   SCStrTrimProc     , 2, -1, -1));
*/
    ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "DEFUN",        SCNoBlock,   SCDefunProc, 1, 1, 1      ,SCBOOLEAN ));
    ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "EXECUTE",      SCNoBlock,   SCExecuteProc     , 1, -1, -1, SCBOOLEAN));
	ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "SETQ",         SCNoBlock,   SCSetqProc, 2, 2, 2        ,SCNOTDEFINED));
    ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "IF",           SCBlock,     SCIfProc, 2, -1, -1        ,SCNOTDEFINED));
	ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "NOT",          SCNoBlock,   SCNotProc, 1, 1, 1        ,SCBOOLEAN));
    ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "AND",          SCNoBlock,   SCAndProc, 2, -1, -1      ,SCBOOLEAN));
    ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "OR",           SCNoBlock,   SCOrProc, 2, -1, -1       ,SCBOOLEAN));
    
	ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "=",			SCNoBlock,   SCEqualProc, 2, 2, 2,SCBOOLEAN));
	ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, ">",            SCNoBlock,   SCGreaterProc, 2, 2, 2,SCBOOLEAN));
    ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "<",            SCNoBlock,   SCSmallerProc , 2, 2, 2,SCBOOLEAN));
    ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, ">=",           SCNoBlock,   SCGreatEquProc, 2, 2, 2,SCBOOLEAN));
	ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "<=",           SCNoBlock,   SCSmallEquProc, 2, 2, 2,SCBOOLEAN));
	ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "+",			SCNoBlock,   SCOpeAddProc , 2, -1, -1,SCFLOAT));
	ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "-",			SCNoBlock,   SCOpeMinusProc, 2, -1, -1,SCFLOAT));
    ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "*",			SCNoBlock,   SCOpeMultProc , 2, -1, -1,SCFLOAT));
    ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "/",			SCNoBlock,   SCOpeDivProc , 2, -1, -1,SCFLOAT));
    ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "Max",          SCNoBlock,   SCMaxIntProc, 2, 2, 2,SCFLOAT));
    ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "Min",          SCNoBlock,   SCMinIntProc , 2, 2, 2,SCFLOAT));

	
	// ADDED
    ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "ELSE",         SCBlock,     SCEvalProc, 1, -1, -1        ,SCNOTDEFINED));
	ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "abs",			SCNoBlock,   SCStrTrimProc     , 1, 1, 1,SCFLOAT));

	ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "AndS",         SCNoBlock,   SCStrTrimProc     , 3, 11, -1,SCBOOLEAN));
	ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "AndPS",        SCNoBlock,   SCStrTrimProc     , 3, 11, -1,SCBOOLEAN));
    ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "AndPTS",        SCNoBlock,  SCStrTrimProc     , 3, 11, -1,SCBOOLEAN));
    ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "AndBS",        SCNoBlock,   SCStrTrimProc     , 3, 11, -1,SCBOOLEAN));
    ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "AndTS",        SCNoBlock,   SCStrTrimProc     , 3, 11, -1,SCBOOLEAN));

	ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "OrS",			SCNoBlock,   SCStrTrimProc     , 3, 11, -1,SCBOOLEAN));
    ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "OrPS",         SCNoBlock,   SCStrTrimProc     , 3, 11, -1,SCBOOLEAN));
    ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "OrPTS",         SCNoBlock,   SCStrTrimProc    , 3, 11, -1,SCBOOLEAN));
	ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "OrBS",         SCNoBlock,   SCStrTrimProc     , 3, 11, -1,SCBOOLEAN));
    ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "OrTS",         SCNoBlock,   SCStrTrimProc     , 3, 11, -1,SCBOOLEAN));

	ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "SValue",       SCNoBlock,   SCStrTrimProc     , 3, 3, 3,SCFLOAT));
	ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "SPValue",		SCNoBlock,   SCStrTrimProc     , 3, 3, 3,SCFLOAT));
	ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "AndV",			SCNoBlock,   SCStrTrimProc     , 3, 11, -1,SCFLOAT));
    ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "AndPV",        SCNoBlock,   SCStrTrimProc    , 3, 11, -1,SCFLOAT));
    ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "AndBV",        SCNoBlock,   SCStrTrimProc    , 3, 11, -1,SCFLOAT));
	ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "OrV",			SCNoBlock,   SCStrTrimProc    , 3, 11, -1,SCFLOAT));
    ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "OrPV",         SCNoBlock,   SCStrTrimProc     , 3, 11, -1,SCFLOAT));
    ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "OrBV",         SCNoBlock,   SCStrTrimProc     , 3, 11, -1,SCFLOAT));
	ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "STime",		SCNoBlock,   SCStrTrimProc     , 3, 3, 3,SCFLOAT));
	ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "SPTime",		SCNoBlock,   SCStrTrimProc     , 3, 3, 3,SCFLOAT));
	ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "SPrice",		SCNoBlock,   SCStrTrimProc     , 3, 3, -1,SCFLOAT));
    ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "SPPrice",      SCNoBlock,   SCStrTrimProc    , 3, 3, -1,SCFLOAT));


    ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "SET" ,         SCNoBlock,   SCSetProc, 2, 2, 2         ,SCNOTDEFINED));

	ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "START",		SCNoBlock,   SCStrTrimProc,   0, 0, 0,SCNOTDEFINED));
	ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "BUY",			SCNoBlock,   SCStrTrimProc,   0, 0, 0,SCNOTDEFINED));
	ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "SELL",			SCNoBlock,   SCStrTrimProc,   0, 0, 0,SCNOTDEFINED));
	ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "EXIT_BUY",		SCNoBlock,   SCStrTrimProc,   0, 0, 0,SCNOTDEFINED));
	ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "EXIT_SELL",	SCNoBlock,   SCStrTrimProc,   0, 0, 0,SCNOTDEFINED));
	ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "EXIT",			SCNoBlock,   SCStrTrimProc,   0, 0, 0,SCNOTDEFINED));
	ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "CLOSE_BUY",	SCNoBlock,   SCStrTrimProc,   0, 0, 0,SCNOTDEFINED));
	ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "CLOSE_SELL",	SCNoBlock,   SCStrTrimProc,   0, 0, 0,SCNOTDEFINED));
	ListInsert (&psc->ListAddedActions,  SCAllocSysAction (psc, "CLOSE",		SCNoBlock,   SCStrTrimProc,   0, 0, 0,SCNOTDEFINED));
	ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "HEDGE_BUY",	SCNoBlock,   SCStrTrimProc,   0, 0, 0,SCNOTDEFINED));
	ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "HEDGE_SELL",	SCNoBlock,   SCStrTrimProc,   0, 0, 0,SCNOTDEFINED));
	ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "CLOSE_HEDGE_SELL",	SCNoBlock,   SCStrTrimProc,   0, 0, 0,SCNOTDEFINED));
	ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "CLOSE_HEDGE_BUY",	SCNoBlock,   SCStrTrimProc,   0, 0, 0,SCNOTDEFINED));
	ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "CLOSE_HEDGE",		SCNoBlock,   SCStrTrimProc,   0, 0, 0,SCNOTDEFINED));

	// END ADDED

    /* related to FILE inout 

    ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "OPEN",         SCNoBlock,   SCOpenProc        , 2, -1, -1));
    ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "CLOSE",        SCNoBlock,   SCCloseProc       , 2, -1, -1));
    ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "READ",         SCNoBlock,   SCReadProc        , 2, -1, -1));
    ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "PUTLINE",      SCNoBlock,   SCPutlineProc     , 2, -1, -1));
    ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "GETLINE",      SCNoBlock,   SCGetlineProc     , 2, -1, -1));
    ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "WRITE",        SCNoBlock,   SCWriteProc       , 2, -1, -1));

    ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "REPLACE",      SCNoBlock,   SCReplaceProc     , 2, -1, -1));
    ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "COPY",         SCNoBlock,   SCCopyProc        , 2, -1, -1));
    ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "RENAME",       SCNoBlock,   SCRenameProc      , 2, -1, -1));
    ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "EXECUTE",      SCNoBlock,   SCExecuteProc     , 1, -1, -1));
    ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "GETERROR",     SCNoBlock,   SCGetLastError    , 2, -1, -1));
    ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "LOOK",         SCNoBlock,   SCLook            , 2, -1, -1));
    ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "STRING",       SCNoBlock,   SCStringProc      , 2, -1, -1));
    ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "GETVERSION",   SCNoBlock,   SCGetVersionProc  , 2, -1, -1));

    ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "GETNAME",      SCNoBlock,   SCGetNameProc     , 2, -1, -1));
    ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "GETFROMNAME",  SCNoBlock,   SCGetFromNameProc , 2, -1, -1));
    ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "GETTYPE",      SCNoBlock,   SCGetTypeProc     , 2, -1, -1));
    ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "GETSIZE",      SCNoBlock,   SCGetSizeProc     , 2, -1, -1));
    ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "GETERROR",     SCNoBlock,   SCGetErrorProc    , 2, -1, -1));
    ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "SETNAME",      SCNoBlock,   SCSetNameProc     , 2, -1, -1));
    ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "SETFROMNAME",  SCNoBlock,   SCSetFromNameProc , 2, -1, -1));
    ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "SETTYPE",      SCNoBlock,   SCSetTypeProc     , 2, -1, -1));

    ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "GETBBUFFER",   SCNoBlock,   SCGetBufferProc   , 2, -1, -1));
    ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "GETBTYPE",     SCNoBlock,   SCGetBTypeProc    , 2, -1, -1));
    ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "GETBSIZE",     SCNoBlock,   SCGetBSizeProc    , 2, -1, -1));
    ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "SETBBUFFER",   SCNoBlock,   SCSetBufferProc   , 2, -1, -1));
    ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "SETBTYPE",     SCNoBlock,   SCSetBTypeProc    , 2, -1, -1));
    ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "SETBSIZE",     SCNoBlock,   SCSetBSizeProc    , 2, -1, -1));
*/
    /* related to MX 

    ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "ADDPORT"    ,  SCNoBlock,   SCMXAddPort       , 2, -1, -1));
    ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "LOADCLASS"  ,  SCNoBlock,   SCMXLoadClass     , 2, -1, -1));
    ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "LOADRESOURCE", SCNoBlock,   SCMXLoadResource  , 2, -1, -1));
    ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "OPENCOM"    ,  SCBlock,     SCMXOpenCom       , 2, -1, -1));
    ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "CLOSECOM"   ,  SCNoBlock,   SCMXCloseCom      , 2, -1, -1));
    ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "CREATEMESSAGE",SCNoBlock,   SCMXCreateMessage , 2, -1, -1));
    ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "CREATEMESSAGECLASS",SCNoBlock,   SCMXCreateMessageClass , 2, -1, -1));
    ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "JOIN",         SCBlock,     SCMXJoinCom       , 2, -1, -1));
    ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "INTERRUPT"   , SCNoBlock,   SCInterruptedCom  , 2, -1, -1));
    ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "SENDMESSAGE" , SCNoBlock,   SCDGSendMessage   , 2, -1, -1));
    ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "PUTMESSAGE" ,  SCNoBlock,   SCMXPutMessage    , 2, -1, -1));
    ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "ONRECVMESSAGE",SCNoBlock,   SCMXOnRecvMessage , 2, -1, -1));
    ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "ONSENDMESSAGE",SCNoBlock,   SCMXOnSendMessage , 2, -1, -1));
    ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "SETCGIFILE"   ,SCNoBlock,   SCMXSetCGIFile    , 2, -1, -1));

    ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "GETFORMVALUE" ,SCNoBlock,   SCCGIGetValueFormEntry, 2, -1, -1));
    ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "ADDVALUE"     ,SCNoBlock,   SCCGIAddValue, 2, -1, -1));
    ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "ADDCOLUMN"    ,SCNoBlock,   SCCGIAddColumn, 2, -1, -1));



    ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "GETTYPE",     SCNoBlock,    SCMXGetType, 2, -1, -1));
    ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "GETNAME",     SCNoBlock,    SCMXGetName , 2, -1, -1));
    ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "GETNBROBJECTS",SCNoBlock,   SCMXGetNbrObjects , 2, -1, -1));
    ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "GETVALUE",    SCNoBlock,    SCMXGetValue, 2, -1, -1));
    ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "SETVALUE",    SCNoBlock,    SCMXSetValue , 2, -1, -1));
    ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "GETRANGEVALUE",    SCNoBlock,    SCMXGetRangeValue, 2, -1, -1));
    ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "SETRANGEVALUE",    SCNoBlock,    SCMXSetRangeValue , 2, -1, -1));
    ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "FREEMESSAGE", SCNoBlock,    SCMXFreeMessage, 2, -1, -1));

    ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "SEND",        SCNoBlock,    SCMXSend             , 2, -1, -1));
    ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "RECV",        SCNoBlock,    SCMXRecv             , 2, -1, -1));
    ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "ADDCOMCALLBACK",        SCNoBlock,SCMXAddComCallBack   , 2, -1, -1));
    ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "REMOVECOMCALLBACK",     SCNoBlock,SCMXRemoveComCallBack, 2, -1, -1));
    ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "ADDENDINGPROCEDURE",    SCNoBlock,SCMXAddEndingProcedure           , 2, -1, -1));
    ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "REMOVEENDINGPROCEDURE", SCNoBlock,SCMXRemoveEndingProcedure        , 2, -1, -1));
    ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "ADDGENERALCONNECTCALLBACK",    SCNoBlock,SCMXAddGeneralConnectCallBack    , 2, -1, -1));
    ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "REMOVEGENERALCONNECTCALLBACK", SCNoBlock,SCMXRemoveGeneralConnectCallBack , 2, -1, -1));
    ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "GETMESSAGECLASSFROMNAMES", SCNoBlock, SCMXGetMessageClassFromNames , 2, -1, -1));
    ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "SETIOMESSAGECLASS", SCNoBlock, SCMXSetIOMessageClassProc , 2, -1, -1));
*/
    

    /* related to MAIL 

    ListInsert (&psc->ListSystemActions, SCAllocSysAction (psc, "SENDMAIL",      SCNoBlock,   SCSendMailProc     , 2, -1, -1));
*/
    return;
}



/*------------------------------------------------------------------------*/


/* to free all dependencies */
void SCInitEntity (SC* psc, SCEntity* Entity)
{

    SCAction*  paction;
    if (!Entity) return;
    if (Entity->Type != SCOPERATOR) return;

    paction = (SCAction *)Entity->Val;
    SCFreeValue (paction->Value);
    paction->Value = SCAllocValue (psc, NULL, SCPOINTER);
    SCInitAction (psc, paction);
    return;
}


/*------------------------------------------------------------------------*/
/*   VARIABLES                                                            */
/*------------------------------------------------------------------------*/


SCVariable* SCCreateVariable (SC* psc, char* Name)
{

    SCVariable* Variable;

    Variable = (SCVariable *)malloc(sizeof(SCVariable));
    if (!Variable) return(NULL);
    Variable->Name = (char *)malloc(strlen(Name) + 1);
    if(!Variable->Name)
    {
        free (Variable);
        return(NULL);
    }
    strcpy (Variable->Name, Name);
    Variable->Value       =  SCAllocValue (psc, NULL, SCPOINTER);
    Variable->Sharable    = NULL;
    Variable->Exportable  = NULL;
    Variable->SetEntity   = NULL;
    /* ListInsert (&psc->ListVariables, Variable);    */
    return Variable;
}



SCVariable* SCFindVariableInList (SC* psc, char* Name, List* plist)
{
    SCVariable* Variable = NULL;

    if (!plist) return NULL;

    while (plist)
    {
        Variable = (SCVariable *)plist->car;
        if (strcmp (Variable->Name, Name) == 0)
            return Variable;
        plist = plist->cdr;
    }
    return NULL;
}



SCVariable* SCFindVariableFromName (SC* psc, char* Name)
{
    SCVariable* Variable = NULL;

    if ((Variable = SCFindVariableInList (psc, Name, psc->ListVariables)))
        return Variable;
    else return NULL;
}


void SCFreeVariable (SCVariable *pVariable)
{
    if (!pVariable) return;
    if (pVariable->SetEntity) SCFreeEntity (pVariable->SetEntity);
    SCFreeValue((SCValue*)pVariable->Value);
    free (pVariable);
}


/*------------------------------------------------------------------------*/
/*   VALUES                                                               */
/*------------------------------------------------------------------------*/

/* var is always string  conversion is done */

SCValue* SCAllocValue (SC* psc, void* Val, SHORT Type)
{
    SCValue* pValue;

    pValue = (SCValue *)malloc(sizeof(SCValue));

    pValue->Defined  = SCNOTDEFINED;
    pValue->Val      = NULL;
    pValue->Size     = 0;
    pValue->Type     = SCPOINTER;
    pValue->Dependency = NULL;


    return SCPutValue (pValue, Val, Type);
}


/* var is always string  conversion is done */

SCValue* SCPutValue (SCValue* pValue, void* Val, SHORT Type)
{
    if (!pValue) return NULL;

    if (SCMallocType (pValue->Type) && (pValue->Val))
        free (pValue->Val);

    pValue->Size = 0;
    pValue->Type = Type;

    if (!Val)
    {
        pValue->Val = NULL;
        return pValue;
    }
    switch (Type) {
    case SCNULL :
        {
            pValue->Val= (void *)0;
            pValue->Size = 0;
        }
        break;
    case SCINTEGER :
    case SCLONG :
    case SCFLOAT :    
        {
            pValue->Val = (void*)malloc(strlen ((char*)Val) + 1);
            strcpy((char*)pValue->Val, (char*)Val);
            pValue->Size = strlen ((char*)Val) + 1;
        }                 
        break;
    case SCPOINTER :
        pValue->Val = Val;
        pValue->Size = sizeof (char *);
        break;
    case SCBOOLEAN :
        {
            int l = 0;
            if (OSStrcmp ((char*)Val, "FALSE") == 0)
                l = 0;
            else l = 1;
            pValue->Val= (void *)l;
            pValue->Size = sizeof (int);
        }
        break;

    case SCSTRING :
    case SCDATE   :
    case SCTIME   :
    case SCCHAR   :
        if (Val)
        {
            pValue->Val = (void*)malloc(strlen ((char*)Val) + 1);
            strcpy((char*)pValue->Val, (char*)Val);
            pValue->Size = strlen ((char*)Val) + 1;
        }
        break;

    }
    return pValue;
}


/*------------------------------------------------------------------------*/


/* var is always of type no conversion is done strings are affected*/

SCValue* SCAffectValue (SCValue* pValue, void* Val, SHORT Type)
{
    if (!pValue) return NULL;

    if (SCMallocType (pValue->Type) && (pValue->Val))
        free (pValue->Val);

    pValue->Size = 0;
    pValue->Type = Type;

    if (!Val)
    {
        pValue->Val = NULL;
        return pValue;
    }
    switch (Type) {
    case SCNULL :
        {
            pValue->Val= (void *)0;
            pValue->Size = 0;
        }
        break;
    case SCINTEGER :
        {
            pValue->Val= Val;
            pValue->Size = sizeof (int);
        }
        break;
    case SCLONG :
        {
            pValue->Val= Val;
            pValue->Size = sizeof (LONG);
        }
        break;
    case SCFLOAT :    /*
             {
                float l = atof ((char*)Val);
                pValue->Val = (float *)l;
                pValue->Size = sizeof (float);
             }                 */
        break;
    case SCPOINTER :
        pValue->Val = Val;
        pValue->Size = sizeof (char *);
        break;
    case SCBOOLEAN :
        {
            pValue->Val= Val;
            pValue->Size = sizeof (BOOL);
        }
        break;
    case SCSTRING :
    case SCDATE   :
    case SCTIME   :
    case SCCHAR   :
        if (Val)
        {
            pValue->Val = Val;
            pValue->Size = strlen ((char*)Val) + 1;
        }
        break;

    }
    return pValue;
}


/*------------------------------------------------------------------------*/
/* only copying value */

SCValue* SCCopyValue (SC* psc, SCValue* destvalue, SCValue* sourcevalue)
{

    if ((!destvalue) || (!sourcevalue)) return NULL;

    if (SCMallocType (destvalue->Type) && (destvalue->Val))
        free (destvalue->Val);


    if (sourcevalue->Type != SCNULL)
        destvalue->Type    = sourcevalue->Type;
    destvalue->Defined = sourcevalue->Defined;
    destvalue->Size    = sourcevalue->Size;
    destvalue->Val     = sourcevalue->Val;

    if (SCMallocType (sourcevalue->Type)  && (sourcevalue->Val))
    {
        destvalue->Val = (void*)malloc(strlen ((char*)sourcevalue->Val) + 1);
        strcpy((char*)destvalue->Val, (char*)sourcevalue->Val);
    }
    return destvalue;
}


SCValue* SCSetValue (SC* psc, SCValue* destvalue, SCValue* sourcevalue)
{

    SCDependency* pDependency;
    if ((destvalue != sourcevalue) &&
            (!SCCopyValue (psc, destvalue, sourcevalue))) return NULL;

    if (destvalue->Defined == SCDEFINED)
    {
        List* listdependency = destvalue->Dependency;
        while (listdependency)
        {
            pDependency = (SCDependency *)listdependency->car;
            listdependency = listdependency->cdr;
            SCExecuteDependency (psc, destvalue, pDependency);
        }
    }
    return destvalue;
}



/*------------------------------------------------------------------------*/


void SCFreeValue (SCValue* pValue)
{
    SCDependency* pDependency;

    if (!pValue) return;

    switch(pValue->Type)
    {
		case SCSTRING :
		case SCDATE   :
		case SCTIME   :
		case SCCHAR   :
		case SCFLOAT  :
		case SCINTEGER :
		case SCLONG :
     
			if (pValue->Val)
				free (pValue->Val);
		break;
		default:
		break;
    }
    while (pValue->Dependency)
    {

        pDependency = (SCDependency *)pValue->Dependency->car;
        ListRemove (&pValue->Dependency, pDependency);
        free (pDependency);
    }
    free (pValue);
    return;
}


/*------------------------------------------------------------------------*/

int SCDefaultTrace (char* buf, LONG LineNumber, void* app)
{
    printf ("%s\n", buf);
    return 1;

}

int SCDefaultPrint (char* buf, void* app)
{
    printf ("%s\n", buf);
    return 1;
}



void SCPrintValue (SC* psc, SCValue* Value, char* Buf)
{

    if (!Value)
    {
        sprintf (Buf, "%s", "Null value Error in expression");
        return;
    }
    if (!Value->Defined)
    {
        sprintf (Buf, "%s", "UNDEFINED");
        return;
    }
    switch (Value->Type) {
    case SCNULL :
        sprintf (Buf, "%s", "NULL");
        break;
    case SCINTEGER :
    case SCLONG :
    case SCFLOAT :
        sprintf (Buf, "%s", (char*)Value->Val);
        break;
    case SCPOINTER :
        sprintf (Buf, "%p", Value->Val);
        break;
    case SCBOOLEAN :
        if (Value->Val)
            sprintf (Buf, "%s", "TRUE");
        else
            sprintf (Buf, "%s", "FALSE");
        break;
    case SCSTRING :
    case SCDATE   :
    case SCTIME   :
    case SCCHAR   :
        sprintf (Buf, "%s", (char*)Value->Val);
        break;
    }
    return;
}



/*------------------------------------------------------------------------*/





void SCAddDependencyValue (SCValue* Value, LONG Identity,  WORD Persistency, MXCom* pcom)
{

    SCDependency* pDependency = NULL;
    SCDependency* pdependency = NULL;
    List* dependencylist = Value->Dependency;

    while (dependencylist)
    {
        pdependency = (SCDependency *)dependencylist->car;
        if ((pdependency->Identity == Identity)  && (pdependency->Com == pcom))
        {
            pDependency = pdependency;
            break;
        }
        dependencylist = dependencylist->cdr;
    }
    if (!pDependency)
    {
        pDependency = (SCDependency *)malloc(sizeof(SCDependency));
        ListNewr (&Value->Dependency, pDependency);
    }
    pDependency->Com      = pcom;
    pDependency->Identity = Identity;
    pDependency->Persistency = Persistency;
    return;

}

void SCCopyDependency (SC* psc, SCEntity* Entity, SCValue* FromValue)
{

    SCValue* ToValue = NULL;
    SCDependency* pdependency;
    List* dependencylist = FromValue->Dependency;
    if (Entity->Type == SCOPERATOR)
        ToValue = ((SCAction *)Entity->Val)->Value;
    else
        if (Entity->Type == SCVARIABLE)
            ToValue = ((SCVariable *)Entity->Val)->Value;

    if ((!FromValue) || (!ToValue)) return;

    while (dependencylist)
    {
        pdependency = (SCDependency *)dependencylist->car;
        SCAddDependencyValue (ToValue, pdependency->Identity, pdependency->Persistency, pdependency->Com);
        dependencylist = dependencylist->cdr;
    }
}



void SCExecuteDependency (SC* psc, SCValue* Value, SCDependency* pDependency)
{
    SCAction* paction;
    WORD Persistency = pDependency->Persistency;

    if (!Persistency)
    {
        ListRemove (&Value->Dependency, pDependency);
    }
    if (pDependency->Com)
    {
        SCSendValue (Value, pDependency->Identity, pDependency->Persistency, 0, pDependency->Com, FALSE);
    }
    else
    {
        paction = SCGetActionFromIdentity (psc, pDependency->Identity);
        if (paction)  /* action doesn't exist persistent ---> not persistent */
        {
            if (Persistency)  SCInitDependentValuesFromAction (psc, paction, Value);
            SCExecuteAction (psc, paction);
        }
        else Persistency = 0;
    }
    /* we only remove non persistent dependencies */
    /* if dependency is persistent Entity and value is also persistent */

    if (!Persistency)
    {
        ListRemove (&Value->Dependency, pDependency);
        free (pDependency);
    }
}

/*------------------------------------------------------------------------*/

void SCAddDependencyValuesToVariables (SCEntity* FromEntity, LONG Identity,  WORD Persistency, MXCom* pcom)
{
    SCEntity* Entity;
    SCAction* paction;
    List* ListParams;
    SCVariable* pvariable;

    if (FromEntity->Type == SCOPERATOR)
    {
        paction = (SCAction*)FromEntity->Val;
        ListParams = paction->ListParams;
        while (ListParams)
        {
            Entity = (SCEntity *)ListParams->car;
            SCAddDependencyValuesToVariables (Entity, Identity, Persistency, pcom);
            ListParams = ListParams->cdr;
        }
    }
    else
        if (FromEntity->Type == SCVARIABLE)
        {
            pvariable = (SCVariable*)FromEntity->Val;
            SCAddDependencyValue (pvariable->Value, Identity, Persistency, pcom);
        }
    return;
}




/*------------------------------------------------------------------------*/

int SCFindValueInEntity(SC* psc, SCEntity* Entity, SCValue* Value)
{

    SCAction*  paction;
    SCVariable* pvariable;

    if (!Entity) return 0;
    if (Entity->Type == SCVARIABLE)
    {
        pvariable = (SCVariable *)Entity->Val;
        if (Value == pvariable->Value) return 1;
        else return 0;
    }
    if (Entity->Type == SCOPERATOR)
    {
        paction = (SCAction *)Entity->Val;
        return SCInitDependentValuesFromAction (psc, paction, Value);
    }
    return 0;
}



/*------------------------------------------------------------------------*/

int SCInitDependentValuesFromAction (SC* psc, SCAction* paction, SCValue* Value)
{

    List* ListParams;
    SCEntity* pEntity;
    int FoundValue = 0;

    if (!paction) return 0;
    if (paction->Value == Value)
    {
        return 1;
    }
    ListParams  = paction->ListParams;
    while (ListParams)
    {
        pEntity = (SCEntity *)ListParams->car;
        if (SCFindValueInEntity(psc, pEntity, Value))
            FoundValue = 1;
        ListParams = ListParams->cdr;
    }
    if (FoundValue)
    {
        SCPutValue (paction->Value, NULL, SCPOINTER);
        paction->State = SCTODO;
    }
    return 1;
}

/*------------------------------------------------------------------------*/

void SCSetEntityRootAction (SC* psc, SCEntity* Entity, SCAction* RootAction)
{

    SCAction*  paction;
    List* ListParams;
    SCEntity* pEntity;

    if (!Entity) return;
    Entity->RootAction = RootAction;
    if (Entity->Type == SCOPERATOR)
    {
        paction = (SCAction *)Entity->Val;
        ListParams  = paction->ListParams;
        while (ListParams)
        {
            pEntity = (SCEntity *)ListParams->car;
            SCSetEntityRootAction(psc, pEntity, RootAction);
            ListParams = ListParams->cdr;
        }
    }
    return;
}

SCVariable* SCGetVariableFromPointer (SC* psc, void* val)
{
    List* plistvariables;
    SCVariable* pvariable;
    SCValue* Value;

    plistvariables        = psc->ListVariables;
    while (plistvariables)
    {
        pvariable = plistvariables->car;
        if (!pvariable->SetEntity)
        {
            Value = (SCValue *)pvariable->Value;
            if (Value->Val == val)
                return pvariable;
        }
        plistvariables = plistvariables->cdr;
    }
    return NULL;
}

int SCDefaultWhenDone (SCAction* paction, void* app)
{
    char Buf[5250];
    char Buf1[5250];
    SCEntity* pentity;
    SCValue* Value;
    LONG LineNumber = paction->LineNumber;
    if (OSStrcmp (paction->ActionClass->Name, "EXECUTE") == 0)
    {
        pentity = (SCEntity *)paction->ListParams->car;
        Value = (SCValue *)pentity->Val;
        sprintf (Buf, "Action Terminated : %s with Error : %ld With Value : ", (char*)Value->Val, paction->Error);
    }
    else
        if (OSStrcmp (paction->ActionClass->Name, "DEFUN") == 0)
        {
            sprintf (Buf, "Action Terminated : %s %s with Error : %ld With Value : ", paction->ActionClass->Name, paction->Section->Name, paction->Error);
            LineNumber = paction->Section->LineNumber;
        }
        else
            sprintf (Buf, "Action Terminated : %s with Error : %ld With Value : ", paction->ActionClass->Name, paction->Error);

    SCPrintValue (paction->ActionClass->SC, paction->Value, Buf1);
    strcat (Buf, Buf1);
    SCTrace (paction->ActionClass->SC, LineNumber, Buf);

    return 1;
}


int SCAffectEntityVarToMessageValue (SCAction* paction, void* app)
{
    MXMessage* pmessage = (MXMessage*)app;
    MXMessageClass* pclassmess = pmessage->MessageClass;
    MXObject* pobject;
    SC* psc = paction->ActionClass->SC;
    SCSection* psection;
    SCValue* Value;
    SCVariable* pvariable;
    List* ListVariables;
    int i, NbObjects;
    Value = SCGetParamValue (psc, paction, 1);

    psection = SCIsSectionDefinedInList(psc->ListSections, (char*)Value->Val);
    if (!psection)  return 1;

    /* first variable is the com */

    ListVariables = psection->ListVariables->cdr;
    i =  0;

    NbObjects = ListNbElt (pclassmess->Objects);
    while (ListVariables && (i <= NbObjects))
    {
        pvariable = (SCVariable*)ListVariables->car;
        Value = pvariable->Value;
        pobject = (MXObject *)ListGetElt (pclassmess->Objects, i);

        switch(Value->Type)  {
            case SCSTRING :
            case SCDATE   :
            case SCTIME   :
            case SCCHAR   :
                MXSetValue (pmessage, pobject->Name , 1, Value->Val);
                break;
            case SCNULL :
                break;
            case SCINTEGER :
            case SCLONG :
            case SCFLOAT :
            case SCBOOLEAN :

                switch (pobject->Type) {
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
                    case MXFILE :
                    {
                        FILEPARM* Val = (FILEPARM *)Value->Val;
                        MXSetValue (pmessage, pobject->Name , 1, Val);
                        free (Val);
                    }
                }
            break;
        }

        ListVariables = ListVariables->cdr;
        i++;
    }
    return 1;
}

int SCAffectMessageValueToEntityVar (SCAction* paction, void* app)
{
    MXMessageClass* pmessclass;
    MXMessage* pmessage = (MXMessage *)app;
    WORD i;
    MXObject* pobject;
    SC* psc = paction->ActionClass->SC;
    SCValue* Value;
    FILEPARM* FileValue;
    BUFFERPARM* BufferValue;



    pmessclass = pmessage->MessageClass;

    for (i = 3; i <= paction->NbParam; i++)
    {
        pobject = (MXObject *)ListGetElt (pmessclass->Objects, i - 3);
        Value = SCGetParamValue (psc, paction, i);
        if (!Value) return 1;
        switch (pobject->Type) {
        case MXSHORT :
            SCAffectValue (Value,MXGetValue (pmessage, pobject->Name, 1), SCINTEGER);
            break;
        case MXWORD :
            SCAffectValue (Value, MXGetValue (pmessage, pobject->Name, 1),  SCINTEGER);
            break;
        case MXDWORD :
            SCAffectValue (Value, MXGetValue (pmessage, pobject->Name, 1),  SCLONG);
            break;
        case MXLONG :
            SCAffectValue (Value, MXGetValue (pmessage, pobject->Name, 1),  SCLONG);
            break;
        case MXCHAR :
        case MXBYTE :
            SCPutValue    (Value, (char *)MXGetValue (pmessage, pobject->Name, 1),SCCHAR);
            break;
        case MXSTRING :
            SCPutValue    (Value, (char *)MXGetValue (pmessage, pobject->Name, 1), SCSTRING);
            break;
        case MXFILE :
            FileValue = (FILEPARM *)MXGetValue (pmessage, pobject->Name, 1);
            SCAffectValue (Value, FileValue,  SCLONG);
            break;
        case MXBUFFER :
            BufferValue = (BUFFERPARM *)MXGetValue (pmessage, pobject->Name, 1);
            SCAffectValue (Value, BufferValue,  SCLONG);
            break;
        default:
            break;
        }
    }
    SCRemoveFunctionFromInstance (psc, paction, SCBEFORE, SCAffectMessageValueToEntityVar,  pmessage);

    return 1;
}


int SCOnConnect (MXCom* pcom, void* pappfield)
{
    SC* psc = (SC*)pappfield;
    MX* pmx = pcom->MX;

    if (pcom->Client->OnPort)  /* only receive function */
    {
        MXAddComCallBack(pmx,  pcom, DG_SYS,  "FileReply"          , MXONSEND,   SCFileReplyCommand,      psc);
        MXAddComCallBack(pmx,  pcom, DG_SYS,  "FileReply"          , MXONRECV,   SCFileReplyCommand,      psc);
        MXAddComCallBack(pmx,  pcom, FTP_SYS, "get_reply"          , MXONRECV,   SCGetReplyCommand,       psc);
        MXAddComCallBack(pmx,  pcom, FTP_SYS, "put_reply"          , MXONRECV,   SCPutReplyCommand,       psc);
        MXAddComCallBack(pmx,  pcom, DG_SYS,  "DirReply"           , MXONRECV,   SCDirReplyCommand,       psc);
        MXAddComCallBack(pmx,  pcom, DG_SYS,  "ListReply"          , MXONRECV,   SCListReplyCommand,      psc);
        MXAddComCallBack(pmx,  pcom, DG_SYS,  "GDirReply"          , MXONRECV,   SCGDirReplyCommand,      psc);
        MXAddComCallBack(pmx,  pcom, DL_SYS,  "Login"              , MXONRECV,   SYS_Login,               psc);
        MXAddComCallBack(pmx,  pcom, DL_SYS,  "LoginReply"         , MXONRECV,   SYS_LoginReply,          psc);
        MXAddComCallBack(pmx,  pcom, DL_SYS,  "ParseString"        , MXONRECV,   SYS_ParseString,         psc);
        MXAddComCallBack(pmx,  pcom, DL_SYS,  "ParseReply"         , MXONRECV,   SYS_ParseReply,          psc);
        MXAddComCallBack(pmx,  pcom, DL_SYS,  "ShareVariable"      , MXONRECV,   SYS_ShareVariable,       psc);
        MXAddComCallBack(pmx,  pcom, DL_SYS,  "ShareReply"         , MXONRECV,   SYS_ShareReply,          psc);
        MXAddComCallBack(pmx,  pcom, DL_SYS,  "GetEntityValue"     , MXONRECV,   SYS_GetEntityValueReply, psc);
        MXAddComCallBack(pmx,  pcom, DL_SYS,  "GetEntityValueReply", MXONRECV,   SYS_GetVariableValue,    psc);
        MXAddComCallBack(pmx,  pcom, DL_SYS,  "SetEntityValue"     , MXONRECV,   SYS_SetEntityValueReply, psc);
        MXAddComCallBack(pmx,  pcom, DL_SYS,  "RemoteDebug"        , MXONRECV,   SYS_RemoteDebug,         psc);

        MXAddEndingProcedure (pcom, SCOnBreakClose, pappfield);
        ListNewr (&psc->Communications, pcom);
        printf ("Connection OK\n");
    }
    return 1;
}



int SCOnBreakClose (MXCom* pcom, void* pappfield)
{

    SC* psc = (SC*)pappfield;
    /*
     SCAction* paction = NULL;
     SCVariable* psccom;

     while (paction = SCGetRootActionFromCom (psc,  pcom))
        SCDeleteAction (psc, paction);

     psccom = SCGetVariableFromPointer (psc, pcom);
     if (psccom)
      {
         SCSetValue (psc, psccom->Value, psccom->Value);
      }
     */
    ListRemove (&psc->Communications, pcom);
    printf ("Connection Closed\n");
    return 1;
}


int SCOnClose (MXCom* pcom, void* pappfield)
{

    SCAction* thisaction= (SCAction*)pappfield;
    SC* psc = thisaction->ActionClass->SC;
    /*  SCAction* paction = NULL;
      SCVariable* psccom;
      List* dependencylist;
      SCDependency* pdependency = NULL;


      while (paction = SCGetRootActionFromCom (psc,  pcom))
         if (thisaction != paction) 
          SCDeleteAction (psc, paction);
         else if (paction->Block) ListRemove (&psc->ListActions, paction);
              else ListRemove (&psc->ListWaitingActions, paction);


      psccom = SCGetVariableFromPointer (psc, pcom);

      dependencylist = psccom->Value->Dependency;
      while (dependencylist)
       {
          pdependency = (SCDependency *)dependencylist->car;
          pdependency->Identity = -1;
          dependencylist = dependencylist->cdr;
       }



      if (psccom)
       {
          SCSetValue (psc, psccom->Value, psccom->Value);
       }
     */
    ListRemove (&psc->Communications, pcom);
    /*  printf ("Connection Closed in OnClose \n");*/
    return 1;
}



int SCSetTraceProcedure(SC* psc, int (*funct)(char* , LONG LineNumber, void*), void* app)
{
    SCTraceProcedure* traceprocedure;

    if (psc->TraceProcedure) free (psc->TraceProcedure);
    traceprocedure = (SCTraceProcedure *) malloc (sizeof (SCTraceProcedure));

    if (!traceprocedure)  return -1;

    traceprocedure->ApplicationField = app;
    traceprocedure->TraceProcedure = funct;
    psc->TraceProcedure = traceprocedure;
    return 1;
}


void SCTrace (SC* psc, LONG LineNumber, char* buf)
{
    if (psc->TraceProcedure)
        psc->TraceProcedure->TraceProcedure (buf, LineNumber, psc->TraceProcedure->ApplicationField);
}




/*------------------------------------------------------------------------*/

int SCInsertBreakPoint (SC* psc, char* FileName, WORD LineNumber)
{
    List* BreakPoints = psc->BreakPoints;
    SCBreakPoint* BreakPoint;

    while (BreakPoints)
    {
        BreakPoint = (SCBreakPoint *)BreakPoints->car;
        if ((strcmp (BreakPoint->FileName, FileName) == 0) &&
                (BreakPoint->LineNumber == LineNumber))
        {
            ListRemove (&psc->BreakPoints, BreakPoint);
            return 1;
        }
        BreakPoints = BreakPoints->cdr;
    }

    BreakPoint = (SCBreakPoint *)malloc (sizeof (SCBreakPoint));
    strcpy (BreakPoint->FileName, FileName);
    BreakPoint->LineNumber = LineNumber;
    ListNewr (&psc->BreakPoints, BreakPoint);
    return 0;
}

void SCRemoveBreakPoints (SC* psc)
{
    SCBreakPoint* BreakPoint;

    while (psc->BreakPoints)
    {
        BreakPoint = (SCBreakPoint *)psc->BreakPoints->car;
        ListRemove (&psc->BreakPoints, BreakPoint);
        free (BreakPoint);
    }

}
