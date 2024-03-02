/* NetProg is an API in C language allowing an homogeneous programming of  */
/* communicating applications                                              */
/* Copyright (C) 2002,2003,2004,2005                                       */
/* Gabriel Jureidini                                                       */
/* Version 2.0 - 17 July 2005                                              */
/*-------------------------------------------------------------------------*/


#define _CRT_SECURE_NO_WARNINGS
#include "sc.h"                                                                       



SCAction* SCAllocAction(SC* psc, SCSection* section,  SCSysAction* ActionClass, BOOL Block)
{
    SCAction *pAction;

    pAction = (SCAction *)malloc(sizeof(SCAction));
    if (!pAction) return(NULL);

    pAction->ExecCondition    = NULL;
    pAction->Identity         = psc->LastActionIdentity++;
    pAction->Block            = Block;
    pAction->Persistency      = FALSE;
    pAction->Com              = NULL;
    pAction->ActionClass      = ActionClass;
    pAction->Section          = section;
    pAction->ListParams       = NULL;
    pAction->NbParam          = 0;
    pAction->Error            = 0;
    pAction->State            = SCNOTTODO;
    pAction->ExecuteBefore    = NULL;
    pAction->ExecuteWhenDone  = NULL;
    pAction->ExecuteCondition = TRUE;;
    pAction->Value = SCAllocValue (psc, NULL, SCPOINTER);
    return(pAction);
}

SCAction* SCCreateAction (SC* psc, char* name, WORD nb_param, ...)
{
    SCSysAction* psysaction = SCGetSysAction (psc, name);
    SCAction* paction;
    SCEntity* Entity;
    va_list p_liste;
    int i;
    SHORT ret;

    if (!psysaction)
    {
        SCError (psc, 6, NULL);
        return NULL;
    }

    paction = SCAllocAction(psc, NULL, psysaction, psysaction->Block);

    if (!paction)
    {
        SCError (psc, 1, NULL);
        return NULL;
    }

    paction->NbParam = (WORD)nb_param;

    va_start(p_liste, nb_param);
    for (i = 1; i <= nb_param; i++)
    {
        ret = va_arg(p_liste, int);
        Entity = SCAllocEntity (psc, va_arg(p_liste, char *), ret);
        if (!Entity)
        {
            SCFreeAction (paction);
            SCError (psc, 0, NULL);
            return NULL;
        }
        Entity->RootAction = paction;
        ListNewr (&paction->ListParams, Entity);
    }
    return paction;
}



SCAction* SCInsertAction (SC* psc, char* name, WORD nb_param, ...)
{
    SCSysAction* psysaction = SCGetSysAction (psc, name);
    SCAction* paction;
    SCEntity* Entity;
    va_list p_liste;
    int i;
    SHORT ret;

    if (!psysaction)
    {
        SCError (psc, 6, NULL);
        return NULL;
    }


    paction = SCAllocAction(psc, NULL, psysaction,  psysaction->Block);
    if (!paction)
    {

        SCError (psc, 1, NULL);
        return NULL;
    }
    if (psysaction->Block) ListNewr (&psc->ListActions , paction);
    else  ListNewr (&psc->ListWaitingActions , paction);

    paction->NbParam = (WORD)nb_param;

    va_start(p_liste, nb_param);
    for (i = 1; i <= nb_param; i++)
    {
        ret = va_arg(p_liste, int);
        Entity = SCAllocEntity (psc,  va_arg(p_liste, char *), ret);
        Entity->RootAction = paction;
        if (!Entity)
        {
            SCFreeAction (paction);
            SCError (psc, 0, NULL);
            return NULL;
        }
        ListNewr (&paction->ListParams, Entity);
    }
    return paction;
}



/*------------------------------------------------------------------------*/


