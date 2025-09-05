/* NetProg is an API in C language allowing an homogeneous programming of  */
/* communicating applications                                              */
/* Copyright (C) 2002,2003,2004,2005                                       */
/* Gabriel Jureidini                                                       */
/* Version 2.0 - 17 July 2005                                              */
/*-------------------------------------------------------------------------*/


#define _CRT_SECURE_NO_WARNINGS
#include "mxmanager.h"


/*=========================================================================*/
/* CLASS                                                                   */
/*=========================================================================*/


MXDialogClass * MXCreateDialogClass(MX* pmx, char* name, WORD code)
{
    MXDialogClass* Class;

    Class = MXGetDialogClassFromName (pmx, name);
    if (Class)     MXFreeDialogClass (pmx, Class);

    Class = MXGetDialogClassFromCode (pmx, code);
    if (Class)     MXFreeDialogClass (pmx, Class);


    Class = (MXDialogClass*)malloc (sizeof (MXDialogClass));
    if (!Class)
    {
        OSError ("TEDAG051", " (%s)", name);
        return NULL;
    }

    strcpy(Class->Name,name);
    Class->Code = code;
    Class->MessageClasses = NULL;
    Class->MX = pmx;
    ListNewr (&pmx->Classes, Class);

    return Class;
}



void MXFreeDialogClass (MX* pmx, MXDialogClass* pclass)
{
    while (pclass->MessageClasses)
    {
        MXMessageClass* pmessageclass = (MXMessageClass *)pclass->MessageClasses->car;
        ListRemove (&pclass->MessageClasses, pmessageclass);
        MXFreeMessageClass (pmx, pmessageclass);
    }
    free (pclass);
}



/*=========================================================================*/
/* MESSAGECLASS                                                            */
/*=========================================================================*/


MXMessageClass* MXCreateMessageClass(MX* pmx, MXDialogClass* pclass, char* name, WORD code, int nb_param, ...)
{
    va_list p_liste;
    int i;
    DWORD offset = 0;
    int ret;
    MXMessageClass* pclassmess;
    MXObject* pobject;
    pclassmess = MXGetMessageClassFromName (pclass, name);
    if (pclassmess)
        MXFreeObjectsFromMessageClass (pclassmess);
    else
    {
        pclassmess = (MXMessageClass *)malloc (sizeof (MXMessageClass));
    }

    strcpy(pclassmess->Name, name);
    pclassmess->Code            = code;
    pclassmess->Objects         = NULL;
    pclassmess->Size            = 0;
    pclassmess->ExecuteOutputs  = NULL;
    pclassmess->ExecuteInputs   = NULL;
    pclassmess->ExecuteAck      = NULL;
    pclassmess->BlockedOnInput  = FALSE;
    pclassmess->BlockedOnOutput = FALSE;
    pclassmess->Class           = pclass;


    va_start(p_liste,nb_param);
    for (i = 1; i <= nb_param; i++)
    {
        pobject = (MXObject*)malloc (sizeof (MXObject));
        if (!pobject) return NULL;
		
		pobject->MessageClass = pclassmess;

        pobject->Size = 1;
        ret = MXCheckType2 (va_arg(p_liste, char *));
        pobject->Type = ret;
        pobject->Size = va_arg(p_liste, int);
        strcpy(pobject->Name, va_arg(p_liste, char *));
        pobject->Offset = offset;
        switch (ret) {
        case MXSHORT :
            pobject->InternalType = OSSHORT;
            break;
        case MXLONG :
            pobject->InternalType = OSLONG;
            break;
        case MXWORD :
            pobject->InternalType = OSWORD;
            break;
        case MXDWORD :
            pobject->InternalType = OSDWORD;
            break;
        case MXDOUBLE :
            pobject->InternalType = OSDOUBLE;
            break;
        case MXBYTE :
            pobject->InternalType = OSBYTE;
            break;
        case MXFILE :
            pobject->InternalType = OSFILE ;
            break;
        case MXSTRING :
            pobject->InternalType = OSSTRING;
            break;
        case MXCHAR :
            pobject->InternalType = OSCHAR;
            break;
        case MXBUFFER :
            pobject->InternalType = OSBUFFER;
            break;
        }
        pobject->InternalLength = pobject->Size;
        pobject->InternalScale  = -1;
        offset += pobject->Size;
        ListNewr (&pclassmess->Objects, pobject);
    }

    pclassmess->Size = (WORD)offset;
    ListNewr (&pclass->MessageClasses, pclassmess);
    return pclassmess;
}



void MXFreeMessageClass(MX* pmx, MXMessageClass* pmessageclass)
{

    MXCallBack*  pexecuteprocedure;

    if (!pmessageclass) return;
    if (!pmessageclass->Class) return;

    ListRemove (&pmessageclass->Class->MessageClasses, pmessageclass);


    while (pmessageclass->ExecuteOutputs)
    {
        pexecuteprocedure = (MXCallBack *)pmessageclass->ExecuteOutputs->car;
        ListRemove (&pmessageclass->ExecuteOutputs, pexecuteprocedure);
        free (pexecuteprocedure);
    }
    while (pmessageclass->ExecuteInputs)
    {
        pexecuteprocedure = (MXCallBack *)pmessageclass->ExecuteInputs->car;
        ListRemove (&pmessageclass->ExecuteInputs, pexecuteprocedure);
        free (pexecuteprocedure);
    }
    while (pmessageclass->ExecuteAck)
    {
        pexecuteprocedure = (MXCallBack *)pmessageclass->ExecuteAck->car;
        ListRemove (&pmessageclass->ExecuteAck, pexecuteprocedure);
        free (pexecuteprocedure);
    }
    while (pmessageclass->Objects)
    {
        MXObject* pobject = (MXObject *)pmessageclass->Objects->car;
        ListRemove (&pmessageclass->Objects, pobject);
        free (pobject);
    }
    free (pmessageclass);
}



void MXFreeObject (MX* pmx, MXObject* pobject)
{

    ListRemove (&pobject->MessageClass->Objects, pobject);
    free (pobject);
}


void MXFreeObjectsFromMessageClass (MXMessageClass* pmessageclass)
{
    while (pmessageclass->Objects)
    {
        MXObject* pobject = (MXObject *)pmessageclass->Objects->car;
        MXFreeObject (pmessageclass->Class->MX, pobject);
    }
    pmessageclass->Objects         = NULL;
    pmessageclass->Size            = 0;

}

MXObject* MXAddObjectToMessageClass (MXMessageClass* pmess, char* typeAtt, int dim, char* nomAtt)
{
    MXObject* pobject;

    int ret= MXCheckType2 (typeAtt);
    int typecode = (ret > 10) ? ret - 10 : ret;

    pobject = (MXObject*)malloc (sizeof (MXObject));
    if (!pobject) return NULL;


    strcpy(pobject->Name,nomAtt);
    pobject->Type   = typecode;
    pobject->Size   = dim;
    pobject->Offset = pmess->Size;
    pobject->MessageClass = pmess;
    switch (ret) {
    case MXSHORT :
        pobject->InternalType = OSSHORT;
        break;
    case MXLONG :
        pobject->InternalType = OSLONG;
        break;
    case MXWORD :
        pobject->InternalType = OSWORD;
        break;
    case MXDWORD :
        pobject->InternalType = OSDWORD;
        break;
    case MXDOUBLE :
        pobject->InternalType = OSDOUBLE;
        break;
    case MXBYTE :
        pobject->InternalType = OSBYTE;
        break;
    case MXFILE :
        pobject->InternalType = OSFILE;
        break;
    case MXSTRING :
        pobject->InternalType = OSSTRING;
        break;
    case MXCHAR :
        pobject->InternalType = OSCHAR;
        break;
    case MXBUFFER :
        pobject->InternalType = OSBUFFER;
        break;
    }
    pobject->InternalLength = pobject->Size;
    pobject->InternalScale  = -1;
    pmess->Size += pobject->Size;
    ListNewr (&pmess->Objects, pobject);
    return pobject;
}



/*=========================================================================*/
/*COMCLASS                                                                 */
/*and error return; CONNECTION CLASS.....................................  */
/*=========================================================================*/




