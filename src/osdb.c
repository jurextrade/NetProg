/* NetProg is an API in C language allowing an homogeneous programming of  */
/* communicating applications                                              */
/* Copyright (C) 2002,2003,2004,2005                                       */
/* Gabriel Jureidini                                                       */
/* Version 2.0 - 17 July 2005                                              */
/*-------------------------------------------------------------------------*/


#define _CRT_SECURE_NO_WARNINGS
#include "osdb.h"




OSStatement* OSCreateStatement (char *IdentStatic, char* SqlStatement, BYTE Commit)
{
    OSStatement* pstatement;

    pstatement = (OSStatement *)malloc (sizeof (OSStatement));
    memset (pstatement, 0, sizeof (OSStatement));

    if (!pstatement)   return NULL;

    pstatement->SqlType = DBSQLUNDEFINED;
    pstatement->Status = OSREADINIT;

    if (IdentStatic)
    {
        pstatement->IdentStatic  = (char*)malloc (strlen (IdentStatic) + 1);
        strcpy (pstatement->IdentStatic, IdentStatic);
    }
    if (SqlStatement)
    {
        pstatement->SqlStatement = (char*)malloc (strlen (SqlStatement) + 1);
        strcpy (pstatement->SqlStatement, SqlStatement);

        if (OSStrncmp (SqlStatement, "SELECT", 6) == 0)
            pstatement->SqlType     = DBSQLPROCESS;
        else
            if (OSStrncmp (SqlStatement, "SHOW", 4) == 0)
                pstatement->SqlType     = DBSQLSHOW;
            else
                pstatement->SqlType     = DBSQLEXECUTE;
    }

    pstatement->Query         = NULL;
    pstatement->Commit        = Commit;
    pstatement->Ack           = OSMallocAck ();
    return pstatement;
}



void OSFreeStatement (OSStatement* pstatement)
{
    if (pstatement)
    {
        if (pstatement->Ack)          free (pstatement->Ack);
        if (pstatement->IdentStatic)  free (pstatement->IdentStatic);
        if (pstatement->SqlStatement) free (pstatement->SqlStatement);

        if (pstatement->SqlType == DBSQLPROCESS)  OSFreeQuery (pstatement->Query);

        free (pstatement);
    }
    return;
}


OSQuery* OSCreateQuery (DWORD Every, DWORD MaxRows)
{
    OSQuery* pquery;
    pquery = (OSQuery *)malloc (sizeof (OSQuery));

    if (!pquery) return NULL;
    pquery->Every         = Every;
    pquery->MaxRows       = MaxRows;
    pquery->Contents      = NULL;
    pquery->ContextNumber = 0;
    return pquery;
}


void OSFreeQuery (OSQuery* pquery)
{
    if (pquery)
    {
        if (pquery->Contents)
            OSFreeSqlda (pquery->Contents);
        free (pquery);
    }
    return;
}



/*-----------------------------------------------------------------------------*/
/* OSMallocAck : fonction d'allocation d'une structure OSAck                   */
/*-----------------------------------------------------------------------------*/

OSAck* OSMallocAck (void)
{
    OSAck* pack;

    pack = (OSAck *)malloc (sizeof (OSAck));
    if (!pack)  return NULL;

    memset (pack, 0, sizeof(OSAck));
    memset (&pack->SqlErrMsg, ' ', 500);
    pack->AckProcedure  = OSDefaultAckProc;

    return pack;
}

/*-----------------------------------------------------------------------------*/
/* OSMallocShow : fonction d'allocation d'une structure OSShow                 */
/*-----------------------------------------------------------------------------*/

OSShow* OSMallocShow (void)
{
    OSShow* pshow;

    pshow = (OSShow *)malloc (sizeof (OSShow));
    if (!pshow) return NULL;

    pshow->ContextNumber          = 0;
    pshow->Context                = NULL;
    pshow->LastContext            = FALSE;
    pshow->ContentsProcedure      = OSDefaultContentsShowProc;
    pshow->ProcedurePar           = NULL;

    return pshow;
}
/*-----------------------------------------------------------------------------*/
/* OSFreeShow : fonction de lib�ration d'une structure OSShow                  */
/*-----------------------------------------------------------------------------*/

