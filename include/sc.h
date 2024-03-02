/*-------------------------------------------------------------------------*/
/*                    NETPROG (Network Programming)                        */
/* NetProg is an API in C language allowing an homogeneous programming of  */
/* communicating applications                                              */
/* Copyright (C) 2002,2003,2004,2005                                       */  
/* Gabriel Jureidini                                                       */
/* Version 2.0 - 17 July 2005                                              */
/*                                                                         */
/* This program is free software; you can redistribute it and/or modify it */
/* under the terms of the GNU General Public License as published by the   */
/* Free Software Foundation; either version 2 of the License, or (at your  */ 
/* option) any later  version.                                             */
/*                                                                         */
/* This program is distributed in the hope that it will be useful, but     */
/* WITHOUT ANY WARRANTY; without even the implied warranty of              */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the GNU General */
/* Public License for more details.                                        */
/*                                                                         */
/* You should have received a copy of the GNU General Public License along */
/* with this program; if not, write to the Free Software Foundation, Inc., */
/* 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA                   */
/* Copy of GNU General Public License at: http://www.gnu.org/              */
/*                                                                         */
/* Source code home page: http://sourceforge.net/projects/netprog/         */
/* Contact author at: Gabriel_Jureidini@yahoo.com                          */
/*-------------------------------------------------------------------------*/

#ifndef __SC_H__
#define __SC_H__

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdarg.h>
#include "ostypes.h"
#include "oslist.h"
#include "scyacc.h"
#include "mx.h"
#include "dg.h"



#define SCNOTDEFINED   FALSE
#define SCDEFINED      TRUE


#define SCATOM            1
#define SCPOINTER         2
#define SCVARIABLE_BYNAME 3
#define SCVARIABLE_BYVAL  4


#define SCMallocType(type) ((type == SCSTRING) || (type == SCDATE) || \
                            (type == SCTIME) || (type == SCCHAR))


#define SCASSERTNOTNULL(pvalue) if (!(pvalue)) return NULL

#define SCASSERTDEFINEDVALUE(pvalue) SCASSERTNOTNULL (pvalue); \
                                     else if (!(pvalue)->Defined) return pvalue


#define SCASSERTVALUE(pvalue,Type1) SCASSERTNOTNULL(pvalue);  \
                                    else if ((pvalue)->Type != (Type1)) return NULL


#define SCASSERTVALUESIZE(pvalue,size) SCASSERTNOTNULL(pvalue);  \
                                       else if ((pvalue)->Size != (size)) return NULL


#define SCASSERTNOTNULLVALUEVAL(pvalue) SCASSERTNOTNULL(pvalue) ; \
                                   else if (!(pvalue->Val)) return NULL


#define SCASSERTORVALUES(pvalue,Type1,Type2) SCASSERTNOTNULL(pvalue); \
                                             else if (((pvalue)->Type != (Type1)) &&\
                                                     ((pvalue)->Type != (Type2))) return NULL

#define SCASSERT3ORVALUES(pvalue,Type1,Type2, Type3) SCASSERTNOTNULL(pvalue); \
                                             else if (((pvalue)->Type != (Type1)) &&\
                                                      ((pvalue)->Type != (Type2)) &&\
                                                      ((pvalue)->Type != (Type3))) return NULL



#define SCSETRETURNVALUETYPE(pvalue, Type1) SCASSERTNOTNULL(pvalue); \
                                            else  SCPutValue((pvalue), NULL, (Type1))

#define SCASSERTPARAMNUMBER(paction,NbParam1) SCASSERTNOTNULL(paction); \
                                              else if ((paction)->NbParam != (NbParam1)) return NULL


#define SCASSERTMINPARAM(paction,minNbParam) SCASSERTNOTNULL(paction); \
                                             else if ((paction)->NbParam < (minNbParam)) return NULL

#define SCASSERTMAXPARAM(paction,maxNbParam) SCASSERTNOTNULL(paction); \
                                             else if ((paction)->NbParam > (maxNbParam)) return NULL



#define SCASSERTPARAMTYPE(param,Type1)   SCASSERTNOTNULL(param);\
                                         else if ((param)->Type != (Type1)) return NULL


#define SCFREEVARIABLEVALUE(Variable) if ((Variable)->Value) { SCFreeValue ((Variable)->Value);\
                                          (Variable)->Value = NULL;}




#define SCLISTACTIONS     1
#define SCWAITINGACTIONS  2

#define SCALLACTIONS  (SCWAITINGACTIONS & SCLISTACTIONS)

#define SCPROCRESULT    SCValue*



#define SCBEFORE        1
#define SCAFTER         2


#define SCDistant       1
#define SCLocal         0

#define SCBlock         TRUE
#define SCNoBlock       FALSE

