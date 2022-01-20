
#include "sc.h"


char OrgId[10];
char ClientId[10];
char UserId[10];
char SaleId[10];

char OrderSeq[20];
int iOrderSeq;

MXCom* omega_com; 
MXCom* compiere_com; 
int Request = 0;
#define USER_ID     0
#define CLIENT_ID   1
#define ORG_ID      2

int QueryFinish = 0;
int j = 0;
int Count = 250000;
int Year = 2005;
char Parameter[30];

char Statement[3000];


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

void strTakeOffQuote (char* str1, char* str)
{
    while (*str)
    {
        if (*str == '\'') 
        { *str1 = '\'';
          str1++;
          *str1 = '\'';
        }
        else    *str1 = *str;          
        str1++;
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
    MXMessage* pmessage1;
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
    if (QueryFinish)
    {
        sprintf (Statement, "%s%d%s" , "update AD_SEQUENCE set CURRENTNEXT = ",  iOrderSeq, " WHERE NAME = 'I_Order'");
        pmessage1 =  MXPutMessage (compiere_com, DB_SYS, "ExecuteUpdate");
        MXSetValue (pmessage1, "SqlStatement", 1, Statement );
        MXSetValue (pmessage1, "Commit", 1, &Commit);
        MXSend (compiere_com->MX, compiere_com, pmessage1);
        MXFreeMessage (compiere_com->MX, MXRecv (compiere_com->MX, compiere_com));
        MXFreeMessage (compiere_com->MX, pmessage1);
        MXCloseCom (compiere_com->MX, compiere_com);
        MXCloseCom (compiere_com->MX, omega_com);
   }
    return 1;
}


int SelectOrder (MXMessage* pmessage, MXCom* pcom, void* appfield)
{  
    MXMessage* pmessage1;
    BYTE Commit = '1';
    pmessage1 =  MXCreateMessage (compiere_com->MX, DB_SYS, "ExecuteUpdate");
    MXSetValue (pmessage1, "Commit", 1, &Commit);

    STRING Name =             strend ((STRING) MXGetValue (pmessage, "Name", 1), " ");
    STRING ToName =             strend ((STRING) MXGetValue (pmessage, "ToName", 1), " ");
    STRING TSIT_nsQty1  =     strend ((STRING)MXGetValue (pmessage, "TSIT_nsQty1", 1), " ");
    STRING TAccT_sDesc  =     strend ((STRING)MXGetValue (pmessage, "TAccT_sDesc", 1), " ");
  
	STRING TATrs_dOperationDate =    strend ((STRING)MXGetValue (pmessage, "TATrs_dOperationDate", 1), " ");
    STRING TATrs_sTransReference =    strend ((STRING)MXGetValue (pmessage, "TATrs_sTransReference", 1), " ");
    STRING TSIT_ncUnitPrice =    strend ((STRING)MXGetValue (pmessage, "TSIT_ncUnitPrice", 1), " ");
    STRING TSIT_ncUnitPriceLL =    strend ((STRING)MXGetValue (pmessage, "TSIT_ncUnitPriceLL", 1), " ");
    STRING TSIT_ncTotalPrice =    strend ((STRING)MXGetValue (pmessage, "TSIT_ncTotalPrice", 1), " ");
    
    char QName[100];
    char QToName[100];
    
    QName[0]= 0;
    QToName[0]= 0;

    strTakeOffQuote (QName, Name);
    strTakeOffQuote (QToName, ToName);



    char Statement[1000];
    char SDate[100];
    char syear[100];
    char smonth[100];
    char sday[100];
    strcpy (syear, strtok (TATrs_dOperationDate, "-"));
    strcpy (smonth, strtok(NULL, "-"));
    strcpy (sday,  strtok(NULL, " "));
    sprintf (SDate, "%s/%s/%s", smonth, sday, syear);
    

    char UnitPrice[100];
    char DocumentType[50];

    TSIT_nsQty1 =  TSIT_nsQty1 + 1;


    strremove (UnitPrice, TSIT_ncUnitPriceLL, ',');
    if (strcmp (Parameter, "Cash Sale") == 0)
        strcpy (DocumentType, "POS Order");
    else
    if (strcmp (Parameter, "Record Of Material")== 0)
        strcpy (DocumentType, "Record Of Material");
    else
    if (strcmp (Parameter, "Stock Order")== 0)
    {
        strcpy (DocumentType, "Purchase Order");
        TSIT_nsQty1 =  TSIT_nsQty1 - 1;
        strcpy (QToName, TAccT_sDesc);
		if (strncmp (TAccT_sDesc, "ADA", 3) == 0)
			  strcpy (QToName, "ADAM OPEL");
        strremove (UnitPrice, TSIT_ncUnitPrice, ',');
    }

    


    sprintf (Statement, "INSERT INTO I_Order  (I_Order_ID, AD_CLIENT_ID, AD_ORG_ID, CREATEDBY, UPDATEDBY, SALESREP_ID, \
                            DOCTYPENAME, PRODUCTVALUE, PRICEACTUAL, QTYORDERED, BPARTNERVALUE, DOCUMENTNO, DATEORDERED)\
                            VALUES (%d, %s, %s, %s, %s, %s,  '%s', '%s',  %s,   %s , '%s', '%s', %s'%s',%s)",  
                            iOrderSeq, ClientId, OrgId, UserId, UserId, SaleId, DocumentType, QName, UnitPrice,  TSIT_nsQty1 ,QToName,  TATrs_sTransReference, "to_date (", SDate, "'mm/dd/yyyy')");
           
     MXSetValue (pmessage1, "SqlStatement", 1, Statement);
     MXSend (compiere_com->MX, compiere_com, pmessage1);
     MXFreeMessage (compiere_com->MX, MXRecv (compiere_com->MX, compiere_com));
     iOrderSeq++;
     j++; printf ("<--- %d\n", j);
  return 1;

}   

void SelectOrderRows (MX* pmx)
{
    MXMessage* pmessage;
    int every = 1;
    int max = Count;
    int i = 0;
    BYTE Commit = 1;
 
         
    MXAddComCallBack (pmx, omega_com, DB_SYS,  "SelectOrder", MXONRECV, SelectOrder, compiere_com); 


    pmessage = MXCreateMessage (pmx, DB_SYS, "ExecuteQuery");
    MXSetValue (pmessage ,"Every", 1, &every);
    MXSetValue (pmessage ,"MaxRows", 1, &max);
   
    MXSetValue (pmessage ,"Name",    1,    "SelectOrder"  );
    MXSetValue (pmessage ,"SqlStatement", 1, Statement);
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


   if (argc > 2)
   {
       strcpy (Parameter, argv[1]);
       Year = atoi ( argv[2]);
   }
   else
       return;

   
    sprintf (Statement,  "SELECT StkItemsTransactions.TATrs_nlTransactionId, StkItemsTransactions.Name, vew_TransactionRules.TATrs_sTransDescEng, StkItemsTransactions.TSIT_nsQty1, StkItemsTransactions.TSIT_nsQty2, vew_AccTiersCategory.TAccL_sDesc, vew_AccTiersCategory.TAccT_sDesc, AccTransactionHeader.TATrs_dOperationDate, AccTransactionHeader.ToName, AccTransactionHeader.TATrs_sTransReference, StkItems.Devise, StkItemsTransactions.TSIT_ncUnitPrice, StkItemsTransactions.TSIT_ncUnitPriceLL, StkItemsTransactions.TSIT_nsDiscount, StkItemsTransactions.TSIT_ncTotalPrice \
    FROM ((((StkItemsTransactions INNER JOIN AccTransactionHeader ON StkItemsTransactions.TATrs_nlTransactionId = AccTransactionHeader.TATrs_nlTransactionId) INNER JOIN vew_TransactionRules ON AccTransactionHeader.TATrs_nlTransactionType = vew_TransactionRules.TATrs_nlTransactionType) INNER JOIN StkItems ON StkItemsTransactions.Name = StkItems.Name) INNER JOIN AccTransactionDetails ON StkItemsTransactions.TATrs_nlTransactionId = AccTransactionDetails.TATrs_nlTransactionId) INNER JOIN vew_AccTiersCategory ON AccTransactionDetails.TAccT_nlTiersId = vew_AccTiersCategory.TAccT_nlTiersId \
    WHERE  (((AccTransactionHeader.TATrs_dOperationDate)<= #12/31/%d#) AND ((vew_TransactionRules.TATrs_sTransDescEng)= '%s' )) \
    ORDER BY StkItemsTransactions.TATrs_nlTransactionId, AccTransactionHeader.TATrs_dOperationDate", Year, Parameter);






    MXInit (&mx, MXCLIENT, "base", "base", 0, NULL);

   
    omega_com = MXOpenBase (&mx, "omega", "","", DBPROTO_ODBC, TRUE);
    if (!omega_com) return;
    compiere_com = MXOpenBase (&mx, "compiere", "scott","tiger", DBPROTO_ODBC, TRUE);
    if (!compiere_com) return;

    MXAddComCallBack (&mx, omega_com,    DB_SYS, "AckStatement",   MXONRECV,   AckExcuteQuery, compiere_com); 

    
    
    SelectId ("Techno Cars", ClientId, CLIENT_ID);
    SelectId ("Techno Cars", OrgId, ORG_ID);
    SelectId ("SuperUser", UserId, USER_ID); 
    if (strcmp (Parameter, "Stock Order") != 0)
        SelectId ("Antonia Jureidini", SaleId, USER_ID);
    else
        SelectId ("Tony Jureidini", SaleId, USER_ID);


    SelectSeq ("I_Order", OrderSeq);
    iOrderSeq = atol (OrderSeq);
    MXAddComCallBack (&mx, compiere_com, DB_SYS, "AckStatement",   MXONRECV,   AckExcuteUpdate, omega_com); 

    SelectOrderRows (&mx);
 

    MXDispatchEvents (&mx, 0);

    MXEnd (&mx);

}