void OSFreeShow (OSShow* pshow)
{
    int i;

    if (!pshow) return;

    if (pshow->Context)
    {
        for (i = 0; i < pshow->ContextNumber; i++)
        {
            free (pshow->Context[i]->Values);
            free (pshow->Context[i]);
        }
        free (pshow->Context);
    }
    free (pshow);
}
/*-----------------------------------------------------------------------------*/
/* OSAddNewContextToShow : fonction permettant de r�allouer un nouveau         */
/*                         context pour une structure OSShow.                  */
/*-----------------------------------------------------------------------------*/

int OSAddNewContextToShow (OSShow* pshow)
{
    pshow->Context = (OSContext**)realloc (pshow->Context,
                                           (pshow->ContextNumber + 1)* sizeof (OSContext**));
    if (pshow->Context == NULL)
        return -1;

    pshow->Context[pshow->ContextNumber] = (OSContext*)malloc(sizeof (OSContext));
    pshow->Context[pshow->ContextNumber]->Values      = NULL;
    pshow->Context[pshow->ContextNumber]->Size        = 0;
    pshow->Context[pshow->ContextNumber]->Compact     = FALSE;
    return 1;
}
/*-----------------------------------------------------------------------------*/
/* OSMallocSqlda : fonction d'allocation d'une structure OSSqlda               */
/*-----------------------------------------------------------------------------*/

OSSqlda* OSMallocSqlda (int NumberItems)
{
    OSSqlda* psqlda;
    OSSqlvar** psqlvar;
    int i;

    psqlda = (OSSqlda *)malloc (sizeof (OSSqlda));
    if (!psqlda)
    {
        return NULL;
    }
    psqlda->ColumnsNumber          = 0;
    psqlda->ContextNumber          = 0;
    psqlda->RowsNumber             = 0;
    psqlda->Description            = NULL;
    psqlda->Context                = NULL;
    psqlda->LastContext            = FALSE;
    psqlda->ContentsProcedure      = OSDefaultContentsSqldaProc;
    psqlda->HeaderProcedure        = OSDefaultHeaderSqldaProc;
    psqlda->ProcedurePar           = NULL;
    psqlda->S_sqlda                = NULL;
    psqlda->size                   = 0;

    psqlvar = (OSSqlvar **)malloc (sizeof (OSSqlvar *) * NumberItems);
    if (!psqlvar)
    {
        OSFreeSqlda (psqlda);
        return NULL;
    }
    psqlda->Description = psqlvar;
    for (i = 0; i < NumberItems; i++)
    {
        if ((psqlvar[i] = (OSSqlvar *)malloc (sizeof (OSSqlvar))) == NULL)
        {
            OSFreeSqlda (psqlda);
            return NULL;
        }
        else psqlda->ColumnsNumber++;
    }

    return psqlda;
}
/*-----------------------------------------------------------------------------*/
/* OSFreeSqlda : fonction de lib�ration d'une structure OSSqlda                */
/*-----------------------------------------------------------------------------*/

void OSFreeS_sqlda (void* p)
{

#ifdef ODBC
    odbc_freeS_sqlda (p);
#endif
    return;
}


void OSFreeSqlda (OSSqlda* psqlda)
{
    int i;

    if (!psqlda) return;

    if (psqlda->Description)
    {
        for (i = 0; i < psqlda->ColumnsNumber; i++)
        {
            free (psqlda->Description[i]->Name);
            free (psqlda->Description[i]);
        }
        free (psqlda->Description);
    }
    if (psqlda->Context)
    {
        for (i = 0; i < psqlda->ContextNumber; i++)
        {
            free (psqlda->Context[i]->Values);
            free (psqlda->Context[i]);
        }
        free (psqlda->Context);
    }

    if ((psqlda->S_sqlda) && (psqlda->Protocol == DBPROTO_ODBC))
        OSFreeS_sqlda (psqlda->S_sqlda);

    free (psqlda);
}