SCSysAction* SCAllocSysAction (SC* psc, char* Name, BOOL Block,
                               SCPROCRESULT (*Procedure)(SC* psc,  SCAction* thisaction), int MinPar, int MaxPar , int NbrPar, int ReturnType)
{
    SCSysAction* sysaction;

    sysaction = (SCSysAction*) malloc (sizeof (SCSysAction));
    if (!sysaction) return NULL;

    sysaction->Name = (char *)malloc(strlen(Name) + 1);
    if (!sysaction->Name)
    {
        free (sysaction);
        return NULL;
    }
    sysaction->SC = psc;
    sysaction->ExecuteBefore = NULL;
    sysaction->ExecuteWhenDone = NULL;
    sysaction->ActionProcedure = Procedure;
    sysaction->Block = Block;
	sysaction->MinPar = MinPar;
	sysaction->MaxPar = MaxPar;
	sysaction->NbrPar = NbrPar;
	sysaction->ReturnType = ReturnType;
    strcpy(sysaction->Name, Name);
    return sysaction;
}


void SCFreeSysAction (SCSysAction* pSysAction)
{
    SCExecuteProcedure*  pexecuteprocedure;
    if (!pSysAction) return;
    if (pSysAction->Name)
        free(pSysAction->Name);

    while (pSysAction->ExecuteBefore)
    {
        pexecuteprocedure = (SCExecuteProcedure *)pSysAction->ExecuteBefore->car;
        ListRemove (&pSysAction->ExecuteBefore, pexecuteprocedure);
        free (pexecuteprocedure);
    }

    while (pSysAction->ExecuteWhenDone)
    {
        pexecuteprocedure = (SCExecuteProcedure *)pSysAction->ExecuteWhenDone->car;
        ListRemove (&pSysAction->ExecuteWhenDone, pexecuteprocedure);
        free (pexecuteprocedure);
    }
    free (pSysAction);
    return;
}

/*------------------------------------------------------------------------*/



SCSysAction* SCGetSysAction (SC* psc, char* name)
{
    List* l = psc->ListSystemActions;
    SCSysAction* psysaction = NULL;

    if (!l) return NULL;

    while (l)
    {
        psysaction = (SCSysAction*)l->car;

        if ((psysaction->Name) &&
                (OSStrcmp (psysaction->Name, name) == 0))
            return psysaction;
        l= l->cdr;
    }
    return NULL;
}

SCSysAction* SCGetAddedAction (SC* psc, char* name)
{
    List* l = psc->ListAddedActions;
    SCSysAction* psysaction = NULL;

    if (!l) return NULL;

    while (l)
    {
        psysaction = (SCSysAction*)l->car;

        if ((psysaction->Name) &&
                (OSStrcmp (psysaction->Name, name) == 0))
            return psysaction;
        l= l->cdr;
    }
    return NULL;
}


/*------------------------------------------------------------------------*/



/* !!!!!!!!!!!!!!IL FAUT LIBERE LA LISTE DES DEPENDANCES DES VARIABLES */

void SCFreeAction(SCAction *pAction)
{
    SCEntity *pEntity;
    SCExecuteProcedure*  pexecuteprocedure;

    if (!pAction) return;

    if (pAction->Persistency) return;
    if (pAction->ExecCondition)    SCFreeEntity (pAction->ExecCondition);

    while(pAction->ListParams)
    {
        pEntity = (SCEntity *)pAction->ListParams->car;
        ListRemove(&pAction->ListParams, pEntity);
        SCFreeEntity(pEntity);
    }
    while (pAction->ExecuteBefore)
    {
        pexecuteprocedure = (SCExecuteProcedure *)pAction->ExecuteBefore->car;
        ListRemove (&pAction->ExecuteBefore, pexecuteprocedure);
        free (pexecuteprocedure);
    }

    while (pAction->ExecuteWhenDone)
    {
        pexecuteprocedure = (SCExecuteProcedure *)pAction->ExecuteWhenDone->car;
        ListRemove (&pAction->ExecuteWhenDone, pexecuteprocedure);
        free (pexecuteprocedure);
    }
    if (pAction->Value) SCFreeValue (pAction->Value);
    free(pAction);
    return;
}



/*------------------------------------------------------------------------*/
/* copy action copies empty tree actions Values are undefined actions are */
/* to do                                                                  */

