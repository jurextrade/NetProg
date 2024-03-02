/* NetProg is an API in C language allowing an homogeneous programming of  */
/* communicating applications                                              */
/* Copyright (C) 2002,2003,2004,2005                                       */
/* Gabriel Jureidini                                                       */
/* Version 2.0 - 17 July 2005                                              */
/*-------------------------------------------------------------------------*/


#define _CRT_SECURE_NO_WARNINGS

/* Result Sets Interface */
#ifndef SQL_CRSR
#  define SQL_CRSR
struct sql_cursor
{
    unsigned int curocn;
    void *ptr1;
    void *ptr2;
    unsigned int magic;
};
typedef struct sql_cursor sql_cursor;
typedef struct sql_cursor SQL_CURSOR;
#endif /* SQL_CRSR */

/* Thread Safety */
typedef void * sql_context;
typedef void * SQL_CONTEXT;

/* File name & Package Name */
struct sqlcxp
{
    unsigned short fillen;
    char  filnam[43];
};
static const struct sqlcxp sqlfpn =
    {
        42,
        "d:\\projects\\netprog\\src\\database\\sqlora.pc"
    };


static const unsigned long sqlctx = 1006534805;


static struct sqlexd {
    unsigned long    sqlvsn;
    unsigned long   arrsiz;
    unsigned long   iters;
    unsigned short   offset;
    unsigned short   selerr;
    unsigned short   sqlety;
    unsigned short   unused;
    const    short   *cud;
    unsigned char    *sqlest;
    const    char    *stmt;
    unsigned char  * *sqphsv;
    unsigned long   *sqphsl;
    short  * *sqpind;
    unsigned long   *sqparm;
    unsigned long   * *sqparc;
    unsigned char    *sqhstv[3];
    unsigned long    sqhstl[3];
    short   *sqindv[3];
    unsigned long    sqharm[3];
    unsigned long     *sqharc[3];
} sqlstm = {8,3};

/* Prototypes */
extern void sqlcxt (void **, const unsigned long *,
                        struct sqlexd *, const struct sqlcxp *);
extern void sqlcx2t(void **, const unsigned long *,
                        struct sqlexd *, const struct sqlcxp *);
extern void sqlbuft(void **, char *);
extern void sqlgs2t(void **, char *);
extern void sqlorat(void **, const unsigned long *, void *);

/* Forms Interface */
static const int IAPSUCC = 0;
static const int IAPFAIL = 1403;
static const int IAPFTL  = 535;
extern void sqliem(char *, unsigned long *);

typedef struct { unsigned short len; unsigned char arr[1]; } VARCHAR;
typedef struct { unsigned short len; unsigned char arr[1]; } varchar;

/* cud (compilation unit data) array */
static const short sqlcud0[] =
    {8,4130,
     2,0,0,1,0,0,31,40,0,0,0,0,1,0,
     16,0,0,2,0,0,27,76,0,3,3,0,1,0,1,9,0,0,1,9,0,0,1,10,0,0,
     42,0,0,3,0,0,17,154,0,1,1,0,1,0,1,5,0,0,
     60,0,0,3,0,0,45,156,0,0,0,0,1,0,
     74,0,0,3,0,0,15,161,0,0,0,0,1,0,
     88,0,0,3,0,0,20,167,0,1,0,0,1,0,2,32,0,0,
     106,0,0,3,0,0,15,172,0,0,0,0,1,0,
     120,0,0,3,0,0,15,180,0,0,0,0,1,0,
     134,0,0,3,0,0,15,237,0,0,0,0,1,0,
     148,0,0,3,0,0,14,287,0,1,0,0,1,0,2,32,0,0,
     166,0,0,3,0,0,15,292,0,0,0,0,1,0,
     180,0,0,3,0,0,15,306,0,0,0,0,1,0,
     194,0,0,3,0,0,14,318,0,1,0,0,1,0,2,32,0,0,
     212,0,0,3,0,0,15,323,0,0,0,0,1,0,
     226,0,0,3,0,0,15,339,0,0,0,0,1,0,
     240,0,0,3,0,0,15,353,0,0,0,0,1,0,
     254,0,0,3,0,0,15,358,0,0,0,0,1,0,
     268,0,0,3,0,0,17,404,0,1,1,0,1,0,1,5,0,0,
     286,0,0,3,0,0,21,405,0,0,0,0,1,0,
     300,0,0,4,0,0,29,408,0,0,0,0,1,0,
     314,0,0,5,0,0,30,438,0,0,0,0,1,0,
     328,0,0,6,0,0,32,440,0,0,0,0,1,0,
     342,0,0,7,0,0,29,469,0,0,0,0,1,0,
     356,0,0,8,0,0,31,471,0,0,0,0,1,0,
    };


