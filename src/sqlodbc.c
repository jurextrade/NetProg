/* NetProg is an API in C language allowing an homogeneous programming of  */
/* communicating applications                                              */
/* Copyright (C) 2002,2003,2004,2005                                       */
/* Gabriel Jureidini                                                       */
/* Version 2.0 - 17 July 2005                                              */
/*-------------------------------------------------------------------------*/


#define _CRT_SECURE_NO_WARNINGS

#include "ostypes.h"
#include "osio.h"
#include "osdb.h"

#include <sql.h>
#include <sqlext.h>

#define MAXCONTENTSIZE  80000 //3896

HENV        m_hEnv = NULL;

typedef struct S_sqlda
{
    SDWORD*        crsize;
    char*          values;
    HSTMT          m_hStmt;
    HDBC           m_hDbc;
}S_sqlda;


/*-------------------------------------------------------------------------*/

void odbc_freeS_sqlda (void* p)
{

	S_sqlda* psqlda = (S_sqlda*) p;
    if (!psqlda) return;

    free (psqlda->crsize);
    free (psqlda->values);
    free (psqlda);
}


void odbc_ack (OSAck* pack, HSTMT m_hStmt, LONG nRet)
{
    SDWORD NativeError;
    SWORD svErrorMsg;

    pack->SqlCode = 1;

    if (nRet!=SQL_SUCCESS && nRet!=SQL_SUCCESS_WITH_INFO )
        pack->Stop = (SHORT)nRet;


    memset (pack->SqlErrMsg, ' ', 500);

    SQLError(m_hEnv, (HDBC)pack->Connection, m_hStmt,(unsigned char*)pack->SqlErrMsg, &NativeError,(unsigned char*)pack->SqlErrMsg, 500, &svErrorMsg );

    pack->Print = 1;

    if (pack->AckProcedure)
        pack->AckProcedure (pack, pack->ProcedurePar);
    return;
}





/*-------------------------------------------------------------------------*/

int odbc_connect (char* DataBaseName, char* UserName, char* PassWord,
                  int (*AckProcedure)(void* pack, void* papp),
                  void* AckProcedurePar)
{
    OSAck* pack;
    HDBC m_hDBC;

    int nConnect = -1;

    pack = OSMallocAck ();

    if (AckProcedure)      OSSetAckProcedure (pack, AckProcedure);
    if (AckProcedurePar)   OSSetProcedurePar (pack, AckProcedurePar);


    if (m_hEnv == NULL)  nConnect = SQLAllocEnv (&m_hEnv);
    else                 nConnect = SQL_SUCCESS;

	SQLAllocHandle(SQL_HANDLE_DBC, m_hEnv, &m_hDBC);

    if (nConnect == SQL_SUCCESS)
    {
        nConnect = SQLAllocConnect (m_hEnv, &m_hDBC);
        if (nConnect == SQL_SUCCESS)
        {
            //SQLSetConnectOption( m_hDBC,SQL_LOGIN_TIMEOUT,5 );
            SQLSetConnectAttr( m_hDBC,SQL_AUTOCOMMIT,SQL_AUTOCOMMIT_OFF, 0 );
            nConnect=SQLConnect( m_hDBC,(unsigned char*)DataBaseName,SQL_NTS,(unsigned char*)UserName,SQL_NTS,(unsigned char*)PassWord,SQL_NTS );
        }
    }
    if (nConnect < 0)
    {
        odbc_ack (pack, NULL, 1);
        free (pack);
        return -1;
    }
    pack->Connection = (void *)m_hDBC;
    odbc_ack (pack, NULL, 1);
    free (pack);
    return (int)m_hDBC;
}


int odbc_disconnect (void* Connection, BYTE Commit,
                     int (*AckProcedure)(void* pack, void* papp),
                     void* AckProcedurePar)
{
    OSAck*   pack;
    int nRet;

    pack = OSMallocAck ();
    pack->Connection = Connection;


    if (AckProcedure)      OSSetAckProcedure (pack, AckProcedure);
    if (AckProcedurePar)   OSSetProcedurePar (pack, AckProcedurePar);

    nRet = SQLTransact(m_hEnv, (HDBC)Connection, SQL_ROLLBACK);
    if (nRet!=SQL_SUCCESS)
    {
        odbc_ack (pack, NULL, nRet);
        free (pack);
        return -1;
    }
    odbc_ack (pack, NULL, 0);
    free (pack);

    nRet=SQLDisconnect((HDBC)Connection);
    if (nRet != SQL_SUCCESS && nRet != SQL_SUCCESS_WITH_INFO )
        return -1;

    
    return 1;
}



