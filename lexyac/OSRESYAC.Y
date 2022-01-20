/***************************************************************/
/* Ces modifications sont a apporter au source OSRESYAC.C      */
/*                                                             */
/* ajout au niveau du fichier OSRESYAC.H de :                  */
/*            #ifndef  __osresyac_h__                          */
/*            #define  __osresyac_h__                          */
/*              ...                                            */
/*            #endif                                           */
/*                                                             */
/* la fonction int yyparse() devient void *yyparse (XF *pxf)   */
/*                                                             */
/* suppression de la structure YYSTYPE et de define en entête  */
/* du fichier osresyac.c                                       */
/*                                                             */
/* ajout du paramètre pxf au niveau des appels de yylex ()     */
/*         yylex (pxf)                                         */
/*                                                             */
/* #define YYACCEPT return(0)                                  */
/*          ----> #define YYACCEPT return((GlobalOne *)TheOne) */
/* #define YYABORT return(1)                                   */ 
/*          ----> #define YYABORT return(NULL)                 */
/*                                                             */
/*      Transformation des variables et des fonctions de type  */ 
/* yy$var ou YY$var en respectivement zz$var et ZZ$var         */ 
/*                                                             */
/***************************************************************/

%{
  #include "osreslex.h"

  GlobalOne*       TheOne;
  Dialog*          Dial;
  MessageClass*    MessClass;
  Argument*        Argu;
  Machine*         Mach;
  ConnectionClass* ConnClass;
  Connection*      Conn;
  ApplicationClass* AppliClass;
  Application*     Prog;
  XlatStruct*      VarXlat;
  Site*            Sit;
  Scenario*        Sce;
  DataBaseClass*   Data;
  DataBase*        iData;
  JournalClass*    Stor;
  Journal*         iStor; 
  QueueClass*      Que;
  Queue*           iQue;
  LogicalAddress*  La;
  Property*        Pro;
  FolderArgument*  FolderArgu;
  FileArgument*    FileArgu;
  ProcArgument*    ProcArgu;
  VarArgument*     VarArgu;
  Remote*          remote;
int Verif;
  char Buffer[256];
  int Errind = SYNTAX;
%}

%union{
  int Valint;
  char *Valvar;
  char *Valtyp;
};

%token <Valint> DIGIT
%token <Valint> DIMDIGIT 
%token <Valvar> VAR
%token <Valvar> LOCALVAR
%token <Valvar> BIGVAR
%token <Valvar> NFILE
%token <Valvar> IDP
%token <Valvar> LINEHEX
%token <Valvar> COMMENT
%token <Valvar> ANUM

%token <Valtyp> PGCONTEXT
%token <Valtyp> TYPE
%token <Valtyp> RESTYPE
%token <Valtyp> PFILETYPE
%token <Valtyp> EVENT
%token <Valtyp> EVENTSR
%token <Valtyp> RESSYSTEM
%token <Valtyp> YESNO
%token <Valtyp> PASS




%token  SITE MACHINE LA APPLICATIONCLASS APPLICATION CONNECTIONCLASS CONNECTION  PROPERTIES DEB FIN 
%token  DATABASECLASS DATABASE JOURNALCLASS JOURNAL QUEUECLASS QUEUE SCENARIO LENGTH 
%token  MESSAGE MESSAGECLASS OUVRE FERME FROM TO PORT TABLE PROTOCOL TABXLT STRFILES TRANSFERT COLOR
%token  IPADDRESS IPNAME DESCRIPTION APPTYPE SYSTEM PFILE PPROCEDURE PEQUAL DIALOGCLASS XLTCLASS 
%token  CHARERROR MAXREACH ONMACHINE USERNAME BASENAME PASSWORD FHOSTNAME FUSERNAME FPASSWORD 
%token  FPORT LOGONTYPE TRANSTYPE REPERTORY CONNECTED REMOTE SUBTYPE CLASS MODE
%token  LOCALISATION EXECREPERTORY LOCALREPERTORY RECEIVEREPERTORY SENDREPERTORY EXECCOMMAND EXECPARAMETERS FOLDER EXTENSION

%start Principal

%%

/*******************************************************************************
            THE YYPARSE INTERPRETER
*******************************************************************************/
Principal : Principal multdialog 
          | Principal multconclass
          | Principal multcon
          | Principal multstx
          | Principal multsite
          | Principal multmachine
          | Principal multappliclass
          | Principal multla
          | Principal multappli
          | Principal multproperties
          | Principal multremote
          | Principal multscenario
          | Principal multdatabase
          | Principal multdatabaseclass
          | Principal multjournal
          | Principal multjournalclass
          | Principal multqueueclass
          | Principal multqueue
          | Principal CHARERROR { ErrorOccurs(CHARACTER); YYERROR; }
          | multdialog
          | multconclass
          | multcon
          | multstx
          | multsite
          | multmachine
          | multappliclass
          | multla
          | multappli
          | multproperties
          | multscenario
          | multdatabaseclass
          | multdatabase
          | multjournalclass
          | multjournal
          | multqueueclass
          | multqueue
          | multremote
          | CHARERROR { ErrorOccurs(CHARACTER); YYERROR; }
          | Principal error { ErrorOccurs(Errind); YYERROR; };
          
          


/*******************************************************************************
            THE "DIALOGCLASS" INTERPRETER
*******************************************************************************/
multdialog        : DIALOGCLASS STRFILES | 
                    DIALOGCLASS MAXREACH { Errind = MAXFILE; YYERROR; } |
                    DIALOGCLASS DName DEB simplemessag FIN
                    { ListNewr(&TheOne->AllDialog, Dial); };
DName             : DIGIT BIGVAR     
                    {
                       Dial = AllocDialogYaccStruct($2, $1);
                       if (!Dial) { Errind = ALLOCATION; YYERROR; }
                    };
simplemessag      : simplemessag MESSAGECLASS messname OUVRE messargu FERME 
                    { ListNewr(&Dial->AllMessageClasses, MessClass); } |
                    simplemessag MESSAGECLASS messname OUVRE FERME 
                    {ListNewr(&Dial->AllMessageClasses, MessClass); } |
                    MESSAGECLASS messname OUVRE FERME
                    {ListNewr(&Dial->AllMessageClasses, MessClass); } |
                    MESSAGECLASS messname OUVRE messargu FERME
                    {ListNewr(&Dial->AllMessageClasses, MessClass); };
messname          : VAR
                    { 
                       MessClass = AllocMessageClassYaccStruct($1);
                       if (!MessClass) { Errind = ALLOCATION; YYERROR; } 
                    };