/*-------------------------------------------------------------------------*/
/* FICHIER : SQLORA.PC                                                     */
/* DESCRIPTION :                                                           */
/* PROPRIETES : EAGLENET                                                   */
/*-------------------------------------------------------------------------*/

#include "ostypes.h"
#include "osio.h"

#include "osdb.h"

#define MAX_VNAME_LEN     30
#define MAX_INAME_LEN     30



/* EXEC SQL INCLUDE sqlda;
 */ 
/***************************************************************
*      The SQLDA descriptor definition                         *
*--------------------------------------------------------------*
*      80x86 WIN32 Version                                     *
*                                                              *
*  Copyright (c) 1987 by Oracle Corporation                    *
***************************************************************/


#ifndef SQLDA_
#define SQLDA_ 1

#ifdef T
# undef T
#endif
#ifdef F
# undef F
#endif

#ifdef S
# undef S
#endif
#ifdef L
# undef L
#endif

struct SQLDA {
    long         N;   /* Descriptor size in number of entries         */
    char    *   *V;   /* Ptr to Arr of addresses of main variables    */
    long        *L;   /* Ptr to Arr of lengths of buffers             */
    short       *T;   /* Ptr to Arr of types of buffers               */
    short   *   *I;   /* Ptr to Arr of addresses of indicator vars    */
    long         F;   /* Number of variables found by DESCRIBE        */
    char    *   *S;   /* Ptr to Arr of variable name pointers         */
    short       *M;   /* Ptr to Arr of max lengths of var. names      */
    short       *C;   /* Ptr to Arr of current lengths of var. names  */
    char    *   *X;   /* Ptr to Arr of ind. var. name pointers        */
    short         *Y;   /* Ptr to Arr of max lengths of ind. var. names */
    short         *Z;   /* Ptr to Arr of cur lengths of ind. var. names */
};

typedef struct SQLDA SQLDA;

#endif
/* EXEC SQL INCLUDE sqlca;
 */ 
/* Copyright (c) 1985,1986 by Oracle Corporation. */

/*
NAME
  SQLCA : SQL Communications Area.
FUNCTION
  Contains no code. Oracle fills in the SQLCA with status info
  during the execution of a SQL stmt.
NOTES
  If the symbol SQLCA_STORAGE_CLASS is defined, then the SQLCA
  will be defined to have this storage class. For example:

    #define SQLCA_STORAGE_CLASS extern

  will define the SQLCA as an extern.

  If the symbol SQLCA_INIT is defined, then the SQLCA will be
  statically initialized. Although this is not necessary in order
  to use the SQLCA, it is a good pgming practice not to have
  unitialized variables. However, some C compilers/OS's don't
  allow automatic variables to be init'd in this manner. Therefore,
  if you are INCLUDE'ing the SQLCA in a place where it would be
  an automatic AND your C compiler/OS doesn't allow this style
  of initialization, then SQLCA_INIT should be left undefined --
  all others can define SQLCA_INIT if they wish.

  If the symbol SQLCA_NONE is defined, then the SQLCA variable will
  not be defined at all.  The symbol SQLCA_NONE should not be defined
  in source modules that have embedded SQL.  However, source modules
  that have no embedded SQL, but need to manipulate a sqlca struct
  passed in as a parameter, can set the SQLCA_NONE symbol to avoid
  creation of an extraneous sqlca variable.

*/

#ifndef SQLCA
#define SQLCA 1

struct   sqlca
{
    /* ub1 */ char    sqlcaid[8];
    /* b4  */ long    sqlabc;
    /* b4  */ long    sqlcode;
    struct
    {
        /* ub2 */ unsigned short sqlerrml;
        /* ub1 */ char           sqlerrmc[70];
    } sqlerrm;
    /* ub1 */ char    sqlerrp[8];
    /* b4  */ long    sqlerrd[6];
    /* ub1 */ char    sqlwarn[8];
    /* ub1 */ char    sqlext[8];
};

#ifndef SQLCA_NONE 
#ifdef SQLCA_STORAGE_CLASS
SQLCA_STORAGE_CLASS struct sqlca sqlca
#else
struct sqlca sqlca
#endif

# ifdef  SQLCA_INIT
    = {
      {'S', 'Q', 'L', 'C', 'A', ' ', ' ', ' '},
      sizeof(struct sqlca),
      0,
      { 0, {0}},
      {'N', 'O', 'T', ' ', 'S', 'E', 'T', ' '},
      {0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0}
  }
#endif /* SQLCA_INIT */
  ;

#endif 

#endif 

/* end SQLCA */





/*********************************************************************/