#define SCTODO          0
#define SCDOING         1
#define SCDONE          2
#define SCNOTTODO       4

#define SCGetNbParam(Action)  (Action)->NbParam
#define SCGetTypeParam(Param) (Param)->Type
#define SCSetEndingProcedure(psc,proc) (psc)->EndingProcedure=proc
#define SCGetMX(psc) (psc)->MX

extern  BOOL   SCDEBUG;
extern  BOOL   SCSTEP;
extern  BOOL   SCINSTEP;
extern  BOOL   SCCONTINUE;
extern  MXCom* SCDEBUGCOM;


/* related to scripty files */

typedef struct _SCSection
{
    char*        Name;
    List*        ListEntities;
    WORD         NbEntity;
    List*        ListVariables;        /* list of variables located in section*/
    WORD         NbVariables;
    char         FileName[250];
    int          LineNumber;
}SCSection;



/* we can have external dependency and local dependency */
/* we can have persistent dependency and non persistent */

/* time is seen as an external dependency we should have a timer */
/* synchronized with bc timeout                                  */


typedef struct _SCDependency
{
    LONG        Identity;
    WORD        Persistency;
    MXCom*      Com;
}SCDependency;


typedef struct _SCValue
{
    SHORT        Type;           /* SCSTRING SCBYTE SCCHAR ... */
    WORD         Defined;        /* SCNOTDEFINED SCDEFINED     */
    DWORD        Size;           /* atom size                  */
    void*        Val;            /* atom value                 */
    List*        Dependency;
}SCValue;



typedef struct _SCAction
{
    SCSection*   Section;
    struct _SCSysAction* ActionClass;
    struct _SCEntity*    ExecCondition;
    LONG         Identity;
    WORD         Persistency;
    BOOL         Block;
    List*        ListParams;
    WORD         NbParam;
    BYTE         State;
    LONG         Error;
    List*        ExecuteBefore;
    List*        ExecuteWhenDone;
    SCValue*     Value;
    BOOL         ExecuteCondition;     /* WHEN WHILE repeat actions */
    MXCom*       Com;
    int          LineNumber;
	void*		 UserField;
}SCAction;


typedef struct _SCEntity
{
    SHORT        Type;           /* SCVARIABLE SCACTION SCATOM */
    void*        Val;            /* pointer to Value type      */
    SCAction*    RootAction;
    SCAction*    FatherAction;   /* may be null */
    int          LineNumber;
}SCEntity;


typedef struct _SCVariable
{
    char*        Name;
    MXCom*       Sharable;
    MXCom*       Exportable;
    SCValue*     Value;
    SCEntity*    SetEntity;
    int          LineNumber;
}SCVariable;


typedef struct _SCTraceProcedure
{
    void* ApplicationField;
    int (*TraceProcedure) (char* buf, LONG LineNumber, void* papp);
} SCTraceProcedure;


typedef struct _SCBreakPoint
{
    char FileName[250];
    int  LineNumber;
}SCBreakPoint;


typedef struct _SC
{
    MX*          MX;
    MXCom*       Com;
    List*        ListEntities;
    List*        ListActions;                /* all actions to do            */
    List*        ListWaitingActions;         /* noblocking actions           */
    List*        ListDoingActions;
    List*        ListVariables;              /* list of all global variables */
    List*        ListSystemActions;
    List*        ListAddedActions;
	List*        ListSections;
    List*        ListComFunctions;
    List*        ListClientFunctions;
    List*        Communications;
    List*        Clients;
    List*        BreakPoints;
    WORD         NbSections;
    int          LastError;
    SCSection*   CurrentSection;
    LONG         LastActionIdentity;
    SCSection*   InteractiveSection;
    char         CurrentFileName[250];
    void*        ApplicationField;
    SCTraceProcedure* TraceProcedure;
    int (*EndingProcedure) (SCAction* action);
    char         Version[30];
}SC;


typedef struct _SCSysAction
{
    SC*   SC;
    char* Name;
    BOOL  Block;
    List* ExecuteBefore;
    List* ExecuteWhenDone;
	int	  MinPar;
	int   MaxPar;
	int	  NbrPar;
	int	  ReturnType;

    SCPROCRESULT (*ActionProcedure)(struct _SC* psc,  SCAction* thisaction);
}SCSysAction;


typedef struct _SCExecuteProcedure
{
    void* ApplicationField;
    int (*ExecuteProcedure) (SCAction* paction, void* papp);
}SCExecuteProcedure;


typedef struct _SCComFuncStruct        /*MX*/
{
    void* Par1;
    BYTE  Type;
    char Name[100];
    char MessageClassName[100];
    char ClassName[100];
}SCComFuncStruct;



