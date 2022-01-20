
#include "sc.h"



char OrgId[10];
char ClientId[10];
char UserId[10];
char ConversionSeq[20];

int iConversionSeq;

MXCom* omega_com; 
MXCom* compiere_com; 
int Request = 0;
#define USER_ID     0
#define CLIENT_ID   1
#define ORG_ID      2

int i = 0;

int Finish  = 0;
int execution = 0;
int Year = 2005;
int Count = 1000;

int QueryFinish = 0;




typedef struct _CurrencyRate
{
    char EuroDollar[30];
    char DollarLBP [30];
    char EuroLBP[30];
    char Date[30];
}CurrencyDate;


CurrencyDate CurrencyDateTab[1000];
int CurrencyRead = 0;


void SelectId (char* Name, char* Id, int Type)
{
    char Statement[250];
    int every = 1;
    int max = 1;
    MXMessage* pmessage;
    MXMessage* pmessage1;
    char* Table_id;

    switch (Type)
    {
    case ORG_ID :
        if (strcmp (Name, "*") == 0)
             sprintf (Statement, "%s'%s';", "select AD_ORG_ID from AD_ORG where NAME = ", Name);
        else
            sprintf (Statement, "%s'%s'%s%s;", "select AD_ORG_ID from AD_ORG where NAME = ", Name, " and AD_CLIENT_ID = ", ClientId);
         Table_id = "AD_ORG_ID";
       break;
    case CLIENT_ID :
           sprintf (Statement, "%s'%s';", "select AD_CLIENT_ID from AD_CLIENT where NAME = ", Name);
           Table_id = "AD_CLIENT_ID";
       break;
    case  USER_ID :
           sprintf (Statement, "%s'%s';", "select AD_USER_ID from AD_USER where NAME = ", Name);
           Table_id = "AD_USER_ID";
       break;
       default :
            return;
    }
    pmessage =  MXPutMessage (compiere_com, DB_SYS, "ExecuteQuery");
    MXSetValue (pmessage ,"Every", 1, &every);
    MXSetValue (pmessage ,"MaxRows", 1, &max);
    MXSetValue (pmessage ,"Name",    1,         "Statement");
    MXSetValue (pmessage ,"SqlStatement", 1, Statement);
    MXSend (compiere_com->MX, compiere_com, pmessage);

    pmessage1 = MXRecv (compiere_com->MX, compiere_com);    //header
    MXFreeMessage (compiere_com->MX, pmessage1);
    pmessage1 = MXRecv (compiere_com->MX, compiere_com);    //contents
    strcpy (Id, (STRING)MXGetValue (pmessage1, Table_id, 1));
    pmessage1 =MXRecv (compiere_com->MX, compiere_com);  // Ack
    MXFreeMessage (compiere_com->MX, pmessage1);
    MXFreeMessage (compiere_com->MX, pmessage);
}