/*-------------------------------------------------------------------------*/

int odbc_send_sqlda (OSSqlda* psqlda, int i)
{
    psqlda->RowsNumber = i;

    if (psqlda->ContentsProcedure(psqlda, psqlda->ProcedurePar) < 0)
        return -1;


    psqlda->Context[psqlda->ContextNumber - 1]->Size = 0;
    return 1;
}



int odbc_write_sqlda (OSSqlda* psqlda)
{
    int i;
    int Error = 0;
    OSContext* pcontext;
    char* values;
    int length = 0;
    int tlength = 0;
    S_sqlda* s_sqlda;
    int size = psqlda->size;

    pcontext = psqlda->Context[psqlda->ContextNumber-1];
     
    s_sqlda = (S_sqlda *)psqlda->S_sqlda;

    values = s_sqlda->values;

    for (i = 0; i < psqlda->ColumnsNumber; i++)
    {
        if (psqlda->Description[i]->Type != SQL_LONGVARBINARY)
        {
            length = (int)strlen (values);
            if (length <= psqlda->Description[i]->Length)
                memset (values + length, ' ',  psqlda->Description[i]->Length - length);
        }
        values += psqlda->Description[i]->Length;
    }

    *values = 0;
    memcpy (pcontext->Values + pcontext->Size, s_sqlda->values ,size + 1);
    pcontext->Values[pcontext->Size + size] = 0;

    memset (s_sqlda->values, ' ', size + 1);
    pcontext->Size += size + 1;

    return 1;
}



/*-------------------------------------------------------------------------*/