void oracle_ack (OSAck* pack)
{
    /*  pack->Type = 1;     erreur SQL */
    pack->SqlCode = sqlca.sqlcode;
    strncpy (pack->SqlErrMsg, sqlca.sqlerrm.sqlerrmc, 70);
    sqlca.sqlerrm.sqlerrmc[0] = 0;               /* on réinitialise */
    pack->RowsProcessed = sqlca.sqlerrd[3];
    sqlca.sqlerrd[3] = 0;                           /* on réinitialise */
    pack->Print = 1;
    pack->Stop  = 0;
    if (pack->AckProcedure)
        pack->AckProcedure (pack, pack->ProcedurePar);
    if ((sqlca.sqlcode != 0) && (sqlca.sqlcode != 100))
    {
        /* EXEC SQL WHENEVER SQLERROR CONTINUE; */

        /* EXEC SQL ROLLBACK WORK; */

        {
            struct sqlexd sqlstm;

            sqlstm.sqlvsn = 8;
            sqlstm.arrsiz = 0;
            sqlstm.iters = (unsigned long  )1;
            sqlstm.offset = (unsigned short)2;
            sqlstm.cud = sqlcud0;
            sqlstm.sqlest = (unsigned char  *)&sqlca;
            sqlstm.sqlety = (unsigned short)0;
            sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
        }


    }
    return;
}

/*********************************************************************/



int oracle_connect (char* DataBaseName, char* UserName, char* PassWord,
                    int (*AckProcedure)(void* pack, void* papp),
                    void* AckProcedurePar)
{
    OSAck* pack;

    /* EXEC SQL BEGIN DECLARE SECTION; */

    /* VARCHAR databasename[20]; */
    struct { unsigned short len; unsigned char arr[20]; } databasename;

    /* VARCHAR username[20]; */
    struct { unsigned short len; unsigned char arr[20]; } username;

    /* VARCHAR password[20]; */
    struct { unsigned short len; unsigned char arr[20]; } password;

    /* EXEC SQL END DECLARE SECTION; */


    strcpy (databasename.arr, DataBaseName);
    strcpy (username.arr,     UserName);
    strcpy (password.arr,     PassWord);

    databasename.len = strlen (databasename.arr);
    username.len     = strlen (username.arr);
    password.len     = strlen (password.arr);

    /* EXEC SQL WHENEVER SQLERROR GOTO connect_error; */


    pack = OSMallocAck ();

    if (AckProcedure)      OSSetAckProcedure (pack, AckProcedure);
    if (AckProcedurePar)   OSSetProcedurePar (pack, AckProcedurePar);

    /* EXEC SQL CONNECT :username IDENTIFIED BY :password; */

    {
        struct sqlexd sqlstm;

        sqlstm.sqlvsn = 8;
        sqlstm.arrsiz = 3;
        sqlstm.iters = (unsigned long  )10;
        sqlstm.offset = (unsigned short)16;
        sqlstm.cud = sqlcud0;
        sqlstm.sqlest = (unsigned char  *)&sqlca;
        sqlstm.sqlety = (unsigned short)0;
        sqlstm.sqhstv[0] = (unsigned char  *)&username;
        sqlstm.sqhstl[0] = (unsigned long  )22;
        sqlstm.sqindv[0] = (         short *)0;
        sqlstm.sqharm[0] = (unsigned long )0;
        sqlstm.sqhstv[1] = (unsigned char  *)&password;
        sqlstm.sqhstl[1] = (unsigned long  )22;
        sqlstm.sqindv[1] = (         short *)0;
        sqlstm.sqharm[1] = (unsigned long )0;
        sqlstm.sqphsv = sqlstm.sqhstv;
        sqlstm.sqphsl = sqlstm.sqhstl;
        sqlstm.sqpind = sqlstm.sqindv;
        sqlstm.sqparm = sqlstm.sqharm;
        sqlstm.sqparc = sqlstm.sqharc;
        sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
        if (sqlca.sqlcode < 0) goto connect_error;
    }



    printf ("connection SQL OK\n");
    oracle_ack (pack);
    free (pack);
    return 1;

connect_error :
    oracle_ack (pack);
    free (pack);
    return -1;
}

/*********************************************************************/

int oracle_write_sqlda (OSSqlda* psqlda)
{
    int i;
    int Error = 0;
    char* values;
    char* Values;
    OSContext* pcontext;
    SQLDA* select_dp = (SQLDA*)psqlda->S_sqlda;
    int size = psqlda->size;

    pcontext = psqlda->Context[psqlda->ContextNumber];

    if (pcontext->Size + size + 1 > MAXBUFSIZE - 100)
        return -2;

    Values = malloc (size + 1);
    pcontext->Values = (char *)realloc (pcontext->Values,
                                        pcontext->Size + size + 1);
    values = Values;
    for (i = 0; i < select_dp->F; i++)
    {
        sprintf (values , "%-*.*s", (int)select_dp->L[i], (int)select_dp->L[i],
                 select_dp->V[i]);
        values += (int)select_dp->L[i];
    }

    memcpy (pcontext->Values + pcontext->Size, Values , size + 1);
    pcontext->Size += size + 1;
    free (Values);
    return 1;
}