/*-----------------------------------------------------------------------------*/
/* OSAddNewContextToSqlda : fonction permettant de r�allouer un nouveau        */
/*                          context pour une structure OSSqlda.                */
/*-----------------------------------------------------------------------------*/

int OSAddNewContextToSqlda (OSSqlda* psqlda, DWORD Every)
{
    char* block;
    int Size = Every * ( psqlda->size + 1);


    psqlda->Context = (OSContext**)realloc (psqlda->Context,
                                            (psqlda->ContextNumber + 1) * sizeof (OSContext**));
    if (psqlda->Context == NULL)
        return -1;
    psqlda->ContextNumber++;

    block = (char *)malloc (Size);
    if (!block)
        return - 1;

    psqlda->Context[psqlda->ContextNumber - 1] = (OSContext*)malloc(sizeof (OSContext));
    psqlda->Context[psqlda->ContextNumber - 1]->Values      = block;
    psqlda->Context[psqlda->ContextNumber-  1]->Size        =  0;
    psqlda->Context[psqlda->ContextNumber - 1]->Compact     = FALSE;

  

    return 1;
}

/*-----------------------------------------------------------------------------*/
/* OSReuseSameContext :                                                        */
/*                                                                             */
/*-----------------------------------------------------------------------------*/

int OSReuseSameContext (OSSqlda* psqlda)
{

    return 1;
}

/*-----------------------------------------------------------------------------*/
/* OSDisconnect : fonction de d�connection avec commit ou rollback             */
/*-----------------------------------------------------------------------------*/

int OSDisconnect (void* Connection, int protocol, BYTE Commit,
                  int (*AckProcedure)(void* pack, void* papp),
                  void* AckProcedurePar)
{
#ifdef ORACLE
    if (protocol == DBPROTO_ORACLE)
        return oracle_disconnect (Commit, AckProcedure, AckProcedurePar);
#endif
#ifdef SQLDS
    if (protocol == DBPROTO_SQLDS)
        return sqlds_disconnect (Commit, AckProcedure, AckProcedurePar);
#endif
#ifdef ODBC
    if (protocol == DBPROTO_ODBC)
        return odbc_disconnect (Connection, Commit, AckProcedure, AckProcedurePar);
#endif
    return -1;
}

/*-----------------------------------------------------------------------------*/
/* OSCommitOrRollback : fonction permettant d'effectuer un commit ou un        */
/*                      rollback.                                              */
/*-----------------------------------------------------------------------------*/

int OSCommitOrRollback (void* Connection, int protocol, BYTE Action,
                        int (*AckProcedure)(void* pack, void* papp),
                        void* AckProcedurePar)
{
#ifdef ORACLE
    if (protocol == DBPROTO_ORACLE)
        return oracle_commitorrollback (Action, AckProcedure, AckProcedurePar);
#endif
#ifdef SQLDS
    if (protocol == DBPROTO_SQLDS)
        return sqlds_commitorrollback (Action, AckProcedure, AckProcedurePar);
#endif
#ifdef ODBC
    if (protocol == DBPROTO_ODBC)
        return odbc_commitorrollback (Connection, Action, AckProcedure, AckProcedurePar);
#endif
    return -1;
}

/*-----------------------------------------------------------------------------*/
/* OSConnect : fonction effectuant un connect                                  */
/*-----------------------------------------------------------------------------*/