OSSqlda* odbc_process (void* Connection, char* SqlStatement,  DWORD Every,
                       DWORD MaxRows, int (*HeaderProcedure)(void* psqlda, void* papp),int (*ContentsProcedure) (void* psqlda, void* papp),
                       void* ProcedurePar, int (*AckProcedure)(void* pack, void* papp), void* AckProcedurePar)
{
    int         size;
    DWORD       i = 1;
    DWORD       nRet;

    char*       values;
    OSSqlda*    psqlda = NULL;
    OSAck*      pack;

    unsigned char   swColName[256];

    SQLSMALLINT     swColNamelength;
    SQLSMALLINT     swColType;
    SQLSMALLINT     swColScale;
    SQLSMALLINT     swColNullable;
    SQLSMALLINT     swColNumber;
  //  unsigned char   ucOwner[256];
    
    SQLLEN          swColLength;

    HSTMT           m_hStmt;
    SQLINTEGER      cbvar = 0;
    SQLSMALLINT     par;
 //   SQLSMALLINT     columns;

    S_sqlda*        s_sqlda;

    pack = OSMallocAck ();
    pack->Connection = Connection;

    if (AckProcedure)       OSSetAckProcedure (pack, AckProcedure);
    if (AckProcedurePar)    OSSetProcedurePar (pack, AckProcedurePar);

    nRet=SQLAllocStmt((HDBC)Connection,&m_hStmt);

    if(nRet!=SQL_SUCCESS && nRet!=SQL_SUCCESS_WITH_INFO )
    {
        odbc_ack (pack, m_hStmt, nRet);
        free (pack);
        return NULL;
    }

    /*
    nRet = SQLSetStmtOption(m_hStmt, SQL_ASYNC_ENABLE, SQL_ASYNC_ENABLE_ON);
          odbc_ack (pack, m_hStmt, nRet);
    */

    //  SQLBindParameter(m_hStmt, 1, SQL_PARAM_INPUT, SQL_C_SSHORT, SQL_INTEGER, 0, 0, &par, 0, &cbvar);
    //  SQLBindParameter(m_hStmt, 2, SQL_PARAM_INPUT_OUTPUT, SQL_C_SSHORT, SQL_INTEGER, 0, 0, &par, 0, &cbvar);
    par = 1;

    nRet=SQLExecDirect( m_hStmt, (unsigned char*)SqlStatement, SQL_NTS );

    if (nRet != SQL_SUCCESS && nRet != SQL_SUCCESS_WITH_INFO)
    {
        odbc_ack (pack, m_hStmt, nRet);
        SQLFreeStmt (m_hStmt, SQL_DROP);
        free (pack);
        return NULL;
    }

    nRet = SQLNumResultCols(m_hStmt, &swColNumber);
        
//    swColNumber = 0;
//    SQLColAttributes(m_hStmt, 1, SQL_COLUMN_COUNT, ucOwner, 256, &swScale, &swColNumber);
    if (nRet != SQL_SUCCESS && nRet != SQL_SUCCESS_WITH_INFO)
    {
        odbc_ack (pack, m_hStmt, nRet);
        SQLFreeStmt (m_hStmt, SQL_DROP);
        free (pack);
        return NULL;
    }

    psqlda = OSMallocSqlda ((int)swColNumber);
  

    if (!psqlda)
    {
        SQLFreeStmt (m_hStmt, SQL_DROP);
        free (pack);
        return NULL;
    }

    if (ContentsProcedure) OSSetContentsProcedure(psqlda, ContentsProcedure);
    if (HeaderProcedure)   OSSetHeaderProcedure  (psqlda, HeaderProcedure);
    if (ProcedurePar)      OSSetProcedurePar     (psqlda, ProcedurePar);

    psqlda->ColumnsNumber = swColNumber;

    size = 0;
    while(i <= (DWORD)swColNumber)
    {
        nRet = SQLDescribeCol( m_hStmt, (WORD)i, swColName,256, &swColNamelength, &swColType, &swColLength, &swColScale, &swColNullable);

        psqlda->Description[i-1]->Name  =    (char *)malloc (strlen ((char *)swColName));
        strncpy (psqlda->Description[i-1]->Name , (char *)swColName, strlen ((char *)swColName));

        psqlda->Description[i-1]->Type       = (int)swColType;
        psqlda->Description[i-1]->ItemLength = (int)strlen ((char*)swColName);

        switch (swColType)
        {
        case SQL_TIME:                                             //ok
            psqlda->Description[i - 1]->Type = OSTIME;
            psqlda->Description[i - 1]->Length = swColLength + 1;
            size += swColLength + 1;
            break;
        case SQL_TIMESTAMP:
            psqlda->Description[i - 1]->Type = OSTIMESTAMP;
            psqlda->Description[i - 1]->Length = swColLength + 1;
            size += swColLength + 1;
            break;
        case SQL_BIT:       /* LONG */
            psqlda->Description[i - 1]->Type = OSBIT;
            psqlda->Description[i - 1]->Length = swColLength + 1;
            size += swColLength + 1;
            break;

        case SQL_CHAR :     /* CHAR = 1 NCHAR*/
            psqlda->Description[i-1]->Type   = OSCHAR;
            psqlda->Description[i-1]->Length = swColLength + 1;
            size += swColLength + 1;
            break;
        case SQL_INTEGER :  /* UMBER */
            psqlda->Description[i-1]->Type   = OSINTEGER;
            psqlda->Description[i-1]->Length = swColLength + 1;
            size += swColLength + 1;
            break;
        case SQL_DOUBLE:    /* LONG */
            psqlda->Description[i-1]->Type   = OSDOUBLE;
            psqlda->Description[i-1]->Length = swColLength + 1;
            size += swColLength + 1;
            break;
        case SQL_DECIMAL:   /* NUMBER  = 3 */
            psqlda->Description[i-1]->Type   = OSDECIMAL;
            psqlda->Description[i-1]->Length = swColLength + 1;
            size += swColLength + 1;
            break;
        case SQL_FLOAT :    /* ROWID */
            psqlda->Description[i-1]->Type   = OSFLOAT;
            psqlda->Description[i-1]->Length = swColLength + 1;
            size += swColLength + 1;
            break;
        case SQL_DATE :     /* DATE  = 11*/
            psqlda->Description[i-1]->Type   = OSDATE;
            psqlda->Description[i-1]->Length = swColLength + 1;
            size += swColLength + 1;
            break;
        case SQL_NUMERIC :  /* RAW  */
            psqlda->Description[i-1]->Type   = OSNUMERIC;
            psqlda->Description[i-1]->Length = swColLength + 1;
            size += swColLength + 1;
            break;
       case SQL_SMALLINT :  
            psqlda->Description[i-1]->Type   = OSSHORT;
            psqlda->Description[i-1]->Length = swColLength + 1;
            size += swColLength + 1;
            break;
        case SQL_VARCHAR :  /* NVARCHAR2 = 12  VARCHAR2*/
            psqlda->Description[i-1]->Type   = OSVARCHAR;
            psqlda->Description[i-1]->Length = swColLength + 1;
            size += swColLength + 1;
            break;
        case SQL_LONGVARBINARY :    /* LONG RAW  */
            psqlda->Description[i-1]->Type   = OSLONGVARBINARY;
            psqlda->Description[i-1]->Length = MAXCONTENTSIZE;
            size += MAXCONTENTSIZE;
            break;
        case SQL_LONGVARCHAR  :
        case SQL_BINARY       :
        case SQL_VARBINARY    :
        case SQL_BIGINT       :
        case SQL_TINYINT      :
        case SQL_WCHAR	:
        case SQL_WVARCHAR :
            psqlda->Description[i - 1]->Length = 300;
            size += 301;//swColLength + 1;
            break;
        default :
            psqlda->Description[i-1]->Length = 300;
            size += 301;//swColLength + 1;
            break;
        }
        psqlda->Description[i-1]->Scale = swColScale;
        i++;
    }

    psqlda->size = size;

    s_sqlda = (S_sqlda *) malloc (sizeof (S_sqlda));
    psqlda->S_sqlda  = s_sqlda;
    s_sqlda->values = (char *)malloc (size + 1);
    s_sqlda->crsize = (SDWORD *)malloc (sizeof (SDWORD) * psqlda->ColumnsNumber);
    s_sqlda->m_hStmt = m_hStmt;
    s_sqlda->m_hDbc  = (HDBC)Connection;

    memset (s_sqlda->values, ' ', size + 1);
    values = s_sqlda->values;


    for (i = 1; i <= (DWORD)psqlda->ColumnsNumber; i++)
    {
        s_sqlda->crsize[i-1] = psqlda->Description[i-1]->Length;
        if (psqlda->Description[i-1]->Type != SQL_LONGVARBINARY)
            nRet = SQLBindCol(m_hStmt, (WORD)i, SQL_C_CHAR ,values, psqlda->Description[i-1]->Length, &s_sqlda->crsize[i-1]); //SQL_NO_TOTAL);
        else
            nRet = SQLBindCol(m_hStmt, (WORD)i, SQL_C_BINARY ,values, psqlda->Description[i-1]->Length, &s_sqlda->crsize[i-1]); //SQL_NO_TOTAL);

        values += psqlda->Description[i-1]->Length;
    }


    if (psqlda->HeaderProcedure (psqlda, psqlda->ProcedurePar) < 0)
    {
        SQLFreeStmt (m_hStmt, SQL_DROP);
        OSFreeSqlda (psqlda);
        free (pack);
        return NULL;
    }
    else
    {
                 
        OSAddNewContextToSqlda (psqlda, Every);
        free (pack);
        return psqlda;
    }
}