/*********************************************************************/

OSSqlda* oracle_process (char* SqlStatement, 
                        DWORD Every,
                         DWORD MaxRows,
                         int (*HeaderProcedure)   (void* psqlda, void* papp),
                         int (*ContentsProcedure) (void* psqlda, void* papp),
                         void* ProcedurePar,
                         int (*AckProcedure)(void* pack, void* papp),
                         void* AckProcedurePar)
{
    int      null_ok, precision, scale, size;
    DWORD    i = 0, j = 0;
    OSSqlda* psqlda = NULL;
    SQLDA*   select_dp;
    OSAck*   pack;

    /* EXEC SQL BEGIN DECLARE SECTION; */

    char   sql_statement[1024];
    /* EXEC SQL VAR sql_statement IS STRING(1024); */

    /* EXEC SQL END DECLARE SECTION; */


    /* EXEC SQL WHENEVER SQLERROR GOTO process_error; */


    pack = OSMallocAck ();

    if (AckProcedure)       OSSetAckProcedure (pack, AckProcedure);
    if (AckProcedurePar)    OSSetProcedurePar (pack, AckProcedurePar);

    strcpy (sql_statement, SqlStatement);

    /* EXEC SQL PREPARE S FROM :sql_statement; */

    {
        struct sqlexd sqlstm;

        sqlstm.sqlvsn = 8;
        sqlstm.arrsiz = 3;
        sqlstm.stmt = "";
        sqlstm.iters = (unsigned long  )1;
        sqlstm.offset = (unsigned short)42;
        sqlstm.cud = sqlcud0;
        sqlstm.sqlest = (unsigned char  *)&sqlca;
        sqlstm.sqlety = (unsigned short)0;
        sqlstm.sqhstv[0] = (unsigned char  *)sql_statement;
        sqlstm.sqhstl[0] = (unsigned long  )1024;
        sqlstm.sqindv[0] = (         short *)0;
        sqlstm.sqharm[0] = (unsigned long )0;
        sqlstm.sqphsv = sqlstm.sqhstv;
        sqlstm.sqphsl = sqlstm.sqhstl;
        sqlstm.sqpind = sqlstm.sqindv;
        sqlstm.sqparm = sqlstm.sqharm;
        sqlstm.sqparc = sqlstm.sqharc;
        sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
        if (sqlca.sqlcode < 0) goto process_error;
    }


    /* EXEC SQL DECLARE C CURSOR FOR S; */

    /* EXEC SQL OPEN C; */

    {
        struct sqlexd sqlstm;

        sqlstm.sqlvsn = 8;
        sqlstm.arrsiz = 3;
        sqlstm.stmt = "";
        sqlstm.iters = (unsigned long  )1;
        sqlstm.offset = (unsigned short)60;
        sqlstm.cud = sqlcud0;
        sqlstm.sqlest = (unsigned char  *)&sqlca;
        sqlstm.sqlety = (unsigned short)0;
        sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
        if (sqlca.sqlcode < 0) goto process_error;
    }



    if ((select_dp = sqlald (MAXITEMS, MAX_VNAME_LEN, MAX_INAME_LEN)) == NULL)
    {
        printf ("Can't allocate memory\n");
        /* EXEC SQL CLOSE C; */

        {
            struct sqlexd sqlstm;

            sqlstm.sqlvsn = 8;
            sqlstm.arrsiz = 3;
            sqlstm.iters = (unsigned long  )1;
            sqlstm.offset = (unsigned short)74;
            sqlstm.cud = sqlcud0;
            sqlstm.sqlest = (unsigned char  *)&sqlca;
            sqlstm.sqlety = (unsigned short)0;
            sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
            if (sqlca.sqlcode < 0) goto process_error;
        }


        free (pack);
        return NULL;
    }
    select_dp->N = MAXITEMS;

    /* EXEC SQL DESCRIBE SELECT LIST FOR S INTO select_dp; */

    {
        struct sqlexd sqlstm;

        sqlstm.sqlvsn = 8;
        sqlstm.arrsiz = 3;
        sqlstm.iters = (unsigned long  )1;
        sqlstm.offset = (unsigned short)88;
        sqlstm.cud = sqlcud0;
        sqlstm.sqlest = (unsigned char  *)&sqlca;
        sqlstm.sqlety = (unsigned short)0;
        sqlstm.sqhstv[0] = (unsigned char  *)select_dp;
        sqlstm.sqhstl[0] = (unsigned long  )0;
        sqlstm.sqindv[0] = (         short *)0;
        sqlstm.sqharm[0] = (unsigned long )0;
        sqlstm.sqphsv = sqlstm.sqhstv;
        sqlstm.sqphsl = sqlstm.sqhstl;
        sqlstm.sqpind = sqlstm.sqindv;
        sqlstm.sqparm = sqlstm.sqharm;
        sqlstm.sqparc = sqlstm.sqharc;
        sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
        if (sqlca.sqlcode < 0) goto process_error;
    }



    if (select_dp->F < 0)
    {
        printf ("Too many select_list items\n");
        /* EXEC SQL CLOSE C; */

        {
            struct sqlexd sqlstm;

            sqlstm.sqlvsn = 8;
            sqlstm.arrsiz = 3;
            sqlstm.iters = (unsigned long  )1;
            sqlstm.offset = (unsigned short)106;
            sqlstm.cud = sqlcud0;
            sqlstm.sqlest = (unsigned char  *)&sqlca;
            sqlstm.sqlety = (unsigned short)0;
            sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
            if (sqlca.sqlcode < 0) goto process_error;
        }


        free (pack);
        return NULL;
    }
    psqlda = OSMallocSqlda ((int)select_dp->F);

    if (!psqlda)
    {
        /* EXEC SQL CLOSE C; */

        {
            struct sqlexd sqlstm;

            sqlstm.sqlvsn = 8;
            sqlstm.arrsiz = 3;
            sqlstm.iters = (unsigned long  )1;
            sqlstm.offset = (unsigned short)120;
            sqlstm.cud = sqlcud0;
            sqlstm.sqlest = (unsigned char  *)&sqlca;
            sqlstm.sqlety = (unsigned short)0;
            sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
            if (sqlca.sqlcode < 0) goto process_error;
        }


        free (pack);
        return NULL;
    }
    psqlda->S_sqlda = select_dp;

    if (ContentsProcedure) OSSetContentsProcedure(psqlda, ContentsProcedure);
    if (HeaderProcedure)   OSSetHeaderProcedure(psqlda, HeaderProcedure);
    if (ProcedurePar)      OSSetProcedurePar(psqlda, ProcedurePar);

    select_dp->N = select_dp->F;
    for (i = 0; i < (DWORD)select_dp->F; i++)
        select_dp->I[i] = (short *)malloc (sizeof (short *));

    size = 0;
    for (i = 0; i < (DWORD)select_dp->F; i++)
    {
        sqlnul (&(select_dp->T[i]), &(select_dp->T[i]), &null_ok);
        switch ( select_dp->T[i])
        {
        case 1 :     /* char datatype no change in length needed */
            break;
        case 2 :     /* NUMBER */
            sqlprc (&(select_dp->L[i]), &precision, &scale);
            if (precision == 0) precision = 40;
            select_dp->L[i] = precision + 2;
            break;
        case 8:      /* LONG */
            select_dp->L[i] = 240;
            break;
        case 11 :    /* ROWID */
            select_dp->L[i] = 18;
            break;
        case 12 :    /* DATE  */
            select_dp->L[i] = 9;
            break;
        case 23 :    /* RAW  */
            break;
        case 24 :    /* LONG RAW  */
            select_dp->L[i] = 240;
            break;
        default :
            break;
        }
        size += (int)select_dp->L[i];
        select_dp->V[i] = (char*)malloc (select_dp->L[i] + 1);
        if (select_dp->T[i] != 24) select_dp->T[i] = 1;
        psqlda->Description[i]->Type       = select_dp->T[i];
        psqlda->Description[i]->Length     = select_dp->L[i];
        psqlda->Description[i]->ItemLength = select_dp->M[i];
        psqlda->Description[i]->Name  =    (char *)malloc (psqlda->Description[i]->ItemLength);
        strncpy (psqlda->Description[i]->Name , select_dp->S[i], select_dp->M[i]);
    }
    psqlda->size = size;

    if (psqlda->HeaderProcedure (psqlda, psqlda->ProcedurePar) < 0)
    {
        /* EXEC SQL CLOSE C; */

        {
            struct sqlexd sqlstm;

            sqlstm.sqlvsn = 8;
            sqlstm.arrsiz = 3;
            sqlstm.iters = (unsigned long  )1;
            sqlstm.offset = (unsigned short)134;
            sqlstm.cud = sqlcud0;
            sqlstm.sqlest = (unsigned char  *)&sqlca;
            sqlstm.sqlety = (unsigned short)0;
            sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
            if (sqlca.sqlcode < 0) goto process_error;
        }


        OSFreeSqlda (psqlda);
        free (pack);
        return NULL;
    }
    else
    {
        free (pack);
        return psqlda;
    }

process_error :
    printf ("erreur durant le process\n");
    oracle_ack (pack);
    OSFreeSqlda (psqlda);
    free (pack);
    return NULL;
}