MXComClass* MXCreateComClass (MX* pmx, char* Name, WORD Code, int Protocol, BYTE FromTo, char* TableName)
{
    MXComClass *Class;

    if ((MXGetComClassFromName (pmx, Name)) ||
            (MXGetComClassFromCode (pmx, Code)))
        return NULL;

    Class = (MXComClass*)malloc (sizeof (MXComClass));
    if (!Class)
    {
        OSError ("TEDAG051", " (%s)", Name);
        return NULL;
    }
    if (TableName)
        strcpy(Class->TableName,TableName);
    else
        strcpy (Class->TableName, "");
    strcpy (Class->Name, Name);
    Class->Code = Code;
    Class->Protocol = Protocol;
    Class->FromToTranscod = FromTo;
    Class->ExecuteOutputs = NULL;
    Class->ExecuteInputs = NULL;
    Class->ExecuteAck = NULL;
    Class->ConnectionProcedures = NULL;
    Class->EndingProcedures = NULL;
    ListNewr (&pmx->ComClasses , Class);
    return Class;
}


void MXFreeComClass (MX* pmx, MXComClass* pcomclass)
{
    MXCallBack*  pexecuteprocedure;
    MXComCallBack*  pcomprocedure;
    while (pcomclass->ExecuteOutputs)
    {
        pexecuteprocedure = (MXCallBack *)pcomclass->ExecuteOutputs->car;
        ListRemove (&pcomclass->ExecuteOutputs, pexecuteprocedure);
        free (pexecuteprocedure);
    }

    while (pcomclass->ExecuteInputs)
    {
        pexecuteprocedure = (MXCallBack *)pcomclass->ExecuteInputs->car;
        ListRemove (&pcomclass->ExecuteInputs, pexecuteprocedure);
        free (pexecuteprocedure);
    }
    while (pcomclass->ExecuteAck)
    {
        pexecuteprocedure = (MXCallBack *)pcomclass->ExecuteAck->car;
        ListRemove (&pcomclass->ExecuteAck, pexecuteprocedure);
        free (pexecuteprocedure);
    }
    while (pcomclass->EndingProcedures)
    {
        pcomprocedure = (MXComCallBack *)pcomclass->EndingProcedures->car;
        ListRemove (&pcomclass->EndingProcedures, pcomprocedure);
        free (pcomprocedure);
    }
    while (pcomclass->ConnectionProcedures)
    {
        pcomprocedure = (MXComCallBack *)pcomclass->ConnectionProcedures->car;
        ListRemove (&pcomclass->ConnectionProcedures, pcomprocedure);
        free (pcomprocedure);
    }
    free (pcomclass);
}


void MXFreeComClassFromName (MX* pmx, char* Name)
{
    MXComClass* Class;
    if (Class = MXGetComClassFromName (pmx, Name))
    {
        ListRemove (&pmx->ComClasses , Class);
        MXFreeComClass (pmx, Class);
    }
}



/*=========================================================================*/
/* INSTANCES MESSAGE, PCOM,                                                */
/*=========================================================================*/




void MXFreeCom (MX* pmx, MXCom* pcom)
{
    MXMessage* pmessage;
    while (pcom->InputHistory)
    {
        pmessage =  (MXMessage*)pcom->InputHistory->car;
        ListRemove (&pcom->InputHistory, pmessage);
        MXFreeMessage (pmx, pmessage);
    }
    while (pcom->OutputHistory)
    {
        pmessage =  (MXMessage*)pcom->OutputHistory->car;
        ListRemove (&pcom->OutputHistory, pmessage);
        MXFreeMessage (pmx, pmessage);
    }

    free (pcom);

}




MXMessage* MXCreateMessage (MX* pmx, char* ClassName, char* MessClassName)
{

    MXDialogClass* pclass;
    MXMessageClass* pclassmessage;
    pclass = MXGetDialogClassFromName (pmx, ClassName);
    if (!pclass)
    {
        OSError ("TEDAG010", " (%s)", ClassName);
        return NULL;
    }
    pclassmessage = MXGetMessageClassFromName (pclass, MessClassName);
    if (!pclassmessage)
    {
        OSError ("TEDAG011", " (%s)", MessClassName);
        return NULL;
    }
    return MXAllocMessage (pclass, pclassmessage);

}


MXMessage* MXCreateMessageFromCodes (MX* pmx, WORD ClassCode, WORD MessageClassCode)
{
    MXDialogClass* pclass;
    MXMessageClass* pclassmessage;


    pclass = MXGetDialogClassFromCode (pmx, ClassCode);
    pclassmessage = MXGetMessageClassFromCodes (pmx, ClassCode, MessageClassCode);

    if ((!pclass) || (!pclassmessage))
    {
        OSError ("TEDAG018", " (%d, %d)", ClassCode, MessageClassCode);
        return NULL;
    }

    return MXAllocMessage (pclass, pclassmessage);
}






/*=========================================================================*/



/*




MXConnection* MXCreateConnection (MX* pmx, char* ConnectionClassName, char* ConnectionName, WORD Code)
{
  MXConnectionClass* pconnectioclass = MXGetConnectionClassFromName (pmx, ConnectionClassName);
  if (pconnectionclass) 
  {
    MXEntityClass* fromentityclass  = MXGetConnectionClassValue (pconnectionclass, "FROM", 1);
    MXEntityClass* toentityclass    = MXGetConnectionClassValue (pconnectionclass, "TO", 1);
    
  }

}



pmessage = MXPutMessage ("ComName", "ClassName", "MessName");
{

}



int MXSetComValue (MXCom* pcom, char* Name, void* Value)
{

}

*/




/*=========================================================================*/
/* SITE                                                                    */
/*=========================================================================*/

MXSite* MXGetSiteFromName (MX* pmx, char* sitename)
{
    MXManager* pmanager = pmx->Manager;
    MXSite* psite = NULL;
    List* listsites;

    listsites = pmanager->Sites;
    while (listsites)
    {
        psite = (MXSite *)listsites->car;
        if (OSStrcmp (psite->Name, sitename) == 0)
            return psite;
        listsites = listsites->cdr;
    }
    return NULL;
}

MXSite* MXGetSiteFromCode (MX* pmx, WORD code)
{
    MXManager* pmanager = pmx->Manager;
    MXSite* psite = NULL;
    List* listsites = pmanager->Sites;

    while (listsites)
    {
        psite = (MXSite *)listsites->car;
        if (psite->Code == code)
            return psite;
        listsites = listsites->cdr;
    }
    return NULL;
}


MXSite*  MXCreateSite (MX* pmx, char* sitename, WORD code)
{
    MXSite* psite;
    MXManager* pmanager = pmx->Manager;

    if (OSStrcmp (sitename, "") == 0) return NULL;

    psite = MXGetSiteFromCode (pmx, code);
    psite = MXGetSiteFromName (pmx, sitename);
    if (!psite)
    {
        psite = (MXSite *)malloc (sizeof (MXSite));
        if (!psite) return NULL;
        memset (psite, 0, sizeof (MXSite));
        ListNewr (&pmanager->Sites, psite);
    }
    psite->Code    = code;
    strcpy (psite->Name, sitename);
    return psite;
}


void MXFreeSite (MX* pmx, MXSite* psite)
{
    MXManager* pmanager = pmx->Manager;

    while (psite->Machines)
        ListRemove (&psite->Machines, psite->Machines->car);

    ListRemove (&pmanager->Sites, psite);
    free (psite);
}

/*=========================================================================*/
/* MACHINE                                                                 */
/*=========================================================================*/


MXMachine* MXGetMachineFromName (MX* pmx, char* machname)
{
    MXManager* pmanager = pmx->Manager;
    MXMachine* pmachine = NULL;
    List* listmachine;

    listmachine = pmanager->Machines;
    while (listmachine)
    {
        pmachine = (MXMachine *)listmachine->car;
        if (OSStrcmp (pmachine->Name, machname) == 0)
            return pmachine;
        listmachine = listmachine->cdr;
    }
    return NULL;
}

MXMachine* MXGetMachineFromCode (MX* pmx, WORD code)
{
    MXManager* pmanager = pmx->Manager;
    MXMachine* pmachine = NULL;
    List* listmachine = pmanager->Machines;

    while (listmachine)
    {
        pmachine = (MXMachine *)listmachine->car;
        if (pmachine->Code == code)
            return pmachine;
        listmachine = listmachine->cdr;
    }
    return NULL;
}

