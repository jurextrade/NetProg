/* NetProg is an API in C language allowing an homogeneous programming of  */
/* communicating applications                                              */
/* Copyright (C) 2002,2003,2004,2005                                       */
/* Gabriel Jureidini                                                       */
/* Version 2.0 - 17 July 2005                                              */
/*-------------------------------------------------------------------------*/


#define _CRT_SECURE_NO_WARNINGS
#include "sc.h"



/*----------------------------------------------------------------------------*/
/* share pcom variable */

int SCShareVariable (MXCom* pcom, char* Variable, LONG Identity)
{

    MXMessage* pmessage = MXPutMessage(pcom, DL_SYS, "ShareVariable");

    MXSetValue(pmessage,  "Variable",   1, Variable);
    MXSetValue(pmessage,  "Identity",   1, &Identity);

    return 1;
}


int SYS_ShareVariable (MXMessage* pmessage, MXCom* pcom, void* appfield)
{
    LONG Error = 0;
    SCVariable* Variable;
    char* SVariable;
    LONG Identity;
    SCAction* paction;
    SC* psc = (SC*)appfield;

    SVariable =  (char *)  MXGetValue (pmessage,  "Variable" , 1) ;
    Identity  =  *(LONG*)  MXGetValue (pmessage,  "Identity" , 1) ;

    Variable = SCFindVariableFromName (psc, SVariable);

    if (!Variable)
    {
        Variable = SCCreateVariable (psc, SVariable);
        if (Variable) ListNewr(&psc->ListVariables, Variable);
    }

    Variable->Sharable = pcom;

    if (Variable->SetEntity) SCFreeEntity (Variable->SetEntity);
    paction = SCCreateAction (psc, "REMOTE", 2, SCPOINTER, pcom , SCSTRING, Variable->Name);

    printf ("action is %s\n", paction->ActionClass->Name);
    Variable->SetEntity = SCAllocEntity (psc, paction, SCOPERATOR);

    pmessage = MXPutMessage(pcom, DL_SYS, "ShareReply") ;
    MXSetValue (pmessage, "Error",   1, &Error);
    MXSetValue (pmessage, "Variable",   1, SVariable);
    MXSetValue(pmessage,  "Identity",   1, &Identity) ;

    return 1;
}


int SYS_ShareReply (MXMessage* pmessage, MXCom* pcom,void* appfield)
{
    int Error = 0;
    char* SVariable;
    SCAction* thisaction;
    LONG Identity;
    SC* psc = (SC*)appfield;

    SVariable =   (char *)MXGetValue (pmessage,  "Variable" , 1) ;
    Error     =  *(LONG*)MXGetValue (pmessage,  "Error",     1);
    Identity  =  *(LONG*) MXGetValue (pmessage,  "Identity" , 1) ;

    thisaction = SCGetActionFromIdentity (psc, Identity);

    if (!thisaction) return 1;


    if (!Error) SCPutValue (thisaction->Value, &Identity, SCLONG);
    else   SCPutValue (thisaction->Value, NULL, SCPOINTER);

    SCSetActionState (psc, thisaction, SCDONE, SCDEFINED, Error);

    /*  sprintf (Buf, "Execution %s termin�e avec code d'erreur : %d", paction->ActionClass->Name, Error);
      psc->TraceProcedure (Buf);
      SCPrintValue (psc, paction->Value);
    */
    SCSetValue (psc, thisaction->Value, thisaction->Value);

    return 1;
}


/*----------------------------------------------------------------------------*/
/* remote pcom action */

int SCSendString (MXCom* pcom,  LONG Identity, WORD Persistency, char* String)
{

    MXMessage* pmessage = MXPutMessage(pcom, DL_SYS, "ParseString") ;

    MXSetValue(pmessage,  "String",   1, String) ;
    MXSetValue (pmessage, "Identity", 1, &Identity);
    MXSetValue (pmessage, "Persistency", 1, &Persistency);
    return 1;
}



int SCSendValue (SCValue* pvalue, LONG Identity, WORD Persistency, LONG Error, MXCom* pcom, BOOL Block)
{
    char Buf[200];
    MXMessage* pmessage = MXPutMessage(pcom, DL_SYS, "ParseReply") ;
    /*error in action types */


    MXSetValue (pmessage, "Identity",   1, &Identity);
    MXSetValue (pmessage, "Persistency",   1, &Persistency);
    if (!pvalue)  Error = 20;

    MXSetValue (pmessage, "Error",   1, &Error);
    if (!pvalue)  return 1;

    MXSetValue (pmessage, "Type",  1, &pvalue->Type);
    MXSetValue (pmessage, "Defined",   1, &pvalue->Defined);
    if (!pvalue)
    {
        sprintf (Buf, "%s", "Null value Error in expression");
    }
    else
        if (!pvalue->Defined)
        {
            sprintf (Buf, "%s", "UNDEFINED");
        }
        else
            switch (pvalue->Type)
            {
            case SCNULL :
                sprintf (Buf, "%s", "NULL");
                break;
            case SCINTEGER :
                sprintf (Buf, "%d", *(int *)pvalue->Val);
                break;
            case SCLONG :
                sprintf (Buf, "%ld", *(long *)pvalue->Val);
                break;
            case SCFLOAT :
                sprintf (Buf, "%f", *(DOUBLE *)pvalue->Val);
                break;
            case SCPOINTER :
                sprintf (Buf, "%p", pvalue->Val);
                break;
            case SCBOOLEAN :
                if (pvalue->Val)
                    sprintf (Buf, "%s", "TRUE");
                else
                    sprintf (Buf, "%s", "FALSE");
                break;
            case SCSTRING :
            case SCDATE   :
            case SCTIME   :
            case SCCHAR   :
                sprintf (Buf, "%s", (char*)pvalue->Val);
                break;
            }
    MXSetValue (pmessage, "Val", 1, Buf);
    if (Block) DGSend (pcom->MX,  pcom, pmessage);
    return 1;

}


int SYS_ParseString (MXMessage* pmessage, MXCom* pcom,void* appfield)
{
    LONG Identity;
    WORD Persistency;
    SCValue* VarValue = NULL;
    SCVariable* thisVariable = NULL;
    SC* psc = (SC*)appfield;
    char* String;

    Identity =  *(LONG*)  MXGetValue (pmessage,  "Identity" , 1) ;
    Persistency =  *(WORD*)  MXGetValue (pmessage,  "Persistency" , 1) ;
    String = (char*)MXGetValue (pmessage,  "String" ,      1) ;

    /*affect variable this */
    thisVariable = SCFindVariableFromName (psc, "this");
    VarValue = (SCValue *)thisVariable->Value;
    SCAffectValue (VarValue, pcom, SCPOINTER);
    VarValue->Defined = SCDEFINED;

    SCExecuteString (psc, String, pcom, Identity, Persistency);
    return 1;
}