/*********************************************************************/

int oracle_fetch (OSSqlda* psqlda, BYTE StorageType, BYTE TreatmentType,
                  DWORD Every, DWORD MaxRows, DWORD RowsToProcess,
                  int (*AckProcedure)(void* pack, void* papp),
                  void* AckProcedurePar)
{
    DWORD  i, j;
    DWORD  RowsProcessed = 0;
    SQLDA* select_dp = (SQLDA*)psqlda->S_sqlda;
    OSAck* pack;

    pack = OSMallocAck ();

    if (AckProcedure)       OSSetAckProcedure (pack, AckProcedure);
    if (AckProcedurePar)    OSSetProcedurePar (pack, AckProcedurePar);

    /* EXEC SQL WHENEVER NOT FOUND GOTO end_select_loop; */

    /* EXEC SQL WHENEVER SQLERROR GOTO  fetch_error; */



    i = psqlda->RowsNumber;
    j = 0;
    while ((i < MaxRows) && (RowsProcessed < RowsToProcess))
    {
    
            for (j = 0; j < Every; j++)
            {
                if (i == MaxRows) break;
                if (j == 0) OSAddNewContextToSqlda (psqlda, Every);
                /* EXEC SQL FETCH C USING DESCRIPTOR select_dp; */

                {
                    struct sqlexd sqlstm;

                    sqlstm.sqlvsn = 8;
                    sqlstm.arrsiz = 3;
                    sqlstm.iters = (unsigned long  )1;
                    sqlstm.offset = (unsigned short)148;
                    sqlstm.cud = sqlcud0;
                    sqlstm.sqlest = (unsigned char  *)&sqlca;
                    sqlstm.sqlety = (unsigned short)0;
                    sqlstm.sqhstv[0] = (unsigned char  *)select_dp;
                    sqlstm.sqhstl[0] = (unsigned long  )0;
                    sqlstm.sqindv[0] = (         short *)0;
                    sqlstm.sqharm[0] = (unsigned long )0;
                    sqlstm.sqphsv = sqlstm.sqhstv;
                    sqlstm.sqphsl = sqlstm.sqhstl;
                    sqlstm.sqpind = sqlstm.sqindv;
                    sqlstm.sqparm = sqlstm.sqharm;
                    sqlstm.sqparc = sqlstm.sqharc;
                    sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
                    if (sqlca.sqlcode == 1403) goto end_select_loop;
                    if (sqlca.sqlcode < 0) goto fetch_error;
                }


                if (oracle_write_sqlda (psqlda) == -2)
                {
                    if (oracle_send_sqlda (psqlda, i) < 0)
                    {
                        /* EXEC SQL CLOSE C; */

                        {
                            struct sqlexd sqlstm;

                            sqlstm.sqlvsn = 8;
                            sqlstm.arrsiz = 3;
                            sqlstm.iters = (unsigned long  )1;
                            sqlstm.offset = (unsigned short)166;
                            sqlstm.cud = sqlcud0;
                            sqlstm.sqlest = (unsigned char  *)&sqlca;
                            sqlstm.sqlety = (unsigned short)0;
                            sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
                            if (sqlca.sqlcode < 0) goto fetch_error;
                        }


                        free (pack);
                        return -1;
                    }
                    OSAddNewContextToSqlda (psqlda, Every);
                    oracle_write_sqlda (psqlda);
                    j = 0;
                }
                i++;
                RowsProcessed++;
            }
            if ((i == MaxRows) || (RowsProcessed == RowsToProcess)) break;
            if (oracle_send_sqlda (psqlda, i) < 0)
            {
                /* EXEC SQL CLOSE C; */

                {
                    struct sqlexd sqlstm;

                    sqlstm.sqlvsn = 8;
                    sqlstm.arrsiz = 3;
                    sqlstm.iters = (unsigned long  )1;
                    sqlstm.offset = (unsigned short)180;
                    sqlstm.cud = sqlcud0;
                    sqlstm.sqlest = (unsigned char  *)&sqlca;
                    sqlstm.sqlety = (unsigned short)0;
                    sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
                    if (sqlca.sqlcode < 0) goto fetch_error;
                }


                free (pack);
                return -1;
            }
    }   
    if ((RowsProcessed == RowsToProcess) && (i != MaxRows))
    {
        if (i > 0)
            if (oracle_send_sqlda (psqlda, i) < 0)
            {
                /* EXEC SQL CLOSE C; */

                {
                    struct sqlexd sqlstm;

                    sqlstm.sqlvsn = 8;
                    sqlstm.arrsiz = 3;
                    sqlstm.iters = (unsigned long  )1;
                    sqlstm.offset = (unsigned short)226;
                    sqlstm.cud = sqlcud0;
                    sqlstm.sqlest = (unsigned char  *)&sqlca;
                    sqlstm.sqlety = (unsigned short)0;
                    sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
                    if (sqlca.sqlcode < 0) goto fetch_error;
                }


                free (pack);
                return -1;
            }
        free (pack);
        return 1;
    }

end_select_loop :
    if (i > 0)
    {
        psqlda->LastContext = TRUE;
        if (oracle_send_sqlda (psqlda, i) < 0)
        {
            /* EXEC SQL CLOSE C; */

            {
                struct sqlexd sqlstm;

                sqlstm.sqlvsn = 8;
                sqlstm.arrsiz = 3;
                sqlstm.iters = (unsigned long  )1;
                sqlstm.offset = (unsigned short)240;
                sqlstm.cud = sqlcud0;
                sqlstm.sqlest = (unsigned char  *)&sqlca;
                sqlstm.sqlety = (unsigned short)0;
                sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
                if (sqlca.sqlcode < 0) goto fetch_error;
            }


            free (pack);
            return -1;
        }
    }
    /* EXEC SQL CLOSE C; */

    {
        struct sqlexd sqlstm;

        sqlstm.sqlvsn = 8;
        sqlstm.arrsiz = 3;
        sqlstm.iters = (unsigned long  )1;
        sqlstm.offset = (unsigned short)254;
        sqlstm.cud = sqlcud0;
        sqlstm.sqlest = (unsigned char  *)&sqlca;
        sqlstm.sqlety = (unsigned short)0;
        sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
        if (sqlca.sqlcode < 0) goto fetch_error;
    }


    oracle_ack (pack);
    free (pack);
    return -1;

fetch_error :
    printf ("erreur durant le fetch\n");
    oracle_ack (pack);
    free (pack);
    return -1;
}