MXMachine* MXGetMachineFromAddress (MX* pmx, char* IPAddress)
{
    MXManager* pmanager = pmx->Manager;
    MXMachine* pmachine = NULL;
    List* listmachine = pmanager->Machines;

    while (listmachine)
    {
        pmachine = (MXMachine *)listmachine->car;
        if (strcmp (pmachine->AdressIP, IPAddress) == 0)
            return pmachine;
        listmachine = listmachine->cdr;
    }
    return NULL;
}

MXMachine* MXAddMachineToSite (MX* pmx, MXSite* psite, MXMachine* pmachine)
{
    if (!pmachine) return NULL;

    if (pmachine->Site)
        ListRemove (&pmachine->Site->Machines, pmachine);
    pmachine->Site = psite;
    if (psite) ListInsert (&psite->Machines, pmachine);
    return pmachine;
}



MXMachine*  MXCreateMachine (MX* pmx, char* machinename, WORD code, BYTE system, char* AddressIP, char* dname)
{
    MXManager* pmanager = pmx->Manager;
    MXMachine* pmachine;
    int Error = 0;

    if (OSStrcmp (machinename, "") == 0) return NULL;


    pmachine = MXGetMachineFromCode (pmx, code);
    pmachine = MXGetMachineFromName (pmx, machinename);
    if (!pmachine)
    {
        pmachine = (MXMachine *)malloc (sizeof (MXMachine));
        if (!pmachine) return NULL;
        memset (pmachine, 0, sizeof (MXMachine));
        ListNewr (&pmanager->Machines, pmachine);
    }

    strcpy (pmachine->Name, machinename);
    pmachine->Code             = code;
    pmachine->System           = system;
    if (dname)       strcpy (pmachine->AddrName, dname);
    if (AddressIP)   strcpy (pmachine->AdressIP, AddressIP);
    else
        OSGetAddrFromName (machinename, pmachine->AdressIP);
    return pmachine;
}



void MXFreeMachine (MX* pmx, MXMachine* pmachine)
{
    MXManager* pmanager = pmx->Manager;

    while (pmachine->Lads)
        MXFreeLad (pmx, (MXLad *)pmachine->Lads->car);

    while (pmachine->Applications)
        MXFreeApplication (pmx, (MXApplication *)pmachine->Applications->car);

    while (pmachine->DataBases)
        MXFreeDataBase (pmx, (MXDataBase *)pmachine->DataBases->car);

    while (pmachine->Journals)
        MXFreeJournal (pmx, (MXJournal *)pmachine->Journals->car);

    while (pmachine->Applications)
        MXFreeQueue (pmx, (MXQueue *)pmachine->Queues->car);

    if (pmachine->Site)
        ListRemove (&pmachine->Site->Machines, pmachine);

    ListRemove (&pmanager->Machines, pmachine);

    free (pmachine);
}


/*=========================================================================*/
/* LADCLASS                                                                */
/*=========================================================================*/


MXLadClass* MXGetLadClassFromName (MX* pmx, char* ladclassname)
{
    MXManager* pmanager = pmx->Manager;
    MXLadClass* pladclass = NULL;
    List* listlads = pmanager->LadClasses;

    while (listlads)
    {
        pladclass = (MXLadClass *)listlads->car;
        if (OSStrcmp (pladclass->Name, ladclassname) == 0)
            return pladclass;
        listlads = listlads->cdr;
    }
    return NULL;
}

MXLadClass* MXGetLadClassFromCode (MX* pmx, WORD code)
{
    MXManager* pmanager = pmx->Manager;
    MXLadClass* pladclass = NULL;
    List* listlads = pmanager->LadClasses;

    while (listlads)
    {
        pladclass = (MXLadClass *)listlads->car;
        if (pladclass->Code == code)
            return pladclass;
        listlads = listlads->cdr;
    }
    return NULL;
}



MXLadClass* MXCreateLadClass (MX* pmx, char* ladclassname, WORD code)
{
    MXManager* pmanager = pmx->Manager;
    MXLadClass* pladclass;

    if (OSStrcmp (ladclassname, "") == 0) return NULL;

    pladclass = MXGetLadClassFromCode (pmx, code);
    pladclass = MXGetLadClassFromName (pmx, ladclassname);
    if (!pladclass)
    {
        pladclass = (MXLadClass *)malloc (sizeof (MXLadClass));
        if (!pladclass) return NULL;
        memset (pladclass, 0, sizeof (MXLadClass));
        ListNewr (&pmanager->LadClasses, pladclass);
    }
    return pladclass;
}


void MXFreeLadClass (MX* pmx, MXLadClass* pladclass)
{
    MXManager* pmanager = pmx->Manager;
    while (pladclass->Lads)
        MXFreeLad (pmx, (MXLad*)pladclass->Lads->car);

    ListRemove (&pmanager->LadClasses, pladclass);
    free (pladclass);
}



/*=========================================================================*/
/* LAD                                                                     */
/*=========================================================================*/


MXLad* MXGetLadFromName (MX* pmx, char* ladname)
{
    MXManager* pmanager = pmx->Manager;
    MXLad* plad = NULL;
    List* listlads = pmanager->Lads;

    while (listlads)
    {
        plad = (MXLad *)listlads->car;
        if (OSStrcmp (plad->Name, ladname) == 0)
            return plad;
        listlads = listlads->cdr;
    }
    return NULL;
}


MXLad* MXGetLadFromCode (MX* pmx, WORD code)
{
    MXManager* pmanager = pmx->Manager;
    MXLad* plad = NULL;
    List* listlads = pmanager->Lads;

    while (listlads)
    {
        plad = (MXLad *)listlads->car;
        if (plad->Code == code)
            return plad;
        listlads = listlads->cdr;
    }
    return NULL;
}


MXLad*  MXCreateLad (MX* pmx, MXLadClass* pclass, char* ladname, WORD code, MXMachine* pmachine)
{
    MXManager* pmanager = pmx->Manager;
    MXLad* plad;

    if (!pmachine) return NULL;
    if (OSStrcmp (ladname, "") == 0) return NULL;


    plad = MXGetLadFromCode (pmx, code);
    plad = MXGetLadFromName (pmx, ladname);
    if (!plad)
    {
        plad = (MXLad *)malloc (sizeof (MXLad));
        if (!plad) return NULL;
        memset (plad, 0, sizeof (MXLad));
        ListNewr (&pmanager->Lads, plad);
    }
    else
    {
        ListRemove (&plad->Machine->Lads, plad);
    }
    strcpy (plad->Name, ladname);
    plad->Machine       = pmachine;
    plad->Class         = pclass;
    ListNewr (&pmachine->Lads , plad);

    return plad;
}


void MXFreeLad (MX* pmx, MXLad* plad)
{
    MXManager* pmanager = pmx->Manager;

    while (plad->Applications)
        MXFreeApplication (pmx, (MXApplication *)plad->Applications->car);

    while (plad->DataBases)
        MXFreeDataBase (pmx, (MXDataBase *)plad->DataBases->car);

    while (plad->Journals)
        MXFreeJournal (pmx, (MXJournal *)plad->Journals->car);

    while (plad->Queues)
        MXFreeQueue (pmx, (MXQueue *)plad->Queues->car);

    if (plad->Machine)
        ListRemove (&plad->Machine->Lads, plad);

    free (plad);
}

/*=========================================================================*/
/* APPLICATIONCLASS                                                        */
/*=========================================================================*/


MXApplicationClass* MXGetApplicationClassFromName (MX* pmx, char* appliname)
{
    MXManager* pmanager = pmx->Manager;
    MXApplicationClass* papplicationclass;
    List* listapplication = pmanager->ApplicationClasses;

    while (listapplication)
    {
        papplicationclass = (MXApplicationClass *)listapplication->car;
        if (OSStrcmp (papplicationclass->Name, appliname) == 0)
            return papplicationclass;
        listapplication = listapplication->cdr;
    }
    return NULL;
}


MXApplicationClass* MXGetApplicationClassFromCode (MX* pmx, WORD code)
{
    MXManager* pmanager = pmx->Manager;
    MXApplicationClass* papplicationclass;
    List* listapplication = pmanager->ApplicationClasses;
    while (listapplication)
    {
        papplicationclass = (MXApplicationClass *)listapplication->car;
        if (papplicationclass->Code == code)
            return papplicationclass;
        listapplication = listapplication->cdr;
    }
    return NULL;
}