int OSConnect (char* DataBaseName, char* UserName, char* PassWord, int protocol,
               int (*AckProcedure)(void* pack, void* papp),
               void* AckProcedurePar)
{
#ifdef ORACLE
    if (protocol == DBPROTO_ORACLE)
        return oracle_connect (DataBaseName, UserName, PassWord, AckProcedure, AckProcedurePar);
#endif
#ifdef SQLDS
    if (protocol == DBPROTO_SQLDS)
        return   (DataBaseName, UserName, PassWord, AckProcedure, AckProcedurePar);
#endif
#ifdef ODBC
    if (protocol == DBPROTO_ODBC)
        return odbc_connect (DataBaseName, UserName, PassWord, AckProcedure, AckProcedurePar);;
#endif
    return -1;
}

/*-----------------------------------------------------------------------------*/
/* OSExecute : fonction appel�e suite � un DBExecute pour ex�cuter une         */
/*             requ�te de type UPDATE ou INSERT.                               */
/*-----------------------------------------------------------------------------*/

int OSExecute (void* Connection, int protocol, char* SqlStatement, BYTE Commit,
               int (*AckProcedure)(void* pack, void* papp),
               void* AckProcedurePar)
{
#ifdef ORACLE
    if (protocol == DBPROTO_ORACLE)
        return oracle_execute (SqlStatement, Commit, AckProcedure, AckProcedurePar);
#endif
#ifdef SQLDS
    if (protocol == DBPROTO_SQLDS)
        return sqlds_execute  (SqlStatement, Commit, AckProcedure, AckProcedurePar);
#endif
#ifdef ODBC
    if (protocol == DBPROTO_ODBC)
        return odbc_execute  (Connection, SqlStatement, Commit, AckProcedure, AckProcedurePar);
#endif
    return -1;
}



int OSSExecute (void* Connection, int protocol, char* SqlStatement, BYTE Commit,
                int (*AckProcedure)(void* pack, void* papp),
                void* AckProcedurePar,void* content, DWORD ContentSize)
{
#ifdef ORACLE
    if (protocol == DBPROTO_ORACLE)
        return oracle_execute (SqlStatement, Commit, AckProcedure, AckProcedurePar);
#endif
#ifdef SQLDS
    if (protocol == DBPROTO_SQLDS)
        return sqlds_execute  (SqlStatement, Commit, AckProcedure, AckProcedurePar);
#endif
#ifdef ODBC
    if (protocol == DBPROTO_ODBC)
        return odbc_sexecute  (Connection, SqlStatement, Commit, AckProcedure, AckProcedurePar,  content, ContentSize);
#endif
    return -1;
}


/*-----------------------------------------------------------------------------*/
/* OSProcess : fonction appel�e suite � un DBProcess pour ex�cuter une         */
/*             requ�te de type SELECT.                                         */
/*-----------------------------------------------------------------------------*/


OSSqlda* OSProcess (void* Connection, int protocol, char* SqlStatement, DWORD Every, DWORD MaxRows,

                    int (*HeaderProcedure)   (void* psqlda, void* papp),
                    int (*ContentsProcedure) (void* psqlda, void* papp),
                    void* ProcedurePar,
                    int (*AckProcedure)(void* pack, void* papp),
                    void* AckProcedurePar)
{
#ifdef ORACLE
    if (protocol == DBPROTO_ORACLE)
        return oracle_process (SqlStatement,
                               Every, MaxRows,
                               HeaderProcedure, ContentsProcedure, ProcedurePar,
                               AckProcedure, AckProcedurePar);
#endif
#ifdef SQLDS
    if (protocol == DBPROTO_SQLDS)
        return sqlds_process  (SqlStatement, 
                               Every, MaxRows,
                               HeaderProcedure, ContentsProcedure, ProcedurePar,
                               AckProcedure, AckProcedurePar);
#endif
#ifdef ODBC
    if (protocol == DBPROTO_ODBC)
    {
        OSSqlda* psqlda = odbc_process  (Connection, SqlStatement, 
                                         Every, MaxRows,
                                         HeaderProcedure, ContentsProcedure, ProcedurePar,
                                         AckProcedure, AckProcedurePar);
        if (psqlda) psqlda->Protocol     = protocol;
        return psqlda;
    }


#endif
    return NULL;
}