/*********************************************************************/

int oracle_send_sqlda (OSSqlda* psqlda, int i)
{
    psqlda->RowsNumber = i;
    if (psqlda->ContentsProcedure(psqlda, psqlda->ProcedurePar) < 0)
        return -1;
    psqlda->ContextNumber++;
    return 1;
}

/*********************************************************************/

int oracle_execute (char* SqlStatement, BYTE Commit,
                    int (*AckProcedure)(void* pack, void* papp),
                    void* AckProcedurePar)
{
    OSAck*   pack;

    /* EXEC SQL BEGIN DECLARE SECTION; */

    char   sql_statement[1024];
    /* EXEC SQL VAR sql_statement IS STRING(1024); */

    /* EXEC SQL END DECLARE SECTION; */


    pack = OSMallocAck ();

    strcpy (sql_statement, SqlStatement);

    if (AckProcedure)      OSSetAckProcedure (pack, AckProcedure);
    if (AckProcedurePar)   OSSetProcedurePar (pack, AckProcedurePar);

    /* EXEC SQL WHENEVER SQLERROR GOTO  execute_error; */

    /* EXEC SQL WHENEVER NOT FOUND GOTO execute_error; */


    /* EXEC SQL PREPARE S FROM :sql_statement; */

    {
        struct sqlexd sqlstm;

        sqlstm.sqlvsn = 8;
        sqlstm.arrsiz = 3;
        sqlstm.stmt = "";
        sqlstm.iters = (unsigned long  )1;
        sqlstm.offset = (unsigned short)268;
        sqlstm.cud = sqlcud0;
        sqlstm.sqlest = (unsigned char  *)&sqlca;
        sqlstm.sqlety = (unsigned short)0;
        sqlstm.sqhstv[0] = (unsigned char  *)sql_statement;
        sqlstm.sqhstl[0] = (unsigned long  )1024;
        sqlstm.sqindv[0] = (         short *)0;
        sqlstm.sqharm[0] = (unsigned long )0;
        sqlstm.sqphsv = sqlstm.sqhstv;
        sqlstm.sqphsl = sqlstm.sqhstl;
        sqlstm.sqpind = sqlstm.sqindv;
        sqlstm.sqparm = sqlstm.sqharm;
        sqlstm.sqparc = sqlstm.sqharc;
        sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
        if (sqlca.sqlcode < 0) goto execute_error;
    }


    /* EXEC SQL EXECUTE S; */

    {
        struct sqlexd sqlstm;

        sqlstm.sqlvsn = 8;
        sqlstm.arrsiz = 3;
        sqlstm.stmt = "";
        sqlstm.iters = (unsigned long  )1;
        sqlstm.offset = (unsigned short)286;
        sqlstm.cud = sqlcud0;
        sqlstm.sqlest = (unsigned char  *)&sqlca;
        sqlstm.sqlety = (unsigned short)0;
        sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
        if (sqlca.sqlcode == 1403) goto execute_error;
        if (sqlca.sqlcode < 0) goto execute_error;
    }


    if (Commit)
    { printf("execute whith commit\n");
        /* EXEC SQL COMMIT WORK; */

        {
            struct sqlexd sqlstm;

            sqlstm.sqlvsn = 8;
            sqlstm.arrsiz = 3;
            sqlstm.iters = (unsigned long  )1;
            sqlstm.offset = (unsigned short)300;
            sqlstm.cud = sqlcud0;
            sqlstm.sqlest = (unsigned char  *)&sqlca;
            sqlstm.sqlety = (unsigned short)0;
            sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
            if (sqlca.sqlcode < 0) goto execute_error;
        }


    }

    oracle_ack (pack);
    free (pack);
    return 1;

execute_error :
    oracle_ack (pack);
    free (pack);
    return -1;
}

