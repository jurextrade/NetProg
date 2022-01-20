
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
int  iOrderSeq;
char OrderLineSeq[20];
int  iOrderLineSeq;

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
int Count = 15000;
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


int SelectStorage (char* ProductId, char* LocationId)
{
    char Statement[250];
    int every = 1;
    int max = 1;
    MXMessage* pmessage;
    MXMessage* pmessage1;

    sprintf (Statement, "select M_PRODUCT_ID from M_STORAGE WHERE M_PRODUCT_ID = %s AND M_LOCATOR_ID = %s ", ProductId, LocationId);

    pmessage =  MXPutMessage (compiere_com, DB_SYS, "ExecuteQuery");
    MXSetValue (pmessage ,"Every", 1, &every);
    MXSetValue (pmessage ,"MaxRows", 1, &max);
    MXSetValue (pmessage ,"Name",    1,         "Statement");
    MXSetValue (pmessage ,"SqlStatement", 1, Statement);
    MXSend (compiere_com->MX, compiere_com, pmessage);

    pmessage1 = MXRecv (compiere_com->MX, compiere_com);    //header
    MXFreeMessage (compiere_com->MX, pmessage1);
    pmessage1 = MXRecv (compiere_com->MX, compiere_com);    //contents

    if (strcmp (pmessage1->MessageClass->Name, "AckStatement") == 0)
    {
        MXFreeMessage (compiere_com->MX, pmessage1);
        MXFreeMessage (compiere_com->MX, pmessage);
        return NULL;
    }
    else
    {
        MXFreeMessage (compiere_com->MX, pmessage1);
        pmessage1 =MXRecv (compiere_com->MX, compiere_com);  // Ack
        MXFreeMessage (compiere_com->MX, pmessage1);
        MXFreeMessage (compiere_com->MX, pmessage);
        return 1;
    }
}