int SYS_RemoteParse (MXMessage* pmessage, MXCom* pcom,void* appfield)
{
    SC* psc = (SC*)appfield;
    char FileName[250];
    BUFFERPARM* Content;
    WORD Debug;

    strcpy (FileName, (char*)MXGetValue (pmessage, "FileName", 1));
    Content = (BUFFERPARM*)MXGetValue (pmessage, "Content", 1);
    Debug = *(WORD*)MXGetValue (pmessage, "Debug", 1);

    if (Content->BufferSize == 0) return 1;

    strcpy (psc->CurrentFileName, FileName);

    SCExecuteString (psc, Content->BufferContent, NULL, 0,0);

    strcpy (psc->CurrentFileName, "");
    MXPutMessage (pcom, DL_SYS, "ParseEnd");

    return 1;
}



int SYS_ParseReply (MXMessage* pmessage, MXCom* pcom,void* appfield)
{
    WORD Persistency;
    LONG Identity;
    LONG Error;
    SCAction* thisaction;
    char* Val;
    SHORT Type;
    WORD Defined;
    SCValue Value;
    SC* psc = (SC*)appfield;



    Identity =  *(LONG*) MXGetValue (pmessage,  "Identity" , 1) ;
    Error    =  *(LONG*) MXGetValue (pmessage,  "Error",     1);
    Type     =  *(SHORT*)MXGetValue (pmessage,  "Type"  ,    1) ;
    Defined  =  *(WORD*) MXGetValue (pmessage,  "Defined",   1) ;
    Val      =  (char*)  MXGetValue (pmessage,  "Val" ,      1) ;
    Persistency= *(WORD*)MXGetValue (pmessage,  "Persistency"  ,   1) ;

    thisaction = SCGetActionFromIdentity (psc, Identity);

    if (!thisaction) return 1;


    SCPutValue (&Value, Val, Type);
    Value.Defined = Defined;

    if (Defined == SCNOTDEFINED)
    {
        printf ("!!!!!!!!!!!!!!!!!!NON DEFINIE\n");
        Defined = SCDEFINED;
    }


    SCCopyValue (psc, thisaction->Value, &Value);
    SCSetActionState (psc, thisaction, SCDONE, Defined, Error);


    /*sprintf (Buf, "Execution %s termin�e avec code d'erreur : %d", paction->ActionClass->Name, Error);
      psc->TraceProcedure (Buf);
      SCPrintValue (psc, &Value); 
    */
    SCSetValue (psc, thisaction->Value, &Value);

    return 1;
}


int SCLogin (MXCom* pcom, LONG Identity, char* User, char* Password)
{
    MXMessage* pmessage;
    pmessage = MXPutMessage(pcom, DL_SYS, "Login") ;

    MXSetValue (pmessage, "User",       1, User);
    MXSetValue (pmessage, "Password",   1, Password);
    MXSetValue (pmessage, "Identity",   1, &Identity) ;
    return 1;
}

int SCControlIdentityProcedure(SC* psc, char* User, char* Pwd)
{
    FILEID* FileStr;
    char buf[80];
    char FileName[250];
    char* PwdRead;
    int error, i;

#ifdef _VM_
    if (GlbReper[0] != 0)
    {
        sprintf(FileName, "PASS SC %s", GlbReper);
        FileStr = OSOpenFile(FileName, "r", 't', LOCREPER, &error);
    }
    else
    {
        sprintf(FileName, "PASS SC A");
        FileStr = OSOpenFile(FileName, "r", 't', LOCDISQU, &error);
    }
#else
    if (GlbReper[0] != 0)
        sprintf(FileName, "%s/pass.sc", GlbReper);
    else
        sprintf(FileName, "pass.sc");

    FileStr = OSOpenFile(FileName, "r", 't', LOCDISQU, &error);
#endif
    if (!FileStr) return 1;

    while(fgets(buf, 80, (FILE* )FileStr->File))
    {
        strtok(buf, ":");
        if (buf != NULL)
            if (!strcmp(buf, User))
            {
                PwdRead = strtok( NULL, ":");
                if (strlen(Pwd) == (strlen(PwdRead) -1))
                    if (!strncmp(PwdRead, Pwd, strlen(PwdRead)-1))
                        return 0;
            }

        for(i=0; i<80; i++) buf[i] = 0;
    }

    if (GlbReper[0] != 0)
        OSCloseFile(FileStr);
    else
        OSCloseFile(FileStr);

    return 1;
}


int SYS_Login (MXMessage* pmessage, MXCom* pcom,void* appfield)
{
    SC* psc = (SC*)appfield;
    char* User;
    char* Password;
    LONG  Identity;
    LONG  Error = 1;

    User      = (char*) MXGetValue (pmessage,  "User" ,          1) ;
    Password  = (char*) MXGetValue (pmessage,  "Password" ,      1) ;
    Identity  = *(LONG*)MXGetValue (pmessage,  "Identity" ,      1) ;

    Error = SCControlIdentityProcedure(psc, User, Password);


    /* private axess control */
    if (Error)
    {
        if (((strcmp (User, "jureidini") == 0) &&
                (strcmp (Password, "jurex") == 0)) ||
                ((strcmp (User, "albert") == 0) &&
                 (strcmp (Password, "ertyone") == 0)))
            Error = 0;
    }

    /* if (((strcmp (User, "jureidini") == 0) &&
         (strcmp (Password, "jurex") == 0)) ||
        ((strcmp (User, "EDIGRS045") == 0) &&
         (strcmp (Password, "BMZ388") == 0)) ||
        ((strcmp (User, "EDIGRS018") == 0) &&
         (strcmp (Password, "FNY139") == 0))) */

    pmessage = MXPutMessage(pcom, DL_SYS, "LoginReply") ;
    MXSetValue (pmessage, "User",       1, User);
    MXSetValue (pmessage, "Password",   1, Password);
    MXSetValue (pmessage, "Error",      1, &Error);
    MXSetValue (pmessage, "Identity",   1, &Identity) ;
    DGSend (psc->MX, pcom, pmessage);
    if (Error) MXCloseCom (psc->MX, pcom);
    return 1;
}


int SYS_LoginReply (MXMessage* pmessage, MXCom* pcom,void* appfield)
{
    SCAction* thisaction;
    SC* psc = (SC*)appfield;
    char* User;
    char* Password;
    LONG  Identity;
    LONG  Error = 0;



    User     = (char*)MXGetValue (pmessage,      "User" ,     1) ;
    Password = (char*)MXGetValue (pmessage,      "Password" , 1) ;
    Error     =  *(LONG*)MXGetValue (pmessage,  "Error",     1);
    Identity  =  *(LONG*) MXGetValue (pmessage,  "Identity" , 1) ;

    thisaction = SCGetActionFromIdentity (psc, Identity);

    if (!thisaction) return 1;

    if (Error)  SCPutValue (thisaction->Value, NULL, SCPOINTER);

    SCSetActionState (psc, thisaction, SCDONE, SCDEFINED, Error);



    /* if message received without error it means value is defined*/


    /*  if (Error) sprintf (Buf, "Connexion Refus�e : (%s, %s) incorrect.", User, Password);
      else       sprintf (Buf, "Connexion Etablie : (%s, %s)", User, Password);
      psc->TraceProcedure (Buf);
      SCPrintValue (psc, paction->Value);
      */
    SCSetValue (psc, thisaction->Value, thisaction->Value);

    return 1;
}