MXApplicationClass*  MXCreateApplicationClass (MX* pmx, char* appliclassname, WORD code, BYTE type)
{
    MXManager* pmanager = pmx->Manager;
    MXApplicationClass* pappliclass;

    if (OSStrcmp (appliclassname, "") == 0) return NULL;

    pappliclass = MXGetApplicationClassFromCode (pmx, code);

    pappliclass = MXGetApplicationClassFromName (pmx, appliclassname);
    if (!pappliclass)
    {
        pappliclass = (MXApplicationClass *)malloc (sizeof (MXApplicationClass));
        memset (pappliclass, 0, sizeof (MXApplicationClass));
    }

    strcpy (pappliclass->Name, appliclassname);
    pappliclass->Type      = type;
    pappliclass->Code      = code;

    ListNewr (&pmanager->ApplicationClasses, pappliclass);
    return pappliclass;
}


void MXFreeApplicationClass (MX* pmx, MXApplicationClass* pappliclass)
{
    MXManager* pmanager = pmx->Manager;

    while (pappliclass->Applications)
    {
        MXFreeApplication (pmx, (MXApplication *)pappliclass->Applications->car);
    }

    ListRemove (&pmanager->ApplicationClasses, pappliclass);
    free (pappliclass);
}


/*=========================================================================*/
/* DATABASECLASS                                                           */
/*=========================================================================*/


MXDataBaseClass* MXGetDataBaseClassFromName (MX* pmx, char* classname)
{
    MXManager* pmanager = pmx->Manager;
    MXDataBaseClass* pclass = NULL;
    List* list = pmanager->DataBaseClasses;

    while (list)
    {
        pclass = (MXDataBaseClass *)list->car;
        if (OSStrcmp (pclass->Name, classname) == 0)
            return pclass;
        list = list->cdr;
    }
    return NULL;
}

MXDataBaseClass* MXGetDataBaseClassFromCode (MX* pmx, WORD code)
{
    MXManager* pmanager = pmx->Manager;
    MXDataBaseClass* pclass = NULL;
    List* list = pmanager->DataBaseClasses;

    while (list)
    {
        pclass = (MXDataBaseClass *)list->car;
        if (pclass->Code == code)
            return pclass;
        list = list->cdr;
    }
    return NULL;
}



MXDataBaseClass* MXCreateDataBaseClass  (MX* pmx, char* classname, WORD code, BYTE type)
{
    MXManager* pmanager = pmx->Manager;
    MXDataBaseClass* pclass = NULL;

    pclass = MXGetDataBaseClassFromCode (pmx, code);
    pclass = MXGetDataBaseClassFromName (pmx, classname);
    if (!pclass)
    {
        pclass = (MXDataBaseClass *)malloc (sizeof (MXDataBaseClass));
        memset (pclass, 0, sizeof (MXDataBaseClass));
        if (!pclass) return NULL;
        ListNewr (&pmanager->DataBaseClasses, pclass);
    }
    strcpy (pclass->Name, classname);
    pclass->Code               = code;
    pclass->Type               = type;

    return pclass;
}


void MXFreeDataBaseClass (MX* pmx, MXDataBaseClass* pdatabaseclass)
{
    MXManager* pmanager = pmx->Manager;

    while (pdatabaseclass->DataBases)
    {
        MXFreeDataBase (pmx, (MXDataBase *)pdatabaseclass->DataBases->car);
    }


    ListRemove (&pmanager->DataBaseClasses, pdatabaseclass);
    free (pdatabaseclass);
}


/*=========================================================================*/
/* QUEUECLASS                                                              */
/*=========================================================================*/

MXQueueClass* MXGetQueueClassFromName (MX* pmx, char* classname)
{
    MXManager* pmanager = pmx->Manager;
    MXQueueClass* pclass = NULL;
    List* list = pmanager->QueueClasses;

    while (list)
    {
        pclass = (MXQueueClass *)list->car;
        if (OSStrcmp (pclass->Name, classname) == 0)
            return pclass;
        list = list->cdr;
    }
    return NULL;
}

MXQueueClass* MXGetQueueClassFromCode (MX* pmx, WORD code)
{
    MXManager* pmanager = pmx->Manager;
    MXQueueClass* pclass = NULL;
    List* list = pmanager->QueueClasses;

    while (list)
    {
        pclass = (MXQueueClass *)list->car;
        if (pclass->Code == code)
            return pclass;
        list = list->cdr;
    }
    return NULL;
}



MXQueueClass* MXCreateQueueClass  (MX* pmx, char* classname, WORD code, BYTE type)
{
    MXManager* pmanager = pmx->Manager;
    MXQueueClass* pclass = NULL;

    pclass = MXGetQueueClassFromCode (pmx, code);
    pclass = MXGetQueueClassFromName (pmx, classname);
    if (!pclass)
    {
        pclass = (MXQueueClass *)malloc (sizeof (MXQueueClass));
        if (!pclass) return NULL;
        memset (pclass, 0, sizeof (MXQueueClass));
        ListNewr (&pmanager->QueueClasses, pclass);
    }
    strcpy (pclass->Name, classname);
    pclass->Code               = code;
    pclass->Type               = type;


    return pclass;
}


void MXFreeQueueClass (MX* pmx, MXQueueClass* pqueueclass)
{
    MXManager* pmanager = pmx->Manager;

    while (pqueueclass->Queues)
    {
        MXFreeQueue (pmx, (MXQueue *)pqueueclass->Queues->car);
    }

    ListRemove (&pmanager->QueueClasses, pqueueclass);
    free (pqueueclass);
}



/*=========================================================================*/
/* JOURNALCLASS                                                            */
/*=========================================================================*/




MXJournalClass* MXGetJournalClassFromName (MX* pmx, char* classname)
{
    MXManager* pmanager = pmx->Manager;
    MXJournalClass* pclass = NULL;
    List* list = pmanager->JournalClasses;

    while (list)
    {
        pclass = (MXJournalClass *)list->car;
        if (OSStrcmp (pclass->Name, classname) == 0)
            return pclass;
        list = list->cdr;
    }
    return NULL;
}

MXJournalClass* MXGetJournalClassFromCode (MX* pmx, WORD code)
{
    MXManager* pmanager = pmx->Manager;
    MXJournalClass* pclass = NULL;
    List* list = pmanager->JournalClasses;

    while (list)
    {
        pclass = (MXJournalClass *)list->car;
        if (pclass->Code == code)
            return pclass;
        list = list->cdr;
    }
    return NULL;
}


MXJournalClass* MXCreateJournalClass  (MX* pmx, char* classname, WORD code, BYTE format)
{
    MXManager* pmanager = pmx->Manager;
    MXJournalClass* pclass = NULL;

    pclass = MXGetJournalClassFromName (pmx, classname);
    pclass = MXGetJournalClassFromCode (pmx, code);
    if (!pclass)
    {
        pclass = (MXJournalClass *)malloc (sizeof (MXJournalClass));
        if (!pclass) return NULL;
        memset (pclass, 0, sizeof (MXJournalClass));
        ListNewr (&pmanager->JournalClasses, pclass);
    }
    strcpy (pclass->Name, classname);
    pclass->Code               = code;
    pclass->Format             = format;

    return pclass;
}


void MXFreeJournalClass (MX* pmx, MXJournalClass* pjournalclass)
{
    MXManager* pmanager = pmx->Manager;

    while (pjournalclass->Journals)
    {
        MXFreeJournal (pmx, (MXJournal *)pjournalclass->Journals->car);
    }

    ListRemove (&pmanager->JournalClasses, pjournalclass);
    free (pjournalclass);
}

/*=========================================================================*/
/* APPLICATION                                                             */
/*=========================================================================*/


MXApplication* MXGetApplicationFromName (MX* pmx, char* appliname)
{
    MXManager* pmanager = pmx->Manager;
    MXApplication* papplication = NULL;
    List* listmachines = pmanager->Machines;
    List* listlads     = pmanager->Lads;

    while (listmachines)
    {
        MXMachine* pmachine =  (MXMachine*)listmachines->car;
        List* listapplication = pmachine->Applications;
        while (listapplication)
        {
            papplication = (MXApplication *)listapplication->car;
            if (OSStrcmp (papplication->Name, appliname) == 0)
                return papplication;
            listapplication = listapplication->cdr;
        }
        listmachines = listmachines->cdr;
    }
    while (listlads)
    {
        MXLad* plad = (MXLad*)listlads->car;
        List* listapplication = plad->Applications;
        while (listapplication)
        {
            papplication = (MXApplication *)listapplication->car;
            if (OSStrcmp (papplication->Name, appliname) == 0)
                return papplication;
            listapplication = listapplication->cdr;
        }
        listlads = listlads->cdr;
    }

    return NULL;
}