messargu          : messargu TYPE VAR 
                    {
                       Argu = AllocArgumentYaccStruct($2, 1, $3);
                       if (!Argu) { Errind = ALLOCATION; YYERROR; }
                       ListNewr(&MessClass->AllArgument, Argu);
                    } |
                    messargu TYPE DIMDIGIT VAR
                    {
                       Argu = AllocArgumentYaccStruct($2, $3, $4);
                       if (!Argu) { Errind = ALLOCATION; YYERROR; }
                       ListNewr(&MessClass->AllArgument, Argu);
                    } |
                    TYPE DIMDIGIT VAR 
                    {
                       Argu = AllocArgumentYaccStruct($1, $2, $3);
                       if (!Argu) { Errind = ALLOCATION; YYERROR; }
                       ListNewr(&MessClass->AllArgument, Argu);
                    } |
                    TYPE VAR 
                    {
                       Argu = AllocArgumentYaccStruct($1, 1, $2);
                       if (!Argu) { Errind = ALLOCATION; YYERROR; }
                       ListNewr(&MessClass->AllArgument, Argu);
                    };
                    

/*******************************************************************************
            THE "CONNECTIONCLASS" INTERPRETER
*******************************************************************************/
multconclass          : CONNECTIONCLASS CName DEB simpleport FIN
                    { ListNewr(&TheOne->AllConnectionClasses, ConnClass); };
CName             : DIGIT BIGVAR
                    {
                       ConnClass = AllocConnectionClassYaccStruct($2, $1); 
                       if (!ConnClass) { Errind = ALLOCATION; YYERROR; }
                    };
simpleport        : fromcon tocon table protocol transfert Cdescription;
table             : TABLE BIGVAR { memcpy(ConnClass->TableName, $2 + 1, strlen ($2) - 2); }  |
                    TABLE |;
protocol          : PROTOCOL IDP { strcpy(ConnClass->Protocol, $2); };
transfert         : TRANSFERT FROM { strcpy(ConnClass->TransferFormat, "FROM"); } | 
                    TRANSFERT TO { strcpy(ConnClass->TransferFormat, "TO"); } | 
                    TRANSFERT |;
fromcon           : FROM BIGVAR  { memcpy(ConnClass->FromAppliName, $2 + 1, strlen ($2) - 2); } |
                    FROM |;
tocon             : TO BIGVAR    { memcpy(ConnClass->ToAppliName, $2 + 1, strlen ($2) - 2); } |
                    TO |;
Cdescription      : DESCRIPTION COMMENT {strcpy(ConnClass->Description, $2 + 1);
                                         ConnClass->Description[strlen(ConnClass->Description) -1] = 0;}|
                    DESCRIPTION |; 






/*******************************************************************************
            THE "SITE" INTERPRETER
*******************************************************************************/
multsite          : SITE SName DEB Sbody FIN 
                    { ListNewr(&TheOne->AllSites, Sit); };
SName             : DIGIT BIGVAR 
                    {
                       Sit = AllocSiteYaccStruct($2, $1); 
                       if (!Sit) { Errind = ALLOCATION; YYERROR; }
                    };
Sbody             : Sdescription;
Sdescription      : DESCRIPTION COMMENT {strcpy(Sit->Description, $2 + 1);
                                         Sit->Description[strlen(Sit->Description) -1] = 0;}|
                    DESCRIPTION |; 

/*******************************************************************************
            THE "LAD" INTERPRETER
*******************************************************************************/
multla            : LA LName DEB Lbody FIN 
                    { ListNewr(&TheOne->AllLogicalAddress, La); }; 
LName             : DIGIT BIGVAR 
                    {
                       La = AllocAddressYaccStruct($2, $1); 
                       if (!La) { Errind = ALLOCATION; YYERROR; }
                    };
Lbody             : Llocalisation Ldescription ;
Llocalisation     : LOCALISATION BIGVAR {memcpy (La->Localisation,  $2 + 1, strlen ($2) -2);} |
                    LOCALISATION |;
Ldescription      : DESCRIPTION COMMENT {strcpy(La->Description, $2 + 1);
                                         La->Description[strlen(La->Description) -1] = 0;} |
                    DESCRIPTION |; 


/*******************************************************************************
            THE "MACHINE" INTERPRETER
*******************************************************************************/
multmachine       : MACHINE MName DEB Mbody FIN 
                    { ListNewr(&TheOne->AllMachines, Mach); };
MName             : DIGIT BIGVAR 
                    {
                       Mach = AllocMachineYaccStruct($2, $1); 
                       if (!Mach) { Errind = ALLOCATION; YYERROR; }
                    };
Mbody             : Mip Mlocalisation Mipname Msystem MColor Mdescription; 
Mlocalisation     : LOCALISATION BIGVAR {memcpy (Mach->Localisation,  $2 + 1, strlen ($2) -2);} |
                    LOCALISATION |;
Mip               : IPADDRESS ANUM       { strcpy(Mach->IPAddress, $2);} |
                    IPADDRESS |;   
Mipname           : IPNAME VAR           { strcpy(Mach->IPName, $2);}|
                    IPNAME |; 
Mdescription      : DESCRIPTION COMMENT  { strcpy(Mach->Description, $2 + 1);
                                           Mach->Description[strlen(Mach->Description) -1] = 0;} | 
                    DESCRIPTION |; 
MColor            : COLOR DIGIT 
                    { Mach->Color = $2;} |
                    COLOR |;
Msystem           : SYSTEM  RESSYSTEM    {strcpy (Mach->System, $2);}|
                    SYSTEM |;



/*******************************************************************************
            THE "SCENARIO" INTERPRETER
*******************************************************************************/

multscenario      : SCENARIO SCEName DEB SCEbody FIN 
                    { ListNewr(&TheOne->AllScenarios, Sce); }; 
SCEName           : DIGIT BIGVAR 
                    {
                       Sce = AllocScenarioYaccStruct($2, $1); 
                       if (!Sce) { Errind = ALLOCATION; YYERROR; }
                    };
SCEbody           : SCEfile SCEdescription ;
SCEfile           : PFILE NFILE {strcpy (Sce->FileName,  $2);} |
                    PFILE|;
SCEdescription    : DESCRIPTION COMMENT {strcpy(Sce->Description, $2 + 1);
                                        Sce->Description[strlen(Sce->Description) -1] = 0;} |
                    DESCRIPTION |; 


/*******************************************************************************
            THE "QUEUECLASS" INTERPRETER
*******************************************************************************/

multqueueclass      : QUEUECLASS queueName DEB queuebody FIN 
                    { ListNewr(&TheOne->AllQueueClasses, Que); }; 
queueName           : DIGIT BIGVAR 
                    {
                       Que = AllocQueueClassYaccStruct($2, $1); 
                       if (!Que) { Errind = ALLOCATION; YYERROR; }
                    };
queuebody           : queuedescription ;
queuedescription    : DESCRIPTION COMMENT {strcpy(Que->Description, $2 + 1);
                                        Que->Description[strlen(Que->Description) -1] = 0;} |
                      DESCRIPTION |; 



