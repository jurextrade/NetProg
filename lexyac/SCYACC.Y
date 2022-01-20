/***************************************************************/
/* Ces modifications sont a apporter au source SCYACC.C        */
/*                                                             */
/* ajout au niveau du fichier SCYACC.H de :                    */
/*            #ifndef  __SCYACC_H__                            */
/*            #define  __SCYACC_H__                            */
/*              ...                                            */
/*            #endif                                           */
/*                                                             */
/* suppression de la structure YYSTYPE et de define en entête  */
/* du fichier SCYACC.C car déjà défini dans le fichier SCYACC.H*/
/*                                                             */
/* ajout du paramètre SC* psc à la fonction yyparse ()         */
/*         yyparse (SC* psc)                                   */
/*                                                             */
/* ajout du paramètre psc au niveau des appels de yylex ()     */
/*         yylex (psc)                                         */
/*                                                             */
/***************************************************************/

/* declarations */
%union
{
 int    val_i;
 long   val_l;
 double val_f;
 char*  str;
 struct _SCEntity*  pentity;
 struct _SCSection* psection;
};

%token <str>   SCINTEGER
%token <str>   SCLONG
%token <str>   SCFLOAT
%token <val_i> SCBOOLEAN
%token <str>   SCSTRING
%token <str>   SCVARIABLE
%token <str>   SCOPERATOR
%token <str>   SCOPERATORDEFUN
%token <str>   SCOPERATORREMOTE
%token <str>   SCCHAR
%token <str>   SCTIME
%token <str>   SCDATE
%token <str>   SCNULL
%token PAR_DEB PAR_FIN
%token ERR_CHAR ERR_INT ERR_LONG ERR_FLOAT ERR_TIME ERR_DATE ERR_SYNTAX
%token ERR_ALLOC ERR_NOTFOUND

%type <str>      object_var_string
%type <pentity>  object_var object statement
%type <psection> ope_defun function

%start program

%{
    #include "scly.h"

    typedef struct _EntityAction
    {
      SCEntity* pentity;
      BYTE actionremote;
    }EntityAction;

    EntityAction TabEntityAction[NB_ACTIONS_OVERLAPED];

    int niveau;
    BYTE InStringRemote;
    char StringRemote[YYLMAX];
    int  InRemoteList;
    BYTE InModeDefun;      /* pour la gestion de variables */
    List* SCReturnParseSection;

    SCSysAction* psysaction_global;
    SCSection*   psection_global;
    SCSection*   psection_main;
    SCEntity*    pentity_global;
    SCAction*    paction_global;
    SCVariable*  pvariable_global;
%}

/* grammar rules */
%%

program:
  /* rien */
| program object       {
                         Trace("--- Yacc --- Object");
                         /* add object to MAIN section */
                         ListNewr(&psection_main->ListEntities, $2);
                         psection_main->NbEntity++;

                         SCInitYacc(0);
                       }
|  program statement   {
                         Trace("--- Yacc --- Statement");
                         /* add object to MAIN section */
                         ListNewr(&psection_main->ListEntities, $2);
                         psection_main->NbEntity++;

                         SCInitYacc(0);
                       }
|  program function    {
                         Trace("--- Yacc --- Function");
                         /* add function to List */
                         ListNewr(&SCReturnParseSection, $2);

                         /* add entity to MAIN section */
                         psysaction_global = SCGetSysAction(psc, "DEFUN");
                         if (!psysaction_global)
                         {
                           Err(ERR_NOTFOUND, "SCSysAction not found");
                           YYABORT;
                         }
                         paction_global = SCAllocAction(psc, psection_global, psysaction_global, psysaction_global->Block);
                         if (!paction_global)
                         {
                           Err(ERR_ALLOC, "SCAction not allocated");
                           YYABORT;
                         }
						 paction_global->LineNumber = yylineno;
                         /* on ajoute une string du nom de la fonction */
                         pentity_global = SCAllocEntity(psc, (char *)($2->Name), SCSTRING);
                         if (!pentity_global)
                         {
                           Err(ERR_ALLOC, "SCEntity SCSTRING not allocated");
                           YYABORT;
                         }
                         ListNewr(&paction_global->ListParams, pentity_global);
                         paction_global->NbParam++;
                         pentity_global = SCAllocEntity(psc, paction_global, SCOPERATOR);
                         if (!pentity_global)
                         {
                           Err(ERR_ALLOC, "SCEntity SCOPERATOR not allocated");
                           YYABORT;
                         }
                         ListNewr(&psection_main->ListEntities, pentity_global);
                         psection_main->NbEntity++;

                         SCInitYacc(0);
                       }