MXApplication* MXGetApplicationFromCode (MX* pmx, WORD code)
{
    MXManager* pmanager = pmx->Manager;
    MXApplication* papplication = NULL;
    List* listmachines = pmanager->Machines;
    List* listlads     = pmanager->Lads;

    while (listmachines)
    {
        MXMachine* pmachine = (MXMachine*)listmachines->car;
        List* listapplication = pmachine->Applications;
        while (listapplication)
        {
            papplication = (MXApplication *)listapplication->car;
            if (papplication->Code == code)
                return papplication;
            listapplication = listapplication->cdr;
        }
        listmachines = listmachines->cdr;
    }
    while (listlads)
    {
        MXLad* plad = (MXLad*)listlads->car;
        List* listapplication = plad->Applications;
        while (listapplication)
        {
            papplication = (MXApplication *)listapplication->car;
            if (papplication->Code == code)
                return papplication;
            listapplication = listapplication->cdr;
        }
        listlads = listlads->cdr;
    }
    return NULL;
}


MXApplication*  MXCreateApplication (MX* pmx, MXApplicationClass* pclass, char* name, WORD code,  void* OnEntity, BYTE type)
{
    MXManager* pmanager = pmx->Manager;
    MXApplication* pappli;
    MXMachine* pmachine = (MXMachine *)OnEntity;
    MXLad*     plad = (MXLad *)OnEntity;

    pappli = MXGetApplicationFromCode (pmx, code);
    pappli = MXGetApplicationFromName (pmx, name);
    if (!pappli)
    {
        pappli = (MXApplication *)malloc (sizeof (MXApplication));
        if (!pappli) return NULL;
        memset (pappli, 0, sizeof (MXApplication));
        ListNewr (&pmanager->Applications, pappli);
    }
    else
    {
        if (pappli->Machine) ListRemove (&pappli->Machine->Applications, pappli);
        if (pappli->Lad) ListRemove (&pappli->Lad->Applications,     pappli);
        ListRemove (&pappli->Class->Applications, pappli);
    }

    strcpy (pappli->Name, name);

    pappli->Type   = type;
    pappli->Code   = code;
    pappli->Class  = pclass;

    if (type == MXONMACHINE)
    {
        ListInsert (&pmachine->Applications, pappli);
        pappli->Machine = pmachine;
    }
    else
    {
        ListInsert (&plad->Applications, pappli);
        pappli->Lad = plad;
    }
    ListInsert (&pclass->Applications, pappli);
    return pappli;
}


void MXFreeApplication (MX* pmx, MXApplication* pappli)
{
    MXManager* pmanager = pmx->Manager;
    MXApplicationClass* pclass = pappli->Class;

    if (pappli->Machine)
    {
        ListRemove (&pappli->Machine->Applications, pappli);
    }
    if (pappli->Lad)
    {
        ListRemove (&pappli->Lad->Applications, pappli);
    }

    ListRemove (&pclass->Applications, pappli);
    ListRemove (&pmanager->Applications, pappli);
    free (pappli);
}


/*=========================================================================*/
/* DATABASE                                                                */
/*=========================================================================*/


MXDataBase* MXGetDataBaseFromName (MX* pmx, char* name)
{
    MXManager* pmanager = pmx->Manager;
    MXDataBase* pdatabase = NULL;
    List* listmachines = pmanager->Machines;
    List* listlads     = pmanager->Lads;

    while (listmachines)
    {
        MXMachine* pmachine = listmachines->car;
        List* listdatabase = pmachine->DataBases;
        while (listdatabase)
        {
            pdatabase = (MXDataBase *)listdatabase->car;
            if (OSStrcmp (pdatabase->Name, name) == 0)
                return pdatabase;
            listdatabase = listdatabase->cdr;
        }
        listmachines = listmachines->cdr;
    }
    while (listlads)
    {
        MXLad* plad = listlads->car;
        List* listdatabase = plad->DataBases;
        while (listdatabase)
        {
            pdatabase = (MXDataBase *)listdatabase->car;
            if (OSStrcmp (pdatabase->Name, name) == 0)
                return pdatabase;
            listdatabase = listdatabase->cdr;
        }
        listlads = listlads->cdr;
    }
    return NULL;
}



MXDataBase* MXGetDataBaseFromCode (MX* pmx, WORD code)
{
    MXManager* pmanager = pmx->Manager;
    MXDataBase* pdatabase = NULL;
    List* listmachines = pmanager->Machines;
    List* listlads     = pmanager->Lads;

    while (listmachines)
    {
        MXMachine* pmachine = listmachines->car;
        List* listdatabase = pmachine->DataBases;
        while (listdatabase)
        {
            pdatabase = (MXDataBase *)listdatabase->car;
            if (pdatabase->Code == code)
                return pdatabase;
            listdatabase = listdatabase->cdr;
        }
        listmachines = listmachines->cdr;
    }
    while (listlads)
    {
        MXLad* plad = listlads->car;
        List* listdatabase = plad->DataBases;
        while (listdatabase)
        {
            pdatabase = (MXDataBase *)listdatabase->car;
            if (pdatabase->Code == code)
                return pdatabase;
            listdatabase = listdatabase->cdr;
        }
        listlads = listlads->cdr;
    }
    return NULL;
}



MXDataBase*  MXCreateDataBase (MX* pmx, MXDataBaseClass* pclass, char* name, WORD code, void* OnEntity, BYTE type)
{
    MXManager* pmanager = pmx->Manager;
    MXDataBase* pdatabase;
    MXMachine* pmachine = (MXMachine *)OnEntity;
    MXLad*     plad = (MXLad *)OnEntity;


    pdatabase = MXGetDataBaseFromCode (pmx, code);
    pdatabase = MXGetDataBaseFromName (pmx, name);
    if (!pdatabase)
    {
        pdatabase = (MXDataBase *)malloc (sizeof (MXDataBase));
        if (!pdatabase) return NULL;
        memset (pdatabase, 0, sizeof (MXDataBase));
        ListNewr (&pmanager->DataBases, pdatabase);
    }
    else
    {
        if (pdatabase->Machine) ListRemove (&pdatabase->Machine->DataBases, pdatabase);
        if (pdatabase->Lad)     ListRemove (&pdatabase->Lad->DataBases,     pdatabase);
        ListRemove (&pdatabase->Class->DataBases, pdatabase);
    }

    strcpy (pdatabase->Name, name);
    pdatabase->Type   = type;
    pdatabase->Code   = code;
    pdatabase->Class  = pclass;

    if (type == MXONMACHINE)
    {
        ListInsert (&pmachine->DataBases, pdatabase);
        pdatabase->Machine = pmachine;
    }
    else
    {
        ListInsert (&plad->DataBases, pdatabase);
        pdatabase->Lad = plad;
    }
    ListInsert (&pclass->DataBases, pdatabase);
    return pdatabase;
}


void MXFreeDataBase (MX* pmx, MXDataBase* pdatabase)
{
    MXManager* pmanager = pmx->Manager;
    MXDataBaseClass* pclass = pdatabase->Class;

    if (pdatabase->Machine)
    {
        ListRemove (&pdatabase->Machine->DataBases, pdatabase);
    }
    if (pdatabase->Lad)
    {
        ListRemove (&pdatabase->Lad->DataBases, pdatabase);
    }
    ListRemove (&pclass->DataBases  , pdatabase);
    ListRemove (&pmanager->DataBases, pdatabase);
    free (pdatabase);
}



/*=========================================================================*/
/* QUEUE                                                                   */
/*=========================================================================*/


MXQueue* MXGetQueueFromName (MX* pmx, char* name)
{
    MXManager* pmanager = pmx->Manager;
    List* listmachines = pmanager->Machines;
    List* listlads     = pmanager->Lads;
    MXQueue* pqueue = NULL;

    while (listmachines)
    {
        MXMachine* pmachine = listmachines->car;
        List* listqueue = pmachine->Queues;
        while (listqueue)
        {
            pqueue = (MXQueue *)listqueue->car;
            if (OSStrcmp (pqueue->Name, name) == 0)
                return pqueue;
            listqueue = listqueue->cdr;
        }
        listmachines = listmachines->cdr;
    }
    while (listlads)
    {
        MXLad* plad = listlads->car;
        List* listqueue = plad->Queues;
        while (listqueue)
        {
            pqueue = (MXQueue *)listqueue->car;
            if (OSStrcmp (pqueue->Name, name) == 0)
                return pqueue;
            listqueue = listqueue->cdr;
        }
        listlads = listlads->cdr;
    }
    return NULL;
}