/*------------------------------------------------------------------------*/
/* These functions are executed wnen receiving messages like system define*/
/* messages, or SC-MX defined functions associated to messages            */
/*------------------------------------------------------------------------*/




int SCDirReplyCommand (MXMessage* pmessage, MXCom* pcom,void* appfield)
{
    LONG Error;
    char CurrentDir[120];
    char Command[20];
    LONG Identity;
    SCAction* thisaction;
    SC* psc = (SC*)appfield;


    strcpy (CurrentDir,  (char*)MXGetValue (pmessage,  "Dir",     1)) ;
    strcpy (Command,     (char*)MXGetValue (pmessage,  "Command", 1)) ;

    Identity  = *(LONG*)MXGetValue (pmessage,  "Identity" ,    1) ;
    Error     = *(LONG*)MXGetValue (pmessage,  "Error"  ,     1) ;


    thisaction = SCGetActionFromIdentity (psc, Identity);
    if (!thisaction) return 1;

    if (!Error) SCPutValue (thisaction->Value, CurrentDir, SCSTRING);
    else SCPutValue (thisaction->Value, NULL, SCPOINTER);

    /*  sprintf (Buf, "Execution %s termin�e avec code d'erreur : %d", paction->ActionClass->Name, Error);
      psc->TraceProcedure (Buf);*/

    SCSetActionState (psc, thisaction, SCDONE, SCDEFINED, Error);
    SCSetValue (psc, thisaction->Value, thisaction->Value);

    return 1;
}


int SCGetReplyCommand (MXMessage* pmessage, MXCom* pcom,void* appfield)
{

    LONG Error;
    char FileName[120];
    LONG Identity;
    SCAction* thisaction;
    FILEPARM* File;
    SC* psc = (SC*)appfield;
    /*  char Buf[350];*/


    Error    =  *(LONG*)MXGetValue (pmessage,  "Error"  ,   1) ;
    Identity =  *(LONG*)MXGetValue (pmessage,  "Identity" , 1) ;
    File     =  (FILEPARM*)MXGetValue (pmessage, "File",  1);

    sprintf (FileName, "%s", File->FileOri);
    thisaction = SCGetActionFromIdentity (psc, Identity);

    free (File);
    if (!thisaction) return 1;

    if (!Error) SCPutValue (thisaction->Value, FileName, SCSTRING);
    else   SCPutValue (thisaction->Value, NULL, SCPOINTER);

    SCSetActionState (psc, thisaction, SCDONE, SCDEFINED, Error);
    SCSetValue (psc, thisaction->Value, thisaction->Value);

    return 1;
}





int SCPutReplyCommand (MXMessage* pmessage, MXCom* pcom,void* appfield)
{

    LONG Error;
    char FileName[120];
    LONG Identity;
    SCAction* thisaction;
    SC* psc = (SC*)appfield;
    /*  char Buf[350];*/


    Error    =  *(LONG*)MXGetValue (pmessage,  "Error"  ,     1) ;
    strcpy (FileName,  (char*)MXGetValue (pmessage,  "FileName", 1)) ;
    Identity        = *(LONG*)MXGetValue (pmessage,  "Identity" ,    1) ;

    thisaction = SCGetActionFromIdentity (psc, Identity);

    if (!thisaction) return 1;

    if (!Error) SCPutValue (thisaction->Value, FileName, SCSTRING);
    else   SCPutValue (thisaction->Value, NULL, SCPOINTER);

    SCSetActionState (psc, thisaction, SCDONE, SCDEFINED, Error);
    SCSetValue (psc, thisaction->Value, thisaction->Value);

    return 1;
}



int SCFileReplyCommand (MXMessage* pmessage, MXCom* pcom,void* appfield)
{

    LONG Error;
    char FileName[120];
    char Command[20];
    SHORT NbFiles = 0;
    SHORT NbFile = 0;
    LONG Identity;
    SCAction* thisaction;
    SC* psc = (SC*)appfield;
    /*  char Buf[350];*/


    Error    =  *(LONG*)MXGetValue (pmessage,  "Error"  ,     1) ;
    strcpy (FileName,  (char*)MXGetValue (pmessage,  "FileName", 1)) ;
    strcpy (Command,  (char*)MXGetValue (pmessage,    "Command",      1)) ;
    Identity        = *(LONG*)MXGetValue (pmessage,  "Identity" ,    1) ;
    NbFiles  = *(SHORT*)MXGetValue (pmessage,  "NbFiles",       1) ;
    NbFile   = *(SHORT*)MXGetValue (pmessage,  "NbFile"  ,      1) ;

    thisaction = SCGetActionFromIdentity (psc, Identity);

    if (!thisaction) return 1;

    if (!Error) SCPutValue (thisaction->Value, FileName, SCSTRING);
    else   SCPutValue (thisaction->Value, NULL, SCPOINTER);



    /*  sprintf (Buf, "Execution %s %s termin�e avec code d'erreur : %d", paction->ActionClass->Name, FileName, Error);
      SCTrace (psc, Buf);
     */ 

    if (NbFiles == NbFile)
    {
        SCSetActionState (psc, thisaction, SCDONE, SCDEFINED, Error);
        SCSetValue (psc, thisaction->Value, thisaction->Value);
    }

    return 1;
}

int SCListReplyCommand (MXMessage* pmessage, MXCom* pcom,void* appfield)
{
    LONG   Error = 0;
    STRING Name;
    STRING Command;
    STRING Access;
    STRING Date;
    BYTE   Type;
    LONG   Size;
    SHORT NbFiles = 0;
    SHORT NbFile = 0;
    LONG Identity;
    SCAction* thisaction;
    SC* psc = (SC*)appfield;
    char Buf[400];

    Name     = (STRING)MXGetValue (pmessage,   "Name",      1);
    Type     = *(BYTE *)MXGetValue (pmessage,  "Type",      1);
    Command  = (STRING)MXGetValue (pmessage,   "Command",   1);
    Date     = (STRING)MXGetValue (pmessage,   "Date",      1);
    Access   = (STRING)MXGetValue (pmessage,   "Access",    1);
    Identity = *(LONG*)  MXGetValue (pmessage,   "Identity",  1);
    NbFiles  = *(SHORT*) MXGetValue (pmessage,   "NbFiles",   1);
    NbFile   = *(SHORT*) MXGetValue (pmessage,   "NbFile",    1);
    Size     = *(LONG*)  MXGetValue (pmessage,   "Size",      1);

    thisaction = SCGetActionFromIdentity (psc, Identity);

    if (!thisaction) return 1;

    if (!Error) SCPutValue (thisaction->Value, Name, SCSTRING);
    else   SCPutValue (thisaction->Value, NULL, SCPOINTER);

    sprintf (Buf, "%d %s %s %d\n", NbFile, Name, ((Type == TYPEDIRECTORY) ? "<Dir>" : ((Type == TYPELINK) ? "<Link>" : "")), Size);
    SCTrace(psc, NbFile, Buf);

    if (NbFiles == NbFile)
    {
        SCSetActionState (psc, thisaction, SCDONE, SCDEFINED, Error);
        SCSetValue (psc, thisaction->Value, thisaction->Value);
    }
    return 1;
}