SCAction* SCCopyAction(SC* psc, SCAction* RootAction, SCAction* MeAction)
{
    SCAction* pAction;
    SCEntity *Entity, *Entitytemp;
    List*            ListParams;
    List*            plist;
    SCExecuteProcedure* pexecuteprocedure;
    SCExecuteProcedure* pexp;


    pAction = SCAllocAction(psc, MeAction->Section, MeAction->ActionClass, MeAction->Block);
    if (!pAction)
    {
        SCError (psc, 0, NULL);
        return(NULL);
    }

    /* don't copy persistency */
    /*  pAction->Identity         = MeAction->Identity;*/
    pAction->ExecCondition    = SCCopyEntity (psc, NULL, MeAction->ExecCondition);
    pAction->NbParam          = MeAction->NbParam;
    pAction->LineNumber       = MeAction->LineNumber;
    ListParams                = MeAction->ListParams;

    while (ListParams)
    {
        Entity = (SCEntity *)ListParams->car;

        if (!RootAction)
            Entitytemp = SCCopyEntity(psc, pAction, Entity);
        else
            Entitytemp = SCCopyEntity(psc, RootAction, Entity);

        Entitytemp->FatherAction = pAction;

        if (!Entitytemp)
        {
            SCError (psc, 0, NULL);
            return(NULL);
        }
        ListNewr (&pAction->ListParams, Entitytemp);
        ListParams = ListParams->cdr;
    }

    plist = MeAction->ExecuteBefore;
    while (plist)
    {
        pexp = (SCExecuteProcedure*)plist->car;
        pexecuteprocedure = (SCExecuteProcedure *) malloc (sizeof (SCExecuteProcedure));
        pexecuteprocedure->ApplicationField = pexp->ApplicationField;
        pexecuteprocedure->ExecuteProcedure = pexp->ExecuteProcedure;
        ListInsert (&pAction->ExecuteBefore, pexecuteprocedure);
        plist = plist->cdr;
    }

    plist = MeAction->ExecuteWhenDone;
    while (plist)
    {
        pexp = (SCExecuteProcedure*)plist->car;
        pexecuteprocedure = (SCExecuteProcedure *) malloc (sizeof (SCExecuteProcedure));
        pexecuteprocedure->ApplicationField = pexp->ApplicationField;
        pexecuteprocedure->ExecuteProcedure = pexp->ExecuteProcedure;
        ListInsert (&pAction->ExecuteWhenDone, pexecuteprocedure);
        plist = plist->cdr;
    }

    return(pAction);
}


void SCDeleteAction (SC* psc, SCAction* paction)
{
    if (!paction) return;
    if (paction->Block) ListRemove (&psc->ListActions, paction);
    else ListRemove (&psc->ListWaitingActions, paction);
    SCFreeAction (paction);
}


/*------------------------------------------------------------------------*/
/* ACTION IS DONE */

void SCExecuteBefore (SC* psc, SCAction* paction)
{

    SCExecuteProcedure* executeproc;
    List* plist;
    plist = paction->ActionClass->ExecuteBefore;
    while (plist)
    {
        executeproc = (SCExecuteProcedure *)plist->car;
        plist = plist->cdr;
        executeproc->ExecuteProcedure (paction, executeproc->ApplicationField);
    }
    plist = paction->ExecuteBefore;
    while (plist)
    {
        executeproc = (SCExecuteProcedure *)plist->car;
        plist = plist->cdr;
        executeproc->ExecuteProcedure (paction, executeproc->ApplicationField);
    }

    return;
}

void SCExecuteWhenDone (SC* psc, SCAction* paction)
{

    List* plist;
    SCExecuteProcedure* executeproc;
    plist = paction->ExecuteWhenDone;
    while (plist)
    {
        executeproc = (SCExecuteProcedure *)plist->car;
        plist = plist->cdr;
        executeproc->ExecuteProcedure (paction, executeproc->ApplicationField);
    }
    plist = paction->ActionClass->ExecuteWhenDone;
    while (plist)
    {
        executeproc = (SCExecuteProcedure *)plist->car;
        plist = plist->cdr;
        executeproc->ExecuteProcedure (paction, executeproc->ApplicationField);
    }

    if (psc->EndingProcedure) psc->EndingProcedure (paction);
    return;
}


void SCSetActionState (SC* psc, SCAction* paction, BYTE State, WORD Defined, LONG Error)
{

    paction->Error = Error;
    paction->State= State;
    paction->Value->Defined = Defined;

    SCExecuteWhenDone (psc, paction);
}