MXQueue* MXGetQueueFromCode (MX* pmx, WORD code)
{
    MXManager* pmanager = pmx->Manager;
    MXQueue* pqueue = NULL;
    List* listmachines = pmanager->Machines;
    List* listlads     = pmanager->Lads;

    while (listmachines)
    {
        MXMachine* pmachine = listmachines->car;
        List* listqueue = pmachine->Queues;
        while (listqueue)
        {
            pqueue = (MXQueue *)listqueue->car;
            if (pqueue->Code == code)
                return pqueue;
            listqueue = listqueue->cdr;
        }
        listmachines = listmachines->cdr;
    }
    while (listlads)
    {
        MXLad* plad = listlads->car;
        List* listqueue = plad->Queues;
        while (listqueue)
        {
            pqueue = (MXQueue *)listqueue->car;
            if (pqueue->Code == code)
                return pqueue;
            listqueue = listqueue->cdr;
        }
        listlads = listlads->cdr;
    }
    return NULL;
}




MXQueue*  MXCreateQueue (MX* pmx, MXQueueClass* pclass, char* name, WORD code, void* OnEntity, BYTE type)
{
    MXManager* pmanager = pmx->Manager;
    MXQueue* pqueue;
    MXMachine* pmachine = (MXMachine *)OnEntity;
    MXLad*     plad = (MXLad *)OnEntity;

    pqueue = MXGetQueueFromCode (pmx, code);
    pqueue = MXGetQueueFromName (pmx, name);
    if (!pqueue)
    {
        pqueue = (MXQueue *)malloc (sizeof (MXQueue));
        if (!pqueue) return NULL;
        memset (pqueue, 0, sizeof (MXQueue));
        ListNewr (&pmanager->Queues, pqueue);
    }
    else
    {
        if (pqueue->Machine) ListRemove (&pqueue->Machine->Queues, pqueue);
        if (pqueue->Lad)     ListRemove (&pqueue->Lad->Queues,     pqueue);
        ListRemove (&pqueue->Class->Queues, pqueue);
    }

    strcpy (pqueue->Name, name);
    pqueue->Type   = type;
    pqueue->Code   = code;
    pqueue->Class = pclass;

    if (type == MXONMACHINE)
    {
        ListInsert (&pmachine->Queues, pqueue);
        pqueue->Machine = pmachine;
    }
    else
    {
        ListInsert (&plad->Queues, pqueue);
        pqueue->Lad = plad;
    }
    ListInsert (&pclass->Queues, pqueue);
    return pqueue;
}

void MXFreeQueue (MX* pmx, MXQueue* pqueue)
{
    MXManager* pmanager = pmx->Manager;
    MXQueueClass* pclass = pqueue->Class;

    if (pqueue->Machine)
    {
        ListRemove (&pqueue->Machine->Queues, pqueue);
    }
    if (pqueue->Lad)
    {
        ListRemove (&pqueue->Lad->Queues, pqueue);
    }
    ListRemove (&pclass->Queues, pqueue);
    ListRemove (&pmanager->Queues, pqueue);
    free (pqueue);
}





/*=========================================================================*/
/* JOURNAL                                                                 */
/*=========================================================================*/

MXJournal* MXGetJournalFromName (MX* pmx, char* name)
{
    MXManager* pmanager = pmx->Manager;
    List* listmachines = pmanager->Machines;
    List* listlads     = pmanager->Lads;
    MXJournal* pjournal = NULL;

    while (listmachines)
    {
        MXMachine* pmachine = listmachines->car;
        List* listjournal = pmachine->Journals;
        while (listjournal)
        {
            pjournal = (MXJournal *)listjournal->car;
            if (OSStrcmp (pjournal->Name, name) == 0)
                return pjournal;
            listjournal = listjournal->cdr;
        }
        listmachines = listmachines->cdr;
    }
    while (listlads)
    {
        MXLad* plad = listlads->car;
        List* listjournal = plad->Journals;
        while (listjournal)
        {
            pjournal = (MXJournal *)listjournal->car;
            if (OSStrcmp (pjournal->Name, name) == 0)
                return pjournal;
            listjournal = listjournal->cdr;
        }
        listlads = listlads->cdr;
    }
    return NULL;
}



MXJournal* MXGetJournalFromCode (MX* pmx, WORD code)
{
    MXManager* pmanager = pmx->Manager;
    MXJournal* pjournal = NULL;
    List* listmachines = pmanager->Machines;
    List* listlads     = pmanager->Lads;

    while (listmachines)
    {
        MXMachine* pmachine = listmachines->car;
        List* listjournal = pmachine->Journals;
        while (listjournal)
        {
            pjournal = (MXJournal *)listjournal->car;
            if (pjournal->Code == code)
                return pjournal;
            listjournal = listjournal->cdr;
        }
        listmachines = listmachines->cdr;
    }
    while (listlads)
    {
        MXLad* plad = listlads->car;
        List* listjournal = plad->Journals;
        while (listjournal)
        {
            pjournal = (MXJournal *)listjournal->car;
            if (pjournal->Code == code)
                return pjournal;
            listjournal = listjournal->cdr;
        }
        listlads = listlads->cdr;
    }
    return NULL;
}



MXJournal*  MXCreateJournal (MX* pmx, MXJournalClass* pclass, char* name, WORD code, void* OnEntity, BYTE type)
{
    MXManager* pmanager = pmx->Manager;
    MXJournal* pjournal;
    MXMachine* pmachine = (MXMachine *)OnEntity;
    MXLad*     plad = (MXLad *)OnEntity;

    pjournal = MXGetJournalFromCode (pmx, code);
    pjournal = MXGetJournalFromName (pmx, name);
    if (!pjournal)
    {
        pjournal = (MXJournal *)malloc (sizeof (MXJournal));
        if (!pjournal) return NULL;
        memset (pjournal, 0, sizeof (MXJournal));
        ListNewr (&pmanager->Journals, pjournal);
    }
    else
    {
        if (pjournal->Machine)  ListRemove (&pjournal->Machine->Journals, pjournal);
        if (pjournal->Lad)      ListRemove (&pjournal->Lad->Journals,     pjournal);
        ListRemove (&pjournal->Class->Journals, pjournal);
    }
    strcpy (pjournal->Name, name);
    pjournal->Type   = type;
    pjournal->Code   = code;
    pjournal->Class  = pclass;

    if (type == MXONMACHINE)
    {
        ListInsert (&pmachine->Journals, pjournal);
        pjournal->Machine = pmachine;
    }
    else
    {
        ListInsert (&plad->Journals, pjournal);
        pjournal->Lad = plad;
    }

    ListInsert (&pclass->Journals, pjournal);
    return pjournal;
}

void MXFreeJournal (MX* pmx, MXJournal* pjournal)
{
    MXManager* pmanager = pmx->Manager;
    MXJournalClass* pclass = pjournal->Class;
    if (pjournal->Machine)
    {
        ListRemove (&pjournal->Machine->Journals, pjournal);
    }
    if (pjournal->Lad)
    {
        ListRemove (&pjournal->Lad->Journals, pjournal);
    }

    ListRemove (&pclass->Journals, pjournal);
    ListRemove (&pmanager->Journals, pjournal);
    free (pjournal);
}

/*=========================================================================*/
/* END JOURNAL                                                             */
/*=========================================================================*/


/*=========================================================================*/
/* BEGIN CONNETION CLASS                                                   */
/*=========================================================================*/



MXConnectionClass* MXGetConnectionClassFromName (MX* pmx, char* Name)
{
    MXManager* pmanager = pmx->Manager;
    List* ConnectionClasses = pmanager->ConnectionClasses;


    while (ConnectionClasses)
    {
        MXConnectionClass* pconnectionclass = (MXConnectionClass *)ConnectionClasses->car;
        if (OSStrcmp (pconnectionclass->Name, Name) == 0)
            return pconnectionclass;
        ConnectionClasses = ConnectionClasses->cdr;
    }
    return NULL;
}