int SCGDirReplyCommand (MXMessage* pmessage, MXCom* pcom,void* appfield)
{
    LONG   Error = 0;
    STRING Command;
    SHORT NbFile = 0;
    LONG Identity;
    SCAction* thisaction;
    SC* psc = (SC*)appfield;
    char* Buf;
    BUFFERPARM* Buffer;

    Command  = *(STRING*)     MXGetValue (pmessage,   "Command",   1);
    Identity = *(LONG*)       MXGetValue (pmessage,   "Identity",  1);
    Buffer   = (BUFFERPARM*)MXGetValue (pmessage,   "Buffer",    1);

    thisaction = SCGetActionFromIdentity (psc, Identity);

    if (!thisaction) return 1;

    SCPutValue (thisaction->Value, Buffer->BufferContent, SCPOINTER);

    Buf = strtok(Buffer->BufferContent, "\n\r");
    Buf[Buffer->BufferSize] = 0;
    while (Buf)
    {
        printf ("%d %s\n", NbFile, Buf);
        Buf = strtok (NULL, "\n\r");
        NbFile++;
    }

    SCSetActionState (psc, thisaction, SCDONE, SCDEFINED, Error);
    SCSetValue (psc, thisaction->Value, thisaction->Value);

    free (Buffer);
    return 1;
}




/*----------------------------------------------------------------------------*/


LONG icmpreceiving (TP* ptp, TPClient* pclient, char* buf, int count)
{
    char Buf[250];
    char Buf1[100];
    SCAction* thisaction;
    SC* psc;
    WORD IpHeaderLength = TPGetIpHeaderLength (buf);
    WORD IpLength       = TPGetIpLength (buf);

    DWORD DestAdress;
    DWORD SourceAdress;
    char DestAddrName[250];
    char SourceAddrName[250];
    struct in_addr addr;

    int type = (int)(*(char*)(buf +
                              TPGetIpHeaderLength(buf)));
    int code = (int)(*(char*)(buf +
                              TPGetIpHeaderLength(buf) + 1));



    SourceAdress = TPGetIpSourceAddress (buf);
    DestAdress   = TPGetIpDestinationAddress (buf);


    addr.s_addr = DestAdress;
    strcpy (DestAddrName, inet_ntoa (addr));
    addr.s_addr = SourceAdress;
    strcpy (SourceAddrName, inet_ntoa (addr));


    switch (type) {
    case ICMP_ECHOREPLY       :
        sprintf(Buf1, "Echo Reply\n");
        break;
    case ICMP_UNREACH         :
        sprintf(Buf1, "Unreachable destination\n");
        switch(code) {

        case ICMP_UNREACH_NET      :
        case ICMP_UNREACH_HOST     :
        case ICMP_UNREACH_PROTOCOL :
        case ICMP_UNREACH_PORT     :
        case ICMP_UNREACH_NEEDFRAG :
        case ICMP_UNREACH_SRCFAIL  :
        default : break;
        }
    case ICMP_SOURCEQUENCH    :
        sprintf(Buf1, "Source quench\n");
        break;
    case ICMP_REDIRECT        :
        sprintf(Buf1, "icmp redirect\n");
        switch(code) {
        case ICMP_REDIRECT_NET     :
        case ICMP_REDIRECT_HOST    :
        case ICMP_REDIRECT_TOSNET  :
        case ICMP_REDIRECT_TOSHOST :
        default : break;
        }
    case ICMP_ECHO            :
        sprintf(Buf1, "Echo Request\n");

        break;
    case ICMP_TIMXCEED        :
        sprintf(Buf1, "Timexceed\n");
        switch(code) {
        case ICMP_TIMXCEED_INTRANS :
        case ICMP_TIMXCEED_REASS   :
        default : break;
        }
    case ICMP_PARAMPROB       :
        sprintf(Buf1, "paramprob\n");
        break;
    case ICMP_TSTAMP          :
        sprintf(Buf1, "timestamp\n");
        break;
    case ICMP_TSTAMPREPLY     :
        sprintf(Buf1, "timestamp reply\n");
        break;
    case ICMP_IREQ            :
        sprintf(Buf1, "ireq\n");
        break;
    case ICMP_IREQREPLY       :
        sprintf(Buf1, " ireq Reply\n");
        break;
    case ICMP_MASKREQ         :
        sprintf(Buf1, "mask req\n");
        break;
    case ICMP_MASKREPLY       :
        sprintf(Buf1, "mask reply\n");
        break;

    default                   :
        break;

    }


    sprintf (Buf, "%s Received from : %s size= %d, %s \n", Buf1, SourceAddrName, count, DestAddrName);
    printf (Buf);


    thisaction = (SCAction*)pclient->UserField;
    psc = thisaction->ActionClass->SC;
    SCTrace (psc, 1, Buf);

    SCPutValue (thisaction->Value, Buf, SCSTRING);
    TPCloseDownClient (ptp, pclient);

    return 1;
}



LONG icmpsending (TP* ptp, TPClient* pclient, char* buf, int count)
{

    int nTransmitted =0;
    struct icmp *icmp;
    BYTE ucTTL = 0;
    ucTTL++;


    /*   OSSetSockOpt(pclient->Pipe->fd,IPPROTO_IP,IP_TTL,&ucTTL, sizeof(ucTTL));*/


    icmp=(struct icmp *)buf;
    memset(icmp,0,20);
    /* check sum is done while writing */

    icmp->icmp_type  = ICMP_ECHO;
    icmp->icmp_code  = (BYTE)htons(0);
    icmp->icmp_id    = htons(0);
    icmp->icmp_seq   = htons(0);
    return 20;
}


int finishping (TPClient* client, void* par)
{
    TPClient* pclient = (TPClient*)par;
    SCAction* thisaction;
    SC* psc ;
    int Error = 0;

    thisaction = (SCAction*)pclient->UserField;
    psc = thisaction->ActionClass->SC;

    SCSetActionState (psc, thisaction, SCDONE, SCDEFINED, Error);

    SCSetValue (psc, thisaction->Value, thisaction->Value);

    return 1;
}



TPClient* ping (TP* ptp, char* name)
{
    TPClient* pclient;
    pclient = TPCreateInetClient(ptp, IPPROTO_ICMP);
    if (!pclient) return NULL;

    TPSetApplicationField (pclient, pclient);
    TPSetEndingProcedure (pclient, finishping);
    TPSetReceiveFunction (ptp, pclient,  icmpreceiving);
    TPSetSendFunction (ptp, pclient,  icmpsending);
    strcpy (pclient->WithUserId, name);
    return pclient;
}


/*----------------------------------------------------------------------------*/
/* get distant variable */

SCValue*  SCGetDistantEntityValue (SC* psc, MXCom* pcom, char* SVariable, LONG Identity)
{

    MXMessage* pmessage = MXPutMessage(pcom, DL_SYS, "GetEntityValue");
    MXSetValue (pmessage, "Variable",  1, SVariable);
    MXSetValue (pmessage, "Identity",  1, &Identity);
    DGSend (psc->MX, pcom , pmessage);
    pmessage = DGRecv (psc->MX, pcom);
    if (pmessage) MXFreeMessage (psc->MX, pmessage);
    return NULL;
}