void SCInitAction(SC* psc, SCAction* MeAction)
{
    List* ListParams;
    SCEntity* pEntity;

    if (!MeAction) return;
    if (MeAction->State != SCNOTTODO) MeAction->State = SCTODO;
    ListParams  = MeAction->ListParams;
    while (ListParams)
    {
        pEntity = (SCEntity *)ListParams->car;

        SCInitEntity(psc, pEntity);
        ListParams = ListParams->cdr;
    }

    return;
}


SCAction* SCSearchActionInTreeAction (SCAction* paction, SCAction* saction)
{

    List*  ListParams;
    SCEntity* Entity;
    ListParams = paction->ListParams;

    if (paction == saction) return paction;

    while (ListParams)
    {
        Entity = (SCEntity *)ListParams->car;
        if (Entity->Type == SCOPERATOR)
            if (paction = SCSearchActionInTreeAction ((SCAction *)Entity->Val, saction))
                return paction;
        ListParams = ListParams->cdr;
    }
    return NULL;
}


SCAction* SCGetRootActionFromAction (SC* psc, SCAction* saction)
{
    List* plistaction;
    List* plistwaitaction;
    List* plistvariables;
    SCAction* paction;
    SCAction* raction;
    SCVariable* pvariable;

    plistaction           = psc->ListActions;
    plistwaitaction       = psc->ListWaitingActions;
    plistvariables        = psc->ListVariables;

    while (plistwaitaction)
    {
        paction = plistwaitaction->car;
        if (raction = SCSearchActionInTreeAction (paction, saction)) return paction;
        plistwaitaction = plistwaitaction->cdr;
    }

    while (plistaction)
    {
        paction = plistaction->car;
        if (raction = SCSearchActionInTreeAction (paction, saction)) return paction;
        plistaction = plistaction->cdr;
    }
    while (plistvariables)
    {
        pvariable = plistvariables->car;
        if ((pvariable->SetEntity) && (pvariable->SetEntity->Type == SCOPERATOR))
        {
            paction = (SCAction *)pvariable->SetEntity->Val;
            if (raction = SCSearchActionInTreeAction (paction, saction)) return pvariable->SetEntity->RootAction;
        }
        plistvariables = plistvariables->cdr;
    }

    return NULL;
}


/*------------------------------------------------------------------------*/

SCAction* SCSearchComInTreeAction (SCAction* paction, MXCom* pcom)
{

    List*  ListParams;
    SCEntity* Entity;
    ListParams = paction->ListParams;

    if (paction->Com == pcom) return paction;

    while (ListParams)
    {
        Entity = (SCEntity *)ListParams->car;
        if (Entity->Type == SCOPERATOR)
            if (paction = SCSearchComInTreeAction ((SCAction *)Entity->Val, pcom))
                return paction;
        ListParams = ListParams->cdr;
    }
    return NULL;
}




SCAction* SCGetRootActionFromCom (SC* psc, MXCom* pcom)
{
    List* plistaction;
    List* plistwaitaction;
    List* plistvariables;
    SCAction* paction;
    SCAction* raction;

    plistaction           = psc->ListActions;
    plistwaitaction       = psc->ListWaitingActions;
    plistvariables        = psc->ListVariables;

    while (plistwaitaction)
    {
        paction = plistwaitaction->car;
        if (raction = SCSearchComInTreeAction (paction, pcom)) return paction;
        plistwaitaction = plistwaitaction->cdr;
    }

    while (plistaction)
    {
        paction = plistaction->car;
        if (raction = SCSearchComInTreeAction (paction, pcom)) return paction;
        plistaction = plistaction->cdr;
    }
    return NULL;
}





SCAction* SCSearchIdentityInTreeAction (SCAction* paction, LONG Identity)
{

    List*  ListParams;
    SCEntity* Entity;
    ListParams = paction->ListParams;

    if (paction->Identity == Identity) return paction;

    while (ListParams)
    {
        Entity = (SCEntity *)ListParams->car;
        if (Entity->Type == SCOPERATOR)
            if (paction = SCSearchIdentityInTreeAction ((SCAction *)Entity->Val, Identity))
                return paction;
        ListParams = ListParams->cdr;
    }
    return NULL;
}