/*-------------------------------------------------------------------------*/

int odbc_fetch (OSSqlda* psqlda, 
                DWORD Every, DWORD MaxRows, DWORD RowsToProcess,
                int (*AckProcedure)(void* pack, void* papp),void* AckProcedurePar)
{
    DWORD  i = 0;
    DWORD  j;
    DWORD  RowsProcessed = 0;
    OSAck* pack;
    int nRet = 0;
    BOOL fin = FALSE;

    UDWORD crow;
    HSTMT m_hStmt    = ((S_sqlda *)psqlda->S_sqlda)->m_hStmt;
    HDBC  Connection = ((S_sqlda *)psqlda->S_sqlda)->m_hDbc;


    pack = OSMallocAck ();
    pack->Connection = Connection;
    if (AckProcedure)       OSSetAckProcedure (pack, AckProcedure);
    if (AckProcedurePar)    OSSetProcedurePar (pack, AckProcedurePar);

 


    i = psqlda->RowsNumber;

    j = 0;

    while ((i < MaxRows) && (RowsProcessed < RowsToProcess))
    {


        for (j = 0; j < Every; j++)
        {
            if (i == MaxRows) break;


 //           nRet = SQLFetch(m_hStmt);
            nRet=SQLExtendedFetch (m_hStmt, SQL_FETCH_NEXT, 1, &crow, NULL);
            if (nRet != SQL_SUCCESS && nRet != SQL_SUCCESS_WITH_INFO)
            {
                fin = TRUE;
                pack->RowsProcessed = i;
                break;
            }

            if (odbc_write_sqlda (psqlda) == -2)
            {
                if (odbc_send_sqlda (psqlda, i) < 0)
                {
                    SQLFreeStmt (m_hStmt, SQL_DROP);
                    free (pack);
                    return -1;
                }
                
                odbc_write_sqlda (psqlda);
                j = 0;
            }
            i++;
            RowsProcessed++;
            pack->RowsProcessed = i;

        }
        if (fin) break;

        if ((i == MaxRows) || (RowsProcessed == RowsToProcess)) break;

        if (odbc_send_sqlda (psqlda, i) < 0)
        {
            SQLFreeStmt (m_hStmt, SQL_DROP);
            free (pack);
            return -1;
        }
      
    }

    if (fin)
    {
        if (nRet == SQL_NO_DATA_FOUND)
        {
            if (i > 0)
            {
                psqlda->LastContext = TRUE;
                if (odbc_send_sqlda (psqlda, i) < 0)
                {
                    SQLFreeStmt (m_hStmt, SQL_DROP);
                    free (pack);
                    return -1;
                }
            }
            nRet = 0;
        }
        odbc_ack (pack, m_hStmt, nRet);
        SQLFreeStmt (m_hStmt, SQL_DROP);
        free (pack);
        return 1;
    }

    if ((RowsProcessed == RowsToProcess) && (i != MaxRows))
    {
        if (i > 0)

            if (odbc_send_sqlda (psqlda, i) < 0)
            {
                SQLFreeStmt (m_hStmt, SQL_DROP);
                free (pack);
                return -1;
            }
      
        
        free (pack);
        return 1;
    }

    if (i > 0)
    {
        psqlda->LastContext = TRUE;
        if (odbc_send_sqlda (psqlda, i) < 0)
        {
            SQLFreeStmt (m_hStmt, SQL_DROP);
            free (pack);
            return -1;
        }
        odbc_ack (pack, m_hStmt, 0);
        SQLFreeStmt (m_hStmt, SQL_DROP);
        free (pack);
        return 1;
    }
    return 0;
}