/*******************************************************************************
            THE "QUEUE" INTERPRETER
*******************************************************************************/
multqueue          : QUEUE iqueueName DEB iqueuebody FIN 
                  { ListNewr(&TheOne->AllQueues, iQue); }; 
iqueueName          : DIGIT BIGVAR 
                    {
                       iQue = AllocQueueYaccStruct($2, $1); 
                       if (!Que) { Errind = ALLOCATION; YYERROR; }
                    };
iqueuebody           : iqueueclass iqueuelocalisation iqueuedescription ;
iqueueclass          : CLASS BIGVAR {memcpy (iQue->Class, $2 + 1, strlen ($2) -2);};                      
iqueuelocalisation   : LOCALISATION BIGVAR  {memcpy (iQue->Localisation,  $2 + 1, strlen ($2) -2);}|
                  LOCALISATION |;
iqueuedescription    : DESCRIPTION COMMENT {strcpy(iQue->Description, $2 + 1);
                                         iQue->Description[strlen(iQue->Description) -1] = 0;} |
                      DESCRIPTION |; 



/*******************************************************************************
            THE "DATABASECLASS" INTERPRETER
*******************************************************************************/
multdatabaseclass : DATABASECLASS dataName DEB databody FIN 
                  { ListNewr(&TheOne->AllDataBaseClasses, Data); }; 
dataName          : DIGIT BIGVAR 
                    {
                       Data = AllocDataBaseClassYaccStruct($2, $1); 
                       if (!Data) { Errind = ALLOCATION; YYERROR; }
                    };
databody           : datadescription ;
datadescription    : DESCRIPTION COMMENT {strcpy(Data->Description, $2 + 1);
                                         Data->Description[strlen(Data->Description) -1] = 0;} |
                    DESCRIPTION |; 



/*******************************************************************************
            THE "DATABASE" INTERPRETER
*******************************************************************************/
multdatabase      : DATABASE idataName DEB idatabody FIN 
                  { ListNewr(&TheOne->AllDataBases, iData); }; 
idataName          : DIGIT BIGVAR 
                    {
                       iData = AllocDataBaseYaccStruct($2, $1); 
                       if (!Data) { Errind = ALLOCATION; YYERROR; }
                    };
idatabody           : idataclass ibasename idatalocalisation idatadescription ;
idataclass          : CLASS BIGVAR {memcpy (iData->Class, $2 + 1, strlen ($2) -2);};                      
ibasename           : BASENAME VAR {strcpy (iData->BaseName,  $2);} | BASENAME |;
idatalocalisation   : LOCALISATION BIGVAR  {memcpy (iData->Localisation,  $2 + 1, strlen ($2) -2);}|
                      LOCALISATION |;
idatadescription    : DESCRIPTION COMMENT {strcpy(iData->Description, $2 + 1);
                                         iData->Description[strlen(iData->Description) -1] = 0;} |
                    DESCRIPTION |; 




/*******************************************************************************
            THE "JOURNALCLASS" INTERPRETER
*******************************************************************************/
multjournalclass  : JOURNALCLASS stoName DEB stobody FIN 
                    { ListNewr(&TheOne->AllJournalClasses, Stor); }; 
stoName           : DIGIT BIGVAR 
                    {
                       Stor = AllocJournalClassYaccStruct($2, $1); 
                       if (!Stor) { Errind = ALLOCATION; YYERROR; }
                    };
stobody           : stomessag stotype stolength stodescription |
                    stotype stolength stodescription;
stomessag         : stomessag MESSAGECLASS stomessname OUVRE FERME
                    {ListNewr(&Stor->AllMessage, MessClass); } |
                    stomessag MESSAGECLASS stomessname OUVRE stomessargu FERME
                    {ListNewr(&Stor->AllMessage, MessClass); } |
                    MESSAGECLASS stomessname OUVRE FERME             
                    {ListNewr(&Stor->AllMessage, MessClass); } |     
                    MESSAGECLASS stomessname OUVRE stomessargu FERME 
                    {ListNewr(&Stor->AllMessage, MessClass); };    
stomessname       : VAR
                    { 
                       MessClass = AllocMessageClassYaccStruct($1);
                       if (!MessClass) { Errind = ALLOCATION; YYERROR; } 
                    };
stomessargu       : stomessargu TYPE VAR 
                    {
                       Argu = AllocArgumentYaccStruct($2, 1, $3);
                       if (!Argu) { Errind = ALLOCATION; YYERROR; }
                       ListNewr(&MessClass->AllArgument, Argu);
                    } |
                    stomessargu TYPE DIMDIGIT VAR
                    {
                       Argu = AllocArgumentYaccStruct($2, $3, $4);
                       if (!Argu) { Errind = ALLOCATION; YYERROR; }
                       ListNewr(&MessClass->AllArgument, Argu);
                    } |
                    TYPE DIMDIGIT VAR 
                    {
                       Argu = AllocArgumentYaccStruct($1, $2, $3);
                       if (!Argu) { Errind = ALLOCATION; YYERROR; }
                       ListNewr(&MessClass->AllArgument, Argu);
                    } |
                    TYPE VAR 
                    {
                       Argu = AllocArgumentYaccStruct($1, 1, $2);
                       if (!Argu) { Errind = ALLOCATION; YYERROR; }
                       ListNewr(&MessClass->AllArgument, Argu);
                    };
stotype           : APPTYPE IDP     { Stor->Format = ((strcmp ($2, "STORE") == 0) ? 1 : 0);} |;
stolength         : LENGTH DIGIT   { Stor->Length = $2;} | ;
stodescription    : DESCRIPTION COMMENT {strcpy(Stor->Description, $2 + 1);
                                         Stor->Description[strlen(Stor->Description) -1] = 0;} |
                    DESCRIPTION |; 


/*******************************************************************************
            THE "JOURNAL" INTERPRETER
*******************************************************************************/
multjournal       : JOURNAL journalName DEB journalbody FIN 
                  { ListNewr(&TheOne->AllJournals, iStor); }; 
journalName       : DIGIT BIGVAR 
                  {
                     iStor = AllocJournalYaccStruct($2, $1); 
                     if (!iStor) { Errind = ALLOCATION; YYERROR; }
                  };
journalbody       : journalclass journallocalisation journaldescription ;
journalclass      : CLASS BIGVAR {memcpy (iStor->Class, $2 + 1, strlen ($2) -2);};                      
journallocalisation  : LOCALISATION BIGVAR  {memcpy (iStor->Localisation,  $2 + 1, strlen ($2) -2);}|
                       LOCALISATION |;
journaldescription : DESCRIPTION COMMENT {strcpy(iStor->Description, $2 + 1);
                                         iStor->Description[strlen(iStor->Description) -1] = 0;} |
                     DESCRIPTION |; 


/*******************************************************************************
            THE "APPLICATIONCLASS" INTERPRETER
*******************************************************************************/
multappliclass    : APPLICATIONCLASS AName DEB Abody FIN 
                    { ListNewr(&TheOne->AllApplicationClasses, AppliClass); };