/*-----------------------------------------------------------------------------*/

int OSFetch (OSSqlda* psqlda, int protocol, 
             DWORD Every, DWORD MaxRows, DWORD RowsToProcess,
             int (*AckProcedure)(void* pack, void* papp),
             void* AckProcedurePar)
{
#ifdef ORACLE
    if (protocol == DBPROTO_ORACLE)
        return oracle_fetch (psqlda,  Every,
                             MaxRows, RowsToProcess,
                             AckProcedure, AckProcedurePar);

#endif
#ifdef SQLDS
    if (protocol == DBPROTO_SQLDS)
        return sqlds_fetch (psqlda, Every,
                            MaxRows, RowsToProcess,
                            AckProcedure, AckProcedurePar);
#endif                                        
#ifdef ODBC
    if (protocol == DBPROTO_ODBC)
        return odbc_fetch (psqlda, Every,
                           MaxRows, RowsToProcess,
                           AckProcedure, AckProcedurePar);
#endif          
    return -1;
}

/*-----------------------------------------------------------------------------*/
/* OSRecupShow :fonction appel�e suite � un DBShow pour ex�cuter une           */
/*             requ�te de type SHOW .                                          */
/*-----------------------------------------------------------------------------*/

OSShow* OSRecupShow  (void* Connection, int protocol, char* SqlStatement,
                      int (*ContentsProcedure) (void* pshow, void* papp),
                      void* ProcedurePar,
                      int (*AckProcedure)(void* pack, void* papp),
                      void* AckProcedurePar)
{
#ifdef ORACLE
    /* non d�fini */
    if (protocol == DBPROTO_ORACLE)
        return NULL;
#endif
#ifdef SQLDS
    if (protocol == DBPROTO_SQLDS)
        return sqlds_show (SqlStatement, 
                           ContentsProcedure, ProcedurePar,
                           AckProcedure, AckProcedurePar);
#endif
#ifdef ODBC
    if (protocol == DBPROTO_ODBC)
        return NULL;
#endif          
    return NULL;
}


/*-----------------------------------------------------------------------------*/
/* OSDefaultAckProc :      fonction affect�e par d�fault �                     */
/*                         pack->AckProcedure pour traiter le code retour      */
/*                         d'une requ�te SQL.                                  */
/*                         Cette fonction affiche � l'�cran le code retour     */
/*                         (sqlcode).                                          */
/*-----------------------------------------------------------------------------*/

int OSDefaultAckProc (void* ack, void* par)
{

    OSAck* pack = (OSAck*)ack;

#ifdef MESSAGE
    char Column[200];
    sprintf (Column,"ACKPROCEDURE rowsprocessed(%ld) sqlcode(%ld) msg(%s)\n",
             pack->RowsProcessed,
             pack->SqlCode,
             pack->SqlErrMsg);
    MessageBox (NULL, Column, "MOEHREL", MB_OK);

#else
    /*   printf ("ACKPROCEDURE rowsprocessed(%ld) sqlcode(%ld) msg(%s)\n",
    pack->RowsProcessed,
    pack->SqlCode,
    pack->SqlErrMsg);*/

#endif
    return 1;
}

/*-----------------------------------------------------------------------------*/
/* OSDefaultContentsShowProc :  fonction affect�e par d�fault �                */
/*                              pshow->ContentsProcedure pour traiter les      */
/*                              valeurs de retour d'une requ�te de type        */
/*                              SHOW.                                          */
/*                              Cette fonction effectue l'affichage �          */
/*                              l'�cran des donn�es.                           */
/*-----------------------------------------------------------------------------*/