SCAction* SCGetActionFromIdentity (SC* psc, LONG Identity)
{
    List* plistaction;
    List* plistwaitaction;
    List* plistvariables;
    SCAction* paction;
    SCAction* raction;

    plistaction           = psc->ListActions;
    plistwaitaction       = psc->ListWaitingActions;
    plistvariables        = psc->ListVariables;

    while (plistwaitaction)
    {
        paction = plistwaitaction->car;
        if (raction = SCSearchIdentityInTreeAction (paction, Identity)) return raction;
        plistwaitaction = plistwaitaction->cdr;
    }

    while (plistaction)
    {
        paction = plistaction->car;
        if (raction = SCSearchIdentityInTreeAction (paction, Identity)) return raction;
        plistaction = plistaction->cdr;
    }
    /*  while (plistvariables)
       {
         pvariable = plistvariables->car;
         if ((pvariable->SetEntity) && (pvariable->SetEntity->Type == SCOPERATOR))
          {
            paction = (SCAction *)pvariable->SetEntity->Val;
            if (raction = SCSearchIdentityInTreeAction (paction, Identity)) return raction;
          }
         plistvariables = plistvariables->cdr;
       }
     */
    return NULL;
}



void SCSetBlocking (SC* psc, SCAction* paction, BOOL Block)
{
    if (!paction) return;
    if (paction->Block == Block) return;

    if (paction->Block)
        ListRemove (&psc->ListActions, paction);
    else
        ListRemove (&psc->ListWaitingActions, paction);

    paction->Block = Block;
    if (paction->Block)
        ListInsert (&psc->ListActions, paction);
    else
        ListInsert (&psc->ListWaitingActions, paction);
    return;

}



void SCAddFunctionToSystemActions (SC* psc,
                                   BYTE mode, int (*funct)(SCAction*, void*), void* applicationfield)
{
    List* lsysactions;
    SCSysAction* psysaction;

    lsysactions = psc->ListSystemActions;

    while (lsysactions)
    {
        psysaction = (SCSysAction *)lsysactions->car;
        SCAddFunction (psc, psysaction->Name, mode, funct, applicationfield);
        lsysactions = lsysactions->cdr;
    }

}

void SCRemFuncFromSystemActions (SC* psc,
                                 BYTE mode, int (*funct)(SCAction*, void*), void* applicationfield)
{
    List* lsysactions;
    SCSysAction* psysaction;

    lsysactions = psc->ListSystemActions;

    while (lsysactions)
    {
        psysaction = (SCSysAction *)lsysactions->car;
        SCRemoveFunction (psc, psysaction->Name, mode, funct, applicationfield);
        lsysactions = lsysactions->cdr;
    }

}
SCExecuteProcedure* SCFindExecuteProcedure (List* ExecuteList, int (*funct)(SCAction*, void*), void* applicationfield)
{
    int found = 0;
    List* plist = ExecuteList;
    SCExecuteProcedure* pexecuteprocedure = NULL;


    while ((found == 0) && (plist != NULL))
    {
        pexecuteprocedure = (SCExecuteProcedure*)plist->car;

        if ((pexecuteprocedure->ExecuteProcedure == funct) &&
                (pexecuteprocedure->ApplicationField == applicationfield))
            found = 1;
        else plist = plist->cdr;
    }
    return pexecuteprocedure;
}


int SCAddFunction (SC* psc, char* actionclassname,
                   BYTE mode, int (*funct)(SCAction*, void*), void* applicationfield)
{

    SCExecuteProcedure* executeprocedure;
    SCSysAction* pSysAction = SCGetSysAction (psc, actionclassname);
    if (!pSysAction) return -1;


    if (mode == SCBEFORE)
    {
        if ((executeprocedure = SCFindExecuteProcedure (pSysAction->ExecuteBefore, funct, applicationfield)) != NULL)
            return -1;
        else
        {
            executeprocedure = (SCExecuteProcedure *) malloc (sizeof (SCExecuteProcedure));
            if (!executeprocedure)
                return -1;

            executeprocedure->ApplicationField = applicationfield;
            executeprocedure->ExecuteProcedure = funct;
            ListInsert (&pSysAction->ExecuteBefore, executeprocedure);
        }
        return 1;
    }
    if (mode == SCAFTER)
    {
        if ((executeprocedure = SCFindExecuteProcedure (pSysAction->ExecuteWhenDone, funct, applicationfield)) != NULL)
            return -1;
        else
        {
            executeprocedure = (SCExecuteProcedure *) malloc (sizeof (SCExecuteProcedure));
            if (!executeprocedure)
                return -1;

            executeprocedure->ApplicationField = applicationfield;
            executeprocedure->ExecuteProcedure = funct;
            ListInsert (&pSysAction->ExecuteWhenDone, executeprocedure);
        }
        return 1;
    }
    return -1;
}