AName             : DIGIT BIGVAR 
                    {
                       AppliClass = AllocApplicationClassYaccStruct($2, $1); 
                       if (!AppliClass) { Errind = ALLOCATION; YYERROR; }
                    }; 
Abody             : Atype AColor Asubtype Arepertory Adescription; 
Atype             : APPTYPE RESTYPE      {strcpy (AppliClass->Type, $2);}|
                    APPTYPE |;
Asubtype          : SUBTYPE DIGIT
                    {AppliClass->SubType = $2;}; |
                    SUBTYPE |;
Arepertory        : REPERTORY NFILE {strcpy (AppliClass->Repertory,  $2);} |
                    REPERTORY |; 
AColor            : COLOR DIGIT 
                    { AppliClass->Color = $2;} |
                    COLOR |;
Adescription      : DESCRIPTION COMMENT  {strcpy(AppliClass->Description, $2 + 1);
                                          AppliClass->Description[strlen(AppliClass->Description) -1] = 0;}|
                    DESCRIPTION |; 



/*******************************************************************************
            THE "APPLICATION" INTERPRETER
*******************************************************************************/
multappli          : APPLICATION PrName DEB Prbody FIN 
                    { ListNewr(&TheOne->AllApplications, Prog); };
PrName             : DIGIT BIGVAR 
                    {
                       Prog = AllocApplicationYaccStruct($2, $1); 
                       if (!Prog) { Errind = ALLOCATION; YYERROR; }
                    };
Prbody             : Pronapplication Prlocalisation Prrepertory Prexecrepertory Prexeccommand Prexecparameters Prrecrepertory Prsendrepertory PrFolders |
                     Pronapplication Prlocalisation Prrepertory Prexecrepertory Prexeccommand Prexecparameters Prrecrepertory Prsendrepertory; 
Pronapplication    : CLASS BIGVAR {memcpy (Prog->Class, $2 + 1, strlen ($2) -2);};                      
Prlocalisation     : LOCALISATION BIGVAR  {memcpy (Prog->Localisation,  $2 + 1, strlen ($2) -2);}|
                     LOCALISATION |;
Prrepertory        : REPERTORY NFILE {strcpy (Prog->Repertory,  $2);} |
                     REPERTORY |; 
Prexecrepertory    : EXECREPERTORY NFILE {strcpy (Prog->ExecRepertory, $2);} |
                     EXECREPERTORY|;
Prexeccommand      : EXECCOMMAND NFILE { strcpy (Prog->ExecCommand, $2); } | 
                     EXECCOMMAND| ;
Prexecparameters   : EXECPARAMETERS NFILE {strcpy (Prog->ExecParameters, $2);} | 
                     EXECPARAMETERS |;
Prrecrepertory     : RECEIVEREPERTORY NFILE {strcpy (Prog->ReceiveRepertory,  $2);} | 
                     RECEIVEREPERTORY|;
Prsendrepertory    : SENDREPERTORY NFILE {strcpy (Prog->SendRepertory, $2);} | 
                     SENDREPERTORY|;
PrFolders          : PrFolders PrFolder PrExtension PrFiles |
                     PrFolders PrFolder PrExtension|  
                     PrFolder PrExtension PrFiles |
                     PrFolder PrExtension;
PrFolder           : FOLDER BIGVAR 
                    {
                       FolderArgu = AllocFolderArgumentYaccStruct($2, "");
                       if (!FolderArgu) { Errind = ALLOCATION; YYERROR; }
                       AddFolderToApplication (Prog, FolderArgu);
                    };
PrExtension        : EXTENSION VAR
                    {
                       strcpy (FolderArgu->Extensions , $2);
                    };
PrFiles            : PrFiles PFILE NFILE 
                    {
                       FileArgu = AllocFileArgumentYaccStruct($3, "OTHER");
                       if (!FileArgu) { Errind = ALLOCATION; YYERROR; }
                       AddFileToFolder (FolderArgu, FileArgu);
                    } |
                     PFILE NFILE
                    {
                       FileArgu = AllocFileArgumentYaccStruct($2, "OTHER");
                       if (!FileArgu) { Errind = ALLOCATION; YYERROR; }
                       AddFileToFolder (FolderArgu, FileArgu);
                    }; 

/*******************************************************************************
            THE "PROPERTIES" INTERPRETER
*******************************************************************************/
multproperties   :  PROPERTIES PName DEB Pbody FIN 
                    { ListNewr(&TheOne->AllProperties, Pro); } |
                    PROPERTIES PName DEB FIN             
                     { ListNewr(&TheOne->AllProperties, Pro); };
PName             : DIGIT BIGVAR 
                    {
                       Pro = AllocPropertyYaccStruct($2, $1); 
                       if (!Pro) { Errind = ALLOCATION; YYERROR; }
                    };
Pbody             : Pbody PFILE PFILETYPE NFILE 
                    {
                       FileArgu = AllocFileArgumentYaccStruct($4, $3);
                       if (!FileArgu) { Errind = ALLOCATION; YYERROR; }
                       AddFileToProperty (Pro, FileArgu);
                    } |
                    PFILE PFILETYPE NFILE
                    {
                       FileArgu = AllocFileArgumentYaccStruct($3, $2);
                       if (!FileArgu) { Errind = ALLOCATION; YYERROR; }
                       AddFileToProperty (Pro, FileArgu);
                    } |
                    Pbody PRSProcedure PVariables |
                    PRSProcedure PVariables|
                    PProcedure |
                    Pbody PProcedure;
PRSProcedure      : PContext PSREvent
PProcedure        : PContext PEvent                    
                    {
                    };
PContext          : PPROCEDURE PGCONTEXT 
                    {
                       ProcArgu = AllocProcArgumentYaccStruct("","",$2);
                       if (!ProcArgu) { Errind = ALLOCATION; YYERROR; }
                       AddProcToProperty (Pro, ProcArgu);
                    };
PSREvent          : EVENTSR VAR
                    {
                       strcpy (ProcArgu->Type, $1);
                       strcpy (ProcArgu->Name, $2);
                    };
PEvent            : EVENT VAR
                    {
                       strcpy (ProcArgu->Type, $1);
                       strcpy (ProcArgu->Name, $2);
                    };
PVariables        : PClass PMessageClass OUVRE PVar FERME  
                    {
                    };
PClass            : VAR { strcpy (ProcArgu->Class, $1);} | 
                    IDP { strcpy (ProcArgu->Class, $1);};
PMessageClass     : VAR { strcpy (ProcArgu->MessageClass, $1);};
PVar              : PVar PTypeVar PVarInstance |
                    PTypeVar PVarInstance;
PTypeVar          : VAR
                    {
                      VarArgu = AllocVarArgumentYaccStruct("", $1, "");
                      if (!VarArgu) { Errind = ALLOCATION; YYERROR; }
                      AddVarToProcedure (ProcArgu, VarArgu);
                    };