int SelectPrice (char* EURPriceListVersionId, char* ProductId, char* PriceList, char* StandardPrice, char* LimitPrice)
{
    char Statement[250];
    int every = 1;
    int max = 1;
    MXMessage* pmessage;
    MXMessage* pmessage1;

    sprintf (Statement, "select PRICELIST, PRICESTD, PRICELIMIT from M_PRODUCTPRICE WHERE M_PRODUCT_ID = %s AND M_PRICELIST_VERSION_ID = %s ", ProductId, EURPriceListVersionId);

    pmessage =  MXPutMessage (compiere_com, DB_SYS, "ExecuteQuery");
    MXSetValue (pmessage ,"Every", 1, &every);
    MXSetValue (pmessage ,"MaxRows", 1, &max);
    MXSetValue (pmessage ,"Name",    1,         "Statement");
    MXSetValue (pmessage ,"SqlStatement", 1, Statement);
    MXSend (compiere_com->MX, compiere_com, pmessage);

    pmessage1 = MXRecv (compiere_com->MX, compiere_com);    //header



    MXFreeMessage (compiere_com->MX, pmessage1);
    pmessage1 = MXRecv (compiere_com->MX, compiere_com);    //contents

    if (strcmp (pmessage1->MessageClass->Name, "AckStatement") == 0)
    {
        MXFreeMessage (compiere_com->MX, pmessage1);
        MXFreeMessage (compiere_com->MX, pmessage);
        return NULL;
    }
    else
    {
        strcpy (PriceList,     strend ((STRING) MXGetValue (pmessage1, "PRICELIST", 1), " "));
        strcpy (StandardPrice, strend ((STRING) MXGetValue (pmessage1, "PRICESTD", 1),  " "));
        strcpy (LimitPrice,    strend ((STRING) MXGetValue (pmessage1, "PRICELIMIT", 1), " "));
        MXFreeMessage (compiere_com->MX, pmessage1);
        pmessage1 =MXRecv (compiere_com->MX, compiere_com);  // Ack
        MXFreeMessage (compiere_com->MX, pmessage1);
        MXFreeMessage (compiere_com->MX, pmessage);
        return 1;
    }

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
        if (LastTransReference[0] != 0)
        {
          pmessage1 =  MXCreateMessage (compiere_com->MX, DB_SYS, "ExecuteUpdate");
          if (strcmp (Tax, TVAId) != 0)
              iTotalTax = 0;
          
          sprintf (GrandTotal, "%2.2f", iGrandTotal - iTotalTax);

          sprintf (Statement, "INSERT INTO C_ORDERTAX (C_ORDER_ID, C_TAX_ID, AD_CLIENT_ID, AD_ORG_ID, CREATEDBY, UPDATEDBY, TAXBASEAMT, TAXAMT, PROCESSED) \
                              VALUES (%d, %s, %s, %s, %s, %s, %s, %s, '%s')", 
                              iOrderSeq-1, Tax, ClientId, OrgId, UserId, UserId,  
                              GrandTotal, TotalTax, "Y");

          MXSetValue (pmessage1, "Commit", 1, &Commit);
          MXSetValue (pmessage1, "SqlStatement", 1, Statement);
          MXSend (compiere_com->MX, compiere_com, pmessage1);
          MXFreeMessage (compiere_com->MX, MXRecv (compiere_com->MX, compiere_com));
          
          
          sprintf (Statement, "update C_ORDER set TOTALLINES = %s, GRANDTOTAL = %s where DOCUMENTNO = '%s'", GrandTotal, TotalWithTax, LastTransReference);
          MXSetValue (pmessage1, "Commit", 1, &Commit);
          MXSetValue (pmessage1, "SqlStatement", 1, Statement);
          MXSend (compiere_com->MX, compiere_com, pmessage1);
          MXFreeMessage (compiere_com->MX, MXRecv (compiere_com->MX, compiere_com));
          MXFreeMessage (compiere_com->MX, pmessage1);
        }
        pmessage1 =  MXPutMessage (compiere_com, DB_SYS, "ExecuteUpdate");
        sprintf (Statement, "%s%d%s" , "update AD_SEQUENCE set CURRENTNEXT = ",  iOrderSeq, " WHERE NAME = 'C_Order'");
        MXSetValue (pmessage1, "SqlStatement", 1, Statement );
        MXSetValue (pmessage1, "Commit", 1, &Commit);
        MXSend (compiere_com->MX, compiere_com, pmessage1);
        MXFreeMessage (compiere_com->MX, MXRecv (compiere_com->MX, compiere_com));


        sprintf (Statement, "%s%d%s" , "update AD_SEQUENCE set CURRENTNEXT = ",  iOrderLineSeq, " WHERE NAME = 'C_OrderLine'");
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
    char LocationId[100];



    pmessage1 =  MXCreateMessage (compiere_com->MX, DB_SYS, "ExecuteUpdate");

    STRING Name                 =    strend ((STRING) MXGetValue (pmessage, "name", 1), " ");
    STRING TSIT_nsQty1          =    strend ((STRING)MXGetValue (pmessage, "TSIT_nsQty1", 1), " ");
    STRING Location             =    strend ((STRING)MXGetValue (pmessage, "Location", 1), " ");
    STRING TATrs_dOperationDate =    strend ((STRING)MXGetValue (pmessage, "TATrs_dOperationDate", 1), " ");
    STRING TATrs_sTransReference=    strend ((STRING)MXGetValue (pmessage, "TATrs_sTransReference", 1), " ");
    STRING TATrs_sTransDescEng  =    strend ((STRING)MXGetValue (pmessage, "TATrs_sTransDescEng", 1), " ");
    STRING ToName               =    strend ((STRING)MXGetValue (pmessage, "ToName", 1), " ");
    STRING Devise               =    strend ((STRING)MXGetValue (pmessage, "Devise", 1), " ");
    STRING TATrs_ncVATLL        =    strend ((STRING)MXGetValue (pmessage, "TATrs_ncVATLL", 1), " ");
    STRING TATDt_ncAmountLL     =    strend ((STRING)MXGetValue (pmessage, "TATDt_ncAmountLL", 1), " ");
    STRING TSIT_ncVATLL         =    strend ((STRING)MXGetValue (pmessage, "TSIT_ncVATLL", 1), " ");
    STRING TSIT_ncUnitPriceLL   =    strend ((STRING)MXGetValue (pmessage, "TSIT_ncUnitPriceLL", 1), " ");

    TSIT_nsQty1 = TSIT_nsQty1 + 1;
    char QToName[100];
    strTakeOffQuote (QToName, ToName);
    
    for (int k = 0; k < (int)strlen (Location); k++)
        Location[k] = toupper(Location[k]);

    SelectId (Location,    LocationId,         LOCATOR_ID);
    SelectId (Name,        ProductId,          PRODUCT_ID);
    SelectId (ToName,      BPartnerId,         BPARTNER_ID);
    SelectId (BPartnerId,  BPartnerLocationId, BPARTNERLOCATION_ID);
    SelectId (ProductId,   UOMId,              UOM_ID);

    char SDate[100];
    char syear[100];
    char smonth[100];
    char sday[100];
    strcpy (syear, strtok (TATrs_dOperationDate, "-"));
    strcpy (smonth, strtok(NULL, "-"));
    strcpy (sday,  strtok(NULL, " "));
    sprintf (SDate, "%s/%s/%s", smonth, sday, syear);


    int  iTSIT_nsQty1 = atoi (TSIT_nsQty1);

 
    STRING CurrencyId;
    STRING PriceListId;
    STRING PriceListVersionId;



    PriceListId = LBPPriceListId;
    PriceListVersionId = LBPPriceListVersionId;
    CurrencyId  = LBPCurrencyId;
   
  
   
    STRING PaymentTermId;

    if (strncmp (TATrs_sTransDescEng, "Credit", 6) == 0)
    PaymentTermId = CreditPaymentTermId;
    else
    PaymentTermId = ImmediatePaymentTermId;
    if (strcmp (TATrs_sTransReference, "NCS005393") == 0)
        printf ("%s", "Error");

    if (strcmp (TATrs_sTransReference, LastTransReference) != 0)
    {
//INOUT
        if (LastTransReference[0] != 0)
        {
         
          if (strcmp (Tax, TVAId) != 0)
              iTotalTax = 0;
          
          sprintf (GrandTotal, "%2.2f", iGrandTotal - iTotalTax);
        
          sprintf (Statement, "INSERT INTO C_ORDERTAX (C_ORDER_ID, C_TAX_ID, AD_CLIENT_ID, AD_ORG_ID, CREATEDBY, UPDATEDBY, TAXBASEAMT, TAXAMT, PROCESSED) \
                              VALUES (%d, %s, %s, %s, %s, %s, %s, %s, '%s')", 
                              iOrderSeq-1, Tax, ClientId, OrgId, UserId, UserId,  
                              GrandTotal, TotalTax, "Y");

          MXSetValue (pmessage1, "Commit", 1, &Commit);
          MXSetValue (pmessage1, "SqlStatement", 1, Statement);
          MXSend (compiere_com->MX, compiere_com, pmessage1);
          MXFreeMessage (compiere_com->MX, MXRecv (compiere_com->MX, compiere_com));
          
          
          sprintf (Statement, "update C_ORDER set TOTALLINES = %s, GRANDTOTAL = %s where DOCUMENTNO = '%s'", GrandTotal, TotalWithTax, LastTransReference);
          MXSetValue (pmessage1, "Commit", 1, &Commit);
          MXSetValue (pmessage1, "SqlStatement", 1, Statement);
          MXSend (compiere_com->MX, compiere_com, pmessage1);
          MXFreeMessage (compiere_com->MX, MXRecv (compiere_com->MX, compiere_com));
        }

        sprintf (TotalTax, "%s", TATrs_ncVATLL);
        sprintf (TotalWithTax, "%s", TATDt_ncAmountLL);
        
        iGrandTotal  = atof (TotalWithTax);
        iTotalTax = atof (TotalTax);
        
        strcpy (LastTransReference, TATrs_sTransReference);
        sprintf (Statement, "INSERT INTO C_ORDER  (C_ORDER_ID, AD_CLIENT_ID, AD_ORG_ID, CREATEDBY, UPDATEDBY, \
                                                   ISSOTRX, DOCUMENTNO, DOCACTION, DOCSTATUS, PROCESSING, PROCESSED, C_DOCTYPE_ID, C_DOCTYPETARGET_ID, DESCRIPTION, \
                                                   ISAPPROVED, SALESREP_ID, DATEORDERED, DATEPROMISED, DATEACCT,  C_BPARTNER_ID, C_BPARTNER_LOCATION_ID, \
                                                   C_CURRENCY_ID, PAYMENTRULE, C_PAYMENTTERM_ID, INVOICERULE, DELIVERYRULE, FREIGHTCOSTRULE, DELIVERYVIARULE,\
                                                   PRIORITYRULE, TOTALLINES, GRANDTOTAL, M_WAREHOUSE_ID, M_PRICELIST_ID, C_CONVERSIONTYPE_ID)\
                             VALUES (%d, %s, %s, %s, %s, \
                             '%s', '%s', '%s', '%s', '%s', '%s', %s, %s, '%s',\
                             '%s', %s,  %s'%s',%s, %s'%s',%s,  %s'%s',%s, %s, %s, \
                             %s, '%s', %s, '%s', '%s', '%s', '%s', \
                             '%s', %s, %s, %s, %s, %s)",  
                             iOrderSeq, ClientId, OrgId, UserId, UserId,  
                             "Y", LastTransReference,  "CL", "CO", "N", "Y", DocumentId, DocumentId, "", 
                             "Y", SalesRepId, "to_date (", SDate, "'mm/dd/yyyy')", "to_date (", SDate, "'mm/dd/yyyy')", "to_date (", SDate, "'mm/dd/yyyy')", BPartnerId, BPartnerLocationId, 
                             CurrencyId, "B", PaymentTermId, "D", "F", "I", "S", 
                             "5", "0", "0", WarehouseId, PriceListId, ConversionTypeId);
        MXSetValue (pmessage1, "Commit", 1, &Commit);
        MXSetValue (pmessage1, "SqlStatement", 1, Statement);
        MXSend (compiere_com->MX, compiere_com, pmessage1);
        MXFreeMessage (compiere_com->MX, MXRecv (compiere_com->MX, compiere_com));
        iOrderSeq++;
        Line = 10;
    }

//NOUTLINE
    char PriceList[30];
    char StandardPrice[30];
    char LimitPrice[30];
    char Discount[30];
    char TotalPrice[30];
   

   SelectPrice (PriceListVersionId, ProductId, PriceList, StandardPrice, LimitPrice);
    

   double iStandardPrice, iPriceList, iTotalPrice, iDiscount, iLimitPrice;

   iStandardPrice= atof (StandardPrice);
   iPriceList = atof (PriceList);

   strcpy (Tax, TVAId);
   


   double iUnitPrice = atof (TSIT_ncUnitPriceLL);
  
   if (iStandardPrice == 0)
   {
       iStandardPrice = iUnitPrice;
       iPriceList = iUnitPrice;
       iLimitPrice = iUnitPrice;
       sprintf (PriceList, "%2.3f", iUnitPrice);
       sprintf (StandardPrice, "%2.3f", iUnitPrice);
       sprintf (LimitPrice, "%2.3f", iUnitPrice);

       sprintf (Statement, "UPDATE M_PRODUCTPRICE SET PRICELIST = %s, PRICESTD = %s, PRICELIMIT = %s where M_PRICELIST_VERSION_ID = %s AND M_PRODUCT_ID = %s", 
                PriceList, StandardPrice, LimitPrice, PriceListVersionId, ProductId);

       MXSetValue (pmessage1, "Commit", 1, &Commit);
       MXSetValue (pmessage1, "SqlStatement", 1, Statement);
       MXSend (compiere_com->MX, compiere_com, pmessage1);
       MXFreeMessage (compiere_com->MX, MXRecv (compiere_com->MX, compiere_com));
   }
   iUnitPrice = atof (TSIT_ncVATLL);
   iUnitPrice = iUnitPrice * 10;
   

   iTotalPrice = iUnitPrice;


   double iQty = atof (TSIT_nsQty1);
   iUnitPrice = iUnitPrice / iQty;
   iStandardPrice = iUnitPrice;
   


   sprintf (StandardPrice, "%2.3f", iUnitPrice);
   

 
   iDiscount =  0;
   
   if (iPriceList != 0)
       iDiscount  = (iPriceList - iStandardPrice)* 100/ iPriceList;
   
   sprintf (Discount, "%2.0f", iDiscount);
   sprintf (TotalPrice, "%2.2f", iTotalPrice);

   



    sprintf (Statement, "INSERT INTO C_ORDERLINE  (C_ORDERLINE_ID, AD_CLIENT_ID, AD_ORG_ID, CREATEDBY, UPDATEDBY, \
                                                   LINE, C_ORDER_ID, C_BPARTNER_ID, C_BPARTNER_LOCATION_ID,\
                                                   DATEORDERED, DATEPROMISED, DESCRIPTION, M_WAREHOUSE_ID, M_PRODUCT_ID, C_UOM_ID,\
                                                   QTYORDERED, QTYRESERVED, C_CURRENCY_ID, PRICELIST, PRICEACTUAL, PRICELIMIT, LINENETAMT, DISCOUNT,\
                                                   C_TAX_ID, PROCESSED, QTYENTERED, PRICEENTERED)\
                         VALUES (%d, %s, %s, %s, %s, \
                         %d,  %d, %s, %s,\
                         %s'%s',%s, %s'%s',%s, '%s', %s, %s, %s,\
                         %s, %s, %s, %s, %s, %s, %s, %s, \
                         %s, '%s', %s, %s)",
                         iOrderLineSeq, ClientId, OrgId, UserId, UserId,
                         Line, iOrderSeq - 1, BPartnerId, BPartnerLocationId, 
                         "to_date (", SDate, "'mm/dd/yyyy')", "to_date (", SDate, "'mm/dd/yyyy')", "", WarehouseId, ProductId, UOMId,
                         TSIT_nsQty1, TSIT_nsQty1, CurrencyId, PriceList, StandardPrice, LimitPrice, TotalPrice, Discount, 
                         Tax, "Y", TSIT_nsQty1, StandardPrice);
    Line += 10;
    iOrderLineSeq++;
    MXSetValue (pmessage1, "Commit", 1, &Commit);
    MXSetValue (pmessage1, "SqlStatement", 1, Statement);
    MXSend (compiere_com->MX, compiere_com, pmessage1);
    MXFreeMessage (compiere_com->MX, MXRecv (compiere_com->MX, compiere_com));

    if (SelectStorage (ProductId, LocationId) == 1)
    { 
        sprintf (Statement, "update M_STORAGE SET QTYRESERVED = QTYRESERVED + %s WHERE M_PRODUCT_ID = %s AND M_LOCATOR_ID = %s ", TSIT_nsQty1, ProductId, LocationId); 
    }
    else
    { 
      
        sprintf (Statement, "insert into M_STORAGE (M_PRODUCT_ID, M_LOCATOR_ID, AD_CLIENT_ID, AD_ORG_ID, CREATEDBY, UPDATEDBY,   QTYRESERVED) \
                         VALUES (%s, %s, %s, %s, %s, %s, %s)",
                         ProductId, LocationId, ClientId, OrgId, UserId, UserId, TSIT_nsQty1);
    
    }
    MXSetValue (pmessage1, "Commit", 1, &Commit);
    MXSetValue (pmessage1, "SqlStatement", 1, Statement);
    MXSend (compiere_com->MX, compiere_com, pmessage1);
    MXFreeMessage (compiere_com->MX, MXRecv (compiere_com->MX, compiere_com));

    MXFreeMessage (compiere_com->MX, pmessage1);
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
    LastTransReference[0] = 0;
    char SPriceList[200];
    char SPriceListVersion[200];
    
   if (argc > 2)
   {
       strcpy (Parameter, argv[1]);
       Year = atoi ( argv[2]);
   }
   else
       return;

   
    sprintf (Statement,  "SELECT StkItemsTransactions.TATrs_nlTransactionId, StkItems.Location, StkItemsTransactions.Name, AccTransactionHeader.TATrs_nlVoucherNbr, vew_TransactionRules.TATrs_sTransDescEng, StkItemsTransactions.TSIT_nsQty1, AccTransactionHeader.TATrs_dOperationDate, AccTransactionHeader.ToName, AccTransactionHeader.TATrs_sTransReference, StkItems.Devise, StkItemsTransactions.TSIT_ncVATLL, StkItemsTransactions.TSIT_ncUnitPriceLL, StkItemsTransactions.TSIT_nsDiscount, StkItemsTransactions.TSIT_ncTotalPrice, AccTransactionDetails.TATDt_ncAmountLL, AccTransactionDetails.TATDt_sDesc, AccTransactionHeader.TATrs_ncVATLL, AccTransactionDetails.TATDt_bIsDebit \
                            FROM (((StkItemsTransactions INNER JOIN AccTransactionHeader ON StkItemsTransactions.TATrs_nlTransactionId = AccTransactionHeader.TATrs_nlTransactionId) INNER JOIN vew_TransactionRules ON AccTransactionHeader.TATrs_nlTransactionType = vew_TransactionRules.TATrs_nlTransactionType) INNER JOIN StkItems ON StkItemsTransactions.Name = StkItems.Name) INNER JOIN AccTransactionDetails ON AccTransactionHeader.TATrs_nlTransactionId = AccTransactionDetails.TATrs_nlTransactionId \
                            WHERE (((vew_TransactionRules.TATrs_sTransDescEng) Like '%s') AND ((AccTransactionHeader.TATrs_dOperationDate) %s) AND ((AccTransactionDetails.TATDt_bIsDebit)=-1)) \
                            ORDER BY StkItemsTransactions.TATrs_nlTransactionId, StkItemsTransactions.Name",  "Credit Sale", (Year == 2004) ? "<= #12/31/2004#" : "> #12/31/2004#");


    MXInit (&mx, MXCLIENT, "base", "base", 0, NULL);

   
    omega_com = MXOpenBase (&mx, "omega", "","", DBPROTO_ODBC, TRUE);
    if (!omega_com) return;
    compiere_com = MXOpenBase (&mx, "compiere", "scott","tiger", DBPROTO_ODBC, TRUE);
    if (!compiere_com) return;

    MXAddComCallBack (&mx, omega_com,    DB_SYS, "AckStatement",   MXONRECV,   AckExcuteQuery, compiere_com); 

    
    
    SelectId ("Techno Cars",     ClientId,           CLIENT_ID);
    SelectId ("Techno Cars",     OrgId,              ORG_ID);
    SelectId ("SuperUser",       UserId,             USER_ID);
    SelectId ("Standard",        WarehouseId,        WAREHOUSE_ID);
    SelectId ("POS Order",  DocumentId,         DOCUMENT_ID);
    
    sprintf (SPriceList, "Sale Prices for %d in EUR", Year);
    sprintf (SPriceListVersion, "Sale EUR %d", Year);
    SelectId (SPriceList,        EURPriceListId,        PRICELIST_ID);
    SelectId (SPriceListVersion, EURPriceListVersionId, PRICELIST_VERSION_ID);
    
    sprintf (SPriceList, "Sale Prices for %d in USD", Year);
    sprintf (SPriceListVersion, "Sale USD %d", Year);
    SelectId (SPriceList,        USDPriceListId,        PRICELIST_ID);
    SelectId (SPriceListVersion, USDPriceListVersionId, PRICELIST_VERSION_ID);
    
    sprintf (SPriceList, "Sale Prices for %d in LBP", Year);
    sprintf (SPriceListVersion, "Sale LBP %d", Year);
    SelectId (SPriceList,        LBPPriceListId,        PRICELIST_ID);
    SelectId (SPriceListVersion, LBPPriceListVersionId, PRICELIST_VERSION_ID);

    SelectId ("Immediate",       ImmediatePaymentTermId,      PAYMENTTERM_ID);
    SelectId ("Credit",          CreditPaymentTermId,         PAYMENTTERM_ID);
    SelectId ("EUR",             EURCurrencyId,         CURRENCY_ID);
    SelectId ("USD",             USDCurrencyId,         CURRENCY_ID);
    SelectId ("LBP",             LBPCurrencyId,         CURRENCY_ID);

    SelectId ("Spot",            ConversionTypeId,   CONVERSIONTYPE_ID);
    SelectId ("Joseph Jureidini",  SalesRepId,         USER_ID);
    SelectId ("Standard",        TaxId,              TAX_ID);
    SelectId ("TVA",             TVAId,              TAX_ID);


    SelectSeq ("M_Transaction", TransactionSeq);
    iTransactionSeq = atol (TransactionSeq);

    SelectSeq ("C_Order", OrderSeq);
    iOrderSeq = atol (OrderSeq);

    SelectSeq ("C_OrderLine", OrderLineSeq);
    iOrderLineSeq = atol (OrderLineSeq);

    MXAddComCallBack (&mx, compiere_com, DB_SYS, "AckStatement",   MXONRECV,   AckExcuteUpdate, omega_com); 
 
    SelectOrderRows (&mx);
    MXDispatchEvents (&mx, 0);

    MXEnd (&mx);

}