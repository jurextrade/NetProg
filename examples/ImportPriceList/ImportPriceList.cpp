
#include "sc.h"


char Parameter[100];
char Statement[10000];

#define USER_ID             0
#define CLIENT_ID           1
#define ORG_ID              2
#define LOCATOR_ID          3
#define BPARTNER_ID         4
#define WAREHOUSE_ID        5
#define DOCUMENT_ID         6
#define BPARTNERLOCATION_ID 7
#define PRODUCT_ID          8
#define UOM_ID              9
#define CURRENCY_ID         10
#define PAYMENTTERM_ID      11
#define PRICELIST_ID        12
#define CONVERSIONTYPE_ID   13
#define TAX_ID              14
#define PRICELIST_VERSION_ID 15


char OrgId[10];
char ClientId[10];
char UserId[10];
char LocationId[10];
char BPartnerId[10];
char BPartnerLocationId[10];
char WarehouseId[10];
char ProductId[10];
char UOMId[10];
char DocumentId[10];
char EURPriceListId[10];
char EURPriceListVersionId[10];
char LBPPriceListId[10];
char LBPPriceListVersionId[10];
char USDPriceListId[10];
char USDPriceListVersionId[10];

char ImmediatePaymentTermId[10];
char CreditPaymentTermId[10];
char EURCurrencyId[10];
char USDCurrencyId[10];
char LBPCurrencyId[10];
char SalesRepId[10];
char ConversionTypeId[10];
char TaxId[10];
char TVAId[10];

char OrderSeq[20];
int  iInvoiceSeq;
char OrderLineSeq[20];
int  iInvoiceLineSeq;

int Line = 10;
double iGrandTotal = 0;
char GrandTotal[20];

double iTotalTax = 0;
char TotalTax[20];
char Tax[20];

char TotalWithTax[20];


char TransactionSeq[20];
int iTransactionSeq; 

char LastTransReference[30];


MXCom* omega_com; 
MXCom* compiere_com; 
int Request = 0;


int QueryFinish = 0;
int j = 0;
int Count = 200000;
int Year = 2005;

int AfterUpdate = 0;