PVarInstance      : PEQUAL VAR OUVRE PInitVar FERME
                    {
                      strcpy (VarArgu->Name, $2);
                    }; 
PInitVar          : VAR {strcpy (VarArgu->Init, $1);} |;




/*******************************************************************************
            THE "REMOTE" INTERPRETER
*******************************************************************************/
multremote        : REMOTE RName DEB Rbody FIN 
                    { ListNewr(&TheOne->AllRemotes, remote); };
RName             : DIGIT BIGVAR 
                    {
                       remote = AllocRemoteYaccStruct($2, $1); 
                       if (!remote) { Errind = ALLOCATION; YYERROR; }
                    };
Rbody             : Ronmachine Rusername Rpassword Rtype Rport Rconnected Rrepertory Rfhostname Rfusername Rfpassword Rfport Rlogontype  Rtranstype;
Ronmachine        : ONMACHINE BIGVAR {memcpy (remote->OnMachine, $2 + 1, strlen ($2) - 2);};
Rusername         : USERNAME VAR {strcpy (remote->UserName,  $2);};
Rpassword         : PASSWORD VAR {strcpy (remote->PassWord,  $2);};
Rtype             : APPTYPE IDP    {strcpy (remote->Type, $2);}|APPTYPE|;
Rport             : PORT DIGIT { remote->Port = $2; } | PORT | ;
Rconnected        : CONNECTED YESNO {remote->Connected = (strcmp ($2, "YES") ? 1 : 0);}| CONNECTED |;
Rrepertory        : REPERTORY NFILE {strcpy (remote->DistantRepertory,  $2);} | REPERTORY|;
Rfhostname        : FHOSTNAME VAR {strcpy (remote->FHostName, $2);} | FHOSTNAME|;
Rfusername        : FUSERNAME VAR {strcpy (remote->FUserName,  $2);}| FUSERNAME|;
Rfpassword        : PASSWORD VAR {strcpy (remote->FPassWord,  $2);}| FPASSWORD|;
Rfport            : FPORT DIGIT { remote->FPort = $2; } | FPORT|;
Rtranstype        : TRANSTYPE PASS {remote->TransType = ((strcmp ($2, "PASSIVE") == 0) ? 1 : 0);} | TRANSTYPE |;
Rlogontype        : LOGONTYPE DIGIT {remote->LogonType = $2;} | LOGONTYPE |;

/*******************************************************************************
            THE "CONNECTION" INTERPRETER
*******************************************************************************/
multcon          : CONNECTION CIName DEB CIsimpleport FIN
                   { ListNewr(&TheOne->AllConnections, Conn); };
CIName             : DIGIT BIGVAR
                    {
                       Conn = AllocConnectionYaccStruct($2, $1); 
                       if (!Conn) { Errind = ALLOCATION; YYERROR; }
                    };
CIsimpleport      : CIclass CIfromcon CItocon CIusername CIpassword CIport CIfilename CIreadmode CIdescription;
CIclass           : CLASS BIGVAR {memcpy (Conn->Class, $2 + 1, strlen ($2) -2);};                      
CIfromcon         : FROM BIGVAR  { memcpy(Conn->FromAppliName, $2 + 1, strlen ($2) - 2); } |
                    FROM |;
CItocon           : TO BIGVAR    { memcpy(Conn->ToAppliName, $2 + 1, strlen ($2) - 2); } |
                    TO |;
CIusername        : USERNAME VAR {strcpy (Conn->UserName,  $2);} | USERNAME |;
CIpassword        : PASSWORD VAR {strcpy (Conn->PassWord,  $2);} | PASSWORD |;
CIport            : PORT DIGIT { Conn->Port = $2; } | PORT|;
CIfilename        : PFILE NFILE { strcpy (Conn->FileName,  $2);} | PFILE |; 
CIreadmode        : MODE VAR {strcpy (Conn->ReadMode,  $2);} | MODE |;
CIdescription     : DESCRIPTION COMMENT {strcpy(Conn->Description, $2 + 1);
                                         Conn->Description[strlen(Conn->Description) -1] = 0;}|
                    DESCRIPTION |; 




/*******************************************************************************
            THE "XF" INTERPRETER
*******************************************************************************/
multstx            : XLTCLASS STRFILES |
                     XLTCLASS MAXREACH { Errind = MAXFILE; YYERROR; }  |
                     XLTCLASS XName DEB simplestx FIN
                     { ListNewr(&TheOne->AllXlatStruct, VarXlat); };
XName              : DIGIT BIGVAR 
                     {
                        VarXlat = AllocXlatYaccStruct($2, $1); 
                        if (!VarXlat) {Errind = ALLOCATION; YYERROR; }
                     };
simplestx          : fromstx tostx TABXLT tablehex 
                     {
                        if (Verif != 16) { Verif = 0; Errind = XLTCAPTION; YYERROR; }
                        Verif = 0; 
                     }; 
fromstx            : FROM DIGIT { sprintf(VarXlat->FromName, "%d\0", $2); } |
                     FROM VAR   { strcpy(VarXlat->FromName, $2); };
tostx              : TO DIGIT { sprintf(VarXlat->ToName, "%d\0", $2); }     |
                     TO VAR { strcpy(VarXlat->ToName, $2); };
tablehex           : tablehex LINEHEX
                     {
                        if (Verif >= 16) { Verif = 0; Errind = XLTCAPTION; YYERROR; }
                        strcpy((char *)VarXlat->Tabxlt[Verif], $2);
                        Verif++; 
                     } |
                     LINEHEX 
                     {
                        if (Verif >= 16) { Verif = 0; Errind = XLTCAPTION; YYERROR; }
                        strcpy((char *)VarXlat->Tabxlt[Verif], $1);
                        Verif++; 
                     };
            
%%

/*******************************************************************************
            DEFINITION OF THE FUNCTIONS
*******************************************************************************/
zzerror (char *s)
{
  ErrorOccurs(Errind);
}


int InitGlob()
{
  if (!TheOne)
   {
     TheOne = AllocGlobalOneYaccStruct();
      if (!TheOne) return 0;
   }
  return 1;
}

void CrashAll()
{
  if (TheOne)
    FreeGlobalOneYaccStruct(TheOne);
  return;
}

GlobalOne *AllocGlobalOneYaccStruct()
{
  GlobalOne *Glob;

  Glob = (GlobalOne *)malloc(sizeof(GlobalOne));

  if (!Glob) return(NULL);
  memset (Glob, 0, sizeof (GlobalOne));
  return((GlobalOne *)Glob);
}


Dialog *AllocDialogYaccStruct(char *Name, int Id)
{
  Dialog *Dial;

  Dial = (Dialog *)malloc(sizeof(Dialog));
  if (!Dial) return(NULL);

  memset(Dial->DialName, '\0', 256);
  memcpy(Dial->DialName, Name + 1, strlen(Name) - 2);
  Dial->DialId = Id;
  Dial->AllMessageClasses = NULL;

  return Dial;
}