int OSDefaultContentsShowProc (void* show, void* par)
{
    OSContext* pcontext;
    OSShow* pshow = (OSShow *)show;
    DWORD i = 0;
    char* values;

    pcontext = pshow->Context[pshow->ContextNumber];
    values = pshow->Context[pshow->ContextNumber]->Values;
    /* printf ("Buffer Size IS %ld\n", pcontext->Size);*/

    while (i < pcontext->Size)
    {
#ifdef MESSAGE
        MessageBox (NULL, values, "MOEHREL", MB_OK);
#else
        printf("%s",values);
#endif
        i+= (int)strlen (values) + 1;
        values+= strlen(values) + 1;
        printf ("\n");
    }

    return 1;
}
/*-----------------------------------------------------------------------------*/
/* OSDefaultHeaderSqldaProc : fonction affect�e par d�fault �                  */
/*                            psqlda->HeaderProcedure pour traiter le          */
/*                            Header d'une requ�te de type SELECT.             */
/*                            Cette fonction effectue l'affichage � l'�cran    */
/*                            du Header.                                       */
/*-----------------------------------------------------------------------------*/

int OSDefaultHeaderSqldaProc (void* sqlda, void* par)
{
    /*
       int i;
       char Column[5000];
      
       OSSqlda* psqlda = (OSSqlda *)sqlda;
       for (i = 0; i < psqlda->ColumnsNumber; i++)
       {
           sprintf (Column, "%.*s", (int)psqlda->Description[i]->ItemLength,
           psqlda->Description[i]->Name);
    #ifdef MESSAGE
           MessageBox (NULL, Column, "MOEHREL", MB_OK);
       }
    #else
          printf("%s ",Column);
       }

       printf("\n");
    #endif
      */
    return 1;
}
/*-----------------------------------------------------------------------------*/
/* OSDefaultContentsSqldaProc : fonction affect�e par d�fault �                */
/*                              psqlda->ContentsProcedure pour traiter les     */
/*                              valeurs de retour d'une requ�te de type        */
/*                              SELECT.                                        */
/*                              Cette fonction effectue l'affichage �          */
/*                              l'�cran des donn�es.                           */
/*-----------------------------------------------------------------------------*/

int OSDefaultContentsSqldaProc (void* sqlda, void* par)
{
    OSContext* pcontext;
    OSSqlda* psqlda = (OSSqlda *)sqlda;
    DWORD i = 0;
    char* values;

    pcontext = psqlda->Context[psqlda->ContextNumber];
    values = psqlda->Context[psqlda->ContextNumber]->Values;
    /* printf ("RowsNumber IS %d\n", psqlda->RowsNumber);
     printf ("Buffer Size IS %ld\n", pcontext->Size);*/

    while (i < pcontext->Size)
    {
#ifdef MESSAGE
        MessageBox (NULL, values, "MOEHREL", MB_OK);
#else
        printf("%s",values);
#endif
        i+= (int)strlen (values) + 1;
        values+= strlen(values) + 1;
        printf ("\n");
    }

    return 1;
}