void SelectSeq (char* Name, char* Id)
{
    char Statement[250];
    int every = 1;
    int max = 1;
    MXMessage* pmessage;
    MXMessage* pmessage1;

    sprintf (Statement, "select CURRENTNEXT  from AD_SEQUENCE where NAME = '%s'", Name);
    pmessage =  MXPutMessage (compiere_com, DB_SYS, "ExecuteQuery");
    MXSetValue (pmessage ,"Every", 1, &every);
    MXSetValue (pmessage ,"MaxRows", 1, &max);
    MXSetValue (pmessage ,"Name",    1,         "Statement");
    MXSetValue (pmessage ,"SqlStatement", 1, Statement);
    MXSend (compiere_com->MX, compiere_com, pmessage);

    pmessage1 = MXRecv (compiere_com->MX, compiere_com);    //header
    MXFreeMessage (compiere_com->MX, pmessage1);
    pmessage1 = MXRecv (compiere_com->MX, compiere_com);    //contents
    strcpy (Id, (STRING)MXGetValue (pmessage1, "CURRENTNEXT", 1));
    pmessage1 =MXRecv (compiere_com->MX, compiere_com);  // Ack
    MXFreeMessage (compiere_com->MX, pmessage1);
    MXFreeMessage (compiere_com->MX, pmessage);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

char* strend (char* str, char* schar)
{
    int i = 0;
    for (i = strlen (str) - 1; i >= 0; i--)
      if (str[i] != schar[0])
        break;
    str[i+1] = 0;
    return str;
}

void strremove (char* str1, char* str, char character)
{
    while (*str)
    {
        if (*str != character)
        {
            *str1 = *str;
            str1++;
        }
        str++;
    }
   *str1 = 0;
}

int AckExcuteUpdate (MXMessage* pmessage, MXCom* pcom, void* appfield)
{
    BYTE Commit = '1';
    STRING SqlErrMsg = (STRING)MXGetValue (pmessage, "SqlErrMsg", 1);
    LONG RowProcessed = (LONG)MXGetValue (pmessage, "RowsProcessed", 1);
    STRING SqlStatement = (STRING)MXGetValue (pmessage, "SqlStatement", 1);
    SHORT Stop = (SHORT)MXGetValue (pmessage, "Stop", 1);


   if (Stop)
      printf ("Base = Compiere, Statement = %s\n, RowProcessed = %d, Error = %s\n",  SqlStatement,  RowProcessed, SqlErrMsg);
    else
      printf ("Base = Compiere,  RowProcessed = %d\n",  RowProcessed);
    
    return 1;
}

int AckExcuteQuery (MXMessage* pmessage, MXCom* pcom, void* appfield)
{

    MXCom* compiere_com = (MXCom*)appfield;
    BYTE Commit = '1';
    STRING SqlErrMsg = (STRING)MXGetValue (pmessage, "SqlErrMsg", 1);
    LONG RowProcessed = (LONG)MXGetValue (pmessage, "RowsProcessed", 1);
    STRING SqlStatement = (STRING)MXGetValue (pmessage, "SqlStatement", 1);
    SHORT Stop = (SHORT)MXGetValue (pmessage, "Stop", 1);


    if (Stop)
      printf ("Base = Omega, Statement = %s\n, RowProcessed = %d, Error = %s\n",  SqlStatement,  RowProcessed, SqlErrMsg);
    else
      printf ("Base = Omega, RowProcessed = %d\n",  RowProcessed);
   

    QueryFinish = 1;

    return 1;
}

    
void UpdateImportTable ()
{
    int j = 0;
    MXMessage* pmessage;
    BYTE Commit = '1';
    pmessage =  MXCreateMessage (compiere_com->MX, DB_SYS, "ExecuteUpdate");


    char SDate[100];
    char syear[100];
    char smonth[100];
    char sday[100];
    char Statement[1000];
    while (j < CurrencyRead)
    {

       strcpy (syear, strtok (CurrencyDateTab[j].Date, "-"));
       strcpy (smonth, strtok(NULL, "-"));
       strcpy (sday,  strtok(NULL, " "));
       sprintf (SDate, "%s/%s/%s", smonth, sday, syear);

       if ((j == 0) || (strcmp (CurrencyDateTab[j].EuroDollar, CurrencyDateTab[j-1].EuroDollar) != 0))
       {
       sprintf (Statement, "INSERT INTO I_CONVERSION_RATE (I_CONVERSION_RATE_ID, AD_CLIENT_ID, AD_ORG_ID, CREATEDBY, UPDATEDBY, \
                            ISO_CODE, ISO_CODE_TO, VALIDFROM,  MULTIPLYRATE, CREATERECIPROCALRATE)\
                             VALUES (%d, %s,%s,%s,%s,'%s','%s', %s'%s',%s, %s, '%s')",  
                               iConversionSeq, ClientId, OrgId, UserId, UserId, "EUR", "USD", "to_date (", SDate, "'mm/dd/yyyy')", CurrencyDateTab[j].EuroDollar, "Y");
           
       MXSetValue (pmessage, "SqlStatement", 1, Statement);
       MXSetValue (pmessage, "Commit", 1, &Commit);
       MXSend (compiere_com->MX, compiere_com, pmessage);
       MXFreeMessage (compiere_com->MX, MXRecv (compiere_com->MX, compiere_com));
       iConversionSeq++;
       }
       if ((j == 0) || (strcmp (CurrencyDateTab[j].DollarLBP, CurrencyDateTab[j-1].DollarLBP) != 0))
       {
       sprintf (Statement, "INSERT INTO I_CONVERSION_RATE (I_CONVERSION_RATE_ID, AD_CLIENT_ID, AD_ORG_ID, CREATEDBY, UPDATEDBY, \
                            ISO_CODE, ISO_CODE_TO, VALIDFROM, MULTIPLYRATE, CREATERECIPROCALRATE)\
                             VALUES (%d, %s,%s,%s,%s,'%s','%s', %s'%s',%s, %s, '%s')", 
                                   iConversionSeq, ClientId, OrgId, UserId, UserId, "USD", "LBP", "to_date (", SDate, "'mm/dd/yyyy')", CurrencyDateTab[j].DollarLBP,  "Y");
           
       MXSetValue (pmessage, "SqlStatement", 1, Statement);
       MXSend (compiere_com->MX, compiere_com, pmessage);
       MXFreeMessage (compiere_com->MX, MXRecv (compiere_com->MX, compiere_com));
       iConversionSeq++;
       }
       if ((j == 0) || (strcmp (CurrencyDateTab[j].EuroLBP, CurrencyDateTab[j-1].EuroLBP) != 0))
       {
       sprintf (Statement, "INSERT INTO I_CONVERSION_RATE (I_CONVERSION_RATE_ID, AD_CLIENT_ID, AD_ORG_ID, CREATEDBY, UPDATEDBY, \
                            ISO_CODE, ISO_CODE_TO, VALIDFROM, MULTIPLYRATE, CREATERECIPROCALRATE)\
                            VALUES (%d, %s,%s,%s,%s,'%s','%s', %s'%s',%s, %s, '%s')", 
                               iConversionSeq, ClientId, OrgId, UserId, UserId, "EUR", "LBP", "to_date (", SDate, "'mm/dd/yyyy')", CurrencyDateTab[j].EuroLBP,  "Y");
           
       MXSetValue (pmessage, "SqlStatement", 1, Statement);
       MXSend (compiere_com->MX, compiere_com, pmessage);
       MXFreeMessage (compiere_com->MX, MXRecv (compiere_com->MX, compiere_com));
       iConversionSeq++;
       }

       j++; printf ("<--- %d\n", j);
    }

    sprintf (Statement, "%s%d%s" , "update AD_SEQUENCE set CURRENTNEXT = ",  iConversionSeq, " WHERE NAME = 'I_Conversion_Rate'");
    MXSetValue (pmessage, "SqlStatement", 1, Statement );
    MXSend (compiere_com->MX, compiere_com, pmessage);
    MXFreeMessage (compiere_com->MX, MXRecv (compiere_com->MX, compiere_com));
    MXFreeMessage (compiere_com->MX, pmessage);
    MXCloseCom (compiere_com->MX, compiere_com);
    MXCloseCom (compiere_com->MX, omega_com);
    return ;
}


int SelectCurrencyRates (MXMessage* pmessage, MXCom* pcom, void* appfield)
{
    
    STRING TCurr_dDate =             strend ((STRING) MXGetValue (pmessage, "TCurr_dDate", 1), " ");
    STRING TCurr_nbCurrencyID  =     (STRING)MXGetValue (pmessage, "TCurr_nbCurrencyID", 1);
    STRING TCurr_strRates  =    strend ((STRING)MXGetValue (pmessage, "TCurr_strRates", 1), " ");
  
    STRING s1 = TCurr_strRates;
  
    int row = atoi (TCurr_nbCurrencyID);
    
    i++;
    if (i < 7) return 1;

   if (row == 1)
   {
       strtok (s1, "/");
       strremove  (CurrencyDateTab[CurrencyRead].DollarLBP, strtok (NULL, "/"), ',');
       strremove (CurrencyDateTab[CurrencyRead].EuroLBP, strtok (NULL, "/"), ',');
   }    
   else
   {
       strtok (s1, "/");
       strtok (NULL, "/");
       strremove (CurrencyDateTab[CurrencyRead].EuroDollar, strtok (NULL, "/"), ',');
       if (strcmp ("     0.0000000", CurrencyDateTab[CurrencyRead].EuroDollar) == 0)
       {
         strcpy (CurrencyDateTab[CurrencyRead].EuroDollar, CurrencyDateTab[CurrencyRead-1].EuroDollar);
       }
       strcpy (CurrencyDateTab[CurrencyRead].Date, TCurr_dDate);
       CurrencyRead++;

   }



  return 1;

}   






void SelectCurrencyRatesRows (MX* pmx)
{
    MXMessage* pmessage;
    int every = 1;
    int max = Count;
    int i = 0;
    BYTE Commit = 1;
 
         
    MXAddComCallBack (pmx, omega_com, DB_SYS,  "SelectCurrencyRates", MXONRECV, SelectCurrencyRates, compiere_com); 


    pmessage = MXCreateMessage (pmx, DB_SYS, "ExecuteQuery");
    MXSetValue (pmessage ,"Every", 1, &every);
    MXSetValue (pmessage ,"MaxRows", 1, &max);
   
    MXSetValue (pmessage ,"Name",    1,    "SelectCurrencyRates"  );
    MXSetValue (pmessage ,"SqlStatement", 1, "SELECT * from dev order by TCurr_dDate, TCurr_nbCurrencyId");
    MXSend (pmx, omega_com, pmessage);
    while (!QueryFinish)
    {
        MXFreeMessage (omega_com->MX, MXRecv (omega_com->MX, omega_com));

    }
    MXFreeMessage (pmx, pmessage);
}




void main(int argc, char* argv[])
{
    MX mx;
    Year = 2005;


    if (argc > 1)
        Year = atoi(argv[1]);
    if (argc > 2)
        Count = atoi(argv[2]);

    MXInit (&mx, MXCLIENT, "base", "base", 0, NULL);

   
    omega_com = MXOpenBase (&mx, "omega", "","", DBPROTO_ODBC, TRUE);
    if (!omega_com) return;
    compiere_com = MXOpenBase (&mx, "compiere", "scott","tiger", DBPROTO_ODBC, TRUE);
    if (!compiere_com) return;

    MXAddComCallBack (&mx, omega_com,    DB_SYS, "AckStatement",   MXONRECV,   AckExcuteQuery, compiere_com); 

    
    
    SelectId ("Techno Cars", ClientId, CLIENT_ID);
    SelectId ("Techno Cars", OrgId, ORG_ID);
    SelectId ("SuperUser", UserId, USER_ID);
    SelectSeq ("I_Conversion_Rate", ConversionSeq);
    iConversionSeq = atol (ConversionSeq);
    MXAddComCallBack (&mx, compiere_com, DB_SYS, "AckStatement",   MXONRECV,   AckExcuteUpdate, omega_com); 

    SelectCurrencyRatesRows (&mx);
    UpdateImportTable ();    

    MXDispatchEvents (&mx, 0);

    MXEnd (&mx);

}