Property* AllocPropertyYaccStruct (char* Name, int Id)
{
  Property* Pro;
  Pro = (Property *)malloc (sizeof (Property));
  if (!Pro) return NULL;
  memset (Pro, 0, sizeof (Property));
  memcpy(Pro->ApplicationName, Name + 1, strlen(Name) - 2);
  Pro->Id = Id;
  return Pro;
}

FileArgument* AllocFileArgumentYaccStruct(char* Name, char* Type)
{
  FileArgument* FileArgu;
  FileArgu = (FileArgument*)malloc (sizeof (FileArgument));
  if (!FileArgu)  return NULL;
  memset (FileArgu, 0, sizeof (FileArgument));
  strcpy (FileArgu->Type, Type);
  strcpy (FileArgu->Name, Name);
  return FileArgu;
}

ProcArgument* AllocProcArgumentYaccStruct(char* Name, char* Type, char* Context)
{
  ProcArgument* ProcArgu;
  ProcArgu = (ProcArgument*)malloc (sizeof (ProcArgument));
  if (!ProcArgu)  return NULL;
  memset (ProcArgu, 0, sizeof (ProcArgument));
  strcpy (ProcArgu->Name, Name);
  strcpy (ProcArgu->Type, Type);
  strcpy (ProcArgu->Context, Context);
  return ProcArgu;
}

VarArgument* AllocVarArgumentYaccStruct(char* Name, char* Type, char* Init)
{
  VarArgument* VarArgu;
  VarArgu = (VarArgument*)malloc (sizeof (VarArgument));
  if (!VarArgu)  return NULL;
  memset (VarArgu, 0, sizeof (VarArgument));
  strcpy (VarArgu->Type, Type);
  strcpy (VarArgu->Name, Name);
  strcpy (VarArgu->Init, Init);
  return VarArgu;
}



void AddFileToFolder (FolderArgument* Fold, FileArgument* Argu)
{
    ListNewr (&Fold->Files, Argu);
}

void AddFileToProperty (Property* Pro, FileArgument* Argu)
{
  char* Type = Argu->Type;
  if (strcmp (Type, "SOURCE") == 0)
    ListNewr (&Pro->Source, Argu);
  if (strcmp (Type, "INCLUDE") == 0)
    ListNewr (&Pro->Include, Argu);
  if (strcmp (Type, "SCRIPT") == 0)
    ListNewr (&Pro->Script, Argu);
  if (strcmp (Type, "MAKE") == 0)
    ListNewr (&Pro->Make, Argu);
  if (strcmp (Type, "OTHER") == 0)
    ListNewr (&Pro->Other, Argu);
 }

void AddFolderToApplication (Application* Prog, FolderArgument* Argu)
{
   ListNewr (&Prog->Folders, Argu);
}



void AddProcToProperty (Property* Pro, ProcArgument* Argu)
{
  ListNewr (&Pro->Procedures, Argu);
}

void AddVarToProcedure (ProcArgument* Proc, VarArgument* Argu)
{
  ListNewr (&Proc->Variables, Argu);
}

void SetProcMessageClass (ProcArgument* Proc, char* Class, char* MessageClass)
{
  strcpy (Proc->Class, Class);
  strcpy (Proc->MessageClass, MessageClass);
}


MessageClass *AllocMessageClassYaccStruct(char *Name)
{
  MessageClass *Mess;

  Mess = (MessageClass *)malloc(sizeof(MessageClass));
  if (!Mess) return(NULL);

  memset(Mess->MessName, '\0', 256);
  memcpy(Mess->MessName, Name, strlen(Name));
  Mess->AllArgument = NULL;
  return((MessageClass *)Mess); 
}


Message *AllocMessageYaccStruct(char *Name)
{
  Message *Mess;

  Mess = (Message *)malloc(sizeof(Message));
  if (!Mess) return(NULL);

  memset(Mess->MessName, '\0', 256);
  memcpy(Mess->MessName, Name, strlen(Name));
  Mess->AllArgument = NULL;

  return((Message *)Mess); 
}

Argument *AllocArgumentYaccStruct(char *Type, int Size, char *Name)
{
  Argument *Argu;

  Argu = (Argument *)malloc(sizeof(Argument));
  if (!Argu) return(NULL);

  memset(Argu->ArguType, '\0', 10);
  memset(Argu->ArguName, '\0', 256);

  memcpy(Argu->ArguType, Type, strlen(Type));
  Argu->ArguSize = Size;
  memcpy(Argu->ArguName, Name, strlen(Name));

  return((Argument *)Argu);
}


ConnectionClass *AllocConnectionClassYaccStruct(char *Name, int Id)
{
  ConnectionClass *Conn;                                         
                                                         
  Conn = (ConnectionClass *)malloc(sizeof(ConnectionClass));             
  if (!Conn) return(NULL);                               
  memset (Conn, 0, sizeof (ConnectionClass));                    
  memcpy(Conn->Name, Name + 1, strlen(Name) - 2);         
  Conn->Id = Id;                                  
  return Conn;                               
}


Connection *AllocConnectionYaccStruct(char *Name, int Id)
{
  Connection *Conn;                                         
                                                         
  Conn = (Connection *)malloc(sizeof(Connection));             
  if (!Conn) return(NULL);                               
  memset (Conn, 0, sizeof (Connection));                    
  memcpy(Conn->Name, Name + 1, strlen(Name) - 2);         
  Conn->Id = Id;                                  
  return Conn;                               
}





Remote *AllocRemoteYaccStruct(char *Name, int Id)
{
  Remote* Conn;

  Conn = (Remote *)malloc(sizeof(Remote));
  if (!Conn) return(NULL);
  memset (Conn, 0, sizeof (Remote));
  memcpy(Conn->Name, Name + 1,  strlen(Name) - 2);
  Conn->Id = Id;
  Conn->Port = 21;
  Conn->Connected = 0;            
  return Conn;
}

ApplicationClass* AllocApplicationClassYaccStruct(char *Name, int Id)
{
  ApplicationClass* appli;
  appli = (ApplicationClass *)malloc(sizeof(ApplicationClass));
  if (!appli) return(NULL);
  memset (appli, 0, sizeof (ApplicationClass));
  memcpy(appli->Name, Name + 1,  strlen(Name) - 2);
  appli->Id = Id;
  return appli;
}


Application* AllocApplicationYaccStruct(char *Name, int Id)
{
  Application* Pro;

  Pro = (Application *)malloc(sizeof(Application));
  if (!Pro) return(NULL);
  memset (Pro, 0, sizeof (Application));
  memcpy(Pro->Name, Name + 1,  strlen(Name) - 2);
  Pro->Id = Id;
  return Pro;
}