/*********************************************************************/

int oracle_disconnect (BYTE Commit,
                       int (*AckProcedure)(void* pack, void* papp),
                       void* AckProcedurePar)
{
    OSAck*   pack;
    pack = OSMallocAck ();

    if (Commit)  printf("Commit RELEASE\n");
    else printf("Rollback RELEASE\n");

    /* EXEC SQL WHENEVER SQLERROR GOTO disconnect_error; */

    if (AckProcedure)      OSSetAckProcedure (pack, AckProcedure);
    if (AckProcedurePar)   OSSetProcedurePar (pack, AckProcedurePar);

    if (Commit)
        /* EXEC SQL COMMIT WORK RELEASE; */

    {
        struct sqlexd sqlstm;

        sqlstm.sqlvsn = 8;
        sqlstm.arrsiz = 3;
        sqlstm.iters = (unsigned long  )1;
        sqlstm.offset = (unsigned short)314;
        sqlstm.cud = sqlcud0;
        sqlstm.sqlest = (unsigned char  *)&sqlca;
        sqlstm.sqlety = (unsigned short)0;
        sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
        if (sqlca.sqlcode < 0) goto disconnect_error;
    }


    else
        /* EXEC SQL ROLLBACK WORK RELEASE; */

    {
        struct sqlexd sqlstm;

        sqlstm.sqlvsn = 8;
        sqlstm.arrsiz = 3;
        sqlstm.iters = (unsigned long  )1;
        sqlstm.offset = (unsigned short)328;
        sqlstm.cud = sqlcud0;
        sqlstm.sqlest = (unsigned char  *)&sqlca;
        sqlstm.sqlety = (unsigned short)0;
        sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
        if (sqlca.sqlcode < 0) goto disconnect_error;
    }



    oracle_ack (pack);
    free (pack);
    return 1;

disconnect_error :
    oracle_ack (pack);
    free (pack);
    return -1;
}