int SCRemoveFunction (SC* psc, char* actionclassname,
                      BYTE mode, int (*funct)(SCAction*, void*), void* applicationfield)
{
    SCExecuteProcedure* executeprocedure;
    SCSysAction* pSysAction = SCGetSysAction (psc, actionclassname);
    if (!pSysAction) return -1;

    if (mode == SCBEFORE)
    {
        if ((executeprocedure = SCFindExecuteProcedure (pSysAction->ExecuteBefore, funct, applicationfield)) == NULL)
            return -1;
        ListRemove (&pSysAction->ExecuteBefore, executeprocedure);
        free (executeprocedure);
        return 1;
    }
    if (mode == SCAFTER)
    {
        if ((executeprocedure = SCFindExecuteProcedure (pSysAction->ExecuteWhenDone, funct, applicationfield)) == NULL)
            return -1;
        ListRemove (&pSysAction->ExecuteWhenDone, executeprocedure);
        free (executeprocedure);
        return 1;
    }
    return -1;
}


/*------------------------------------------------------------------------*/



int SCAddFunctionToInstance (SC* psc, SCAction* paction,
                             BYTE mode, int (*funct)(SCAction*, void*), void* applicationfield)
{

    SCExecuteProcedure* executeprocedure;
    if (!paction) return -1;


    if (mode == SCBEFORE)
    {
        if ((executeprocedure = SCFindExecuteProcedure (paction->ExecuteBefore, funct, applicationfield)) != NULL)
            return -1;
        else
        {
            executeprocedure = (SCExecuteProcedure *) malloc (sizeof (SCExecuteProcedure));
            if (!executeprocedure)     return -1;

            executeprocedure->ApplicationField = applicationfield;
            executeprocedure->ExecuteProcedure = funct;
            ListInsert (&paction->ExecuteBefore, executeprocedure);
        }
        return 1;
    }
    if (mode == SCAFTER)
    {
        if ((executeprocedure = SCFindExecuteProcedure (paction->ExecuteWhenDone, funct, applicationfield)) != NULL)
            return -1;
        else
        {

            executeprocedure = (SCExecuteProcedure *) malloc (sizeof (SCExecuteProcedure));
            if (!executeprocedure)     return -1;

            executeprocedure->ApplicationField = applicationfield;
            executeprocedure->ExecuteProcedure = funct;
            ListInsert (&paction->ExecuteWhenDone, executeprocedure);
        }
        return 1;
    }
    return -1;
}


int SCRemoveFunctionFromInstance (SC* psc, SCAction* paction,
                                  BYTE mode, int (*funct)(SCAction*, void*), void* applicationfield)
{
    int found = 0;
    SCExecuteProcedure* executeprocedure = NULL;
    List* plist;

    if (!paction) return -1;
    if (mode == SCBEFORE)  plist = paction->ExecuteBefore;
    else                   plist = paction->ExecuteWhenDone;


    while ((found == 0) && (plist != NULL))
    {
        executeprocedure = (SCExecuteProcedure*)plist->car;

        if ((executeprocedure->ExecuteProcedure == funct) &&
                (executeprocedure->ApplicationField == applicationfield))
            found = 1;
        else plist = plist->cdr;
    }

    if (found)
        if (mode == SCBEFORE)
            ListRemove (&paction->ExecuteBefore, executeprocedure);
        else
            ListRemove (&paction->ExecuteWhenDone, executeprocedure);

    return 1;
}




/*------------------------------------------------------------------------*/