|  program error       { Err(ERR_SYNTAX, NULL); yyerrok; yyclearin; YYABORT; }
;

function:
   PAR_DEB ope_defun PAR_DEB PAR_FIN funclist PAR_FIN
                       {
                         if (!InStringRemote)
                         {
                           Trace("--- Yacc --- FUNCTION without paramlist");
                           $$ = $2;
                         }
                         else
                           InRemoteList++;
                       }
|  PAR_DEB ope_defun PAR_DEB paramlist PAR_FIN funclist PAR_FIN
                       {
                         if (!InStringRemote)
                         {
                           Trace("--- Yacc --- FUNCTION with paramlist");
                           $$ = $2;
                         }
                         else
                           InRemoteList++;
                       }
;

ope_defun:
   SCOPERATORDEFUN object_var_string
                       {
                         if (!InStringRemote)
                         {
                           Trace("--- Yacc --- Ope (%s)", $2);
                           psection_global = SCCreateSection(SCReturnParseSection, $2, psc->CurrentFileName);
                           if (!psection_global)
                           {
                             Err(ERR_ALLOC, "SCSection not allocated or SCSection just created");
                             YYABORT;
                           }
                           $$ = psection_global;
                           InModeDefun = 1;
                         }
                       }
;

paramlist:
   object_var_local    {
                           Trace("--- Yacc --- ADD object_local1 PARAMLIST");
                       }
|  paramlist object_var_local
                       {
                           Trace("--- Yacc --- ADD object_local PARAMLIST");
                       }
;

funclist:
   object              {
                         if (!InStringRemote)
                         {
                           Trace("--- Yacc --- ADD object1 FUNCLIST");
                           SCAddEntityToList($1);
                         }
                         else
                           InRemoteList--;
                       }
|  statement           {
                         if (!InStringRemote)
                         {
                           Trace("--- Yacc --- ADD statement1 FUNCLIST");
                           SCAddEntityToList($1);
                         }
                         else
                           InRemoteList--;
                       }
|  funclist object     {
                         if (!InStringRemote)
                         {
                           Trace("--- Yacc --- ADD object FUNCLIST");
                           SCAddEntityToList($2);
                         }
                         else
                           InRemoteList--;
                       }
|  funclist statement  {
                         if (!InStringRemote)
                         {
                           Trace("--- Yacc --- ADD statement FUNCLIST");
                           SCAddEntityToList($2);
                         }
                         else
                           InRemoteList--;
                       }
;


statement:
   PAR_DEB ope PAR_FIN {
                         if (!InStringRemote)
                         {
                           Trace("--- Yacc --- STATEMENT without list");
                           niveau--;
                           $$ = TabEntityAction[niveau].pentity;
                           TabEntityAction[niveau].pentity      = NULL;
                           TabEntityAction[niveau].actionremote = 0;

                           if (((niveau - 1) >= 0) && (TabEntityAction[niveau - 1].actionremote))
                             InStringRemote = 1;
                         }
                         else
                           InRemoteList++;
                       }
|  PAR_DEB ope statlist PAR_FIN
                       {
                         if (!InStringRemote)
                         {
                           Trace("--- Yacc --- STATEMENT with list");
                           niveau--;
                           $$ = TabEntityAction[niveau].pentity;
                           TabEntityAction[niveau].pentity      = NULL;
                           TabEntityAction[niveau].actionremote = 0;

                           if (((niveau - 1) >= 0) && (TabEntityAction[niveau - 1].actionremote))
                             InStringRemote = 1;
                         }
                         else
                           InRemoteList++;
                       }