/*********************************************************************/

int oracle_commitorrollback (BYTE Action,
                             int (*AckProcedure)(void* pack, void* papp),
                             void* AckProcedurePar)
{
    OSAck*   pack;
    pack = OSMallocAck ();

    if (Action)  printf("Commit\n");
    else printf("Rollback\n");

    /* EXEC SQL WHENEVER SQLERROR GOTO rollback_error; */

    if (AckProcedure)      OSSetAckProcedure (pack, AckProcedure);
    if (AckProcedurePar)   OSSetProcedurePar (pack, AckProcedurePar);

    if (Action)
        /* EXEC SQL COMMIT WORK; */

    {
        struct sqlexd sqlstm;

        sqlstm.sqlvsn = 8;
        sqlstm.arrsiz = 3;
        sqlstm.iters = (unsigned long  )1;
        sqlstm.offset = (unsigned short)342;
        sqlstm.cud = sqlcud0;
        sqlstm.sqlest = (unsigned char  *)&sqlca;
        sqlstm.sqlety = (unsigned short)0;
        sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
        if (sqlca.sqlcode < 0) goto rollback_error;
    }


    else
        /* EXEC SQL ROLLBACK WORK; */

    {
        struct sqlexd sqlstm;

        sqlstm.sqlvsn = 8;
        sqlstm.arrsiz = 3;
        sqlstm.iters = (unsigned long  )1;
        sqlstm.offset = (unsigned short)356;
        sqlstm.cud = sqlcud0;
        sqlstm.sqlest = (unsigned char  *)&sqlca;
        sqlstm.sqlety = (unsigned short)0;
        sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
        if (sqlca.sqlcode < 0) goto rollback_error;
    }



    oracle_ack (pack);
    free (pack);
    return 1;

rollback_error :
    oracle_ack (pack);
    free (pack);
    return -1;
}