/*-------------------------------------------------------------------------*/


int odbc_execute (void* Connection, char* SqlStatement, BYTE Commit,
                  int (*AckProcedure)(void* pack, void* papp),
                  void* AckProcedurePar)
{
    OSAck*   pack;
    int      nRet;
    HSTMT    m_hStmt;
    char   sql_statement[4000];

    pack = OSMallocAck ();
    pack->Connection = Connection;

    strcpy (sql_statement, SqlStatement);


    if (AckProcedure)      OSSetAckProcedure (pack, AckProcedure);
    if (AckProcedurePar)   OSSetProcedurePar (pack, AckProcedurePar);

    nRet = SQLAllocStmt((HDBC)Connection, &m_hStmt);

    if (nRet != SQL_SUCCESS && nRet != SQL_SUCCESS_WITH_INFO)
    {
        odbc_ack (pack, m_hStmt, nRet);
        free (pack);
        return -1;
    }


    nRet = SQLExecDirect(m_hStmt, (unsigned char*)sql_statement, SQL_NTS );


    if (nRet != SQL_SUCCESS && nRet != SQL_SUCCESS_WITH_INFO )
    {
        odbc_ack (pack, m_hStmt, nRet);
        SQLFreeStmt (m_hStmt, SQL_DROP);
        free (pack);
        return -1;
    }

    if (Commit)
    {
        SQLTransact(m_hEnv, (HDBC)Connection, SQL_COMMIT);
        if (nRet != SQL_SUCCESS)
        {
            odbc_ack (pack, m_hStmt, nRet);
            SQLFreeStmt (m_hStmt, SQL_DROP);
            free (pack);
            return -1;
        }
    }

    odbc_ack (pack, m_hStmt, 0);
    SQLFreeStmt (m_hStmt, SQL_DROP);

    free (pack);
    return 1;
}