MXConnectionClass* MXGetConnectionClassFromCode (MX* pmx, WORD code)
{
    MXManager* pmanager = pmx->Manager;
    List* ConnectionClasses = pmanager->ConnectionClasses;

    while (ConnectionClasses)
    {
        MXConnectionClass* pconnectionclass = (MXConnectionClass *)ConnectionClasses->car;
        if (pconnectionclass->Code == code)
            return pconnectionclass;
        ConnectionClasses = ConnectionClasses->cdr;
    }
    return NULL;
}



MXConnectionClass* MXCreateConnectionClass (MX* pmx, char* name, WORD code, int protocol, BYTE FromTo, char* TableName, char* fromclassname, char* toclassname)
{
    MXManager* pmanager = pmx->Manager;
    MXConnectionClass*  Class;
    void* pfromapplicationclass;
    void* ptoentityclass     = NULL;

    pfromapplicationclass = MXGetApplicationClassFromName (pmx, fromclassname);
    if (!pfromapplicationclass)
        return NULL;

    switch (protocol)
    {
    case DBPROTO_ORACLE :
    case DBPROTO_ODBC   :
    case DBPROTO_SQLDS  :
        {
            ptoentityclass = MXGetDataBaseClassFromName (pmx, toclassname);
            if (!ptoentityclass)
                return NULL;
        }
        break;
    case IOPROTO_STORE  :
    case IOPROTO_FIOP  :
        {
            ptoentityclass  = MXGetJournalClassFromName (pmx, toclassname);
            if (!ptoentityclass)
                return NULL;;
        }
        break;
    case IPPROTO_ICMP :
    case IPPROTO_IP   :
    case IPPROTO_TCP  :
	case IPPROTO_MTT  :
    case IPPROTO_DG   :
    case IPPROTO_CB2A :
    case IPPROTO_UDP  :
    case IPPROTO_DNS  :
    case IPPROTO_HTTP :
    case IPPROTO_FTP  :
    case IPPROTO_SMTP :
    case IPPROTO_POP  :
    case IPPROTO_BC   :
        {
            ptoentityclass  = MXGetApplicationClassFromName (pmx, toclassname);
            if (!ptoentityclass)
                return NULL;
        }
        break;
    default :
        return NULL;
        break;
    }
    Class = MXGetConnectionClassFromCode (pmx, code);
    Class = MXGetConnectionClassFromName (pmx, name);
    if (!Class)
    {
        Class = (MXConnectionClass*)malloc (sizeof (MXConnectionClass));
        if (!Class)   return NULL;
        memset (Class, 0, sizeof (MXConnectionClass));
        ListNewr (&pmanager->ConnectionClasses , Class);
    }
    if (TableName)      strcpy(Class->TableName,TableName);
    else                strcpy (Class->TableName, "");

    Class->FromApplicationClass = pfromapplicationclass;
    strcpy (Class->Name, name);
    Class->ToEntityClass = ptoentityclass;
    Class->Code           = code;
    Class->Protocol       = protocol;
    Class->FromToTranscod = FromTo;
    return Class;
}


MXConnection* MXGetConnectionFromName (MX* pmx, char* Name)
{
    MXManager* pmanager = pmx->Manager;
    List* Connections = pmanager->Connections;

    while (Connections)
    {
        MXConnection* pconnection = (MXConnection *)Connections->car;
        if (OSStrcmp (pconnection->Name, Name) == 0)
            return pconnection;
        Connections = Connections->cdr;
    }
    return NULL;
}

MXConnection* MXGetConnectionFromCode (MX* pmx, WORD code)
{
    MXManager* pmanager = pmx->Manager;
    List* Connections = pmanager->Connections;

    while (Connections)
    {
        MXConnection* pconnection = (MXConnection *)Connections->car;
        if (pconnection->Code ==  code)
            return pconnection;
        Connections = Connections->cdr;
    }
    return NULL;
}



MXConnection* MXCreateConnection (MX* pmx, char* name, WORD code, MXConnectionClass* pclass, char* fromname, char* toname)
{
    MXManager* pmanager = pmx->Manager;
    MXConnection* Connection;
    MXApplication* pfromapplication;
    void* ptoentity;
    int protocol = pclass->Protocol;

    pfromapplication = MXGetApplicationFromName (pmx, fromname);
    if (!pfromapplication)
        return NULL;

    switch (protocol)
    {
    case DBPROTO_ORACLE :
    case DBPROTO_ODBC   :
    case DBPROTO_SQLDS  :
        {
            ptoentity = MXGetDataBaseFromName (pmx, toname);
            if (!ptoentity)
                return NULL;
        }
        break;
    case IOPROTO_STORE  :
    case IOPROTO_FIOP  :
        {
            ptoentity = MXGetJournalFromName (pmx, toname);
            if (!ptoentity)
                return NULL;
        }
        break;
    case IPPROTO_ICMP :
    case IPPROTO_IP   :
    case IPPROTO_TCP  :
    case IPPROTO_DG   :
    case IPPROTO_CB2A :
    case IPPROTO_UDP  :
    case IPPROTO_DNS  :
    case IPPROTO_HTTP :
    case IPPROTO_FTP  :
	case IPPROTO_MTT  :
    case IPPROTO_SMTP :
    case IPPROTO_POP  :
    case IPPROTO_BC   :
        {
            ptoentity = MXGetApplicationFromName (pmx, toname);
            if (!ptoentity)
                return NULL;
        }
        break;
    default :
        return NULL;
        break;
    }

    Connection = MXGetConnectionFromCode (pmx, code);
    Connection = MXGetConnectionFromName (pmx, name);
    if (!Connection)
    {
        Connection = (MXConnection*)malloc (sizeof (MXConnection));
        if (!Connection) return NULL;
        memset (Connection, 0, sizeof (MXConnection));
        ListNewr (&pmanager->Connections, Connection);
    }
    else
        ListRemove (&pclass->Connections, Connection);

    strcpy (Connection->Name, name);
    Connection->FromApplication = pfromapplication;
    Connection->ToEntity = ptoentity;
    Connection->FileData  = NULL;
    Connection->Connected = FALSE;
    Connection->Class = pclass;
    ListInsert (&pclass->Connections, Connection);
    return Connection;
}




MXFolder* MXGetFolderFromName (MX* pmx, MXApplication* pappli, char* Name)
{
    List* Folders = pappli->Folders;
    while (Folders)
    {
        MXFolder* folder = (MXFolder*)Folders->car;
        if (OSStrcmp (folder->Name, Name) == 0)
            return folder;
        Folders = Folders->cdr;
    }
    return NULL;

}


MXFolder*  MXCreateFolder (MX* pmx, MXApplication* pappli, char* Name, char* Extension)
{
    MXFolder* pfolder;

    pfolder = (MXFolder *)malloc (sizeof (MXFolder));
    memset (pfolder, 0, sizeof (MXFolder));
    sprintf (pfolder->Extension, "%s", Extension);
    sprintf (pfolder->Name, "%s", Name);
    ListNewr (&pappli->Folders, pfolder);
    return pfolder;
}


void MXFreeFolder (MX* pmx, MXFolder* pfolder)
{
    if (!pfolder) return;

    while (pfolder->Files)
    {
        MXFile* File = (MXFile *)pfolder->Files->car;
        MXFreeFile (pmx, File);
    }
    free (pfolder);
}


BYTE MXGetTypeFromFileName (char* filename)
{
    char extension[100];

    OSGetFileNameExtension (extension, filename);

    if (OSStrcmp (extension ,  MXPROJEXTENSION) == 0)
        return MXPROJFILE;
    if ((OSStrcmp (extension , MXCEXTENSION) == 0) ||
            (OSStrcmp (extension , MXCPLUSEXTENSION) == 0))
        return MXSOURCEFILE;
    if (OSStrcmp (extension ,  MXINCLUDEEXTENSION) == 0)
        return MXINCLUDEFILE;
    if (OSStrcmp (extension ,  MXDIALOGEXTENSION) == 0)
        return MXDIALOGFILE;
    if (OSStrcmp (extension ,  MXSCRIPTEXTENSION) == 0)
        return MXSCRIPTFILE;
    if (OSStrcmp (extension ,  MXTRANSCODEXTENSION) == 0)
        return MXTRANSCODFILE;
    if (OSStrcmp (extension ,  MXMAKEEXTENSION) == 0)
        return MXMAKEFILE;
    return MXOTHERFILE;
}