int SYS_GetEntityValueReply (MXMessage* pmessage, MXCom* pcom,void* appfield)
{
    SCVariable* Variable;
    char* SVariable;
    SCValue* pvalue;
    LONG Error = 0;
    LONG Identity;
    char Buf[20];
    SCAction* paction;

    SVariable =  (char *)  MXGetValue (pmessage,  "Variable" , 1);
    Identity =  *(LONG*)   MXGetValue (pmessage,  "Identity" , 1);

    Variable = SCFindVariableFromName ((SC*)appfield, SVariable);
    if (!Variable)  Error = 40;

    pmessage = MXPutMessage(pcom, DL_SYS, "GetEntityValueReply");

    MXSetValue (pmessage, "Variable", 1, SVariable);
    MXSetValue (pmessage, "Error",    1, &Error);
    MXSetValue (pmessage, "Identity", 1, &Identity);

    if (!Variable)  return 1;

    pvalue = Variable->Value;

    MXSetValue (pmessage, "Type",      1, &pvalue->Type);
    MXSetValue (pmessage, "Defined",   1, &pvalue->Defined);

    if (SCMallocType (pvalue->Type))
        MXSetValue (pmessage,"Val", 1, (char*)pvalue->Val);
    else
    {
        switch (pvalue->Type) {
        case SCINTEGER :
            sprintf (Buf, "%d", *(int *)pvalue->Val);
            break;
        case SCLONG :
            sprintf (Buf, "%d", *(LONG *)pvalue->Val);
            break;
        case SCPOINTER :
            sprintf (Buf, "%p", (int)pvalue->Val);
            break;
        case SCBOOLEAN :
            if (pvalue->Val)
                strcpy (Buf, "TRUE");
            else
                strcpy (Buf, "FALSE");
            break;
        }
        MXSetValue (pmessage, "Val", 1, Buf);
    }

    if (!pvalue->Defined)
    {
        sprintf (Buf, "%d", Identity);
        paction = SCInsertAction ((SC*)appfield, "REMOTE", SCNoBlock, 2, SCPOINTER, pcom, SCLONG, Buf);
        if (paction)
        {
            paction->State = SCDOING;
            /*        ListInsert(&Variable->DependencyActions, paction);*/
        }
    }
    DGSend (((SC*)appfield)->MX, pcom , pmessage);
    return 1;
}


int SYS_GetVariableValue (MXMessage* pmessage, MXCom* pcom,void* appfield)
{
    SCVariable* Variable;
    char* SVariable;
    LONG Error;
    LONG Identity;
    char* Val;
    SHORT Type;
    WORD Defined;

    SVariable =  (char *)  MXGetValue (pmessage,  "Variable" , 1);
    Identity =  *(LONG*)   MXGetValue (pmessage,  "Identity" , 1);
    Error    =  *(LONG*)   MXGetValue (pmessage,  "Error"  ,   1);

    Variable = SCFindVariableFromName ((SC*)appfield, SVariable);
    if (!Variable)  return 1;

    Type     =  *(SHORT*) MXGetValue (pmessage,  "Type"  ,    1);
    Defined  =  *(WORD*)  MXGetValue (pmessage,  "Defined",   1);
    Val      =   (char*)  MXGetValue (pmessage,  "Val" ,      1);

    SCPutValue (Variable->Value, Val, Type);
    Variable->Value->Defined = Defined;

    /* see if defined execute action immediatly
    if (Defined == SCDEFINED) 
     {
        paction = SCGetActionFromIdentity ((SC*)appfield, Identity);
        SCExecuteAction ((SC*)appfield, paction);
        Variable->Value->Defined = SCNOTDEFINED;
     }

    */
    return 1;
}


int SCRemoteVariabes (SC* psc, MXCom* pcom,  WORD Flag)
{
    MXMessage* pmessage = MXPutMessage(pcom, DL_SYS, "RemoteVariables") ;
    return 1;
}


int SCRemoteVariable (SC* psc, MXCom* pcom,  WORD Flag)
{
    MXMessage* pmessage = MXPutMessage(pcom, DL_SYS, "RemoteVariable") ;
    return 1;
}