/*-------------------------------------------------------------------------*/


int odbc_sexecute (void* Connection, char* SqlStatement, BYTE Commit,
                   int (*AckProcedure)(void* pack, void* papp),
                   void* AckProcedurePar, void* Content, DWORD ContentSize)
{
    OSAck*  pack;
    int     nRet;
    HSTMT   m_hStmt;
    char    sql_statement[4000];
    SDWORD  cbvar = ContentSize;


    pack = OSMallocAck ();
    pack->Connection = Connection;
    strcpy (sql_statement, SqlStatement);


    if (AckProcedure)      OSSetAckProcedure (pack, AckProcedure);
    if (AckProcedurePar)   OSSetProcedurePar (pack, AckProcedurePar);

    nRet=SQLAllocStmt((HDBC)Connection,&m_hStmt );

    if(nRet!=SQL_SUCCESS && nRet!=SQL_SUCCESS_WITH_INFO )
    {
        odbc_ack (pack, m_hStmt, nRet);
        free (pack);
        return -1;
    }
    /*
      nRet = SQLPrepare( m_hStmt, sql_statement, SQL_NTS ; 
      
      if(nRet!=SQL_SUCCESS && nRet!=SQL_SUCCESS_WITH_INFO ) 
      {
          odbc_ack (pack, m_hStmt, nRet);
          SQLFreeStmt (m_hStmt, SQL_DROP);
          free (pack);
          return -1;
      }

      nRet = SQLExecute(m_hStmt); 

    */  

    nRet = SQLExecDirect(m_hStmt, (unsigned char*)sql_statement, SQL_NTS );
    if(nRet!=SQL_SUCCESS && nRet!=SQL_SUCCESS_WITH_INFO )
    {
        odbc_ack (pack, m_hStmt, nRet);
        SQLFreeStmt (m_hStmt, SQL_DROP);
        free (pack);
        return -1;
    }


    if (Commit)
    {
        SQLTransact(m_hEnv, (HDBC)Connection, SQL_COMMIT);
        if(nRet!=SQL_SUCCESS)
        {
            odbc_ack (pack, m_hStmt, nRet);
            SQLFreeStmt (m_hStmt, SQL_DROP);
            free (pack);
            return -1;
        }
    }


    odbc_ack (pack, m_hStmt, 0);
    SQLFreeStmt (m_hStmt, SQL_DROP);
    free (pack);

    return 1;
}


/*-------------------------------------------------------------------------*/


int odbc_commitorrollback (void* Connection, BYTE Action,
                           int (*AckProcedure)(void* pack, void* papp),
                           void* AckProcedurePar)
{
    OSAck*   pack;
    int nRet;

    pack = OSMallocAck ();
    pack->Connection = Connection;


    if (AckProcedure)      OSSetAckProcedure (pack, AckProcedure);
    if (AckProcedurePar)   OSSetProcedurePar (pack, AckProcedurePar);

    if (Action)
    {
        nRet = SQLTransact(m_hEnv, (HDBC)Connection, SQL_COMMIT);
        if (nRet!=SQL_SUCCESS)
        {
            odbc_ack (pack, NULL, nRet);
            free (pack);
            return -1;
        }
    }
    else
    {
        nRet = SQLTransact(m_hEnv, (HDBC)Connection, SQL_ROLLBACK);
        if (nRet!=SQL_SUCCESS)
        {
            odbc_ack (pack, NULL, nRet);
            free (pack);
            return -1;
        }
    }

    odbc_ack (pack, NULL, 0);
    free (pack);
    return 1;
}