MXFile* MXGetFileFromName (MX* pmx,  char* filename, MXFolder* pfolder)
{
    MXFile* pfile = NULL;
    List* listfile;

    if (!pfolder)  return NULL;

    listfile = pfolder->Files;
    while (listfile)
    {
        pfile = (MXFile *)listfile->car;
        if (OSStrcmp (pfile->Name, filename) == 0)
            return pfile;
        listfile = listfile->cdr;
    }
    return NULL;
}



MXFile*  MXCreateFile (MX* pmx, MXFolder* folder, char* f_filename)
{

    MXMachine* pmachine = NULL;
    MXFile* pfile = NULL;
    BYTE Type;
    char filename[250];
    char repertory[250];

    if (OSStrcmp (f_filename, "") == 0) return NULL;

    Type = MXGetTypeFromFileName (f_filename);
    if (pfile = MXGetFileFromName (pmx, f_filename, folder));
    return pfile;
    OSGetOnlyName (f_filename, filename, repertory, 'D');


    pfile = (MXFile *)malloc (sizeof (MXFile));
    memset (pfile , 0, sizeof (MXFile));
    strcpy (pfile->Name, f_filename);
    pfile->Folder = folder;
    pfile->Type = Type;
    ListNewr (&folder->Files, pfile);
}


void MXFreeFile (MX* pmx, MXFile* pfile)
{
    ListRemove (&pfile->Folder->Files, pfile);
    free (pfile);
}



BYTE MXGetCodeFromSystem (char* Name)
{
    if      (OSStrcmp (Name, "UNIX")  ==0)   return MXSYSTEMUNIX;
    else if (OSStrcmp (Name, "WIN32") ==0)   return MXSYSTEMWIN32;
    else if (OSStrcmp (Name, "AS400") ==0)   return MXSYSTEMAS400;
    else if (OSStrcmp (Name, "HP-UX") ==0)   return MXSYSTEMHPUX;
    else if (OSStrcmp (Name, "VM")    ==0)   return MXSYSTEMVM;
    else if (OSStrcmp (Name, "OS2")   ==0)   return MXSYSTEMOS2;
    else if (OSStrcmp (Name, "VOS")   ==0)   return MXSYSTEMVOS;
    return 0;
}

char* MXGetSystemFromCode (BYTE system, char* Name)
{
    switch (system)
    {
    case MXSYSTEMUNIX :
        strcpy (Name, "UNIX");
        break;
    case  MXSYSTEMWIN32:
        strcpy (Name, "WIN32");
        break;
    case MXSYSTEMAS400 :
        strcpy (Name, "AS400");
        break;
    case MXSYSTEMHPUX  :
        strcpy (Name, "HP-UX");
        break;
    case MXSYSTEMVM :
        strcpy (Name, "VM");
        break;
    case MXSYSTEMOS2 :
        strcpy (Name, "OS2");
        break;
    case MXSYSTEMVOS :
        strcpy (Name, "VOS");
        break;
    default :
        return NULL;
        break;
    }
    return Name;
}

void* MXGetEntityFromString (MX* pmx, char* s, BYTE Type, char* EntityName)
{

    char name[100];
    int i = 0;
    int j = 0;

    memset (name, 0, 100);

    if (s[0] == 0) return NULL;
    while (s[i] != 0)
    {
        name[j] = s[i]; i++; j++;
    }

    if (Type == MXSITE)
    {
        strcpy (EntityName, name);
        if (strcmp (EntityName, "z__") == 0)
            return NULL;
        else
        {
           
            return MXGetSiteFromName (pmx, name);
        }
    }
    else
        if (Type == MXLAD)
        {
            strcpy (EntityName, name);
            if (strcmp (EntityName, "z__") == 0)
                return NULL;
            else
            {
                
                return MXGetLadFromName (pmx, name);
            }
        }
    if (Type == MXMACHINE)
    {
        strcpy (EntityName, name);
        if (strcmp (EntityName, "z__") == 0)
            return NULL ;
        else
        {
           
            return MXGetMachineFromName (pmx, name);
        }
    }
    if (Type == MXAPPLICATION)
    {
        strcpy (EntityName, name);
        if (strcmp (EntityName, "z__") == 0)
            return NULL;
        else
        {
           
            return MXGetApplicationClassFromName (pmx, name);
        }
    }
    return NULL;
}



MXCom* MXConnect (MX* pmx, char* ConnectionName)
{
    MXConnection*  pconnection = MXGetConnectionFromName (pmx, ConnectionName);
    MXConnectionClass* pconnectionclass;
    MXCom* pcom = NULL;

    if (!pconnection) return NULL;

    if (OSStrcmp (pconnection->FromApplication->Name, pmx->AppliName) != 0)
        return NULL;

    if (OSStrcmp (pmx->Manager->LocalMachineName, pconnection->FromApplication->Machine->Name) != 0)
        return NULL;

    pconnectionclass = pconnection->Class;

    if (!pconnectionclass->ComClass)
        pconnectionclass->ComClass = MXCreateComClass (pmx, pconnectionclass->Name, pconnectionclass->Code, pconnectionclass->Protocol, pconnectionclass->FromToTranscod, pconnectionclass->TableName);

    switch (pconnectionclass->Protocol)
    {
    case DBPROTO_ORACLE :
    case DBPROTO_ODBC   :
    case DBPROTO_SQLDS  :
        {
            MXDataBase* pdatabase = pconnection->ToEntity;
            pcom = MXOpenBase (pmx, pdatabase->BaseName, pconnection->UserName, pconnection->PassWord, pconnectionclass->Protocol, TRUE);
        }
        break;
    case IOPROTO_STORE  :
    case IOPROTO_FIOP  :
        {
            MXJournal* pjournal = pconnection->ToEntity;
            pcom = MXOpenFile (pmx, pconnection->FileName, pconnection->ReadMode, pconnectionclass->Protocol, 'D', 0);
            if (pcom && pjournal->Class && pjournal->Class->MessageClasses)
                MXSetIOMessageClass(pcom, (MXMessageClass*)pjournal->Class->MessageClasses->car);
        }
        break;
    case IPPROTO_ICMP :
    case IPPROTO_IP   :
    case IPPROTO_TCP  :
    case IPPROTO_DG   :
    case IPPROTO_CB2A   :
    case IPPROTO_UDP  :
    case IPPROTO_DNS  :
    case IPPROTO_HTTP :
    case IPPROTO_FTP  :
	case IPPROTO_MTT  :
    case IPPROTO_SMTP :
    case IPPROTO_POP  :
    case IPPROTO_BC   :
        {
            MXApplication* papplication = pconnection->ToEntity;
            MXMachine* pmachine;
            if (papplication->Lad)  pmachine = papplication->Lad->Machine;
            else                    pmachine = papplication->Machine;
            pcom = MXOpenTCP (pmx, pmachine->Name,  pconnection->Port, pconnectionclass->Protocol, NULL, NULL, TRUE);
        }
        break;
    }
    pconnection->Com = pcom;
    if (pcom) pcom->ComClass = pconnectionclass->ComClass;
    return pcom;
}



void MXDisconnect (MX* pmx, char* ConnectionName)
{
    MXConnection*  pconnection = MXGetConnectionFromName (pmx, ConnectionName);
    if (pconnection && pconnection->Com)
        MXCloseCom (pmx, pconnection->Com);
}



MXMessage* MXManagePut (MX* pmx, char* connectionname, char* classname, char* messageclassname)
{
    MXConnection*  pconnection = MXGetConnectionFromName (pmx, connectionname);
    MXCom* pcom;
    MXMessage* pmessage;
    if (!pconnection) return NULL;

    pcom = pconnection->Com;

    if (!pcom) pcom = MXConnect (pmx, connectionname);
    if (!pcom) return NULL;

    pmessage = MXCreateMessage (pmx, classname, messageclassname);
    if (!pmessage) return NULL;
    MXPutThisMessage (pcom, pmessage);
    return pmessage;
}


MXMessage* MXManageSend (MX* pmx, char* connectionname, char* classname, char* messageclassname)
{
    MXConnection*  pconnection = MXGetConnectionFromName (pmx, connectionname);
    MXCom* pcom;
    MXMessage* pmessage;
    if (!pconnection) return NULL;

    pcom = pconnection->Com;

    if (!pcom) pcom = MXConnect (pmx, connectionname);
    if (!pcom) return NULL;

    pmessage = MXCreateMessage (pmx, classname, messageclassname);
    if (!pmessage) return NULL;
    MXSend (pmx, pcom, pmessage);
    return pmessage;
}