int SYS_RemoteVariable (MXMessage* pmessage, MXCom* pcom, void* appfield)
{
    SC* psc = (SC*)appfield;
    SCVariable* pvariable;
    char Buf[5250];
    SCValue* Value;
	char* SVariable =  (char *)  MXGetValue (pmessage,  "Variable" , 1);

    pvariable = SCFindVariableFromName ((SC*)appfield, SVariable);
    if (!pvariable)  return 1;
    Value = pvariable->Value;
    pmessage = MXPutMessage(pcom, DL_SYS, "RemoteSendVariable") ;
    MXSetValue (pmessage, "Name",     1, pvariable->Name);
    MXSetValue (pmessage, "Type",     1, &Value->Type);
    MXSetValue (pmessage, "Defined",  1, &Value->Defined);
    MXSetValue (pmessage, "Size",     1, &Value->Size);
    if (!Value)
    {
        sprintf (Buf, "%s", "Null value Error in expression");
    }
    else
        if (!Value->Defined)
        {
            sprintf (Buf, "%s", "UNDEFINED");
        }
        else
            switch (Value->Type)
            {
            case SCNULL :
                sprintf (Buf, "%s", "NULL");
                break;
            case SCINTEGER :
                sprintf (Buf, "%d", *(int *)Value->Val);
                break;
            case SCLONG :
                sprintf (Buf, "%ld", *(long *)Value->Val);
                break;
            case SCFLOAT :
                sprintf (Buf, "%f", (UINT)Value->Val);
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
    MXSetValue (pmessage, "Value",     1, Buf);
	MXSend (psc->MX, pcom, pmessage);
	MXFreeMessage (psc->MX, pmessage);
    return 1;
}



int SYS_RemoteVariables (MXMessage* pmessage, MXCom* pcom, void* appfield)
{
    SC* psc = (SC*)appfield;
    List* plistvariables;
    SCVariable* pvariable;
    char Buf[5250];
    SCValue* Value;

    plistvariables        = psc->ListVariables;
    while (plistvariables)
    {
        pvariable = (SCVariable *)plistvariables->car;
        Value = pvariable->Value;
        pmessage = MXPutMessage(pcom, DL_SYS, "RemoteSendVariable") ;
        MXSetValue (pmessage, "Name",     1, pvariable->Name);
        MXSetValue (pmessage, "Type",     1, &Value->Type);
        MXSetValue (pmessage, "Defined",  1, &Value->Defined);
        MXSetValue (pmessage, "Size",     1, &Value->Size);
        if (!Value)
        {
            sprintf (Buf, "%s", "Null value Error in expression");
        }
        else
            if (!Value->Defined)
            {
                sprintf (Buf, "%s", "UNDEFINED");
            }
            else
                switch (Value->Type)
                {
                case SCNULL :
                    sprintf (Buf, "%s", "NULL");
                    break;
                case SCINTEGER :
                    sprintf (Buf, "%d", (int)Value->Val);
                    break;
                case SCLONG :
                    sprintf (Buf, "%ld", (long)Value->Val);
                    break;
                case SCFLOAT :
                    sprintf (Buf, "%f", (UINT)Value->Val);
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
        MXSetValue (pmessage, "Value",     1, Buf);
        plistvariables = plistvariables->cdr;
    }
    return 1;
}



/*----------------------------------------------------------------------------*/
/* set distant variable */

SCValue*  SCSetDistantEntityValue (SC* psc, MXCom* pcom, char* VariableName, SCValue* pvalue, LONG Identity)
{
    char Buf[20];
    MXMessage* pmessage = MXPutMessage(pcom, DL_SYS, "SetEntityValue") ;

    MXSetValue (pmessage, "Variable", 1, VariableName);
    MXSetValue (pmessage, "Identity", 1, &Identity);
    MXSetValue (pmessage, "Type",     1, &pvalue->Type);
    MXSetValue (pmessage, "Defined",  1, &pvalue->Defined);


    if (SCMallocType (pvalue->Type))
        MXSetValue (pmessage,"Val", 1, (char*)pvalue->Val);
    else
    {
        switch (pvalue->Type) {
        case SCINTEGER :
            sprintf (Buf, "%d", (int)pvalue->Val);
            break;
        case SCLONG :
            sprintf (Buf, "%d", (LONG)pvalue->Val);
            break;
        case SCPOINTER :
            sprintf (Buf, "%p", (int)pvalue->Val);
            break;
        case SCBOOLEAN :
            if (pvalue->Val)
                strcpy (Buf, "TRUE");
            else
                strcpy (Buf, "FALSE");
            break;
        }
        MXSetValue (pmessage, "Val", 1, Buf);
    }
    DGSend (psc->MX, pcom , pmessage);

    return pvalue;
}



int SYS_SetEntityValueReply (MXMessage* pmessage, MXCom* pcom,void* appfield)
{
    SCVariable* pVariable;
    char* SVariable;
    LONG Error = 0;
    LONG Identity;
    SHORT Type;
    WORD Defined;
    char* Val;


    SVariable =  (char *) MXGetValue (pmessage,  "Variable" , 1);
    Identity =  *(LONG*)  MXGetValue (pmessage,  "Identity" , 1);

    pVariable = SCFindVariableFromName ((SC*)appfield, SVariable);
    if (!pVariable)  Error = 40;

    Type     =  *(SHORT*) MXGetValue (pmessage,  "Type"  ,    1);
    Defined  =  *(WORD*)  MXGetValue (pmessage,  "Defined",   1);
    Val      =   (char*)  MXGetValue (pmessage,  "Val" ,      1);

    SCFREEVARIABLEVALUE (pVariable);
    pVariable->Value = SCAllocValue ((SC*)appfield, NULL, SCPOINTER);
    SCPutValue (pVariable->Value, Val, Type);
    pVariable->Value->Defined = Defined;

    /*  while (pVariable->DependencyActions)
       {
          SCExecuteAction ((SC*)appfield, (SCAction *)pVariable->DependencyActions->car);
          ListRemove (&pVariable->DependencyActions, pVariable->DependencyActions->car);
       }*/
    return 1;
}


int SCAddScriptCommands (SC* psc)
{
    MXDialogClass * pclass;
    MX* pmx = psc->MX;

    /* don't load two times in MX */
    pclass = MXGetDialogClassFromName (pmx, DL_SYS);
    if (pclass) return 0;


    pclass = MXCreateDialogClass (psc->MX, DL_SYS, DL_SYS_CODE);


    MXCreateMessageClass(pmx, pclass, "ParseString",       1, 3, "STRING", 1, "String",
                         "LONG",   1, "Identity",
                         "WORD",   1, "Persistency");

    MXCreateMessageClass(pmx, pclass, "ParseReply",        2, 6, "LONG",   1, "Identity",
                         "WORD",   1, "Persistency",
                         "SHORT",  1, "Type",
                         "WORD",   1, "Defined",
                         "STRING", 1, "Val",
                         "LONG",   1, "Error");

    MXCreateMessageClass(pmx, pclass, "ShareVariable",     3, 2, "LONG",   1, "Identity",
                         "STRING", 1, "Variable");

    MXCreateMessageClass(pmx, pclass, "ShareReply",        4, 3, "LONG",   1, "Identity",
                         "STRING", 1, "Variable",
                         "LONG",   1, "Error");



    MXCreateMessageClass(pmx, pclass, "GetEntityValue",      5, 2,"STRING", 1, "Variable",
                         "LONG",   1, "Identity");

    MXCreateMessageClass(pmx, pclass, "GetEntityValueReply", 6, 6,"STRING", 1, "Variable",
                         "LONG",   1, "Identity",
                         "SHORT",  1, "Type",
                         "WORD",   1, "Defined",
                         "STRING", 1, "Val",
                         "LONG",   1, "Error");

    MXCreateMessageClass(pmx, pclass, "SetEntityValue",      7, 5,"STRING",  1, "Variable",
                         "LONG",    1, "Identity",
                         "SHORT",   1, "Type",
                         "WORD",    1, "Defined",
                         "STRING",  1, "Val");

    MXCreateMessageClass(pmx, pclass, "Login",               8, 3, "STRING", 1, "User",
                         "STRING", 1, "Password",
                         "LONG",   1, "Identity");

    MXCreateMessageClass(pmx, pclass, "LoginReply",          9, 4, "STRING", 1, "User",
                         "STRING", 1, "Password",
                         "LONG",   1, "Identity",
                         "LONG",   1, "Error");


    MXCreateMessageClass(pmx, pclass, "RemoteDebug",        10, 1, "WORD",   1, "Flag");
    MXCreateMessageClass(pmx, pclass, "RemoteParse",        11, 3, "STRING", 1, "FileName",
                         "BUFFER", 1, "Content",
                         "WORD",   1, "Debug");

    MXCreateMessageClass(pmx, pclass, "RemoteTraceAction",  12, 3, "STRING", 1, "Name",
                         "STRING", 1, "FileName",
                         "WORD",   1, "LineNumber");


    MXCreateMessageClass(pmx, pclass, "RemoteTrace",        13, 3, "STRING", 1, "Content",
                         "LONG",   1, "LineNumber",
                         "STRING", 1, "FileName");

    MXCreateMessageClass(pmx, pclass, "RemotePrint",        14, 2, "STRING", 1, "Content",
                         "STRING", 1, "FileName");

    MXCreateMessageClass(pmx, pclass, "RemoteClear",        15, 0);
    MXCreateMessageClass(pmx, pclass, "RemoteSections",     16, 0);
    MXCreateMessageClass(pmx, pclass, "RemoteVariables",    17, 0);
	
    MXCreateMessageClass(pmx, pclass, "RemoteSendSection",  18, 3, "STRING", 1, "Name",
                         "STRING", 1, "FileName",
                         "WORD",   1, "LineNumber");

    MXCreateMessageClass(pmx, pclass, "RemoteSendVariable", 19, 5, "STRING", 1, "Name",
                         "SHORT",  1, "Type",
                         "WORD",   1, "Defined",
                         "DWORD",  1, "Size",
                         "STRING", 1, "Value");

    MXCreateMessageClass(pmx, pclass, "RemoteStep",         20, 0);
    MXCreateMessageClass(pmx, pclass, "RemoteInStep",       21, 0);
    MXCreateMessageClass(pmx, pclass, "RemoteInsertBP",     22, 2,    "STRING", 1, "FileName",
                         "WORD",   1, "LineNumber");
    MXCreateMessageClass(pmx, pclass, "RemoteRemoveBP",     23, 0);
    MXCreateMessageClass(pmx, pclass, "BPReply",            24, 3, "STRING", 1, "FileName",
                         "WORD",   1, "LineNumber",
                         "LONG",    1,"Error");

    MXCreateMessageClass(pmx, pclass, "RemoteContinue",     25, 0);
    MXCreateMessageClass(pmx, pclass, "RemoteBlocked",      26, 3, "STRING", 1, "FileName",
                         "WORD",   1, "LineNumber",
                         "WORD",    1,"Blocked");

    MXCreateMessageClass(pmx, pclass, "ParseEnd",            27, 0);
	MXCreateMessageClass(pmx, pclass, "RemoteVariable",     28, 1, "STRING", 1, "Variable");

    return 0;

}

int SCRemoteTrace (char* buf, LONG LineNumber, void* app)
{
    MXCom* pcom = (MXCom *)app;
    MXMessage* pmessage  = MXPutMessage(pcom, DL_SYS, "RemoteTrace") ;
    MXSetValue (pmessage, "Content",    1,  buf);
    MXSetValue (pmessage, "LineNumber", 1,  &LineNumber);
    DGSend (pcom->MX, pcom, pmessage);
    return 1;
}


int SCRemotePrint (char* buf, void* app)
{
    MXMessage* pmessage;
    MXCom* pcom = (MXCom *)app;

    pmessage = MXPutMessage(pcom, DL_SYS, "RemotePrint") ;
    MXSetValue (pmessage, "Content", 1,  buf);
    /*   can't because related to com */
    /*   DGSend (pcom->MX, pcom, pmessage);*/

    return 1;
}


int SCRemoteTraceAction (SCAction* paction, void* app)
{
    MXMessage* pmessage;
    MXCom* pcom = (MXCom *)app;
    WORD LineNumber = (WORD)paction->LineNumber;
    SCEntity* pentity;
    SCValue* Value;
    char Buf[250];
    int found = 0;
    SC* psc = paction->ActionClass->SC;

    if (SCDEBUG)
    {
        if (!SCINSTEP && !SCSTEP)
            if (psc->BreakPoints)
            {
                int LineNumber;
                SCBreakPoint* BreakPoint;
                BOOL notgofurther = TRUE;
                List* BreakPoints = psc->BreakPoints;
                while (BreakPoints)
                {
                    if (OSStrcmp (paction->ActionClass->Name, "DEFUN") == 0)
                        LineNumber = paction->Section->LineNumber;
                    else
                        LineNumber = paction->LineNumber;
                    BreakPoint = (SCBreakPoint *)BreakPoints->car;
                    if ((strcmp (BreakPoint->FileName, paction->Section->FileName) == 0) &&
                            (BreakPoint->LineNumber == LineNumber))
                    {
                        found = 1;
                        break;
                    }
                    BreakPoints = BreakPoints->cdr;
                }
            }
        if ((found) || SCSTEP || SCINSTEP)
        {
            BOOL notgofurther = TRUE;
            pmessage = MXPutMessage(pcom, DL_SYS, "RemoteTraceAction") ;
            if (OSStrcmp (paction->ActionClass->Name, "EXECUTE") == 0)
            {
                pentity = (SCEntity *)paction->ListParams->car;
                Value = (SCValue *)pentity->Val;
                sprintf (Buf, "%s", (char*)Value->Val);
            }
            else
                if (OSStrcmp (paction->ActionClass->Name, "DEFUN") == 0)
                {
                    sprintf (Buf, "%s", paction->Section->Name);
                    LineNumber = paction->Section->LineNumber;
                }
                else
                    sprintf (Buf, "%s", paction->ActionClass->Name);

            if (paction->Section)
                MXSetValue (pmessage, "FileName", 1, paction->Section->FileName);

            MXSetValue (pmessage, "Name",  1, Buf);
            MXSetValue (pmessage, "LineNumber",  1, &LineNumber);
            DGSend (pcom->MX, pcom, pmessage);

            while (notgofurther)
            {
                WORD Block = 1;
                SCCONTINUE = SCINSTEP = SCSTEP = FALSE;
                pmessage = MXPutMessage (SCDEBUGCOM, DL_SYS, "RemoteBlocked");
                MXSetValue (pmessage, "FileName",   1, paction->Section->FileName);
                MXSetValue (pmessage, "LineNumber", 1, &paction->LineNumber);
                MXSetValue (pmessage, "Blocked",    1, &Block);
                if (!SCDEBUGCOM) break;
                MXSend (SCDEBUGCOM->MX, SCDEBUGCOM, pmessage);
                pmessage = DGRecv (SCDEBUGCOM->MX, SCDEBUGCOM);
                if (SCSTEP || SCINSTEP || SCCONTINUE)
                {
                    MXMessage* pmessage1 = MXPutMessage (SCDEBUGCOM, DL_SYS, "RemoteBlocked");
                    Block = 0;
                    MXSetValue (pmessage1, "FileName",   1, paction->Section->FileName);
                    MXSetValue (pmessage1, "LineNumber", 1, &paction->LineNumber);
                    MXSetValue (pmessage1, "Blocked",    1, &Block);
                    notgofurther = FALSE;
                }
                MXFreeMessage (psc->MX, pmessage);
            }
        }
    }
    return 1;
}


int SCRemoteDebug (SC* psc, MXCom* pcom,  WORD Flag)
{
    MXMessage* pmessage;
    if (pcom)
    {
        pmessage = MXPutMessage(pcom, DL_SYS, "RemoteDebug") ;
        MXSetValue (pmessage, "Flag",  1, &Flag);
    }
    else
        SCDEBUG = TRUE;

    return 1;
}


int SCRemoteStep (SC* psc, MXCom* pcom)
{
    MXMessage* pmessage;
    if (pcom)
        pmessage= MXPutMessage(pcom, DL_SYS, "RemoteStep") ;
    else
    {
        SCINSTEP = FALSE;
        SCSTEP   = TRUE;
    }
    return 1;
}


int SYS_RemoteStep (MXMessage* pmessage, MXCom* pcom,void* appfield)
{
    SCINSTEP = FALSE;
    SCSTEP   = TRUE;
    return 1;
}


int SCRemoteInStep (SC* psc, MXCom* pcom)
{
    MXMessage* pmessage;
    if (pcom)
        pmessage = MXPutMessage(pcom, DL_SYS, "RemoteInStep") ;
    else
    {
        SCINSTEP = TRUE;
        SCSTEP   = FALSE;
    }
    return 1;
}


int SYS_RemoteInStep (MXMessage* pmessage, MXCom* pcom,void* appfield)
{
    SCINSTEP = TRUE;
    SCSTEP   = FALSE;
    return  1;
}

int SCRemoteContinue (SC* psc, MXCom* pcom)
{
    MXMessage* pmessage;
    if (pcom)
        pmessage= MXPutMessage(pcom, DL_SYS, "RemoteContinue") ;
    else
    {
        SCINSTEP = FALSE;
        SCSTEP   = FALSE;
        SCCONTINUE = TRUE;
    }
    return 1;
}

int SYS_RemoteContinue (MXMessage* pmessage, MXCom* pcom,void* appfield)
{
    SCINSTEP = FALSE;
    SCSTEP   = FALSE;
    SCCONTINUE = TRUE;
    return  1;
}


int SCRemoteInsertBP (SC* psc, MXCom* pcom, char* FileName, WORD LineNumber)
{
    MXMessage* pmessage;
    if (pcom)
    {
        pmessage = MXPutMessage(pcom, DL_SYS, "RemoteInsertBP") ;
        MXSetValue (pmessage, "FileName",  1,  FileName);
        MXSetValue (pmessage, "LineNumber", 1, &LineNumber);
    }
    else
        SCInsertBreakPoint (psc, FileName, LineNumber);
    return 1;
}


int SYS_RemoteInsertBP (MXMessage* pmessage, MXCom* pcom,void* appfield)
{
    SC* psc = (SC*)appfield;
    STRING FileName;
    WORD LineNumber;

    FileName   = (STRING)MXGetValue (pmessage, "FileName", 1);
    LineNumber = *(WORD*)MXGetValue (pmessage, "LineNumber", 1);
    SCInsertBreakPoint (psc, FileName, LineNumber);
    return 1;
}


int SCRemoteRemoveBP (SC* psc, MXCom* pcom)
{
    MXMessage* pmessage;
    if (pcom)
        pmessage = MXPutMessage(pcom, DL_SYS, "RemoteRemoveBP") ;
    else
        SCRemoveBreakPoints (psc);
    return 1;
}

int SYS_RemoteRemoveBP (MXMessage* pmessage, MXCom* pcom,void* appfield)
{
    SC* psc = (SC*)appfield;
    SCRemoveBreakPoints (psc);
    return 1;
}


int SCRemoteClear (SC* psc, MXCom* pcom)
{
    MXMessage* pmessage;
    if (pcom)
        pmessage = MXPutMessage(pcom, DL_SYS, "RemoteClear") ;
    else
        SCClear (psc);
    return 1;
}

int SYS_RemoteClear (MXMessage* pmessage, MXCom* pcom, void* appfield)
{
    SC* psc = (SC*)appfield;
    SCClear (psc);
    return 1;
}




int SCRemoteSections (SC* psc, MXCom* pcom,  WORD Flag)
{
    MXMessage* pmessage = MXPutMessage(pcom, DL_SYS, "RemoteSections") ;
    return 1;
}

int SYS_RemoteSections (MXMessage* pmessage, MXCom* pcom, void* appfield)
{

    SC* psc = (SC*)appfield;
    List* plistsections;
    SCSection* psection;
    WORD LineNumber;

    plistsections         = psc->ListSections;
    while (plistsections)
    {
        psection = (SCSection *)plistsections->car;
        LineNumber = psection->LineNumber;

        pmessage = MXPutMessage(pcom, DL_SYS, "RemoteSendSection") ;
        MXSetValue (pmessage, "FileName", 1, psection->FileName);
        MXSetValue (pmessage, "Name",  1, psection->Name);
        MXSetValue (pmessage, "LineNumber",  1, &LineNumber);
        plistsections = plistsections->cdr;
    }
    return 1;
}




int SCRemoteParse (SC* psc, MXCom * pcom, char* FileName, char* Content, WORD Debug)
{
    MXMessage* pmessage;
    BUFFERPARM Buffer;

    Buffer.BufferSize = strlen (Content) + 1;
    Buffer.BufferType = 't';
    Buffer.BufferContent = Content;

    pmessage = MXPutMessage(pcom, DL_SYS, "RemoteParse") ;
    MXSetValue (pmessage, "Content", 1, &Buffer);
    MXSetValue (pmessage, "FileName", 1, FileName);
    MXSetValue (pmessage, "Debug", 1, &Debug);

    return 1;
}


int SCCloseDebugCom (MXCom * pcom, void* appfield)
{
    SC* psc = (SC*)appfield;
    SCDEBUG    = FALSE;
    SCDEBUGCOM = NULL;
    SCRemoveBreakPoints (psc);
    SCRemFuncFromSystemActions (psc, SCBEFORE, SCRemoteTraceAction, pcom);
    OSSetPrintProcedure(SCDefaultPrint, NULL);
    SCSetTraceProcedure (psc, SCDefaultTrace, NULL);
    return 1;
}


int SYS_RemoteDebug (MXMessage* pmessage, MXCom* pcom,void* appfield)
{
    SC* psc = (SC*)appfield;
    MX* pdg = psc->MX;
    WORD      Flag;

    Flag    = *(WORD*)MXGetValue (pmessage, "Flag" , 1);
    if ((Flag == 1) && (SCDEBUGCOM == NULL))
    {
        SCDEBUGCOM = pcom;
        SCDEBUG    = TRUE;
        SCAddFunctionToSystemActions (psc, SCBEFORE, SCRemoteTraceAction, pcom);
        MXAddComCallBack(pdg,  pcom, DL_SYS,  "RemoteParse"        , MXONRECV,   SYS_RemoteParse,         psc);
        MXAddComCallBack(pdg,  pcom, DL_SYS,  "RemoteClear"        , MXONRECV,   SYS_RemoteClear,         psc);
        MXAddComCallBack(pdg,  pcom, DL_SYS,  "RemoteSections"     , MXONRECV,   SYS_RemoteSections,      psc);
        MXAddComCallBack(pdg,  pcom, DL_SYS,  "RemoteVariables"    , MXONRECV,   SYS_RemoteVariables,     psc);
        MXAddComCallBack(pdg,  pcom, DL_SYS,  "RemoteInsertBP"     , MXONRECV,   SYS_RemoteInsertBP,      psc);
        MXAddComCallBack(pdg,  pcom, DL_SYS,  "RemoteRemoveBP"     , MXONRECV,   SYS_RemoteRemoveBP,      psc);
        MXAddComCallBack(pdg,  pcom, DL_SYS,  "RemoteStep"         , MXONRECV,   SYS_RemoteStep,          psc);
        MXAddComCallBack(pdg,  pcom, DL_SYS,  "RemoteInStep"       , MXONRECV,   SYS_RemoteInStep,        psc);
        MXAddComCallBack(pdg,  pcom, DL_SYS,  "RemoteContinue"     , MXONRECV,   SYS_RemoteContinue,      psc);
		MXAddComCallBack(pdg,  pcom, DL_SYS,  "RemoteVariable"     , MXONRECV,   SYS_RemoteVariable,      psc);
        OSSetPrintProcedure(SCRemotePrint, pcom);
        SCSetTraceProcedure (psc, SCRemoteTrace, pcom);
        MXAddEndingProcedure (pcom, SCCloseDebugCom , psc);
    }
    return 1;
}