|  PAR_DEB ope_remote object_var remotelist PAR_FIN
                       {
                         Trace("--- Yacc --- STATEMENT REMOTE [%d]", InStringRemote);
                         if (InStringRemote && (InRemoteList == 0))
                            InStringRemote = 0;

                         if (!InStringRemote)
                         {
                           /* ne pas oublier de supprimer la derniere parenthese */
                           StringRemote[strlen(StringRemote) - 1] = 0;
                           Trace("--- Yacc --- STATEMENT REMOTE [%s]", StringRemote);

                           niveau--;
                           /*  on ajoute l'object_var */
                           paction_global = (SCAction *)((SCEntity *)(TabEntityAction[niveau].pentity)->Val);
                           ListNewr(&paction_global->ListParams, $3);
                           paction_global->NbParam++;
                           /* on ajoute la string */
                           pentity_global = SCAllocEntity(psc, StringRemote, SCSTRING);
                           if (!pentity_global)
                           {
                             Err(ERR_ALLOC, "SCEntity SCSTRING not allocated");
                             YYABORT;
                           }
                           ListNewr(&paction_global->ListParams, pentity_global);
                           paction_global->NbParam++;
                           StringRemote[0] = 0;

                           $$ = TabEntityAction[niveau].pentity;
                           TabEntityAction[niveau].pentity      = NULL;
                           TabEntityAction[niveau].actionremote = 0;

                           if (((niveau - 1) >= 0) && (TabEntityAction[niveau - 1].actionremote))
                             InStringRemote = 1;
                         }
                         else
                            InRemoteList--;  /* pour decrementer l'object_var */
                       }
|  PAR_DEB ope_remote statement remotelist PAR_FIN
                       {
                         Trace("--- Yacc --- STATEMENT REMOTE [%d]", InStringRemote);
                         if (InStringRemote && (InRemoteList == 0))
                           InStringRemote = 0;

                         if (!InStringRemote)
                         {
                            /* ne pas oublier de supprimer la derniere parenthese */
                           StringRemote[strlen(StringRemote) - 1] = 0;
                           Trace("--- Yacc --- STATEMENT REMOTE [%s]", StringRemote);

                           niveau--;
                           /*  on ajoute le statement */
                           paction_global = (SCAction *)((SCEntity *)(TabEntityAction[niveau].pentity)->Val);
                           ListNewr(&paction_global->ListParams, $3);
                           paction_global->NbParam++;
                           /* on ajoute la string */
                           pentity_global = SCAllocEntity(psc, StringRemote, SCSTRING);
                           if (!pentity_global)
                           {
                             Err(ERR_ALLOC, "SCEntity SCSTRING not allocated");
                             YYABORT;
                           }
                           ListNewr(&paction_global->ListParams, pentity_global);
                           paction_global->NbParam++;
                           StringRemote[0] = 0;

                           $$ = TabEntityAction[niveau].pentity;
                           TabEntityAction[niveau].pentity      = NULL;
                           TabEntityAction[niveau].actionremote = 0;


                           if (((niveau - 1) >= 0) && (TabEntityAction[niveau - 1].actionremote))
                             InStringRemote = 1;
                         }
                         else
                            InRemoteList--;  /* pour decrementer le statement */
                       }
;

ope:
   object_var_string   {
                         if (!InStringRemote)
                         {
                           Trace("--- Yacc --- OPE (%s)", $1);
                           psysaction_global = SCGetSysAction(psc, "EXECUTE");
                           if (!psysaction_global)
                           {
                             Err(ERR_NOTFOUND, "SCSysAction not found");
                             YYABORT;
                           }
                           paction_global = SCAllocAction(psc, psection_global, psysaction_global, psysaction_global->Block);
                           if (!paction_global)
                           {
                             Err(ERR_ALLOC, "SCAction not allocated");
                             YYABORT;
                           }
  						   paction_global->LineNumber = yylineno;
                           /* on ajoute une string du nom de l'operateur */
                           pentity_global = SCAllocEntity(psc, $1, SCSTRING);
                           if (!pentity_global)
                           {
                             Err(ERR_ALLOC, "SCEntity SCSTRING not allocated");
                             YYABORT;
                           }
                           ListNewr(&paction_global->ListParams, pentity_global);
                           paction_global->NbParam++;

                           pentity_global = SCAllocEntity(psc, paction_global, SCOPERATOR);
                           if (!pentity_global)
                           {
                             Err(ERR_ALLOC, "SCEntity SCOPERATOR not allocated");
                             YYABORT;
                           }
                           TabEntityAction[niveau].pentity       = pentity_global;
                           TabEntityAction[niveau].actionremote  = 0;
                           niveau++;
                         }
                       }
|  SCOPERATOR          {
                         if (!InStringRemote)
                         {
                           Trace("--- Yacc --- OPE (%s)", $1);
                           psysaction_global = SCGetSysAction(psc, $1);
                           if (!psysaction_global)
                           {
                             Err(ERR_NOTFOUND, "SCSysAction not found");
                             YYABORT;
                           }
                           paction_global = SCAllocAction(psc, psection_global, psysaction_global, psysaction_global->Block);
                           if (!paction_global)
                           {
                             Err(ERR_ALLOC, "SCAction not allocated");
                             YYABORT;
                           }
						   paction_global->LineNumber = yylineno;

                           pentity_global = SCAllocEntity(psc, paction_global, SCOPERATOR);
                           if (!pentity_global)
                           {
                             Err(ERR_ALLOC, "SCEntity SCOPERATOR not allocated");
                             YYABORT;
                           }
                           TabEntityAction[niveau].pentity       = pentity_global;
                           TabEntityAction[niveau].actionremote  = 0;
                           niveau++;
                         }
                       }