typedef struct _SCClientFuncStruct /*DB*/
{
    void* Par1;
    BYTE  Type;
    char Name[100];
}SCClientFuncStruct;




extern SC* PSC;

#ifdef __cplusplus
extern "C" {
#endif
	extern List*		SCReturnParseSections ();
    extern int          SCInit (SC* psc, MX* pmx, BOOL Interactive, char* filename);
    extern int          SCSetTraceProcedure(SC* psc, int (*funct)(char* , LONG, void*), void* app);
    extern void         SCTrace (SC* psc, LONG LineNumber, char* buf);
    extern int          SCDefaultTrace (char* buf, LONG LineNumber, void* app);
    extern int          SCDefaultPrint (char* buf, void* app);

    extern int          SCOnConnect (MXCom* pcom,void* appfield);
    extern int          SCOnClose (MXCom* pcom,  void* pappfield);
    extern int          SCOnBreakClose (MXCom* pcom, void* pappfield);

	extern char*		SCGetStringError ();

    extern int          SCAffectEntityVarToMessageValue (SCAction* paction, void* app);
    extern int          SCAffectMessageValueToEntityVar (SCAction* thisaction, void* app);
    extern int          SCDefaultWhenDone     (SCAction* paction, void* app);
	extern void			SCSetUserField (SCAction* paction, void* userfield);


    extern void         SCAddSystemActions (SC* psc);
    extern int          SCAddScriptCommands (SC* psc);
	extern SCSysAction* SCGetAddedAction (SC* psc, char* name);
    extern SCSysAction* SCGetSysAction (SC* psc, char* name);
    extern SCAction*    SCInsertAction (SC* psc, char* name, WORD nb_param, ...);
    extern SCAction*    SCCreateAction (SC* psc, char* name, WORD nb_param, ...);
    extern SCValue*     SCExecuteAction (SC* psc, SCAction* paction);
    extern SCAction*    SCExecuteString (SC* psc, char* Buffer, MXCom* pcom, LONG Identity, WORD Persistency);
    extern SCAction*    SCExecuteFile (SC* psc, char* FileName, MXCom* pcom, LONG Identity, WORD Persistency);
    extern SCAction*    SCGetNextActionToExecute (SC* psc, BYTE WaitingActions);

    extern List*        SCFindDebListEntity(SC* psc, SCEntity* pentity);

    extern void         SCCopyFromListEntity (SC* psc, SCAction* thisaction, List* listentities);
    extern SCAction*    SCGetActionFromIdentity (SC* psc, LONG Identity);
    extern SCAction*    SCGetRootActionFromAction (SC* psc, SCAction* scaction);
    extern SCAction*    SCGetRootActionFromCom (SC* psc, MXCom* pcom);
    extern void         SCExecuteBefore (SC* psc, SCAction* paction);
    extern void         SCExecuteWhenDone (SC* psc, SCAction* paction);
    extern BOOL         SCActionDone (SCAction* Action);
    extern SCAction*    SCPrepareToExecute (SC* psc, SCAction* paction, SCAction* curraction);


    extern int          SCEngine (MX* pmx, void* par);
    extern void         SCClear (SC* psc);
    extern void         SCEnd (SC* psc);
    extern void         SCError (SC* psc, int id, char* charparam, ...);

    extern SCValue*     SCGetEntityValue (SC* psc, SCEntity* Entity);
    extern SCValue*     SCSetEntityValue (SC* psc, SCEntity* Entity, SCValue* pvalue);
    extern SCEntity*    SCGetParam (SC* psc, SCAction* Action , WORD range);
    extern SCValue*     SCGetParamValue (SC* psc, SCAction* Action , WORD range);
    extern SCValue*     SCSetParamValue (SC* psc, SCAction* Action , WORD range, SCValue* pvalue);

    extern SCValue*     SCGetDistantEntityValue (SC* psc, MXCom* pcom, char* SVariable, LONG Identity);
    extern SCValue*     SCSetDistantEntityValue (SC* psc, MXCom* pcom, char* VariableName, SCValue* pvalue, LONG Identity);
    extern SCVariable*  SCCreateVariable (SC* psc, char* Name);
    extern SCVariable*  SCFindVariableFromName (SC* psc, char* Name);
    extern SCVariable*  SCFindVariableInList (SC* psc, char* Name, List* plist);
    extern SCVariable*  SCGetVariableFromPointer (SC* psc, void* val);


    extern int          SCAddFunction (SC* psc, char* actionclassname, BYTE mode, int (*funct)(SCAction*, void*), void* applicationfield);
    extern int          SCRemoveFunction (SC* psc, char* actionclassname, BYTE mode, int (*funct)(SCAction*, void*), void* applicationfield);

    extern int          SCAddFunctionToInstance (SC* psc, SCAction* paction, BYTE mode, int (*funct)(SCAction*, void*), void* applicationfield);
    extern int          SCRemoveFunctionFromInstance (SC* psc, SCAction* paction,BYTE mode, int (*funct)(SCAction*, void*), void* applicationfield);


    extern void         SCAddFunctionToSystemActions (SC* psc, BYTE mode, int (*funct)(SCAction*, void*), void* applicationfield);

    extern void         SCRemFuncFromSystemActions (SC* psc, BYTE mode, int (*funct)(SCAction*, void*), void* applicationfield);
    extern SCExecuteProcedure* SCFindExecuteProcedure (List* ExecuteList, int (*funct)(SCAction*, void*), void* applicationfield);

    extern char*        SCTraceAction (SCAction* paction);
    extern void         SCSetActionState (SC* psc, SCAction* paction, BYTE State, WORD Defined, LONG Error);
    extern SCSection*   SCCreateInteractiveSection (SC* psc);
    extern SCSection*   SCAllocSection(SC* psc, char *Name);
    extern SCEntity*    SCCopySections(SC* psc, List* *listsections);
    extern SCSection*   SCIsSectionDefinedInList(List* listsections, char* name);
    extern SCSection*   SCGetSectionFromList(List* listsections, char* name);
    extern SCSysAction* SCAllocSysAction (SC* psc, char* Name, BOOL Block,
                                              SCPROCRESULT (*Procedure)(SC* psc,  SCAction* thisaction), int MinPar, int MaxPar, int NbrPar, int ReturnType);
    extern SCAction*    SCCopyAction(SC* psc, SCAction* RootAction, SCAction* MeAction);
    extern void         SCSetEntityState (SC* psc, SCEntity* Entity, BYTE State);
    extern SCEntity*    SCCopyEntity (SC* psc, SCAction* RootAction, SCEntity* Entity);
    extern SCValue*     SCCopyValue (SC* psc, SCValue* destvalue, SCValue* sourcevalue);
    extern void         SCInitEntity (SC* psc, SCEntity* Entity);
    extern void         SCInitAction(SC* psc, SCAction* MeAction);
    extern int          SCFindValueInEntity(SC* psc, SCEntity* pEntity, SCValue* Value);
    extern int          SCInitDependentValuesFromAction (SC* psc, SCAction* paction, SCValue* Value);

    extern SCEntity*    SCAllocEntity(SC* psc,void* Val, SHORT Type);
    extern SCValue*     SCAllocValue(SC* psc, void *Val, SHORT Type);
    extern SCAction*    SCAllocAction(SC* psc, SCSection* section, SCSysAction* ActionClass, BOOL Block);
    extern SCValue*     SCPutValue(SCValue* pValue, void* Val, SHORT Type);
    extern SCValue*     SCAffectValue(SCValue* pValue, void* Val, SHORT Type);
    extern SCValue*     SCSetValue (SC* psc, SCValue* destvalue, SCValue* sourcevalue);
    extern void         SCSetEntityRootAction (SC* psc, SCEntity* Entity, SCAction* RootAction);

	
    extern SCEntity*    SCParseFile(SC* psc, char *FileName, int *error);
    extern SCEntity*    SCParseString(SC* psc, char* Chaine, int *error);
    extern SCEntity*    SCGetEntityFromString (SC* psc, char * Buffer);
    extern SCEntity*    SCGetEntityFromFuncName (SC* psc, MXMessage* pmessage, char* FuncName);


    extern int          SCCtrlTime(char* Buf);
    extern int          SCCtrlDate(char* Buf);

    extern int          SCLogin (MXCom* pcom, LONG Identity, char* User, char* Password);
    extern int          SCSendString (MXCom* pcom,  LONG Identity, WORD Persistency, char* String);
    extern int          SCSendValue (SCValue* pvalue, LONG Identity, WORD Persistency, LONG Error, MXCom* pcom, BOOL Block);
    extern void         SCPrintValue (SC* psc, SCValue* Value, char* Buf);

    extern void         SCAddDependencyValue (SCValue* Value, LONG Identity,  WORD Persistency, MXCom* pcom);
    extern void         SCAddDependencyValuesToVariables (SCEntity* FromEntity, LONG Identity,  WORD Persistency, MXCom* pcom);
    extern void         SCExecuteDependency (SC* psc, SCValue* Value, SCDependency* pDependency);
    extern void         SCCopyDependency (SC* psc, SCEntity* Entity, SCValue* FromValue);

    extern void         SCSetBlocking (SC* psc, SCAction* paction, BOOL Block);
    extern int          SCShareVariable (MXCom* pcom, char* Variable, LONG Identity);


    extern void         SCFreeSection(SCSection *pSection);
    extern void         SCFreeSysAction (SCSysAction* pSysAction);
    extern void         SCFreeAction(SCAction *pAction);
    extern void         SCDeleteAction (SC* psc, SCAction* paction);
    extern void         SCFreeEntity(SCEntity *pEntity);
    extern void         SCFreeValue(SCValue *pValue);
    extern void         SCFreeVariable (SCVariable *pVariable);

    extern int          SCInsertBreakPoint (SC* psc, char* FileName, WORD LineNumber);
    extern void         SCRemoveBreakPoints (SC* psc);


    extern SCPROCRESULT  SCExecuteProc   (SC* psc, SCAction* thisaction) ;
    extern SCPROCRESULT  SCDefunProc     (SC* psc, SCAction* thisaction) ;
    extern SCPROCRESULT  SCIfProc        (SC* psc, SCAction* thisaction) ;
    extern SCPROCRESULT  SCWhileProc     (SC* psc, SCAction* thisaction) ;
    extern SCPROCRESULT  SCParseProc     (SC* psc, SCAction* thisaction) ;
    extern SCPROCRESULT  SCExitProc      (SC* psc, SCAction* thisaction) ;
    extern SCPROCRESULT  SCCdProc        (SC* psc, SCAction* thisaction) ;
    extern SCPROCRESULT  SCRcdProc       (SC* psc, SCAction* thisaction) ;
    extern SCPROCRESULT  SCPwdProc       (SC* psc, SCAction* thisaction) ;
    extern SCPROCRESULT  SCPutProc       (SC* psc, SCAction* thisaction) ;
    extern SCPROCRESULT  SCMputProc      (SC* psc, SCAction* thisaction) ;
    extern SCPROCRESULT  SCGetProc       (SC* psc, SCAction* thisaction) ;
    extern SCPROCRESULT  SCMgetProc      (SC* psc, SCAction* thisaction) ;
    extern SCPROCRESULT  SCListProc      (SC* psc, SCAction* thisaction);
    extern SCPROCRESULT  SCDirProc       (SC* psc, SCAction* thisaction);


    extern SCPROCRESULT  SCMkdirProc     (SC* psc, SCAction* thisaction) ;
    extern SCPROCRESULT  SCRmkdirProc    (SC* psc, SCAction* thisaction) ;
    extern SCPROCRESULT  SCRmdirProc     (SC* psc, SCAction* thisaction) ;
    extern SCPROCRESULT  SCRrmdirProc    (SC* psc, SCAction* thisaction) ;
    extern SCPROCRESULT  SCDeleteProc    (SC* psc, SCAction* thisaction) ;
    extern SCPROCRESULT  SCRdeleteProc   (SC* psc, SCAction* thisaction) ;
    extern SCPROCRESULT  SCOrProc        (SC* psc, SCAction* thisaction) ;
    extern SCPROCRESULT  SCAndProc       (SC* psc, SCAction* thisaction) ;
    extern SCPROCRESULT  SCPrintProc     (SC* psc, SCAction* thisaction) ;
    extern SCPROCRESULT  SCNotProc       (SC* psc, SCAction* thisaction) ;
    extern SCPROCRESULT  SCExistProc     (SC* psc, SCAction* thisaction) ;
    extern SCPROCRESULT  SCAtProc        (SC* psc, SCAction* thisaction) ;
    extern SCPROCRESULT  SCEveryProc     (SC* psc, SCAction* thisaction) ;
    extern SCPROCRESULT  SCSetqProc      (SC* psc, SCAction* thisaction) ;
    extern SCPROCRESULT  SCSetProc       (SC* psc, SCAction* thisaction) ;
    extern SCPROCRESULT  SCWhenProc      (SC* psc, SCAction* thisaction) ;
    extern SCPROCRESULT  SCGetCharProc   (SC* psc, SCAction* thisaction) ;
    extern SCPROCRESULT  SCPutCharProc   (SC* psc, SCAction* thisaction) ;
    extern SCPROCRESULT  SCEqualProc     (SC* psc, SCAction* thisaction) ;
    extern SCPROCRESULT  SCTimeProc      (SC* psc, SCAction* thisaction) ;
    extern SCPROCRESULT  SCDateProc      (SC* psc, SCAction* thisaction) ;
    extern SCPROCRESULT  SCEvalProc      (SC* psc, SCAction* thisaction) ;
    extern SCPROCRESULT  SCWaitProc      (SC* psc, SCAction* thisaction) ;
    extern SCPROCRESULT  SCEveryTimeProc (SC* psc, SCAction* thisaction) ;
    extern SCPROCRESULT  SCSystemProc    (SC* psc, SCAction* thisaction) ;
    extern SCPROCRESULT  SCProcessProc   (SC* psc, SCAction* thisaction) ;
    extern SCPROCRESULT  SCRemoteProc    (SC* psc, SCAction* thisaction) ;
    extern SCPROCRESULT  SCPingProc      (SC* psc, SCAction* thisaction) ;
    extern SCPROCRESULT  SCJavaProc      (SC* psc, SCAction* thisaction) ;
    extern SCPROCRESULT  SCTraceProc     (SC* psc, SCAction* thisaction) ;
    extern SCPROCRESULT  SCSetTraceFileProc(SC* psc,  SCAction* thisaction);

    extern SCPROCRESULT  SCShareProc     (SC* psc, SCAction* thisaction) ;
    extern SCPROCRESULT  SCOpeAddProc    (SC* psc, SCAction* thisaction) ;
    extern SCPROCRESULT  SCOpeMinusProc  (SC* psc, SCAction* thisaction) ;
    extern SCPROCRESULT  SCOpeMultProc   (SC* psc, SCAction* thisaction) ;
    extern SCPROCRESULT  SCOpeDivProc    (SC* psc, SCAction* thisaction) ;
    extern SCPROCRESULT  SCMallocProc    (SC* psc, SCAction* thisaction) ;
    extern SCPROCRESULT  SCFreeProc      (SC* psc, SCAction* thisaction) ;
    extern SCPROCRESULT  SCOpenProc      (SC* psc, SCAction* thisaction) ;
    extern SCPROCRESULT  SCCloseProc     (SC* psc, SCAction* thisaction) ;
    extern SCPROCRESULT  SCReadProc      (SC* psc, SCAction* thisaction) ;
    extern SCPROCRESULT  SCGetlineProc   (SC* psc, SCAction* thisaction) ;
    extern SCPROCRESULT  SCPutlineProc   (SC* psc, SCAction* thisaction) ;
    extern SCPROCRESULT  SCWriteProc     (SC* psc, SCAction* thisaction) ;
    extern SCPROCRESULT  SCCopyProc      (SC* psc, SCAction* thisaction) ;
    extern SCPROCRESULT  SCReplaceProc   (SC* psc,  SCAction* thisaction);
    extern SCPROCRESULT  SCRenameProc    (SC* psc, SCAction* thisaction) ;
    extern SCPROCRESULT  SCAtoiProc      (SC* psc, SCAction* thisaction) ;
    extern SCPROCRESULT  SCStringProc    (SC* psc, SCAction* thisaction) ;
    extern SCPROCRESULT  SCGetLastError  (SC* psc, SCAction* thisaction) ;
    extern SCPROCRESULT  SCGetVersionProc(SC* psc, SCAction* thisaction) ;
    extern SCPROCRESULT  SCMaxIntProc    (SC* psc, SCAction* thisaction) ;
    extern SCPROCRESULT  SCMinIntProc    (SC* psc, SCAction* thisaction) ;
    extern SCPROCRESULT  SCGreaterProc   (SC* psc, SCAction* thisaction) ;
    extern SCPROCRESULT  SCSmallerProc   (SC* psc, SCAction* thisaction) ;
    extern SCPROCRESULT  SCGreatEquProc  (SC* psc, SCAction* thisaction) ;
    extern SCPROCRESULT  SCSmallEquProc  (SC* psc, SCAction* thisaction) ;

    /* STRING*/
    extern SCPROCRESULT  SCStrLengthProc (SC* psc, SCAction* thisaction) ;
    extern SCPROCRESULT  SCStrTrimProc  (SC* psc, SCAction* thisaction) ;



    /* FILEPARM*/
    extern SCPROCRESULT  SCGetNameProc   (SC* psc, SCAction* thisaction) ;
    extern SCPROCRESULT  SCGetFromNameProc  (SC* psc, SCAction* thisaction) ;
    extern SCPROCRESULT  SCGetTypeProc   (SC* psc, SCAction* thisaction) ;
    extern SCPROCRESULT  SCGetSizeProc   (SC* psc, SCAction* thisaction) ;
    extern SCPROCRESULT  SCGetErrorProc  (SC* psc, SCAction* thisaction) ;

    extern SCPROCRESULT  SCSetNameProc   (SC* psc, SCAction* thisaction) ;
    extern SCPROCRESULT  SCSetFromNameProc  (SC* psc, SCAction* thisaction) ;
    extern SCPROCRESULT  SCSetTypeProc   (SC* psc, SCAction* thisaction) ;


    /* BUFFERPARM */

    extern SCPROCRESULT  SCGetBufferProc  (SC* psc, SCAction* thisaction) ;
    extern SCPROCRESULT  SCGetBTypeProc   (SC* psc, SCAction* thisaction) ;
    extern SCPROCRESULT  SCGetBSizeProc   (SC* psc, SCAction* thisaction) ;
    extern SCPROCRESULT  SCSetBufferProc  (SC* psc, SCAction* thisaction) ;
    extern SCPROCRESULT  SCSetBTypeProc   (SC* psc, SCAction* thisaction) ;
    extern SCPROCRESULT  SCSetBSizeProc   (SC* psc, SCAction* thisaction) ;






    /* MX*/
    extern SCPROCRESULT  SCMXAddPort       (SC* psc, SCAction* thisaction) ;
    extern SCPROCRESULT  SCMXLoadClass     (SC* psc, SCAction* thisaction) ;
    extern SCPROCRESULT  SCMXLoadResource  (SC* psc, SCAction* thisaction) ;
    extern SCPROCRESULT  SCMXOpenCom       (SC* psc, SCAction* thisaction) ;
    extern SCPROCRESULT  SCMXCloseCom      (SC* psc, SCAction* thisaction) ;
    extern SCPROCRESULT  SCMXPutMessage    (SC* psc, SCAction* thisaction) ;
    extern SCPROCRESULT  SCMXOnSendMessage (SC* psc, SCAction* thisaction) ;
    extern SCPROCRESULT  SCMXOnRecvMessage (SC* psc, SCAction* thisaction) ;
    extern SCPROCRESULT  SCMXCreateMessage (SC* psc, SCAction* thisaction) ;
    extern SCPROCRESULT  SCMXSetCGIFile    (SC* psc, SCAction* thisaction) ;


    extern SCPROCRESULT  SCMXSend          (SC* psc, SCAction* thisaction) ;
    extern SCPROCRESULT  SCMXRecv          (SC* psc, SCAction* thisaction) ;

    extern SCPROCRESULT  SCMXAddComCallBack   (SC* psc, SCAction* thisaction) ;
    extern SCPROCRESULT  SCMXRemoveComCallBack(SC* psc, SCAction* thisaction) ;

    extern SCPROCRESULT  SCMXAddEndingProcedure    (SC* psc, SCAction* thisaction) ;
    extern SCPROCRESULT  SCMXRemoveEndingProcedure (SC* psc, SCAction* thisaction) ;
    extern SCPROCRESULT  SCMXAddGeneralConnectCallBack    (SC* psc, SCAction* thisaction) ;
    extern SCPROCRESULT  SCMXRemoveGeneralConnectCallBack (SC* psc, SCAction* thisaction) ;
    extern SCPROCRESULT  SCMXSetIOMessageClassProc (SC* psc, SCAction* thisaction);
    extern SCPROCRESULT  SCMXGetMessageClassFromNames (SC* psc, SCAction* thisaction);
    extern SCPROCRESULT  SCMXCreateMessageClass (SC* psc,  SCAction* thisaction);

    extern SCPROCRESULT  SCMXGetType       (SC* psc,  SCAction* thisaction);
    extern SCPROCRESULT  SCMXGetName       (SC* psc,  SCAction* thisaction);
    extern SCPROCRESULT  SCMXGetNbrObjects (SC* psc,  SCAction* thisaction);
    extern SCPROCRESULT  SCMXGetRangeValue (SC* psc, SCAction* thisaction) ;
    extern SCPROCRESULT  SCMXSetRangeValue (SC* psc, SCAction* thisaction) ;

    extern SCPROCRESULT  SCMXGetValue      (SC* psc, SCAction* thisaction) ;
    extern SCPROCRESULT  SCMXSetValue      (SC* psc, SCAction* thisaction) ;
    extern SCPROCRESULT  SCMXFreeMessage   (SC* psc, SCAction* thisaction) ;


    extern SCPROCRESULT  SCCGIGetValueFormEntry (SC* psc, SCAction* thisaction);
    extern SCPROCRESULT  SCCGIAddValue (SC* psc, SCAction* thisaction);
    extern SCPROCRESULT  SCCGIAddColumn(SC* psc, SCAction* thisaction);




    extern SCPROCRESULT  SCMXCreateMessage (SC* psc, SCAction* thisaction) ;
    extern SCPROCRESULT  SCMXJoinCom       (SC* psc, SCAction* thisaction) ;
    extern SCPROCRESULT  SCInterruptedCom  (SC* psc, SCAction* thisaction) ;
    extern SCPROCRESULT  SCDGSendMessage   (SC* psc, SCAction* thisaction) ;

    extern SCPROCRESULT  SCLook           (SC* psc,  SCAction* thisaction);


    extern SCPROCRESULT  SCSendMailProc   (SC* psc,  SCAction* thisaction);


    extern TPClient*    ping (TP* ptp, char* name);

    /* associate to mxsys */
    extern int          SCGeneralConnect        (MXCom* pcom, void* par);
    extern int          SCReceiveMessage        (MXMessage* pmessage,  MXCom* pcom, void* appfield);

    extern int          SCDirReplyCommand       (MXMessage* pmessage, MXCom* pcom, void* appfield);
    extern int          SCFileReplyCommand      (MXMessage* pmessage, MXCom* pcom, void* appfield);
    extern int          SCListReplyCommand      (MXMessage* pmessage, MXCom* pcom, void* appfield);
    extern int          SCGDirReplyCommand      (MXMessage* pmessage, MXCom* pcom, void* appfield);

    /* FTP*/
    extern int          SCPutReplyCommand       (MXMessage* pmessage, MXCom* pcom, void* appfield);
    extern int          SCGetReplyCommand       (MXMessage* pmessage, MXCom* pcom, void* appfield);


    extern int          SCRemoteTrace           (char* buf, LONG LineNumber, void* app);
    extern int          SCRemotePrint           (char* buf, void* app);
    extern int          SCRemoteTraceAction     (SCAction* paction, void* app);
    extern int          SCRemoteDebug           (SC* psc, MXCom* pcom,  WORD Flag);
    extern int          SCRemoteParse           (SC* psc, MXCom * pcom, char* FileName, char* Content, WORD Debug);
    extern int          SCRemoteClear           (SC* psc, MXCom* pcom);
    extern int          SCRemoteStep            (SC* psc, MXCom* pcom);
    extern int          SCRemoteInStep          (SC* psc, MXCom* pcom);
    extern int          SCRemoteContinue        (SC* psc, MXCom* pcom);
    extern int          SCRemoteVariabes        (SC* psc, MXCom* pcom,  WORD Flag);
    extern int          SCRemoteSections        (SC* psc, MXCom* pcom,  WORD Flag);
    extern int          SCRemoteInsertBP        (SC* psc, MXCom* pcom, STRING FileName, WORD LineNumber);
    extern int          SCRemoteRemoveBP        (SC* psc, MXCom* pcom);


    /* associate to scsys */

    extern int          SYS_Login               (MXMessage* pmessage, MXCom* pcom, void* appfield);
    extern int          SYS_LoginReply          (MXMessage* pmessage, MXCom* pcom, void* appfield);
    extern int          SYS_ParseString         (MXMessage* pmessage, MXCom* pcom, void* appfield);
    extern int          SYS_ParseReply          (MXMessage* pmessage, MXCom* pcom, void* appfield);
    extern int          SYS_ShareVariable       (MXMessage* pmessage, MXCom* pcom, void* appfield);
    extern int          SYS_ShareReply          (MXMessage* pmessage, MXCom* pcom, void* appfield);
    extern int          SYS_GetEntityValueReply (MXMessage* pmessage, MXCom* pcom, void* appfield);
    extern int          SYS_GetVariableValue    (MXMessage* pmessage, MXCom* pcom, void* appfield);
    extern int          SYS_SetEntityValueReply (MXMessage* pmessage, MXCom* pcom, void* appfield);

    extern int          SYS_RemoteDebug         (MXMessage* pmessage, MXCom* pcom, void* appfield);
    extern int          SYS_RemoteParse         (MXMessage* pmessage, MXCom* pcom, void* appfield);
    extern int          SYS_RemoteClear         (MXMessage* pmessage, MXCom* pcom, void* appfield);
    extern int          SYS_RemoteSections      (MXMessage* pmessage, MXCom* pcom, void* appfield);
    extern int          SYS_RemoteVariables     (MXMessage* pmessage, MXCom* pcom, void* appfield);
    extern int          SYS_RemoteInsertBP      (MXMessage* pmessage, MXCom* pcom, void* appfield);
    extern int          SYS_RemoteRemoveBP      (MXMessage* pmessage, MXCom* pcom, void* appfield);
    extern int          SYS_RemoteStep          (MXMessage* pmessage, MXCom* pcom, void* appfield);
    extern int          SYS_RemoteInStep        (MXMessage* pmessage, MXCom* pcom, void* appfield);
    extern int          SYS_RemoteContinue      (MXMessage* pmessage, MXCom* pcom, void* appfield);



    /* associate to db */
    extern int            SCDBAck               (void *ack, void *par);
    extern int            SCDBShowContents      (void *show, void *par);
    extern int            SCDBQueryContents     (void *sqlda, void *par);
    extern int            SCDBQueryHeader       (void *sqlda, void *par);
    extern int            SCDBAckAction         (void* pclient, void *papp);

#ifdef __cplusplus
}
#endif



#endif /*__SC_H__*/