/****************************************************************************/
/* sqlds_define_sqlda : fonction permettant de difinir les diffirents       */
/*                      select_list_items dans la structure OSSqlda.        */
/****************************************************************************/
void sqlds_define_sqlda (OSSqlda* psqlda, int* size, int* SelectItemNumber,
                         char* SelectItemName, char* type,
                         int SelectItemLength)
{
    int i = *SelectItemNumber;

    *size += SelectItemLength;
    psqlda->Description[i]->Length     = SelectItemLength;
    psqlda->Description[i]->ItemLength = (long)strlen(SelectItemName);
    psqlda->Description[i]->Name       =
        (char *)malloc ((WORD)psqlda->Description[i]->ItemLength);
    strncpy (psqlda->Description[i]->Name , SelectItemName,
             (WORD)psqlda->Description[i]->ItemLength);

    psqlda->Description[i]->Type = 0;

    if (strcmp(type,"DATE") == 0)          /* type 384 ou 385 */
        psqlda->Description[i]->Type = 384;

    if (strcmp(type,"TIME") == 0)          /* type 388 ou 389 */
        psqlda->Description[i]->Type = 388;

    if (strcmp(type,"TIMESTAMP") == 0)     /* type 392 ou 393 */
        psqlda->Description[i]->Type = 392;

    if (strcmp(type,"CHAR") == 0)          /* type 452 ou 453 */
        psqlda->Description[i]->Type = 452;

    if (strcmp(type,"VARCHAR") == 0)       /* type 448 ou 449 */
        psqlda->Description[i]->Type = 448;

    if (strcmp(type,"LONGVARCHAR") == 0)  /* type 456 ou 457 */
        psqlda->Description[i]->Type = 456;

    if (strcmp(type,"NULL") == 0)         /* type 460 ou 461 */
        psqlda->Description[i]->Type = 460;

    if (strcmp(type,"VARGRAPHIC") == 0)   /* type 464 ou 465 */
        psqlda->Description[i]->Type = 464;

    if (strcmp(type,"GRAPHIC") == 0)      /* type 468 ou 469 */
        psqlda->Description[i]->Type = 468;

    if (strcmp(type,"LONGGRAPHIC") == 0)  /* type 472 ou 473 */
        psqlda->Description[i]->Type = 472;

    if (strcmp(type,"FLOAT") == 0)        /* type 480 ou 481 */
        psqlda->Description[i]->Type = 480;

    if (strcmp(type,"DECIMAL") == 0)      /* type 484 ou 485 */
        psqlda->Description[i]->Type = 484;

    if (strcmp(type,"INTEGER") == 0)      /* type 496 ou 497 */
        psqlda->Description[i]->Type = 496;

    if (strcmp(type,"SMALLINT") == 0)      /* type 500 ou 501 */
        psqlda->Description[i]->Type = 500;

    if (psqlda->Description[i]->Type == 0)
        printf("Type Inconnu (%s) \n",type);

    *SelectItemNumber +=1;
    return;
}
/****************************************************************************/
/* sqlds_write_selectitem : fonction permettant d'icrire la valeur de la    */
/*                          select_list_item dans le context de OSSqlda     */
/****************************************************************************/
int sqlds_write_selectitem (OSSqlda* psqlda, char** values,
                            char* SelectItemName, void* val)
{
    int i = 0, indice = 0, found = 0;

    /* recherche de la selectitem dans psqlda */
    while ((found == 0) && (i < psqlda->ColumnsNumber))
    {
        if (strncmp(psqlda->Description[i]->Name, SelectItemName,
                    (WORD)psqlda->Description[i]->ItemLength) == 0)
        {
            indice = i;
            found = 1;
        }
        else i++;
    }
    if (!found)
    {
        printf("item non trouvi\n");
        return(-1);
    }

    switch(psqlda->Description[indice]->Type)
    {
    case 448:
    case 449:
    case 456:
    case 457:  /* VARCHAR et LONGVARCHAR */
    case 384:
    case 385:   /* DATE */
    case 388:
    case 389:   /* TIME */
    case 392:
    case 393:   /* TIMESTAMP */
    case 452:
    case 453:   /* CHAR */
        sprintf (*values , "%-*.*s",
                 (int)psqlda->Description[indice]->Length,
                 (int)psqlda->Description[indice]->Length,
                 val);
        *values += (int)psqlda->Description[indice]->Length;
        break;
    case 480:
    case 481:   /* FLOAT */
        if (psqlda->Description[i]->Length == 4)/* simple precision */
        {
            sprintf (*values , "%7.1d",*(int *)val);
            *values += 7;   /* 7 = 5 + 1 pour le signe    */
            /*       + 1 pour la virgule */
        }
        else   /* double precision */
        {
            sprintf (*values , "%12.1ld",*(long *)val);
            *values += 12;
        }
        break ;
    case 496:
    case 497:   /* INTEGER */
        sprintf (*values , "%11.1d", *(int *)val);
        *values += 11;  /* 11 = 10 + 1 pour le signe */
        break ;
    case 500:
    case 501:   /* SMALLINT */
        sprintf (*values , "%6.1d", *(short *)val);
        *values += 6;   /* 6 = 5 + 1 pour le signe */
        break ;
    default :
        printf("Type inconnu \n");
        return(-2);
    }
    return(0);
}