;

ope_remote:
   SCOPERATORREMOTE    {
                         if (!InStringRemote)
                         {
                           Trace("--- Yacc --- OPE REMOTE (%s)", $1);
                           psysaction_global = SCGetSysAction(psc, $1);
                           if (!psysaction_global)
                           {
                             Err(ERR_NOTFOUND, "SCSysAction	not found");
                             YYABORT;
                           }
                           paction_global = SCAllocAction(psc, psection_global, psysaction_global, psysaction_global->Block);
                           if (!paction_global)
                           {
                             Err(ERR_ALLOC, "SCAction not allocated");
                             YYABORT;
                           }
						   paction_global->LineNumber = yylineno;

                           pentity_global = SCAllocEntity(psc, paction_global, SCOPERATOR);
                           if (!pentity_global)
                           {
                             Err(ERR_ALLOC, "SCEntity SCOPERATOR not allocated");
                             YYABORT;
                           }
                           TabEntityAction[niveau].pentity       = pentity_global;
                           TabEntityAction[niveau].actionremote  = 1;
                           niveau++;
                         }
                         else
                           InRemoteList++;
                       }
;

remotelist:
   object              { InRemoteList--;
                         Trace("--- Yacc --- ADD object1 REMOTELIST (%d)", InRemoteList);
                       }
|  statement           { InRemoteList--;
                         Trace("--- Yacc --- ADD statement1 REMOTELIST (%d)", InRemoteList);
                       }
|  function            { InRemoteList--;
                         Trace("--- Yacc --- ADD function1 REMOTELIST (%d)", InRemoteList);
                       }
|  remotelist object   { InRemoteList--;
                         Trace("--- Yacc --- ADD object REMOTELIST (%d)", InRemoteList);
                       }
|  remotelist statement { InRemoteList--;
                          Trace("--- Yacc --- ADD statement REMOTELIST (%d)", InRemoteList);
                        }
|  remotelist function { InRemoteList--;
                         Trace("--- Yacc --- ADD function REMOTELIST (%d)", InRemoteList);
                       }
;

statlist:
   object              {
                         if (!InStringRemote)
                         {
                           Trace("--- Yacc --- ADD object1 STATLIST");
                           SCAddEntityToList($1);
                         }
                         else
                           InRemoteList--;
                       }
|  statement           {
                         if (!InStringRemote)
                         {
                           Trace("--- Yacc --- ADD statement1 STATLIST");
                           SCAddEntityToList($1);
                         }
                         else
                           InRemoteList--;
                       }
|  statlist object     {
                         if (!InStringRemote)
                         {
                           Trace("--- Yacc --- ADD object STATLIST");
                           SCAddEntityToList($2);
                         }
                         else
                           InRemoteList--;
                       }
|  statlist statement  {
                         if (!InStringRemote)
                         {
                           Trace("--- Yacc --- ADD statement STATLIST");
                           SCAddEntityToList($2);
                         }
                         else
                           InRemoteList--;
                       }
;

object:
   object_var          {
                         $$ = $1;
                       }
|  SCINTEGER           {
                         if (!InStringRemote)
                         {
                           Trace("--- Yacc --- SCINTEGER (%s)", $1);
                           pentity_global = SCAllocEntity(psc, $1, SCINTEGER);
                           if (!pentity_global)
                           {
                             Err(ERR_ALLOC, "SCEntity SCINTEGER not allocated");
                             YYABORT;
                           }
                           $$ = pentity_global;
                         }
                         else
                           InRemoteList++;
                       }
|  SCLONG              {
                         if (!InStringRemote)
                         {
                           Trace("--- Yacc --- SCLONG (%s)", $1);
                           pentity_global = SCAllocEntity(psc, $1, SCLONG);
                           if (!pentity_global)
                           {
                             Err(ERR_ALLOC, "SCEntity SCLONG not allocated");
                             YYABORT;
                           }
                           $$ = pentity_global;
                         }
                         else
                           InRemoteList++;
                       }