FolderArgument *AllocFolderArgumentYaccStruct(char *Name, char* Extensions)
{
  FolderArgument* Fold;

  Fold = (FolderArgument *)malloc(sizeof(FolderArgument));
  if (!Fold) return(NULL);
  memset (Fold, 0, sizeof (FolderArgument));
  memcpy(Fold->Name, Name + 1,  strlen(Name) - 2);
  memcpy(Fold->Extensions, Extensions,  strlen(Extensions));
  return Fold;
}


Scenario* AllocScenarioYaccStruct(char *Name, int Id)
{
  Scenario* scenario;

  scenario = (Scenario *)malloc(sizeof(Scenario));
  if (!scenario) return(NULL);
  memset (scenario, 0, sizeof (Scenario));
  memcpy(scenario->Name, Name + 1,  strlen(Name) - 2);
  scenario->Id = Id;
  return scenario;
}

Queue* AllocQueueYaccStruct(char *Name, int Id)
{
  Queue* queue;

  queue = (Queue *)malloc(sizeof(Queue));
  if (!queue) return(NULL);
  memset (queue, 0, sizeof (Queue));
  memcpy(queue->Name, Name + 1,  strlen(Name) - 2);
  queue->Id = Id;
  return queue;
}



QueueClass* AllocQueueClassYaccStruct(char *Name, int Id)
{
  QueueClass* queueclass;

  queueclass = (QueueClass *)malloc(sizeof(QueueClass));
  if (!queueclass) return(NULL);
  memset (queueclass, 0, sizeof (QueueClass));
  memcpy(queueclass->Name, Name + 1,  strlen(Name) - 2);
  queueclass->Id = Id;
  return queueclass;
}





DataBaseClass* AllocDataBaseClassYaccStruct(char *Name, int Id)
{
  DataBaseClass* database;

  database = (DataBaseClass *)malloc(sizeof(DataBaseClass));
  if (!database) return(NULL);
  memset (database, 0, sizeof (DataBaseClass));
  memcpy(database->Name, Name + 1,  strlen(Name) - 2);
  database->Id = Id;
  return database;
}

DataBase* AllocDataBaseYaccStruct(char *Name, int Id)
{
  DataBase* database;

  database = (DataBase *)malloc(sizeof(DataBase));
  if (!database) return(NULL);
  memset (database, 0, sizeof (DataBase));
  memcpy(database->Name, Name + 1,  strlen(Name) - 2);
  database->Id = Id;
  return database;
}




JournalClass*  AllocJournalClassYaccStruct (char *Name, int Id)
{
  JournalClass* journal;

  journal = (JournalClass *)malloc(sizeof(JournalClass));
  if (!journal) return(NULL);
  memset (journal, 0, sizeof (JournalClass));
  memcpy(journal->Name, Name + 1,  strlen(Name) - 2);
  journal->AllMessage = NULL;
  journal->Format = 0;
  journal->Length = 0;
  journal->Id = Id;
  return journal;
}


Journal*  AllocJournalYaccStruct (char *Name, int Id)
{
  Journal* journal;

  journal = (Journal *)malloc(sizeof(Journal));
  if (!journal) return(NULL);
  memset (journal, 0, sizeof (Journal));
  memcpy(journal->Name, Name + 1,  strlen(Name) - 2);
  journal->Id = Id;
  return journal;
}







Site *AllocSiteYaccStruct(char *Name, int Id)
{
  Site* site;

  site = (Site *)malloc(sizeof(Site));
  if (!site) return(NULL);
  memset (site, 0, sizeof (Site));
  memcpy(site->Name, Name + 1,  strlen(Name) - 2);
  site->Id = Id;
  return site;
}

Machine *AllocMachineYaccStruct(char *Name, int Id)
{
  Machine* machine;

  machine = (Machine *)malloc(sizeof(Machine));
  if (!machine) return(NULL);
  memset (machine, 0, sizeof (Machine));
  memcpy(machine->Name, Name + 1,  strlen(Name) - 2);
  machine->Id = Id;
  return machine;
}

LogicalAddress *AllocAddressYaccStruct(char *Name, int Id)
{
  LogicalAddress* la;

  la = (LogicalAddress *)malloc(sizeof(LogicalAddress));
  if (!la) return(NULL);
  memset (la, 0, sizeof (LogicalAddress));
  memcpy(la->Name, Name + 1,  strlen(Name) - 2);
  la->Id = Id;
  return la;
}



XlatStruct *AllocXlatYaccStruct(char *Name, int Id)
{
  int i;
  XlatStruct *VarXlat;

  VarXlat = (XlatStruct *)malloc(sizeof(XlatStruct));
  if (!VarXlat) return(NULL);

  memset(VarXlat->XlatName, '\0', 256);
  memcpy(VarXlat->XlatName, Name + 1, strlen(Name) - 2);
  VarXlat->XlatId = Id;
  memset(VarXlat->FromName, '\0', 120);
  memset(VarXlat->ToName, '\0', 120);
  for(i=0; i<16; i++)
   {
    VarXlat->Tabxlt[i] = (char *)malloc(33);
    memset((char *)VarXlat->Tabxlt[i], '\0', 33);
   }

  return((XlatStruct *)VarXlat);
}

void FreeArgumentYaccStruct(Argument *pArg)
{
  if (pArg)
    free(pArg);
  return;
}

void FreeMessageClassYaccStruct(MessageClass *pMess)
{
  Argument *Argu;

  if (pMess)
   {
     while(pMess->AllArgument)
      {
        Argu = (Argument *)pMess->AllArgument->car;
        ListRemove(&pMess->AllArgument, Argu);
        FreeArgumentYaccStruct(Argu);
      }
     free(pMess);
   }
  return;
}



void FreeMessageYaccStruct(Message *pMess)
{
  Argument *Argu;

  if (pMess)
   {
     while(pMess->AllArgument)
      {
        Argu = (Argument *)pMess->AllArgument->car;
        ListRemove(&pMess->AllArgument, Argu);
        FreeArgumentYaccStruct(Argu);
      }
     free(pMess);
   }
  return;
}


void FreePropertyYaccStruct (Property* pPro)
{
  FileArgument* Argu;
  while(pPro->Source)
   {
     Argu = (FileArgument *)pPro->Source->car;
     ListRemove(&pPro->Source, Argu);
     free (Argu);
   }
  while(pPro->Include)
   {
     Argu = (FileArgument *)pPro->Include->car;
     ListRemove(&pPro->Include, Argu);
     free (Argu);
   }
  while(pPro->Script)
   {
     Argu = (FileArgument *)pPro->Script->car;
     ListRemove(&pPro->Script, Argu);
     free (Argu);
   }
  while(pPro->Make)
   {
     Argu = (FileArgument *)pPro->Make->car;
     ListRemove(&pPro->Make, Argu);
     free (Argu);
   }
  while(pPro->Other)
   {
     Argu = (FileArgument *)pPro->Other->car;
     ListRemove(&pPro->Other, Argu);
     free (Argu);
   }
  while(pPro->Procedures)
   {
     
     ProcArgument* ProcArgu = (ProcArgument *)pPro->Procedures->car;
     ListRemove(&pPro->Procedures, ProcArgu);
     while (ProcArgu->Variables)
     {
       VarArgument* VarArgu = (VarArgument *)ProcArgu->Variables->car;
       ListRemove(&ProcArgu->Variables, VarArgu);
       free (VarArgu);
     }
     free (Argu);
   }

}