char* strend (char* str, char* schar)
{
    int i = 0;
    for (i = strlen (str) - 1; i >= 0; i--)
      if (str[i] != schar[0])
        break;
    str[i+1] = 0;
    return str;
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
     case TAX_ID :
            sprintf (Statement, "%s'%s'%s%s;", "select C_TAX_ID from C_TAX where NAME = ", Name, " and AD_CLIENT_ID = ", ClientId);
         Table_id = "C_TAX_ID";
       break;
    case CLIENT_ID :
           sprintf (Statement, "%s'%s';", "select AD_CLIENT_ID from AD_CLIENT where NAME = ", Name);
           Table_id = "AD_CLIENT_ID";
       break;
    case  USER_ID :
           sprintf (Statement, "%s'%s';", "select AD_USER_ID from AD_USER where NAME = ", Name);
           Table_id = "AD_USER_ID";
       break;
    case  LOCATOR_ID :
           sprintf (Statement, "%s'%s';", "select M_LOCATOR_ID  from M_LOCATOR where VALUE = ", Name);
           Table_id = "M_LOCATOR_ID";
       break;
    case  WAREHOUSE_ID :
           sprintf (Statement, "%s'%s';", "select M_WAREHOUSE_ID  from M_WAREHOUSE where NAME = ", Name);
           Table_id = "M_WAREHOUSE_ID";
       break;
    case  BPARTNER_ID :
           sprintf (Statement, "%s'%s';", "select C_BPARTNER_ID  from C_BPARTNER where NAME = ", Name);
           Table_id = "C_BPARTNER_ID";
       break;
    case  BPARTNERLOCATION_ID :
           sprintf (Statement, "%s%s;", "select C_BPARTNER_LOCATION_ID  from C_BPARTNER_LOCATION where C_BPARTNER_ID = ", Name);
           Table_id = "C_BPARTNER_LOCATION_ID";
       break;
    case  DOCUMENT_ID :
           sprintf (Statement, "%s'%s'%s%s;", "select C_DOCTYPE_ID  from C_DOCTYPE where NAME = ", Name, " and AD_CLIENT_ID = ", ClientId);
           Table_id = "C_DOCTYPE_ID";
       break;
    case  PRODUCT_ID :
           sprintf (Statement, "%s'%s';", "select M_PRODUCT_ID  from M_PRODUCT where VALUE = ", Name);
           Table_id = "M_PRODUCT_ID";
       break;
    case  UOM_ID :
           sprintf (Statement, "%s%s;", "select C_UOM_ID  from M_PRODUCT where M_PRODUCT_ID = ", Name);
           Table_id = "C_UOM_ID";
       break;
    case  PAYMENTTERM_ID :
           sprintf (Statement, "%s'%s'%s%s;", "select C_PAYMENTTERM_ID  from C_PAYMENTTERM where Name = ", Name, " and AD_CLIENT_ID = ", ClientId);
           Table_id = "C_PAYMENTTERM_ID";
       break;
    case  CURRENCY_ID :
           sprintf (Statement, "%s'%s';", "select C_CURRENCY_ID  from C_CURRENCY where ISO_CODE = ", Name);
           Table_id = "C_CURRENCY_ID";
       break;
    case  PRICELIST_ID :
           sprintf (Statement, "%s'%s';", "select M_PRICELIST_ID  from M_PRICELIST where DESCRIPTION = ", Name);
           Table_id = "M_PRICELIST_ID";
       break;
    case  PRICELIST_VERSION_ID :
           sprintf (Statement, "%s'%s';", "select M_PRICELIST_VERSION_ID  from M_PRICELIST_VERSION where Name = ", Name);
           Table_id = "M_PRICELIST_VERSION_ID";
       break;
    case  CONVERSIONTYPE_ID :
           sprintf (Statement, "%s'%s';", "select C_CONVERSIONTYPE_ID  from C_CONVERSIONTYPE where NAME = ", Name);
           Table_id = "C_CONVERSIONTYPE_ID";
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
    MXFreeMessage (compiere_com->MX, pmessage1);
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
    MXFreeMessage (compiere_com->MX, pmessage1);
    pmessage1 =MXRecv (compiere_com->MX, compiere_com);  // Ack
    MXFreeMessage (compiere_com->MX, pmessage1);
    MXFreeMessage (compiere_com->MX, pmessage);
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
    if (QueryFinish)
    {
       /*
        pmessage1 =  MXPutMessage (compiere_com, DB_SYS, "ExecuteUpdate");
        sprintf (Statement, "%s%d%s" , "update AD_SEQUENCE set CURRENTNEXT = ",  iInvoiceSeq, " WHERE NAME = 'C_Invoice'");
        MXSetValue (pmessage1, "SqlStatement", 1, Statement );
        MXSetValue (pmessage1, "Commit", 1, &Commit);
        MXSend (compiere_com->MX, compiere_com, pmessage1);
        MXFreeMessage (compiere_com->MX, MXRecv (compiere_com->MX, compiere_com));


        sprintf (Statement, "%s%d%s" , "update AD_SEQUENCE set CURRENTNEXT = ",  iInvoiceLineSeq, " WHERE NAME = 'C_InvoiceLine'");
        MXSetValue (pmessage1, "SqlStatement", 1, Statement );
        MXSetValue (pmessage1, "Commit", 1, &Commit);
        MXSend (compiere_com->MX, compiere_com, pmessage1);
        MXFreeMessage (compiere_com->MX, MXRecv (compiere_com->MX, compiere_com));

        MXFreeMessage (compiere_com->MX, pmessage1);
		*/
        MXCloseCom (compiere_com->MX, compiere_com);
        MXCloseCom (compiere_com->MX, omega_com);
   }
    return 1;
}