/****************************************************************************/
/* oracle_define_sqlda : fonction permettant de difinir les diffirents      */
/*                      select_list_items dans la structure OSSqlda.        */
/****************************************************************************/
void oracle_define_sqlda (OSSqlda* psqlda, int* size, int* SelectItemNumber,
                          char* SelectItemName, char* type,
                          int SelectItemLength)
{
    int i = *SelectItemNumber;

    *size += SelectItemLength;
    psqlda->Description[i]->Length     = SelectItemLength;
    psqlda->Description[i]->ItemLength = (long)strlen(SelectItemName);
    psqlda->Description[i]->Name       =
        (char *)malloc ((WORD)psqlda->Description[i]->ItemLength);
    strncpy (psqlda->Description[i]->Name , SelectItemName,
             (WORD)psqlda->Description[i]->ItemLength);

    psqlda->Description[i]->Type = 0;

    if ((strcmp(type,"CHAR") == 0) || (strcmp(type,"VARCHAR") == 0) ||
            (strcmp(type,"LONGVARCHAR") == 0) || (strcmp(type,"STRING") == 0))
        psqlda->Description[i]->Type = 1;

    if (strcmp(type,"NUMBER") == 0)
        psqlda->Description[i]->Type = 2;

    if (strcmp(type,"LONG") == 0)
        psqlda->Description[i]->Type = 8;

    if (strcmp(type,"ROWID") == 0)
        psqlda->Description[i]->Type = 11;

    if (strcmp(type,"DATE") == 0)
        psqlda->Description[i]->Type = 12;

    if (strcmp(type,"RAW") == 0)
        psqlda->Description[i]->Type = 23;

    if (strcmp(type,"LONGRAW") == 0)
        psqlda->Description[i]->Type = 24;

    if (psqlda->Description[i]->Type == 0)
        printf("Type Inconnu (%s) \n",type);

    *SelectItemNumber +=1;
    return;
}
/****************************************************************************/
/* oracle_write_selectitem : fonction permettant d'icrire la valeur de la   */
/*                          select_list_item dans le context de OSSqlda     */
/****************************************************************************/
int oracle_write_selectitem (OSSqlda* psqlda, char** values,
                             char* SelectItemName, void* val)
{
    int i = 0, indice = 0, found = 0;

    /* recherche de la selectitem dans psqlda */
    while ((found == 0) && (i < psqlda->ColumnsNumber))
    {
        if (strncmp(psqlda->Description[i]->Name,SelectItemName,
                    (WORD)psqlda->Description[i]->ItemLength) == 0)
        {
            indice = i;
            found = 1;
        }
        else i++;
    }
    if (!found)
    {
        printf("item non trouvi\n");
        return(-1);
    }

    switch (psqlda->Description[indice]->Type)
    {
    case 1 :     /* char datatype no change in length needed */
        sprintf (*values , "%-*.*s",
                 (int)psqlda->Description[indice]->Length,
                 (int)psqlda->Description[indice]->Length,
                 val);
        *values += (int)psqlda->Description[indice]->Length;
        break;
    case 2 :     /* NUMBER */
        sprintf (*values , "%*d",
                 (int)psqlda->Description[indice]->Length,
                 *(int *)val);
        *values += (int)psqlda->Description[indice]->Length;
        break;
    case 8:      /* LONG */
        /*********** � d�finir ************/
        break;
    case 11 :    /* ROWID */
    case 12 :    /* DATE  */
    case 23 :    /* RAW  */
    case 24 :    /* LONG RAW  */
        sprintf (*values , "%-*.*s",
                 (int)psqlda->Description[indice]->Length,
                 (int)psqlda->Description[indice]->Length,
                 val);
        *values += (int)psqlda->Description[indice]->Length;
        break;
    default :
        printf("Type inconnu \n");
        return(-2);
    }
    return(0);
}