void FreeDialogYaccStruct(Dialog *pDial)
{
  MessageClass *Mess;

  if (pDial)
   {
     while(pDial->AllMessageClasses)
      {
        Mess = (MessageClass *)pDial->AllMessageClasses->car;
        ListRemove(&pDial->AllMessageClasses, Mess);
        FreeMessageClassYaccStruct(Mess);
      }
     free(pDial);
   }
  return;
}

void FreeConnectionClassYaccStruct(ConnectionClass *pConn)
{
  if (pConn) free(pConn);
  return;
}

void FreeConnectionYaccStruct(Connection* pConn)
{
  if (pConn) free(pConn);
  return;
}


void FreeXlatYaccStruct(XlatStruct *pXlat)
{
  if (pXlat) free(pXlat);
  return;
}

void FreeGlobalOneYaccStruct(GlobalOne *pGlob)
{
  Dialog*   Dial;
  ConnectionClass* ConnClass;
  Connection*      Conn;
  XlatStruct* VarXlat;

  if (pGlob)
  { 
   while(pGlob->AllDialog)
   {
    Dial = (Dialog *)pGlob->AllDialog->car;
    ListRemove(&pGlob->AllDialog, Dial);
    FreeDialogYaccStruct(Dial);
   }
   while(pGlob->AllConnectionClasses)
   {
    ConnClass = (ConnectionClass *)pGlob->AllConnectionClasses->car;
    ListRemove(&pGlob->AllConnectionClasses, ConnClass);
    FreeConnectionClassYaccStruct(ConnClass);
   }
   while(pGlob->AllConnections)
   {
    Conn = (Connection *)pGlob->AllConnections->car;
    ListRemove(&pGlob->AllConnections, Conn);
    FreeConnectionYaccStruct(Conn);
   }

   while(pGlob->AllXlatStruct)
   {
    VarXlat = (XlatStruct *)pGlob->AllXlatStruct->car;
    ListRemove(&pGlob->AllXlatStruct, VarXlat);
    FreeXlatYaccStruct(VarXlat);
   }
   while (pGlob->AllProperties)
   {
    Property* Pro = (Property*)pGlob->AllProperties->car;
    ListRemove (&pGlob->AllProperties, Pro);
    free (Pro);
   }
   while (pGlob->AllApplications)
   {
    Application* Prog = (Application*)pGlob->AllApplications->car;
    ListRemove (&pGlob->AllApplications, Prog);
    free (Prog);
   }
   while (pGlob->AllScenarios)
   {
    Scenario* Sce = (Scenario*)pGlob->AllScenarios->car;
    ListRemove (&pGlob->AllScenarios, Sce);
    free (Sce);
   }
   while (pGlob->AllDataBaseClasses)
   {
    DataBaseClass* Data = (DataBaseClass*)pGlob->AllDataBaseClasses->car;
    ListRemove (&pGlob->AllDataBaseClasses, Data);
    free (Data);
   }
   while (pGlob->AllDataBases)
   {
    DataBase* Data = (DataBase*)pGlob->AllDataBases->car;
    ListRemove (&pGlob->AllDataBases, Data);
    free (Data);
   }
   while (pGlob->AllQueueClasses)
   {
    QueueClass* queue = (QueueClass*)pGlob->AllQueueClasses->car;
    ListRemove (&pGlob->AllQueueClasses, queue);
    free (queue);
   }

   while (pGlob->AllQueues)
   {
    Queue* queue = (Queue*)pGlob->AllQueues->car;
    ListRemove (&pGlob->AllQueues, queue);
    free (queue);
   }

   while (pGlob->AllJournalClasses)
   {
    JournalClass* Sto = (JournalClass*)pGlob->AllJournalClasses->car;
    ListRemove (&pGlob->AllJournalClasses, Sto);
    free (Sto);
   }
   
   while (pGlob->AllJournals)
   {
    Journal* iSto = (Journal*)pGlob->AllJournals->car;
    ListRemove (&pGlob->AllJournals, iSto);
    free (iSto);
   }



   free(pGlob);
   TheOne = NULL;
  }
  return;
}

int SearchDialogDoublons(GlobalOne *Glob)
{
  List *Glb1, *Glb2;
  Dialog *Dial1, *Dial2;
  
  Glb1 = Glob->AllDialog;
  while(Glb1)
  {
    Dial1 = (Dialog *)Glb1->car;
    Glb2 = Glb1->cdr;
  
    while(Glb2)
      {
      Dial2 = (Dialog *)Glb2->car;
  
      if (strcmp(Dial1->DialName, Dial2->DialName) == 0) return(0);
      if (Dial1->DialId == Dial2->DialId) return(0);
      Glb2 = Glb2->cdr;
      }
    Glb1 = Glb1->cdr;
  }
  return(1);
} 

int SearchConnectDoublons(GlobalOne *Glob)
{
  List *Glb1, *Glb2;
  ConnectionClass *Conn1, *Conn2;
  
  Glb1 = Glob->AllConnectionClasses;
  while(Glb1)
  {
    Conn1 = (ConnectionClass *)Glb1->car;
    Glb2 = Glb1->cdr;
  
    while(Glb2)
    {
      Conn2 = (ConnectionClass *)Glb2->car;
  
      if (strcmp(Conn1->Name, Conn2->Name) == 0) return(0);
      if (Conn1->Id == Conn2->Id) return(0);
      Glb2 = Glb2->cdr;
    }
    Glb1 = Glb1->cdr;
  }
  return(1);
} 

int SearchXlatStructDoublons(GlobalOne *Glob)
{
  List *Glb1, *Glb2;
  XlatStruct *VarXlat1, *VarXlat2;
  
  Glb1 = Glob->AllXlatStruct;
  while(Glb1)
  {
    VarXlat1 = (XlatStruct *)Glb1->car;
    Glb2 = Glb1->cdr;
  
    while(Glb2)
      {
      VarXlat2 = (XlatStruct *)Glb2->car;
  
      if (strcmp(VarXlat1->XlatName, VarXlat2->XlatName) == 0) return(0);
      if (VarXlat1->XlatId == VarXlat2->XlatId) return(0);
      Glb2 = Glb2->cdr;
      }
    Glb1 = Glb1->cdr;
  }
  return(1);
} 