int SelectPriceList (MXMessage* pmessage, MXCom* pcom, void* appfield)
{  
    MXMessage* pmessage1;
    BYTE Commit = '1';


    pmessage1 =  MXCreateMessage (compiere_com->MX, DB_SYS, "ExecuteUpdate");

    STRING KATNR   =    strend ((STRING) MXGetValue (pmessage, "KATNR", 1), " ");
    STRING GMSNO   =    strend ((STRING)MXGetValue (pmessage, "GMSNO", 1), " ");
    STRING DESCR   =    strend ((STRING)MXGetValue (pmessage, "DESCR", 1), " ");
    STRING ODC     =    strend ((STRING)MXGetValue (pmessage, "ODC", 1), " ");
    STRING DATE    =    strend ((STRING)MXGetValue (pmessage, "DATE", 1), " ");
   
    char SDate[100];
    char syear[100];
    char smonth[100];
    char sday[100];
    strcpy (syear, strtok (DATE, "-"));
    strcpy (smonth, strtok(NULL, "-"));
    strcpy (sday,  strtok(NULL, " "));
    sprintf (SDate, "%s/%s/%s", smonth, sday, syear);
          
    sprintf (Statement, "INSERT INTO PRICELIST2005 (AD_CLIENT_ID, AD_ORG_ID, CREATEDBY, UPDATEDBY, ISACTIVE, NAME, DESCRIPTION, \
						GM, PRICE, PROCESSED, VALIDDATE, VALUE, C_CURRENCY_ID, C_BPARTNER_ID) \
                        VALUES (%s, %s, %s, %s, '%s', '%s', '%s', '%s', %s, '%s', %s'%s',%s, '%s',%s, %s)", 
                        ClientId, OrgId, UserId, UserId, "Y", KATNR, DESCR,  
                        GMSNO, ODC, "Y", "to_date (", SDate, "'mm/dd/yyyy')", KATNR, EURCurrencyId, BPartnerId);

    MXSetValue (pmessage1, "Commit", 1, &Commit);
    MXSetValue (pmessage1, "SqlStatement", 1, Statement);
    MXSend (compiere_com->MX, compiere_com, pmessage1);
    MXFreeMessage (compiere_com->MX, MXRecv (compiere_com->MX, compiere_com));
          


    MXFreeMessage (compiere_com->MX, pmessage1);
    j++; printf ("<--- %d\n", j);
    return 1;

}   

void SelectPriceListRows (MX* pmx)
{
    MXMessage* pmessage;
    int every = 1;
    int max = Count;
    int i = 0;
    BYTE Commit = 1;
 
         
    MXAddComCallBack (pmx, omega_com, DB_SYS,  "SelectPriceList", MXONRECV, SelectPriceList, compiere_com); 


    pmessage = MXCreateMessage (pmx, DB_SYS, "ExecuteQuery");
    MXSetValue (pmessage ,"Every", 1, &every);
    MXSetValue (pmessage ,"MaxRows", 1, &max);
   
    MXSetValue (pmessage ,"Name",    1,    "SelectPriceList"  );
    MXSetValue (pmessage ,"SqlStatement", 1, "select * from PRICELIST2005");
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
    LastTransReference[0] = 0;
    
   if (argc > 1)
   {
       Year = atoi ( argv[1]);
   }
   else
       return;

   
    

    MXInit (&mx, MXCLIENT, "base", "base", 0, NULL);

   
    omega_com = MXOpenBase (&mx, "omega", "","", DBPROTO_ODBC, TRUE);
    if (!omega_com) return;
    compiere_com = MXOpenBase (&mx, "compiere", "scott","tiger", DBPROTO_ODBC, TRUE);
    if (!compiere_com) return;

    MXAddComCallBack (&mx, omega_com,    DB_SYS, "AckStatement",   MXONRECV,   AckExcuteQuery, compiere_com); 

    
    
    SelectId ("Techno Cars",     ClientId,           CLIENT_ID);
    SelectId ("Techno Cars",     OrgId,              ORG_ID);
    SelectId ("SuperUser",       UserId,             USER_ID);
   
    SelectId ("EUR",             EURCurrencyId,         CURRENCY_ID);
    SelectId ("ADAM OPEL AG ACC#182030-025822",       BPartnerId,             BPARTNER_ID);

    MXAddComCallBack (&mx, compiere_com, DB_SYS, "AckStatement",   MXONRECV,   AckExcuteUpdate, omega_com); 
 
    SelectPriceListRows (&mx);
    MXDispatchEvents (&mx, 0);

    MXEnd (&mx);

}