|  SCFLOAT             {
                         if (!InStringRemote)
                         {
                           Trace("--- Yacc --- SCFLOAT (%s)", $1);
                           pentity_global = SCAllocEntity(psc, $1, SCFLOAT);
                           if (!pentity_global)
                           {
                             Err(ERR_ALLOC, "SCEntity SCFLOAT not allocated");
                             YYABORT;
                           }
                           $$ = pentity_global;
                         }
                         else
                           InRemoteList++;
                       }
|  SCBOOLEAN           {
                         if (!InStringRemote)
                         {
                           Trace("--- Yacc --- SCBOOLEAN (%d)", $1);
                           pentity_global = SCAllocEntity(psc, &$1, SCBOOLEAN);
                           if (!pentity_global)
                           {
                             Err(ERR_ALLOC, "SCEntity SCBOOLEAN not allocated");
                             YYABORT;
                           }
                           $$ = pentity_global;
                         }
                         else
                           InRemoteList++;
                       }
|  SCSTRING            {
                         if (!InStringRemote)
                         {
                           Trace("--- Yacc --- SCSTRING (%s)", $1);
                           pentity_global = SCAllocEntity(psc, $1, SCSTRING);
                           if (!pentity_global)
                           {
                             Err(ERR_ALLOC, "SCEntity SCSTRING not allocated");
                             YYABORT;
                           }
                           $$ = pentity_global;
                         }
                         else
                           InRemoteList++;
                       }
|  SCCHAR              {
                         if (!InStringRemote)
                         {
                           Trace("--- Yacc --- SCCHAR (%s)", $1);
                           pentity_global = SCAllocEntity(psc, $1, SCCHAR);
                           if (!pentity_global)
                           {
                             Err(ERR_ALLOC, "SCEntity SCCHAR not allocated");
                             YYABORT;
                           }
                           $$ = pentity_global;
                         }
                         else
                           InRemoteList++;
                       }
|  SCTIME              {
                         if (!InStringRemote)
                         {
                           Trace("--- Yacc --- SCTIME (%s)", $1);
                           pentity_global = SCAllocEntity(psc, $1, SCTIME);
                           if (!pentity_global)
                           {
                             Err(ERR_ALLOC, "SCEntity SCTIME not allocated");
                             YYABORT;
                           }
                           $$ = pentity_global;
                         }
                         else
                           InRemoteList++;
                       }
|  SCDATE              {
                         if (!InStringRemote)
                         {
                           Trace("--- Yacc --- SCDATE (%s)", $1);
                           pentity_global = SCAllocEntity(psc, $1, SCDATE);
                           if (!pentity_global)
                           {
                             Err(ERR_ALLOC, "SCEntity SCDATE not allocated");
                             YYABORT;
                           }
                           $$ = pentity_global;
                         }
                         else
                           InRemoteList++;
                       }
|  SCNULL              {
                         if (!InStringRemote)
                         {
                           Trace("--- Yacc --- SCNULL (%s)", $1);
                           pentity_global = SCAllocEntity(psc, $1, SCNULL);
                           if (!pentity_global)
                           {
                             Err(ERR_ALLOC, "SCEntity SCNULL not allocated");
                             YYABORT;
                           }
                           $$ = pentity_global;
                         }
                         else
                           InRemoteList++;
                       }
;

object_var:
   object_var_string   {
                         if (!InStringRemote)
                         {
                           Trace("--- Yacc --- SCVARIABLE (%s)", $1);

                           if (InModeDefun)
                           {
                              pvariable_global = SCFindVariableInList(psc, (char*)$1, psection_global->ListVariables);
                              if (!pvariable_global)
                              {
                                 pvariable_global = SCFindVariableInList(psc, (char*)$1, psc->ListVariables);
                                 if (!pvariable_global)
                                 {
                                    pvariable_global = SCFindVariableInList(psc, (char*)$1, psection_main->ListVariables);
                                    if (!pvariable_global)
                                    {
                                       pentity_global = SCAllocEntity(psc, $1, SCVARIABLE_BYNAME);
                                       if (!pentity_global)
                                       {
                                          Err(ERR_ALLOC, "SCEntity SCVARIABLE not allocated");
                                          YYABORT;
                                       }
                                       ListNewr(&psection_main->ListVariables, (SCVariable*)pentity_global->Val);
                                       psection_main->NbVariables++;
                                    }
                                 }
                              }
                           }
                           else
                           {
                              pvariable_global = SCFindVariableInList(psc, (char*)$1, psc->ListVariables);
       